#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>

class ProcessInfo {
 public:
  std::string pid_;
  std::string name_;

  ProcessInfo(const std::string& pid, const std::string& name);
};

class ProcessMonitor {
 public:
  std::vector<ProcessInfo> get_process_list();

 private:
  std::string get_cmd_line(const std::string& pid);
};

class Server {
 public:
  Server(int port);
  void run();
  void stop();

 private:
  int port_;
  int server_fd_;

  void client_handler(int client_fd);
  static void signal_handler(int signum);
};

#endif  // SERVER_HPP