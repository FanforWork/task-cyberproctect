#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client {
 public:
  Client(const std::string& server_ip, int port);
  void request_process_list();

 private:
  std::string server_ip_;
  int port_;
};

#endif  // CLIENT_HPP