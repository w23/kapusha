#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include "Socket.h"

namespace kapusha {
inline void sockaddr_to_Address(const sockaddr_in &sockaddr, Socket::Address &addr) {
  addr.host = ntohl(sockaddr.sin_addr.s_addr);
  addr.port = ntohs(sockaddr.sin_port);
}

inline void Address_to_sockaddr(const Socket::Address &addr, sockaddr_in &sockaddr) {
  memset(&sockaddr, 0, sizeof(sockaddr));
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = htonl(addr.host);
  sockaddr.sin_port = htons(addr.port);
}

Socket::Address::Address(int _port) {
  host = INADDR_ANY;
  port = _port;
}

Socket::Address::Address(const char *_host, int _port) {
  host = ntohl(inet_addr(_host));
  port = _port;
}

Socket::Socket(Type type) {
  socket_ = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (socket_ < 0) {
    L("SOCKET FATAL: Failed to create socket: %d", errno);
    abort();
  }
  fcntl(socket_, F_SETFL, O_NONBLOCK);
}

Socket::~Socket() {
  close(socket_);
}

void Socket::bind(Address addr) {
  sockaddr_in local_addr;
  Address_to_sockaddr(addr, local_addr);
  if (0 > ::bind(socket_, (sockaddr*)&local_addr, sizeof(local_addr))) {
    L("SOCKET FATAL: Failed to bind socket: %d", errno);
    abort();
  }
}

void Socket::send_to(Address addr, const void *data, u32 size) {
  sockaddr_in to;
  Address_to_sockaddr(addr, to);
  //L("SOCKET INFO: send %d bytes", size);
  ssize_t sent = ::sendto(socket_, data, size, 0, (sockaddr*)&to, sizeof(to));
  if (static_cast<u32>(sent) != size) {
    /// \todo this is actually recoverable
    L("SOCKET FATAL: Could only send %d bytes of %d: %d", sent, size, errno);
    abort();
  }
}

u32 Socket::recv_from(Address &addr, void *data, u32 max_size) {
  sockaddr_in from;
  socklen_t addrlen = sizeof(from);
  ssize_t size = ::recvfrom(socket_, data, max_size, 0, (sockaddr*)&from, &addrlen);
  if (size == -1) {
    /// \todo assert errno == EWOULDBLOCK || ???
    return 0;
  }
  sockaddr_to_Address(from, addr);
  //L("SOCKET INFO: recv %d bytes", size);
  return static_cast<u32>(size);
}
} // namespace kapusha
