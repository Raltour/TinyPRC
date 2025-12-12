#include "TinyRPC/net/tcp_connection.h"

#include <sys/socket.h>

#include "TinyRPC/common/console_logger.h"


TcpConnection::TcpConnection(int connect_fd, std::function<void()> service,
                             std::function<void(Channel*)>
                             add_connection_callback)
  : service_(service) {
  read_buffer_ = new char[max_buffer_size];
  write_buffer_ = new char[max_buffer_size];

  channel_ = Channel(connect_fd, true, false);
  channel_.set_handle_read([this] {
    LOG_DEBUG("TcpConnection HandleRead called");
    this->HandleRead();
  });
  channel_.set_handle_write([this] {
    LOG_DEBUG("TcpConnection HandleWrite called");
    this->HandleWrite();
  });
  this->add_connection_callback_ = add_connection_callback;
  LOG_DEBUG("TcpConnection called add_connection_callback");
  add_connection_callback_(&channel_);

}

TcpConnection::~TcpConnection() {
  free(write_buffer_);
  free(read_buffer_);
}


void TcpConnection::HandleRead() {
  recv(channel_.event()->data.fd, read_buffer_, max_buffer_size, 0);
  LOG_DEBUG("TcpConnection received data");
  service_();
}

void TcpConnection::HandleWrite() {}