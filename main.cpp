#include <iostream>
#include <string>

#ifdef BUILD_CLIENT
#include "client/client.hpp"
#elif defined(BUILD_SERVER)
#include "server/server.hpp"
#endif

int main(int argc, char* argv[]) {
#ifdef BUILD_CLIENT
  std::string server_ip = "server";
  int port = 8080;
  if (argc >= 2) {
    server_ip = argv[1];
  }
  if (argc >= 3) {
    port = std::stoi(argv[2]);
  }
  Client client(server_ip, port);
  client.request_process_list();
  return 0;
#elif defined(BUILD_SERVER)
  int port = 8080;
  if (argc >= 2) {
    port = std::stoi(argv[1]);
  }
  Server server(port);
  server.run();
  return 0;
#endif
}