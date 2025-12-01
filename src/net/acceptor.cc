#define MAX_LISTEN = 64

#include "TinyRPC/net/acceptor.h"
#include "TinyRPC/common/console_logger.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

Acceptor::Acceptor(const char* ip, int port) {
  struct sockaddr_in address;
  bzero(&address, sizeof(address));
  address.sin_family = AF_INET;
  inet_pton(AF_INET, ip, &address.sin_addr);
  address.sin_port = htons(port);

  this->listenfd_ = socket(PF_INET, SOCK_STREAM, 0);
  if (listenfd_ < 0) {
    LOG_ERROR("create listen_fd failure");
    return;
  }

  int ret = bind(listenfd_, (struct sockaddr *) &address, sizeof(address));
  if (ret < 0) {
    LOG_ERROR("bind listen_fd failure");
    return;
  }

}

void Acceptor::set_new_connection_callback(std::function<int()> callback) {
  new_connection_callback_ = callback;
}

void Acceptor::listen_loop() {
  while (true) {
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int connfd = accept(listenfd_, (struct sockaddr *) &client_addr, &client_addr_len);
    if (connfd < 0) {
      LOG_ERROR("accept failure");
    }

    new_connection_callback_();
  }
}