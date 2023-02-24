#include <sys/socket.h>  // bind(), listen(), accept(), connect()
#include <netinet/in.h>  // INADDR_ANY, INADDR_LOOPBACK, in6addr_any, in6addr_loopback
#include <arpa/inet.h>   // htons(), htonl()
#include "inet_addr.h"

InetAddress::InetAddress(uint16_t port, bool loopback_only, bool ipv6) {

    if (ipv6)
    {
        mem_zero(&this->addr_in6, sizeof this->addr_in6);
        addr_in6.sin6_family = AF_INET6;
        // in6_addr is a struct
        in6_addr ip = loopback_only ? in6addr_loopback : in6addr_any;
        addr_in6.sin6_addr = ip;
        addr_in6.sin6_port = ::htons(port);
    }
    else
    {
        mem_zero(&this->addr_in, sizeof this->addr_in);
        addr_in.sin_family = AF_INET;
        in_addr_t ip = loopback_only ? INADDR_LOOPBACK : INADDR_ANY;
        addr_in.sin_addr.s_addr = ::htonl(ip);
        addr_in.sin_port = ::htons(port);
    }
}