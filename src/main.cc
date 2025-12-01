#include "TinyRPC/common/console_logger.h"
#include "TinyRPC/common/config.h"

int main() {
  LOG_INFO("Start Server");

  Config::GetInstance();

  return 0;
}