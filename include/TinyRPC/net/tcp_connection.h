#ifndef TINYRPC_TCP_CONNECTION_H
#define TINYRPC_TCP_CONNECTION_H

#include <string>

#include "TinyRPC/net/channel.h"

class TcpConnection {
public:
  TcpConnection(int connect_fd, std::function<void()> service,
                std::function<void(Channel*)> add_connection_callback);

  TcpConnection() = delete;

  ~TcpConnection();

private:
  Channel channel_;

  const int max_buffer_size = 1024;
  char* read_buffer_;
  char* write_buffer_;

  void HandleRead();

  void HandleWrite();

  std::function<void()> service_;
  std::function<void(Channel*)> add_connection_callback_;

};


#endif //TINYRPC_TCP_CONNECTION_H