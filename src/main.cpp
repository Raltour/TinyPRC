#include "TinyRPC/common/ConsoleLogger.h"
#include "TinyRPC/common/Config.h"
#include <iostream>

int main() {
  std::cout << "Hello, World!" << std::endl;
  LOG_INFO("Start Server");

  Config::get_instance();

  return 0;
}