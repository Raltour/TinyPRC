#include "TinyRPC/net/tcp_server.h"

#include "TinyRPC/common/console_logger.h"



TcpServer::TcpServer(std::function<void()> Service)
  : reactor_read_(Service), reactor_write_(Service) {

  acceptor_.set_start_listen_callback([this](Channel* channel) {
              LOG_DEBUG("Acceptor called listen_callback");
              event_loop_.AddChannel(channel);
            }
            );


  acceptor_.set_new_connection_callback(
      [this, Service](int connect_fd) {
        fd_connection_map_.insert({
            connect_fd,
            std::make_unique<TcpConnection>(
                connect_fd, Service, Service,
                [this](Channel* channel) {
                  event_loop_.AddChannel(channel);
                })});

        LOG_DEBUG(
            "TcpServer: New TcpConnection with fd: " + std::to_string(
              connect_fd
            ));
      });

  acceptor_.StartListen();
}

void TcpServer::RunLoop() {
  event_loop_.Loop();
}

void TcpServer::AddChannel(Channel* channel) {
  event_loop_.AddChannel(channel);
}