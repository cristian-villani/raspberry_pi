#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <signal.h>
#include <bcm2835.h>
#include <time.h>

#define BLINK_LED_PIN RPI_V2_GPIO_P1_32 // GPIO 12
#define BUTTON_PIN RPI_V2_GPIO_P1_36     // GPIO 16

void cleanup(int sig)
{
  printf("\nStopping program...\n");
  bcm2835_gpio_write(BLINK_LED_PIN, LOW);  // stop blink LED
  bcm2835_close();
  printf("GPIO cleaned up\n");
  exit(0);
}

// CO2-read-command
unsigned char co2_command[] = {0xFF, 0x01, 0x86, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x79};

// open serial ports and set up parameters
int open_serial_port(const char *device) {
  int fd = open(device, O_RDWR | O_NOCTTY);
  if(fd < 0) {
    perror("Unable to open serial port");
    return -1;
  }

  struct termios options;
  tcgetattr(fd, &options);

  // set bound rate 9600
  cfsetispeed(&options, B9600);
  cfsetospeed(&options, B9600);

  options.c_cflag &= ~PARENB;
  options.c_cflag &= ~CSTOPB;
  options.c_cflag &= ~CSIZE;
  options.c_cflag |= CS8;

  options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  options.c_oflag &= ~OPOST;

  tcsetattr(fd, TCSANOW, &options);

  return fd;
}

#define TIMEOUT 5  // Timeout in seconds

// read CO2-values with Timeout
int read_co2_with_timeout(int fd, unsigned char *buffer, int buffer_size) {
  int n;
  unsigned long start_time = time(NULL);

  // Sending read command to the sensor
  printf("writing\n");
  n = write(fd, co2_command, sizeof(co2_command));
  if(n < 0){
    perror("Write failed");
    return -1;
  }

  printf("Starting to read\n");
  while(difftime(time(NULL), start_time) < TIMEOUT){
    printf("Inside the reading function\n");
    n = read(fd, buffer, buffer_size);
    if(n > 0){
      return n;  // Successfully read
    }
    usleep(100000);  // wait 100ms and try again
  }

  // Timeout, no answer from the Sensor
  printf("Timeout\n");
  return -1;
}

// Function which extracts the CO2-value
unsigned int parse_co2(unsigned char *response){
  unsigned int co2 = (response[2] << 8) | response[3];
  return co2;
}

int main(){
  // Initialize the BCM2835 library
  if (!bcm2835_init()){
    printf("BCM2835 initialization failed!\n");
    return 1;
  }

  signal(SIGINT, cleanup); // Ctrl-C Handler
  bcm2835_gpio_fsel(BUTTON_PIN, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(BLINK_LED_PIN, BCM2835_GPIO_FSEL_OUTP);

  int fd = open_serial_port("/dev/serial0");
  if(fd == -1) {
    return 1;
  }

  FILE *fres = fopen("results.txt", "w");
  if(fres == NULL){
    printf("Error opening new result file\n");
    return -1;
  }

  unsigned char buffer[9]; // Sensor response has 9 Bytes
  int mCount = 0;

  time_t start_time, current_time, last_press_time, last_print_time;
  start_time = time(NULL);
  last_print_time = start_time;
  last_press_time = time(NULL);
  while(1){
    // Wait, do nothing till the button is pressed
    bcm2835_delay(100);
    double debounce_time = 0.01;
    current_time = time(NULL);
    double elapsed_time = current_time - start_time;
    if (bcm2835_gpio_lev(BUTTON_PIN) == HIGH) {
      if(elapsed_time > debounce_time){
        printf("Button pressed!\n");
        last_press_time = current_time;
        bcm2835_delay(200);  // wait 200 ms
        bcm2835_gpio_write(BUTTON_PIN, LOW); // Set pin LOW after press
        mCount++;

        time_t start_time = time(NULL);

        struct tm *tm_info = localtime(&start_time);

        char time_buffer[26];
        strftime(time_buffer,sizeof(time_buffer),"%Y-%m-%d %H:%M:%S", tm_info);
        fprintf(fres,"\n\nMessung Nr. %d: %s\n",mCount,time_buffer);

        // Send short signal to BLINK_LED
        bcm2835_gpio_write(BLINK_LED_PIN, HIGH);
        bcm2835_delay(1000);
        bcm2835_gpio_write(BLINK_LED_PIN, LOW);
        printf("Starting the measurement...\n");

        while (difftime(time(NULL), start_time) < 20) {

          int len = read_co2_with_timeout(fd, buffer, sizeof(buffer));
          if(len < 0){
            return 1;
          }

          unsigned int co2 = parse_co2(buffer);
          printf("CO2 Concentration: %u ppm\n", co2);
          fprintf(fres, " %u ", co2);
          fflush(fres);
          sleep(2);
        }
      }
    }
    else{
      if((int)elapsed_time % 10 == 0 && current_time - last_print_time >= 10){
        // write time elapsed each ten seconds
        printf("Time: %lf \n", elapsed_time);
        usleep(10000);
        last_print_time = current_time;
      }
    }
  }
  fclose(fres);
  close(fd);
  return 0;
}
