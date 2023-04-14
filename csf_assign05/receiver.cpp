#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

void error(const char *msg)
{
  std::cout << msg;
  exit(1);
}
int main(int argc, char **argv)
{
  if (argc != 5)
  {
    std::cerr << "Usage: ./receiver [server_address] [port] [username] [room]\n";
    return 1;
  }
  std::string port = argv[2];
  std::string server_hostname = argv[1];
  // int server_port = std::stoi(argv[2]);
  std::string username = argv[3];
  std::string room_name = argv[4];
  Connection conn;
  int fd = open_clientfd(server_hostname.c_str(), port.c_str());
  if (fd < 0)
  {
    error("Could not connect to server.\n");
  }
  // TODO: send rlogin and join messages (expect a response from
  //       the server for each one)

  std::string rlogin_message = "rlogin:" + username + "\n";
  std::string join_message = "join:" + room_name + "\n";
  rio_writen(fd, rlogin_message.c_str(), rlogin_message.length());
  rio_t rio;
  rio_readinitb(&rio, fd);
  char buf[Message::MAX_LEN + 1];
  ssize_t n = rio_readlineb(&rio, buf, Message::MAX_LEN);
  buf[n] = '\0';
  if (n <= 0)
  {
    error("Error: not reived from server.\n");
  }
  else if (n >= 4 && ((std::string)buf).substr(0, 4) == "err:")
  {
    error(((std::string)buf).substr(4).c_str());
  }
  rio_writen(fd, join_message.c_str(), join_message.length());
  n = rio_readlineb(&rio, buf, Message::MAX_LEN);
  buf[n] = '\0';
  if (n <= 0)
  {
    error("Error: not received from server.\n");
  }
  else if (n >= 4 && ((std::string)buf).substr(0, 4) == "err:")
  {
    error(((std::string)buf).substr(4).c_str());
  }

  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)

  while (1)
  {
    n = rio_readlineb(&rio, buf, Message::MAX_LEN);
    buf[n] = '\0';
    if (n >= 9 + room_name.length() &&
        ((std::string)buf).substr(0, 9 + room_name.length()) == "delivery:" + room_name)
    {
      std::string text = ((std::string)buf).substr(9 + room_name.length() + 1);
      std::string delimiter = ":";
      int pos = text.find(delimiter);
      std::cout << text.substr(0, pos + 1) << " " << text.substr(pos + 1);
      continue;
    }
    else if (n >= 4 && ((std::string)buf).substr(0, 4) == "err:")
    {
      // std::cout << ((std::string)buf).substr(4).c_str();
      error(((std::string)buf).substr(4).c_str());
    }
  }
  return 0;
}
