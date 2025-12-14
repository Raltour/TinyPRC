#include "TinyRPC/rpc/rpc_server.h"
#include "TinyRPC/common/console_logger.h"

RpcServer::RpcServer() : tcp_server(
    [this](std::string& read, std::string& write) {
      LOG_DEBUG("Reactor Service");
      this->HandleRequest(read, write);
    }
    ) {}

void RpcServer::StartServer() {
  LOG_DEBUG("Start Server");
  tcp_server.RunLoop();
}

void RpcServer::HandleRequest(std::string& request, std::string& response) {
  response = request;
}