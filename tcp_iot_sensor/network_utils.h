#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <netdb.h>

// Functions
char *get_ip_address(struct addrinfo *ai, char *ipstr);
void print_address_info(struct addrinfo *paddrinfo);
void print_sockaddr_info(struct sockaddr_in *addr);

#endif
