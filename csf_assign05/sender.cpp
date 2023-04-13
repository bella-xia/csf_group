#include <iostream>
#include <string>
#include <sstream>
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
  if (argc != 4)
  {
    std::cerr << "Usage: ./sender [server_address] [port] [username]\n";
    return 1;
  }

  std::string server_hostname;
  int server_port;
  std::string username;
  std::string port = argv[2];
  server_hostname = argv[1];
  // server_port = std::stoi(argv[2]);
  username = argv[3];

  int fd = open_clientfd(server_hostname.c_str(), port.c_str());
  if (fd < 0)
  {
    error("Error: could not connect to server.");
  }
  // TODO: send rlogin and join messages (expect a response from
  //       the server for each one)

  std::string rlogin_message = "slogin:" + username + "\n";
  std::string leave_message = "leave:\n";
  std::string quit_message = "quit:\n";
  std::string join_message = "join:";
  std::string sendall_message = "sendall:";

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
  std::string input;
  while (std::getline(std::cin, input))
  {
    if (input.length() >= 6 && input.substr(0, 6) == "/leave")
    {
      rio_writen(fd, leave_message.c_str(), leave_message.length());
    }
    else if (input.length() >= 5 && input.substr(0, 5) == "/join")
    {
      std::string join_message_complete = join_message + input.substr(6) + "\n";
      rio_writen(fd, join_message_complete.c_str(), join_message_complete.length());
    }
    else if (input.length() >= 5 && input.substr(0, 5) == "/quit")
    {
      rio_writen(fd, quit_message.c_str(), quit_message.length());
      n = rio_readlineb(&rio, buf, Message::MAX_LEN);
      buf[n] = '\0';
      if (n <= 0)
      {
        error("Error: not reived from server.\n");
      }
      else if (n >= 4 && ((std::string)buf).substr(0, 4) == "err:")
      {
        // std::cout << ((std::string)buf).substr(4).c_str();
        error(((std::string)buf).substr(4).c_str());
      }
      else if (n >= 3 && ((std::string)buf).substr(0, 3) == "ok:")
      {
        exit(0);
      }
    }
    else
    {
      std::string sendall_message_complete = sendall_message + input + "\n";
      rio_writen(fd, sendall_message_complete.c_str(), sendall_message_complete.length());
    }
    n = rio_readlineb(&rio, buf, Message::MAX_LEN);
    buf[n] = '\0';
    if (n <= 0)
    {
      error("Error: not reived from server.\n");
    }
    else if (n >= 4 && ((std::string)buf).substr(0, 4) == "err:")
    {
      std::cout << ((std::string)buf).substr(4).c_str();
      // error(((std::string)buf).substr(4).c_str());
    }
  }

  // TODO: connect to server

  // TODO: send slogin message

  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate

  return 0;
}
