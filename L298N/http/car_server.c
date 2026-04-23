#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <time.h>
#include <bcm2835.h>
#include "../motor/motor.h"
#include "../sensor/ultrasonic.h"

#define PORT 5000
#define SPEED 280
#define MAX_DISTANCE 30

int server_fd, client_fd;

void cleanup(int sig)
{
  printf("\nStopping program...\n");

  motor_stop();
  bcm2835_close();
  close(client_fd);

  printf("GPIO cleaned up\n");
  exit(0);
}

void http_react(const char *request){
  if(strstr(request, "right") != NULL){
    printf("right\n");
    current_state = RIGHT;
    current_speed = SPEED;
  }
  else if(strstr(request, "left") != NULL){
    printf("left\n");
    current_state = LEFT;
    current_speed = SPEED;
  }
  else if(strstr(request, "up") != NULL){
    printf("up\n");
    current_state = FORWARD;
    current_speed = SPEED;
  }
  else if(strstr(request, "down") != NULL){
    printf("down\n");
    current_state = BACKWARD;
    current_speed = SPEED;
  }
  else if(strstr(request, "fire") != NULL){
    printf("fire\n");
    current_state = STOP;
    current_speed = 0;
  }
}

int main() {

  struct sockaddr_in address;
  char buffer[1024];

  // Initialize the BCM2835 library
  if (!bcm2835_init()) {
    printf("BCM2835 initialization failed!\n");
    return 1;
  }
  motor_init();
  ultrasonic_init();

  server_fd = socket(AF_INET, SOCK_STREAM, 0);

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  bind(server_fd, (struct sockaddr*)&address, sizeof(address));

  listen(server_fd, 5);

  printf("Server running on port %d\n", PORT);

  signal(SIGINT, cleanup); // Ctrl-C Handler

  while (1) {
    float d = ultrasonic_get_distance();
    if(d < MAX_DISTANCE && current_state == FORWARD){
      current_state = STOP;
      current_speed = 0;
    }
    bcm2835_delay(80);
    motor_update();
    fd_set set;
    struct timeval timeout;

    FD_ZERO(&set);
    FD_SET(server_fd, &set);

    timeout.tv_sec = 0;
    timeout.tv_usec = 50000; // 50 ms

    int rv = select(server_fd + 1, &set, NULL, NULL, &timeout);
    if (rv > 0){
      client_fd = accept(server_fd, NULL, NULL);
      if (client_fd < 0) continue;
      int n = read(client_fd, buffer, sizeof(buffer)-1);
      if(n > 0){
        buffer[n] = '\0';
        char method[8];
        char path[64];

        sscanf(buffer, "%7s %63s", method, path);

        if(strcmp(path, "/up") == 0) http_react("up");
        else if (strcmp(path, "/down") == 0) http_react("down");
        else if (strcmp(path, "/left") == 0) http_react("left");
        else if (strcmp(path, "/right") == 0) http_react("right");
        else if (strcmp(path, "/fire") == 0) http_react("fire");
        const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<!DOCTYPE html>"
        "<html>"
        "<head>"
        "<style>"
        "body { font-family: Arial; text-align: center; background: #111; color: white; }"
        ".grid { display: grid; grid-template-columns: 100px 100px 100px; "
        "justify-content: center; gap: 10px; margin-top: 50px; }"
        "button { width: 100px; height: 60px; font-size: 16px; border-radius: 10px; }"
        ".up { grid-column: 2; }"
        ".left { grid-column: 1; grid-row: 2; }"
        ".stop { grid-column: 2; grid-row: 2; background: red; color: white; }"
        ".right { grid-column: 3; grid-row: 2; }"
        ".down { grid-column: 2; grid-row: 3; }"
        "</style>"
        "</head>"
        "<body>"
        "<h1>Raspberry Pi Control</h1>"
        "<div class='grid'>"

        "<a href='/up' class='up'><button>Forward</button></a>"
        "<a href='/left' class='left'><button>Left</button></a>"
        "<a href='/fire' class='stop'><button>STOP</button></a>"
        "<a href='/right' class='right'><button>Right</button></a>"
        "<a href='/down' class='down'><button>Backward</button></a>"

        "</div>"
        "</body>"
        "</html>";

        write(client_fd, response, strlen(response));
        close(client_fd);
      }
    }
  }
  return 0;
}
