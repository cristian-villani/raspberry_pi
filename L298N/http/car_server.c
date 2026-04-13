#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <bcm2835.h>
#include "motor.h"

#define PORT 5000
#define SPEED 280

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

void led_react(char *request){
  if(strstr(request, "right") != NULL){
    printf("right\n");
    motor_right(SPEED);
  }
  else if(strstr(request, "left") != NULL){
    printf("left\n");
    motor_left(SPEED);
  }
  else if(strstr(request, "up") != NULL){
    printf("up\n");
    motor_forward(SPEED);
  }
  else if(strstr(request, "down") != NULL){
    printf("down\n");
    motor_backward(SPEED);
  }
  else if(strstr(request, "fire") != NULL){
    printf("fire\n");
    motor_stop();
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

  server_fd = socket(AF_INET, SOCK_STREAM, 0);

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  bind(server_fd, (struct sockaddr*)&address, sizeof(address));

  listen(server_fd, 5);

  printf("Server running on port %d\n", PORT);

  signal(SIGINT, cleanup); // Ctrl-C Handler

  while (1) {
    client_fd = accept(server_fd, NULL, NULL);
    read(client_fd, buffer, sizeof(buffer)-1);
    if (strstr(buffer, "User-Agent: HttpShortcuts")) {
      // headless response for shortcuts
      write(client_fd, "HTTP/1.1 200 OK\r\nContent-Length:0\r\n\r\n", 37);
      led_react(buffer);
    }else{
      // full HTML page for browser
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
      led_react(buffer);
    }
    close(client_fd);
  }
  return 0;
}
