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

void add(int arg1, int arg2, int& ret1) {
  ret1 = arg1 + arg2;
}

void RpcServer::HandleRequest(std::string& request, std::string& response) {
  rpc::Request request_prototype;
  request_prototype.ParseFromString(request);
  rpc::AddMethod add_method;
  add_method.ParseFromString(request_prototype.method_args());
  int ret1;
  add(add_method.arg1(), add_method.arg2(), ret1);
  rpc::AddMethod AddResponse;
  AddResponse.set_arg1(0);
  AddResponse.set_arg2(0);
  AddResponse.set_ret1(ret1);
  AddResponse.SerializeToString(&response);
}

