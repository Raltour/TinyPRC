#ifndef TINYRPC_ACCEPTOR_H
#define TINYRPC_ACCEPTOR_H

#include "TinyRPC/net/event_loop.h"

class Acceptor {
public:
  Acceptor(const char* ip, int port);
  void set_new_connection_callback(std::function<int()> callback);
  void listen_loop();

private:
  int listenfd_;
  std::function<int()> new_connection_callback_;

  Channel* listen_channel();
};


#endif //TINYRPC_ACCEPTOR_H