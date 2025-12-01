#include "TinyRPC/net/tcp_server.h"

#include "TinyRPC/common/console_logger.h"

TcpServer::TcpServer() {
  acceptor_.set_start_listen_callback([this](Channel* channel) {
    LOG_DEBUG("Acceptor called listen_callback");
    event_loop_.AddChannel(channel);
  });
}

void TcpServer::RunLoop() {
  event_loop_.Loop();
}

void TcpServer::AddChannel(Channel* channel) {
  event_loop_.AddChannel(channel);
}
