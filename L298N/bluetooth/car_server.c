#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bcm2835.h>
#include "../motor/motor.h"
#include "../sensor/ultrasonic.h"

#define SPEED 280
#define MAX_DISTANCE 30

int server_sock = -1;
int client_sock = -1;

int bt_server_start(){
  struct sockaddr_rc loc_addr = { 0 };

  server_sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
  if(server_sock < 0){
    perror("socket");
    return -1;
  }

  loc_addr.rc_family = AF_BLUETOOTH;
  bdaddr_t any_addr = {{0, 0, 0, 0, 0, 0}};
  bacpy(&loc_addr.rc_bdaddr, &any_addr);
  // loc_addr.rc_bdaddr = *BDADDR_ANY;
  loc_addr.rc_channel = (uint8_t) 1;

  if(bind(server_sock, (struct sockaddr *)&loc_addr, sizeof(loc_addr)) < 0){
    perror("bind");
    return -1;
  }

  listen(server_sock, 1);

  printf("Waiting for Bluetooth connection...\n");

  struct sockaddr_rc rem_addr = { 0 };
  socklen_t opt = sizeof(rem_addr);

  client_sock = accept(server_sock, (struct sockaddr *)&rem_addr, &opt);
  if(client_sock < 0){
    perror("accept");
    return -1;
  }

  char addr[19] = { 0 };
  ba2str(&rem_addr.rc_bdaddr, addr);
  printf("Connected to %s\n", addr);

  return client_sock;
}

void cleanup(int sig){
  printf("\nCleaning up...\n");

  if (client_sock > 0) close(client_sock);
  if (server_sock > 0) close(server_sock);

  motor_stop();
  bcm2835_close();

  exit(0);
}

int main() {


  // Initialize the BCM2835 library
  if (!bcm2835_init()) {
    printf("BCM2835 initialization failed!\n");
    return 1;
  }
  motor_init();
  ultrasonic_init();

  signal(SIGINT, cleanup);
  signal(SIGPIPE, SIG_IGN);

  char buf[256] = { 0 };

  fd_set set;
  struct timeval timeout;

  int fd = bt_server_start();

  while(1){
    if(fd < 0){
      fd = bt_server_start();
      continue;
    }
    FD_ZERO(&set);
    FD_SET(fd, &set);

    timeout.tv_sec = 0;
    timeout.tv_usec = 50000;

    int rv = select(fd + 1, &set, NULL, NULL, &timeout);

    if(rv > 0){

      // memset(buffer2, 0, sizeof(buffer2));
      int n = read(fd, buf, sizeof(buf)-1);
      if(n <= 0){
        printf("Client disconnected\n");
        close(fd);
        fd = -1;
        continue;
      }
      buf[n] = '\0';
      printf("Received: %s\n", buf);

      if(strncmp(buf, "F", 1) == 0){
        current_state = FORWARD;
        current_speed = SPEED;
      }
      else if(strncmp(buf, "B", 1) == 0){
        current_state = BACKWARD;
        current_speed = SPEED;
      }
      else if(strncmp(buf, "L", 1) == 0){
        current_state = LEFT;
        current_speed = SPEED;
      }
      else if(strncmp(buf, "R", 1) == 0){
        current_state = RIGHT;
        current_speed = SPEED;
      }
      else if(strncmp(buf, "S", 1) == 0){
        current_state = STOP;
        current_speed = 0;
      }
      else if(strncmp(buf, "Q", 1) == 0){
        printf("Restart request sent\n");
        current_state = STOP;
        current_speed = 0;
        motor_update();
        system("shutdown -r now");
      }
      else if(strncmp(buf, "X", 1) == 0){
        printf("Shutdown request sent\n");
        current_state = STOP;
        current_speed = 0;
        motor_update();
        system("shutdown -h now");
      }
    }
    usleep(20000);
    float d = ultrasonic_get_distance();
    if(d > 0.05 && d < MAX_DISTANCE && current_state == FORWARD){
      current_state = STOP;
      current_speed = 0;
    }
    motor_update();
  }
}
