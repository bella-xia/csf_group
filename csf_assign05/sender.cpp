#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv)
{
  if (argc != 4)
  {
    std::cerr << "Usage: ./sender [server_address] [port] [username]\n";
    return 1;
  }
  std::string server_hostname = argv[1];
  int server_port = std::stoi(argv[2]);
  std::string username = argv[3];
  Connection conn;
  
  conn.connect(server_hostname, server_port);
  Message login_message = Message(TAG_SLOGIN, username + "\n");
  Message* received_message = new Message();
  if (!conn.send(login_message)) {
    std::cerr << "Fail to send.\n";
    exit(1);
  }
  if (!conn.receive(*received_message)) {
    std::cerr << "Fail to receive.\n";
    exit(1);
  }
  if (received_message->tag == TAG_ERR) {
    std::cerr << received_message->data;
    exit(1);
  }
  std::string input;
  while (std::getline(std::cin, input))
  {
    Message input_message = Message();
    if (input.length() >= 6 && input.substr(0, 6) == "/leave")
    {
      input_message.tag = TAG_LEAVE;
      input_message.data = "bye\n";
    }
    else if (input.length() > 5 && input.substr(0, 5) == "/join")
    {
      input_message.tag = TAG_JOIN;
      input_message.data = input.substr(6) + "\n";
    }
    else if (input.length() >= 5 && input.substr(0, 5) == "/quit")
    {
      input_message.tag = TAG_QUIT;
      input_message.data = "bye\n";
      if (!conn.send(input_message)) {
      std::cerr<<"Fail to send\n";
      exit(1);
      }
      if (!conn.receive(*received_message)) {
        std::cerr << "Fail to receive.\n";
        exit(1);
      }
      if (received_message->tag == TAG_ERR) {
        std::cerr << received_message->data;
      }
      exit(0);
      }
    else
    { input_message.tag = TAG_SENDALL;
      input_message.data = input + "\n";
    }

    /*err handling*/
    if (!conn.send(input_message)) {
      std::cerr<<"Fail to send\n";
      exit(1);
    }
    if (!conn.receive(*received_message)) {
      std::cerr << "Fail to receive.\n";
      exit(1);
    }
    if (received_message->tag == TAG_ERR) {
      std::cerr << received_message->data;
    }
  }
  return 0;
}
