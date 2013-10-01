#pragma once
#include <kapusha/core.h>

namespace kapusha {
  class Socket {
  public:
    struct Address {
      u32 host; // yay ipv4 only!
      u16 port;

      Address() : host(0), port(0) {}
      Address(int port);
      Address(const char *host, int port);
      inline bool operator==(const Address &right) {
        return host == right.host && port == right.port;
      }
      inline bool operator!=(const Address &right) {
        return host != right.host || port != right.port;
      }
    };
    
    enum Type {
      UDP
    };

    Socket(Type type = UDP);
    ~Socket();

    void bind(Address addr);
    void send_to(Address addr, const void *data, u32 size);
    u32 recv_from(Address &addr, void *data, u32 max_size);

  private:
    int socket_;
  };

} // namespace kapusha