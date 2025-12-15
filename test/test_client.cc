#include <assert.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <strings.h>
#include <cstring>
#include <unistd.h>

#include "TinyRPC/protocol/request.pb.h"
#include "TinyRPC/protocol/add.pb.h"
#include "TinyRPC/net/codec.h"

int main() {
  const char* ip = "127.0.0.1";
  int port = 12345;

  struct sockaddr_in server_address;
  bzero(&server_address, sizeof(server_address));
  server_address.sin_family = AF_INET;
  inet_pton(AF_INET, ip, &server_address.sin_addr);
  server_address.sin_port = htons(port);

  int sockfd = socket(PF_INET, SOCK_STREAM, 0);
  assert(sockfd >= 0);

  if (connect(sockfd, (struct sockaddr*)&server_address,
              sizeof(server_address)) < 0) {
    printf("error!\n");
  }

  int arg1 = 5;
  int arg2 = 6;
  rpc::Request request;
  request.set_request_id(1);
  request.set_method_name("add");
  rpc::AddMethod add_method;
  add_method.set_arg1(arg1);
  add_method.set_arg2(arg2);
  add_method.set_ret1(0);
  request.set_method_args(add_method.SerializeAsString());
  std::string message = request.SerializeAsString();

  std::string encoded_message = Codec::encode(message);
  send(sockfd, encoded_message.c_str(), encoded_message.size(), 0);
  char buffer[1024];
  int read_size = recv(sockfd, buffer, 1024, 0);
  buffer[read_size] = '\0';
  std::string response = std::string(buffer + 4);

  rpc::AddMethod response_method;
  response_method.ParseFromString(response);

  std::cout << "Received from server: " << response_method.ret1() << std::endl;

  close(sockfd);
  return 0;
}