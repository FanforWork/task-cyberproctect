#include "server.hpp"

#include <arpa/inet.h>
#include <dirent.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <sstream>

ProcessInfo::ProcessInfo(const std::string& pid, const std::string& name)
    : pid_(pid), name_(name) {}

std::vector<ProcessInfo> ProcessMonitor::get_process_list() {
  std::vector<ProcessInfo> processes;
  DIR* proc_dir = opendir("/proc");
  if (!proc_dir) {
    perror("opendir failed, server.cpp:17");
    return processes;
  }
  struct dirent* entry;
  while ((entry = readdir(proc_dir)) != nullptr) {
    if (isdigit(*entry->d_name)) {
      std::string pid = entry->d_name;
      std::string cmdline = get_cmd_line(pid);
      processes.emplace_back(pid, cmdline);
    }
  }
  closedir(proc_dir);
  return processes;
}

std::string ProcessMonitor::get_cmd_line(const std::string& pid) {
  std::ifstream cmdline_file("/proc/" + pid + "/cmdline");
  std::string cmdline;
  std::getline(cmdline_file, cmdline, '\0');
  if (!cmdline.empty()) {
    return cmdline;
  } else {
    std::ifstream comm_file("/proc/" + pid + "/comm");
    std::string comm;
    if (std::getline(comm_file, comm)) {
      return comm;
    } else {
      return "unknown process";
    }
  }
}

Server::Server(int port) : port_(port), server_fd_(-1) {}

void Server::run() {
  signal(SIGINT, Server::signal_handler);
  server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd_ == 0) {
    perror("socket failed, server.cpp:56");
    exit(EXIT_FAILURE);
  }
  sockaddr_in address{};
  int opt = 1;
  if (setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    perror("setsockopt failed, server.cpp:63");
    exit(EXIT_FAILURE);
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port_);
  if (bind(server_fd_, (sockaddr*)&address, sizeof(address)) < 0) {
    perror("bind failed, server.cpp:70");
    exit(EXIT_FAILURE);
  }
  if (listen(server_fd_, 3) < 0) {
    perror("listen failed, server.cpp:74");
    exit(EXIT_FAILURE);
  }
  std::cout << "~~~~~~~~~| server is running on port " << port_ << std::endl;
  while (true) {
    int client_fd;
    sockaddr_in client_address{};
    socklen_t addrlen = sizeof(client_address);
    if ((client_fd = accept(server_fd_, (sockaddr*)&client_address, &addrlen)) <
        0) {
      perror("accept failed, server.cpp:83");
      continue;
    }
    std::cout << "~~~~~~~~~| accepted connection from "
              << inet_ntoa(client_address.sin_addr) << ":"
              << ntohs(client_address.sin_port) << std::endl;
    client_handler(client_fd);
    close(client_fd);
  }
}

void Server::client_handler(int client_fd) {
  char buffer[1024] = {0};
  read(client_fd, buffer, 1024);
  std::string command(buffer);
  if (command == "GET_PROCESS_LIST") {
    ProcessMonitor monitor;
    auto processes = monitor.get_process_list();
    std::ostringstream oss;
    for (const auto& proc : processes) {
      oss << "PID: " << proc.pid_ << ", CMD: " << proc.name_ << "\n";
    }
    std::string response = oss.str();
    send(client_fd, response.c_str(), response.size(), 0);
  } else {
    std::string response = "invalid command\n";
    send(client_fd, response.c_str(), response.size(), 0);
  }
}

void Server::stop() {
  if (server_fd_ != -1) {
    close(server_fd_);
  }
}

void Server::signal_handler(int signum) {
  std::cout << "interrupt signal (" << signum << ") pop out.\n";
  exit(signum);
}