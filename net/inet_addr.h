 #ifndef INET_ADDR_H
 #define INET_ADDR_H

 #include <arpa/inet.h>   // struct sockaddr, struct sockaddr_in, struct sockaddr_in6
 #include "../utils.h"    // implicit_cast

// struct in_addr {
//   in_addr_t       s_addr;
// };

// struct sockaddr_in {
//   sa_family_t    sin_family;
//   in_port_t      sin_port;
//   struct in_addr sin_addr;
// };

// struct in6_addr {
//      uint8_t        s6_addr[16];    /* IPv6 address */
// };

// struct sockaddr_in6 {
//      sa_family_t
//      in_port_t
//      uint32_t
//      struct in6_addr sin6_addr;      /* IPv6 address */
//      uint32_t        sin6_scope_id;  /* set of interfaces for scope */
// };

// sockaddr_in and sockaddr_in6 can be cast to sockaddr


 class InetAddress {
    
private:
    // Only for Linux
    // InetAdress has same layout as sockaddr_in/sockaddr_in6
    union {
        struct sockaddr_in  addr_in;
        struct sockaddr_in6 addr_in6;
    };

public:
    
    InetAddress(uint16_t port = 0, bool loopbackOnly = false, bool ipv6 = false);
    // sockaddr_in and sockaddr_in6 have the same layout, so addr_in.sin_family == addr_in6.sin_family
    sa_family_t family() const { return addr_in.sin_family;}
    const struct sockaddr *sockaddr() const {return static_cast<const struct sockaddr*>(implicit_cast<const void*>(&addr_in6));}
 };

 #endif


