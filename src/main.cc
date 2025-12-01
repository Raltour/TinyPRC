#include "TinyRPC/common/console_logger.h"
#include "TinyRPC/common/config.h"
#include "TinyRPC/net/tcp_server.h"
#include <iostream>

int main() {
  std::cout << "Hello, World!" << std::endl;
  LOG_INFO("Start Server");

  Config& configer = Config::GetInstance();

  TcpServer server(configer.server_host_.c_str(), configer.server_port_);

  return 0;
}