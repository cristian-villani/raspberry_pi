#include <stdio.h>             // printf, perror
#include <stdlib.h>            // exit
#include <string.h>            // memset
#include <unistd.h>            // for gethostname()
#include <time.h>              // for random seed
#include <sys/types.h>         // needed for sockets
#include <sys/socket.h>        // socket structures
#include <netdb.h>             // getaddrinfo, struct addrinfo
#include <arpa/inet.h>         // inet_ntop
#include "network_utils.h"     // get_ip_address

#define DEFAULT_HOST "raspberrypi"  // matches /etc/hosts on my PC
#define DEFAULT_PORT "5000"

int humidity_sensor(){
  return 40 + rand()%30;
}

int main(int argc, char *argv[]){

  srand(time(NULL));
  char hostname[128];
  if (gethostname(hostname, sizeof(hostname)) == 0) {
    printf("Hostname: %s\n", hostname);
  } else {
    perror("gethostname failed");
  }

  struct addrinfo *alist;
  struct addrinfo hints;
  struct addrinfo *paddrinfo;

  const char *server = (argc > 1) ? argv[1]: DEFAULT_HOST;
  // The port can be selected only if the server name is given
  const char *port = (argc > 2) ? argv[2]: DEFAULT_PORT;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;      // allow IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM;  // TCP stream
  hints.ai_flags = AI_CANONNAME;    // Get FQDN

  int ret_value = getaddrinfo(server, port, &hints, &alist);
  if(ret_value == 0){
    printf("Got addrinfo information\n");
    paddrinfo = alist;



  while(1){
    while( paddrinfo != NULL){
      print_address_info(paddrinfo);
      struct sockaddr *address = paddrinfo->ai_addr;
      int sockfd = socket(paddrinfo->ai_family, paddrinfo->ai_socktype,
                          paddrinfo->ai_protocol);
      printf("socket: %d\n", sockfd);
      ret_value = connect(sockfd, address, paddrinfo->ai_addrlen);
      if(ret_value == 0){
        printf("Connection established, success!\n");
        int sensor_value = humidity_sensor();
        printf("Sending sensor value: %d%%\n", sensor_value);
        char buffer[128];
        snprintf(buffer, sizeof(buffer), "humidity: %d%%", sensor_value);
        send(sockfd, hostname, strlen(hostname)+1, 0);
        send(sockfd, buffer, strlen(buffer)+1, 0);
        sleep(5);
        break;
      }
      else{
        printf("Connection failed.\n");
      }
      paddrinfo = paddrinfo->ai_next;
    }
  }



  }
  else{
    printf("Could not get address information, program exits abnormally\n");
    freeaddrinfo(alist);
    exit(-1);
  }
  freeaddrinfo(alist);
}

