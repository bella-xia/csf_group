#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv)
{
  if (argc != 5)
  {
    std::cerr << "Usage: ./receiver [server_address] [port] [username] [room]\n";
    return 1;
  }
  std::string server_hostname = argv[1];
  int server_port = std::stoi(argv[2]);
  std::string username = argv[3];
  std::string room_name = argv[4];
  Connection conn;

  conn.connect(server_hostname, server_port);
  Message received_message;
  Message login_message = Message(TAG_RLOGIN, username + "\n");
  Message join_message = Message(TAG_JOIN, room_name + "\n");
  if (!conn.send(login_message)) {
    std::cerr << "Fail to send.\n";
    exit(1);
  }
  if (!conn.receive(received_message)) {
    std::cerr << "Fail to receive.\n";
    exit(1);
  }
  if (received_message.tag == TAG_ERR) {
    std::cerr << received_message.data;
    exit(1);
  }
  if (!conn.send(join_message)) {
    std::cerr << "Fail to send.\n";
    exit(1);
  }
  if (!conn.receive(received_message)) {
    std::cerr << "Fail to receive.\n";
    exit(1);
  }
  if (received_message.tag == TAG_ERR) {
    std::cerr << received_message.data;
    exit(1);
  }
  while (1)
  {
    if (!conn.receive(received_message)) {
    std::cerr << "Fail to receive.\n";
    exit(1);
    }
    if (received_message.tag == TAG_ERR) {
      std::cerr << received_message.data;
      exit(1);
    } else if (received_message.tag == TAG_DELIVERY) {
      std::string delimiter = ":";
      std::string text = received_message.data;
      int pos = text.find(delimiter);
      if (text.substr(0, pos) == room_name) {
        text = text.substr(pos + 1);
        pos = text.find(delimiter);
        std::cout << text.substr(0, pos + 1) << " " << text.substr(pos + 1);
      }
    }
  }
  return 0;
}
