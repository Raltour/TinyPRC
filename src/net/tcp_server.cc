#include "TinyRPC/net/tcp_server.h"

#include "TinyRPC/common/console_logger.h"

TcpServer::TcpServer()
  : acceptor_(Acceptor(
      [this](Channel* channel) {
        LOG_DEBUG("Acceptor called listen_callback");
        event_loop_.AddChannel(channel);
      }
      ,
      [this](int connect_fd) {
        fd_connection_map_.insert({
            connect_fd,
            std::make_unique<TcpConnection>(connect_fd)});

        LOG_DEBUG(
            "TcpServer: New TcpConnection with fd: " + std::to_string(connect_fd
            ));
      })) {
  // acceptor_.set_start_listen_callback();

  // acceptor_.set_new_connection_callback([this](int connect_fd) {
  //   fd_connection_map_.insert({
  //       connect_fd,
  //       std::make_unique<TcpConnection>(connect_fd)});
  //
  //   LOG_DEBUG(
  //       "TcpServer: New TcpConnection with fd: " + std::to_string(connect_fd));
  // });
}

void TcpServer::RunLoop() {
  event_loop_.Loop();
}

void TcpServer::AddChannel(Channel* channel) {
  event_loop_.AddChannel(channel);
}