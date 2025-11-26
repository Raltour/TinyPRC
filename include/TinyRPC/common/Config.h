#ifndef TINYRPC_CONFIG_H
#define TINYRPC_CONFIG_H

#include <string>

class Config {
public:
  Config();

  std::string server_host_;
  int server_port_;
};


#endif //TINYRPC_CONFIG_H