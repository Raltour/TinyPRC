#ifndef PHOTONRPC_RPC_SERVER_H
#define PHOTONRPC_RPC_SERVER_H

#include <google/protobuf/service.h>

#include <string>
#include "photonrpc/rpc_message.pb.h"
#include "../net/tcp_server.h"

// TODO: implement a tcp_client to maintain the tcp connection for better performance
class RpcServer {
 public:
  RpcServer();

  ~RpcServer() = default;

  void StartServer();

  void ServiceRegister(std::unique_ptr<google::protobuf::Service>);

 private:
  TcpServer tcp_server_;

  void HandleRequest(std::string& request, std::string& response);

  bool CheckRequest(rpc::RpcMessage request);

  std::map<std::string, std::unique_ptr<google::protobuf::Service>> service_map_;
};

#endif  //PHOTONRPC_RPC_SERVER_H