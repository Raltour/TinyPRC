#include "TinyRPC/rpc/rpc_server.h"
#include "TinyRPC/common/console_logger.h"
#include "TinyRPC/protocol/request.pb.h"
#include "TinyRPC/protocol/add.pb.h"

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

std::string add(std::string arguments) {
  rpc::AddMethod add_method;
  add_method.ParseFromString(arguments);
  std::string response;
  add_method.set_ret1(add_method.arg1() + add_method.arg2());
  add_method.SerializeToString(&response);
  return response;
}

void RpcServer::HandleRequest(std::string& request, std::string& response) {
  rpc::Request request_prototype;
  request_prototype.ParseFromString(request);

  response = add(request_prototype.method_args());
}

