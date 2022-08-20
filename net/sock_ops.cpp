 #include <arpa/inet.h>   // struct sockaddr, struct sockaddr_in, struct sockaddr_in6
 #include <sys/socket.h>  // bind(), listen(), accept(), connect()
 #include <fcntl.h>       // fcntl
 #include <errno.h>       // errno, EAGAIN
 #include <iostream>

#include "inet_addr.h"

// TODO: better error handling, e.g., exit the process when error happens ?

// Create non-blocking TCP socket fd
int create_tcp_sockfd(sa_family_t family, bool blocking) {
    
    // sock_fd < 0 if error happens
    int type = 0;
    if (blocking) {
        type = SOCK_STREAM | SOCK_CLOEXEC;
    } else {
        type = SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC;
    }
    int sock_fd = ::socket(family, type, IPPROTO_TCP);
    return sock_fd;
}

int bind_sockfd(int sockfd, const InetAddress &addr)
{
    // WARN: why use sizeof(sockaddr_in6) no matter what type the addr is ?
    // ret < 0 if error happens
    int ret = ::bind(sockfd, addr.sockaddr(), static_cast<socklen_t>(sizeof(struct sockaddr_in6)));
    return ret;
}

int listen_on_sockfd(int sockfd)
{
    int ret = ::listen(sockfd, SOMAXCONN);
    return ret;
}

int set_nonblk_cloexec_flags(int sockfd)
{
  // non-block
  int flags = ::fcntl(sockfd, F_GETFL, 0);
  flags |= O_NONBLOCK;
  int ret = ::fcntl(sockfd, F_SETFL, flags);
  // FIXME check
  if (ret < 0)
    return ret;

  // close-on-exec
  flags = ::fcntl(sockfd, F_GETFD, 0);
  flags |= FD_CLOEXEC;
  ret = ::fcntl(sockfd, F_SETFD, flags);
  // FIXME check

  return ret;
}

int accept_from_sockfd(int sockfd, InetAddress *paddr)
{

    socklen_t addrlen = 0;
    socklen_t *plen = nullptr;
    if (paddr != nullptr) {
        addrlen = static_cast<socklen_t>(sizeof(*paddr));
        plen = &addrlen;
    }

    int connfd = ::accept(sockfd, static_cast<struct sockaddr *>(implicit_cast<void*>(paddr)), plen);
    set_nonblk_cloexec_flags(connfd);
    
    std::cout << sockfd << " accept return fd " << connfd << std::endl;
    if (connfd < 0)
    {
        int savedErrno = errno;  // errno is thread-local
        
        switch (savedErrno)
        {
        case EAGAIN:
            std::cout << "accept return EAGAIN" << savedErrno << std::endl;
            break;
        case ECONNABORTED:
        case EINTR:
        case EPROTO: // ???
        case EPERM:
        case EMFILE: // per-process lmit of open file desctiptor ???
            // expected errors
            std::cout << "accept return expected errors" << savedErrno << std::endl;
            break;
        case EBADF:  // accepting on nonblocking socket set errno=9 when no incomming connection.
        case EFAULT:
        case EINVAL:
        case ENFILE:
        case ENOBUFS:
        case ENOMEM:
        case ENOTSOCK:
        case EOPNOTSUPP:
            // unexpected errors
            std::cout << "accept return unexpected errors " << savedErrno << std::endl;
            break;
        default:
            break;
        }
    }
    return connfd;
}

int connect_to_svr(int cli_sockfd, const InetAddress &svr_addr) {

    int ret = ::connect(cli_sockfd, svr_addr.sockaddr(), static_cast<socklen_t>(sizeof(struct sockaddr_in6)));
    if (ret < 0) {
        std::cout << "connect_svr_sock failed" << std::endl; 
    }
    return ret;
}