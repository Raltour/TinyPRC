#include "TinyRPC/common/console_logger.h"
#include "TinyRPC/net/tcp_server.h"

int main() {
  LOG_INFO("Start Server");

  TcpServer server(
    [] () {
      LOG_DEBUG("Reactor Service");
    }
  );

  server.RunLoop();

  return 0;
}