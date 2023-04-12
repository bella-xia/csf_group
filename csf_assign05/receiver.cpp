#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

void error (const char *msg) {
  std::cout << msg << std::endl;
  exit(1);
}
int main(int argc, char **argv) {
  if (argc != 5) {
    std::cerr << "Usage: ./receiver [server_address] [port] [username] [room]\n";
    return 1;
  }
  std::string port = argv[2];
  std::string server_hostname = argv[1];
  int server_port = std::stoi(argv[2]);
  std::string username = argv[3];
  std::string room_name = argv[4];
  Connection conn;
  int fd = open_clientfd(server_hostname.c_str(), port.c_str());
  if (fd < 0) {
    error("Error: could not connect to server.");
  }
  // TODO: send rlogin and join messages (expect a response from
  //       the server for each one)
  
  std::string rlogin_message = "rlogin:" + username + "\n";
  std::string join_message = "join:" + room_name;
  rio_writen(fd, rlogin_message.c_str(), sizeof(rlogin_message));
  rio_t rio;
  rio_readinitb(&rio, fd);
  char buf[Message::MAX_LEN + 1];
  ssize_t n = rio_readlineb(&rio, buf, Message::MAX_LEN);
  buf[n] = '\0';
  if (n <= 0) {
    error("Error: not reived from server.");
  } else if (n >= 4 && ((std::string) buf).substr(0, 4) == "err:") {
    error(buf);
  }
  rio_writen(fd, join_message.c_str(), sizeof(join_message));
  n = rio_readlineb(&rio, buf, Message::MAX_LEN);
  buf[n] = '\0';
  if (n <= 0) {
    error("Error: not reived from server.");
  } else if (n >= 4 && ((std::string) buf).substr(0, 4) == "err:") {
    error(buf);
  }

  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)

  while(1) {
    n = rio_readlineb(&rio, buf, Message::MAX_LEN);
    buf[n] = '\0';
    if (n >= 9 && ((std::string) buf).substr(0, 9) == "delivery:") {
      continue;
    }
    else {
      error("Error: fail to receive delivered message.");
    }
  }
  return 0;
}

