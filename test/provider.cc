#include "../include/photonrpc/rpc.h"
#include "calculate_service.pb.h"
#include "echo_service.pb.h"

#include <csignal>
#include <memory>

// Global pointer for signal handler
RpcServer* g_rpc_server = nullptr;

void SignalHandler(int signal) {
  if (g_rpc_server) {
    delete g_rpc_server;
    g_rpc_server = nullptr;
  }
  exit(0);
}

class CalculateServiceImpl : public rpc::CalculateService {
 public:
  void Add(google::protobuf::RpcController* controller,
           const rpc::AddRequest* request, rpc::AddResponse* response,
           google::protobuf::Closure* done) override {
    response->set_result(request->a() + request->b());
  }

  void Sub(google::protobuf::RpcController* controller,
           const rpc::SubRequest* request, rpc::SubResponse* response,
           google::protobuf::Closure* done) override {
    response->set_result(request->a() - request->b());
  }
};

class EchoServiceImpl : public rpc::EchoService {
 public:
  void Echo(google::protobuf::RpcController* controller,
            const rpc::EchoRequest* request, rpc::EchoResponse* response,
            google::protobuf::Closure* done) override {
    response->set_result(request->sentence());
  }
};

int main() {
  RpcServer rpc_server;

  // rpc_server.ServiceRegister(std::make_unique<EchoServiceImpl>());
  // rpc_server.ServiceRegister(std::make_unique<CalculateServiceImpl>());

  auto cal_service = new CalculateServiceImpl();
  auto echo_service = new EchoServiceImpl();
  rpc_server.ServiceRegister(echo_service);
  rpc_server.ServiceRegister(cal_service);

  rpc_server.StartServer();

  delete cal_service;
  delete echo_service;

  return 0;
}