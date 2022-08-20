 #ifndef SOCK_OPS_H
 #define SOCK_OPS_H

 #include <arpa/inet.h>   // struct sockaddr, struct sockaddr_in, struct sockaddr_in6
 #include <sys/socket.h>  // bind(), listen(), accept(), connect()

#include "inet_addr.h"       // InetAddress

int create_tcp_sockfd(sa_family_t family, bool blocking=true);
int bind_sockfd(int sockfd, const InetAddress &addr);
int listen_on_sockfd(int sockfd);
int set_nonblk_cloexec_flags(int sockfd);
int accept_from_sockfd(int sockfd, InetAddress *paddr);
int connect_to_svr(int cli_sockfd, const InetAddress &svr_addr);

#endif

