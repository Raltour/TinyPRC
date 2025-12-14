#ifndef TINYRPC_RPC_SERVER_H
#define TINYRPC_RPC_SERVER_H

#include "TinyRPC/net/tcp_server.h"
#include <string>

class RpcServer {
public:
  RpcServer();

  void StartServer();

private:
  TcpServer tcp_server;

  void HandleRequest(std::string& request, std::string& response);
};


#endif //TINYRPC_RPC_SERVER_H