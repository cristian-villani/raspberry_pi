#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <signal.h>
#include <bcm2835.h>
#include "../motor/motor.h"
#include "../sensor/ultrasonic.h"

#define SPEED 280
#define MAX_DISTANCE 30

int fd2;

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

void cleanup(int sig){
  printf("\nStopping program...\n");
  motor_stop();
  close(fd2);
  bcm2835_close();

  printf("GPIO cleaned up\n");
  exit(0);
}

int main() {

  printf("Server running and ready for connections\n");
  fflush(stdout);

  const char *rfcomm_path = "/dev/rfcomm0";
  fd2 = wait_for_rfcomm(rfcomm_path, 2); // infinite retries(0), 2s interval
  if(fd2 < 0){
    perror("Could not open RFCOMM device. Exiting. \n");
    return 1;
  }
  printf("/dev/rfcomm0 is ready and opened!\n");

  char buffer2[100];

  // Initialize the BCM2835 library
  if (!bcm2835_init()) {
    printf("BCM2835 initialization failed!\n");
    return 1;
  }
  motor_init();

  signal(SIGINT, cleanup); // Ctrl-C Handler
  signal(SIGPIPE, SIG_IGN); // Ctrl-C Handler

  while(1){
    bcm2835_delay(200);
    float d = ultrasonic_get_distance();
    if(d < MAX_DISTANCE && current_state == FORWARD){
      current_state = STOP;
      current_speed = 0;
    }
    bcm2835_delay(200);
    motor_update();
    fd_set set;
    struct timeval timeout;

    FD_ZERO(&set);
    FD_SET(fd2, &set);

    timeout.tv_sec = 0;
    timeout.tv_usec = 50000;

    int rv = select(fd2 + 1, &set, NULL, NULL, &timeout);

    if(rv > 0){

      memset(buffer2, 0, sizeof(buffer2));
      int n = read(fd2, buffer2, sizeof(buffer2)-1);
      if(n > 0){
        buffer2[n] = '\0';
        printf("Received: %s\n", buffer2);

        if(strncmp(buffer2, "F", 1) == 0){
          write(fd2, "-----\n", 6);
          write(fd2, "F\n", 2);
          write(fd2, "-----\n", 6);
          // motor_forward(SPEED);
          current_state = FORWARD;
          current_speed = SPEED;
        }
        else if(strncmp(buffer2, "B", 1) == 0){
          write(fd2, "-----\n", 6);
          write(fd2, "B\n", 2);
          write(fd2, "-----\n", 6);
          // motor_backward(SPEED);
          current_state = BACKWARD;
          current_speed = SPEED;
        }
        else if(strncmp(buffer2, "L", 1) == 0){
          write(fd2, "-----\n", 6);
          write(fd2, "L\n", 2);
          write(fd2, "-----\n", 6);
          // motor_left(SPEED);
          current_state = LEFT;
          current_speed = SPEED;
        }
        else if(strncmp(buffer2, "R", 1) == 0){
          write(fd2, "-----\n", 6);
          write(fd2, "R\n", 2);
          write(fd2, "-----\n", 6);
          // motor_right(SPEED);
          current_state = RIGHT;
          current_speed = SPEED;
        }
        else if(strncmp(buffer2, "S", 1) == 0){
          write(fd2, "-----\n", 6);
          write(fd2, "S\n", 2);
          write(fd2, "-----\n", 6);
          // motor_stop();
          current_state = STOP;
          current_speed = 0;
        }
        else if(strncmp(buffer2, "Q", 1) == 0){
          write(fd2, "-----\n", 6);
          write(fd2, "Restart request sent\n", 21);
          write(fd2, "-----\n", 6);
          // motor_stop();
          current_state = STOP;
          current_speed = 0;
          motor_update();
          system("shutdown -r now");
        }
        else if(strncmp(buffer2, "X", 1) == 0){
          write(fd2, "-----\n", 6);
          write(fd2, "Shut down request sent\n", 23);
          write(fd2, "-----\n", 6);
          // motor_stop();
          current_state = STOP;
          current_speed = 0;
          motor_update();
          system("shutdown -h now");
        }
      }
      else{
        perror("Bluetooth disconnected or device gone, waiting for restart\n");
        close(fd2);
        fd2 = wait_for_rfcomm(rfcomm_path, 2);
        if(fd2 < 0){
          perror("Reopen failed");
          continue;
        }
        printf("Reconnected to /dev/rfcomm0\n");
      }
    }
  }
  close(fd2);
  return 0;
}
