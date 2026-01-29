#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include"network_utils.h"

#define DEFAULT_PORT 5000
#define MAX_SIZE 100

int main(){
  // AF_INET     -> IPv4
  // SOCK_STREAM -> TCP
  // 0           -> default protocol (TCP)
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(server_fd < 0){
    perror("socket failed");
    exit(EXIT_FAILURE);
  }
  printf("Socket created successfully!\n");
  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;            // IPv4
  server_addr.sin_addr.s_addr = INADDR_ANY;    // all local interfaces
  server_addr.sin_port = htons(DEFAULT_PORT);
  if (bind(server_fd,
         (struct sockaddr *)&server_addr,
         sizeof(server_addr)) < 0) {
    perror("bind failed");
    close(server_fd);
    return 1;
  }
  printf("Socket bound to port %d\n", DEFAULT_PORT);
  if (listen(server_fd, 5) < 0) {
    perror("listen failed");
    close(server_fd);
    return 1;
  }
  printf("Server listening on port %d\n", DEFAULT_PORT);

  FILE *pfile = fopen("sensor_log.csv", "a+");
  if(pfile != NULL){
    long position = ftell(pfile);
    if(position < 0){
      perror("ftell failed: impossible to determine position in file");
    }
    else if(position == 0L){
      // Write CSV file header
      fprintf(pfile, "timestamp, node, signal\n");
    }
    char node[128];
    char sensor_info[128];
    time_t t;
    struct tm *now;
    char buffer[MAX_SIZE];
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd;
    while(1){
      client_fd = accept(server_fd,(struct sockaddr *)&client_addr,&client_len);
      if(client_fd < 0){
        perror("accept failed");
        continue;
      }
      recv(client_fd, node, sizeof(node), 0);
      recv(client_fd, sensor_info, sizeof(sensor_info), 0);
      printf("Client connected!\n");
      print_sockaddr_info(&client_addr);
      printf("Hostname: %s\n", node);
      printf("Sensor data: %s\n", sensor_info);
      t = time(NULL);
      now = localtime(&t);
      strftime(buffer, MAX_SIZE, "%Y-%m-%d %H:%M:%S", now);
      fprintf(pfile,"%s, %s, %s\n", buffer, node, sensor_info);
      fflush(pfile);
      sleep(1);

      close(client_fd); // done with this client
    }
    // fclose(pfile); 
  }
  return 0;
}
