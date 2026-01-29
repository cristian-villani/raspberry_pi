#include <stdio.h>          // printf, perror
#include <sys/types.h>      // needed for sockets
#include <sys/socket.h>     // socket structures
#include <arpa/inet.h>      // inet_ntop
#include "network_utils.h"


char *get_ip_address(struct addrinfo *ai, char *ipstr);
/*
  struct addrinfo {
    int              ai_flags;
    int              ai_family;
    int              ai_socktype;
    int              ai_protocol;
    socklen_t        ai_addrlen;
    struct sockaddr *ai_addr;
    char            *ai_canonname;
    struct addrinfo *ai_next;
  };
 */

char *get_ip_address(struct addrinfo *ai, char *ipstr) {
  // We assume IPv4 only
  if (ai->ai_family != AF_INET) {
    printf("Not an IPv4 address, skipping\n");
    return NULL;
  }
  struct sockaddr_in *ipv4 = (struct sockaddr_in *)ai->ai_addr;
  // char ipstr[INET_ADDRSTRLEN];  // enough for IPv4
  if (inet_ntop(AF_INET, &(ipv4->sin_addr), ipstr, INET_ADDRSTRLEN) == NULL) {
    perror("inet_ntop");
    return NULL;
  }
  return ipstr;
}

void print_address_info(struct addrinfo *paddrinfo){
  char ipstr[INET_ADDRSTRLEN];
  int flags = paddrinfo-> ai_flags;
  int family = paddrinfo-> ai_family;
  int socktype = paddrinfo-> ai_socktype;
  int protocol = paddrinfo-> ai_protocol;
  char *canonname = paddrinfo-> ai_canonname ?
                                    paddrinfo-> ai_canonname: "NULL";
  printf("=======================\n");
  printf("Printing Information:\n");
  printf("=======================\n");
  printf("flags: %d\n", flags);
  printf("family: %d\n", family);
  printf("socktype: %d\n", socktype);
  printf("protocol: %d\n", protocol);
  printf("address: %s\n", get_ip_address(paddrinfo, ipstr));
  printf("canonname: %s\n", canonname);
}

void print_sockaddr_info(struct sockaddr_in *client_addr){
  printf("Client connected from: %s, Port: %d\n",
  inet_ntoa(client_addr->sin_addr),
  ntohs(client_addr->sin_port));
}
