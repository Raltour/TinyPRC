#include "TinyRPC/net/tcp_connection.h"
#include "TinyRPC/common/console_logger.h"


TcpConnection::TcpConnection(int connect_fd, std::function<void()> service,
                             std::function<void(Channel*)>
                             add_connection_callback) {

  channel_ = Channel(connect_fd, true, true);
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


void TcpConnection::HandleRead() {
  service_();
}

void TcpConnection::HandleWrite() {}