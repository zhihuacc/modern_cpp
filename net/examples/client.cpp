#include "../sock_ops.h"
#include <iostream>
#include <unistd.h>  // sleep()

int connect_retry(uint16_t port, int times) {

    InetAddress addr(port);

    int cli_fd = create_tcp_sockfd(addr.family());
    if (cli_fd < 0) {
        std::cout << "Failed create_nb_tcp_sockfd()" << std::endl;
        return -1;
    }

    int fd = -1;
    for (int i = 0; i < times; ++i) {

        fd = connect_to_svr(cli_fd, addr);
        if (fd < 0) {
            ::sleep(1);
        }
    }
    return fd;
}

int main(int argc, char **argv) {

    int cli_fd = connect_retry(9997, 5);
}