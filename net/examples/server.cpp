#include <iostream>

#include "../sock_ops.h"



int init_server(uint16_t port) {

    InetAddress addr(port);

    int listen_fd = create_tcp_sockfd(addr.family());
    if (listen_fd < 0) {
        std::cout << "Failed create_nb_tcp_sockfd()" << std::endl;
    }
    int ret = -1;
    ret = bind_sockfd(listen_fd, addr);
    if (ret < 0) {
        std::cout << "Failed bind_sockfd()" << std::endl;
    }
    ret = listen_on_sockfd(listen_fd);
    if (ret < 0) {
        std::cout << "Failed listen_on_sockfd()" << std::endl;
    }
    
    std::cout << "Accepting on listener socket: " << std::endl;
    InetAddress client_addr;
    int client_sockfd = accept_from_sockfd(listen_fd, NULL);
    if (client_sockfd < 0) {
        std::cout << "Failed accept_from_sockfd()" << std::endl;
    }

    return ret;
}

int main(int argc, char **argv) {

    int ret = init_server(9997);
}