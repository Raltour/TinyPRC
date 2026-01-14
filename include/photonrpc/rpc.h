#ifndef PHOTONRPC_RPC_H
#define PHOTONRPC_RPC_H

#include "../src/core/rpc/rpc_channel.h"

#include <google/protobuf/service.h>

class RpcChannel;

class RpcServer {
 public:
  RpcServer();

  void StartServer();

  // void ServiceRegister(std::unique_ptr<google::protobuf::Service>);
  void ServiceRegister(google::protobuf::Service*);
};

#endif  //PHOTONRPC_RPC_H
