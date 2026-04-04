#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <sys/stat.h>
#include <bcm2835.h>


// CO2-read-command
unsigned char co2_command[] = {0xFF, 0x01, 0x86, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x79};

// Function which opens the serial ports and set up parameters
int open_serial_port(const char *device) {
  int fd = open(device, O_RDWR | O_NOCTTY);
  if(fd < 0) {
    perror("Unable to open serial port");
    return -1;
  }

  struct termios options;
  tcgetattr(fd, &options);

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

void handleSigint(int sig){
  printf("\nCaught Ctrl+C, cleaning up...\n");
  // close(fd2);
  sleep(2);
  exit(0);
}

// function which reads the CO2-value
int read_co2(int fd, unsigned char *buffer, int buffer_size){
  int n = write(fd, co2_command, sizeof(co2_command));
  if(n < 0){
    perror("Write failed");
    return -1;
  }

  usleep(1000000); // wait 1 second

  n = read(fd, buffer, buffer_size);
  if(n < 0){
    perror("Read failed");
    return -1;
  }

  return n;
}

// function to extract the CO2 value
unsigned int parse_co2(unsigned char *response){
  unsigned int co2 = (response[2] << 8) | response[3];
  return co2;
}

uint16_t read_mcp3008(uint8_t channel) {
  char buf[3];

  buf[0] = 0x01; // Start bit
  buf[1] = (0x08 | channel) << 4;
  buf[2] = 0x00;

  bcm2835_spi_transfern(buf, 3);

  uint16_t value = ((buf[1] & 0x03) << 8) | buf[2];
  return value;
}

int main(){
  int fd = open_serial_port("/dev/serial0");
  if(fd == -1) {
    return 1;
  }

  int mCount = 0;
  unsigned char buffer[9]; // The sensor answer has 9 Bytes


  char buffer2[100];
  int fd3;
  FILE *file_ptr;

  fd3 = open("results.txt", O_RDWR);
  if(fd3 < 0){
    perror("Failed to open result file");
    return 1;
  }
  file_ptr = fdopen(fd3, "a");
  if(file_ptr == NULL) {
    perror("Failed to convert file descriptor to FILE*");
    close(fd3);
    return 1;
  }

  // Initialize the BCM2835-Bibliothek and SPI
  if(!bcm2835_init()){
    printf("BCM2835-Bibliothek konnte nicht initialisiert werden!\n");
    return 1;
  }

  // Initialize SPI
  bcm2835_spi_begin();
  bcm2835_spi_set_speed_hz(1000000);  // SPI speed set to 1 MHz
  bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
  bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);  // Mode0 (CPOL=0, CPHA=0)

  bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_65536);
  bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
  bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);

  signal(SIGINT, handleSigint);

  while(1){
    mCount++;
    time_t start_time = time(NULL);
    struct tm *tm_info = localtime(&start_time);

    char time_buffer[26];
    strftime(time_buffer,sizeof(time_buffer),"%Y-%m-%d %H:%M:%S", tm_info);

    int len = read_co2(fd, buffer, sizeof(buffer));
    if(len < 0){
      perror("CO2 read failed");
      continue;
    }

    unsigned int co2 = parse_co2(buffer);
    fprintf(file_ptr, "%d. c(CO2): %u ppm %s\n", mCount, co2, time_buffer);
    fflush(file_ptr);
    sleep(1);

    uint16_t mq135_value = read_mcp3008(0); // CH0
    uint16_t mq7_value = read_mcp3008(1);   // CH1
    fprintf(file_ptr, "MQ135 Value: %d\n", mq135_value);
    fprintf(file_ptr, "MQ7 Value: %d\n", mq7_value);
    fflush(file_ptr);
    bcm2835_delay(5000);
  }
  bcm2835_spi_end();
  bcm2835_close();
  fclose(file_ptr);
  close(fd);
  return 0;
}
