#include "TinyRPC/rpc/rpc_server.h"

int main() {
  RpcServer rpc_server;

  rpc_server.StartServer();

  return 0;
}