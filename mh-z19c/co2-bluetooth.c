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


// CO2-read-command
unsigned char co2_command[] = {0xFF, 0x01, 0x86, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x79};

int fd2;

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
  close(fd2);
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

// waits till the device /dev/rfcmm0 is available
int wait_for_rfcomm(const char *path, int delay_sec){
  struct stat st;
  int attempts = 0;
  int fde;

  while(1){
    // Check if file exists
    if(stat(path, &st) == 0){
      fde = open(path, O_RDWR | O_NOCTTY);
      if(fde >= 0) return fde; // Success
      perror("Failed to open /dev/rfcomm0\n");
    }
    sleep(delay_sec);
  }
}

int main(){
  const char *rfcomm_path = "/dev/rfcomm0";
  fd2 = wait_for_rfcomm(rfcomm_path, 2); // infinite retries(0), 2s interval
  if(fd2 < 0){
    perror("Could not open RFCOMM device. Exiting. \n");
    return 1;
  }
  printf("/dev/rfcomm0 is ready and opened!\n");

  int fd = open_serial_port("/dev/serial0");
  if(fd == -1) {
    return 1;
  }

  int mCount = 0;
  unsigned char buffer[9]; // The sensor answer has 9 Bytes


  char buffer2[100];
  int fd3;
  FILE *file_ptr;

  fd3 = open("co2-results.txt", O_RDWR);
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

  signal(SIGINT, handleSigint);

  while(1){
    memset(buffer2, 0, sizeof(buffer2));
    int n = read(fd2, buffer2, sizeof(buffer2)-1);
    if(n > 0){
      buffer2[n] = '\0';
      printf("Received: %s\n", buffer2);

      if(strncmp(buffer2, "go", 2) == 0) {
        int len = read_co2(fd, buffer, sizeof(buffer));
        if(len < 0){
          perror("CO2 read failed");
          continue;
        }
        mCount++;
        time_t start_time = time(NULL);
        struct tm *tm_info = localtime(&start_time);

        char time_buffer[26];
        strftime(time_buffer,sizeof(time_buffer),"%Y-%m-%d %H:%M:%S", tm_info);

        unsigned int co2 = parse_co2(buffer);
        char message[256];
        sprintf(message, "c(CO2): %u ppm\n", co2);
        write(fd2, "-----\n", 6);
        write(fd2, message, strlen(message));
        write(fd2, "-----\n", 6);
        fprintf(file_ptr, "%d. c(CO2): %u ppm %s\n", mCount, co2, time_buffer);
        fflush(file_ptr);
        sleep(1);
      }
      else if(strncmp(buffer2, "sd", 2) == 0){
        write(fd2, "-----\n", 6);
        write(fd2, "shut down request sent\n", 23);
        write(fd2, "-----\n", 6);
        fclose(file_ptr);
        close(fd2);
        close(fd);
        system("/usr/bin/sudo /usr/sbin/shutdown -h now");
        exit(0);
      }
      else{
        fprintf(file_ptr, "%s\n", buffer2);
        fflush(file_ptr);
      }
    }
    else{
      perror("Bluetooth disconnected or device gone, waiting for restart\n");
      close(fd2);
      fclose(file_ptr);
      close(fd);
    }
  }
  fclose(file_ptr);
  close(fd);
  close(fd2);
  return 0;
}
