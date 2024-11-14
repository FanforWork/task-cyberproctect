#include "client.hpp"

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

Client::Client(const std::string& server_ip, int port)
    : server_ip_(server_ip), port_(port) {}

void Client::request_process_list() {
  int fd = 0;
  char buffer[4096] = {0};
  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket failed, client.cpp:16");
    return;
  }
  struct addrinfo hints{}, *res;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  std::string port_str = std::to_string(port_);
  int status = getaddrinfo(server_ip_.c_str(), port_str.c_str(), &hints, &res);
  if (status != 0) {
    fprintf(stderr, "error while getaddrinfo: %s\n", gai_strerror(status));
    close(fd);
    return;
  }

  if (connect(fd, res->ai_addr, res->ai_addrlen) < 0) {
    perror("connection failed, client.cpp:32");
    freeaddrinfo(res);
    close(fd);
    return;
  }
  freeaddrinfo(res);
  std::string command = "GET_PROCESS_LIST";
  send(fd, command.c_str(), command.size(), 0);
  int valread = read(fd, buffer, sizeof(buffer) - 1);
  if (valread > 0) {
    buffer[valread] = '\0';
    std::cout << buffer << std::endl;
  } else {
    perror("error occured while reading, client.cpp:41");
  }
  close(fd);
}