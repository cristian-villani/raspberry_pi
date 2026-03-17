#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <time.h>

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

int main(){
  int fd = open_serial_port("/dev/serial0");
  if(fd == -1) {
    return 1;
  }

  int mCount = 0;
  unsigned char buffer[9]; // The sensor answer has 9 Bytes
  while(1){
    int len = read_co2(fd, buffer, sizeof(buffer));
    if(len < 0){
      return 1;
    }

    mCount++;
    time_t start_time = time(NULL);
    struct tm *tm_info = localtime(&start_time);

    char time_buffer[26];
    strftime(time_buffer,sizeof(time_buffer),"%Y-%m-%d %H:%M:%S", tm_info);

    unsigned int co2 = parse_co2(buffer);
    printf("%d. c(CO2): %u ppm %s\n", mCount, co2, time_buffer);
    fflush(stdout);
    sleep(5);
  }

  close(fd);
  return 0;
}
