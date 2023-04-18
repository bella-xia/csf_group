#include <sstream>
#include <cctype>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <cassert>
#include <iostream>
#include "csapp.h"
#include "message.h"
#include "connection.h"

Connection::Connection()
  : m_fd(-1)
  , m_last_result(SUCCESS) {
}

Connection::Connection(int fd)
  : m_fd(fd)
  , m_last_result(SUCCESS) {
  rio_readinitb(&m_fdbuf, m_fd);
  // TODO: call rio_readinitb to initialize the rio_t object
}

void Connection::connect(const std::string &hostname, int port) {
  m_fd = open_clientfd(hostname.c_str(), std::to_string(port).c_str());
  if (m_fd < 0) {
    m_last_result = EOF_OR_ERROR;
    close_connection();
    std::cerr<<"Error: could not connect to server.\n";
    exit(1);
  }
  // TODO: call open_clientfd to connect to the server
  rio_readinitb(&m_fdbuf, m_fd);
  // TODO: call rio_readinitb to initialize the rio_t object
  buf = new char[Message::MAX_LEN + 1];
}

Connection::~Connection() {
  close_connection();
  // TODO: close the socket if it is open
}

bool Connection::is_open() const {
  return m_fd > 0;
  // TODO: return true if the connection is open
}

void Connection::close_connection() {
  close(m_fd);
  m_fd = -1;
  free(buf);
  // TODO: close the connection if it is open
}

bool Connection::send(const Message &msg) {
  // TODO: send a message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  std::string message = msg.tag + ":" + msg.data;
  size_t n;
  n = rio_writen(m_fd, message.c_str(), message.length());
  if (n != message.length()) {
    m_last_result = EOF_OR_ERROR;
    return false;
  }
  m_last_result = SUCCESS;
  return true;
}

bool Connection::receive(Message &msg) {
  // TODO: receive a message, storing its tag and data in msg
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  int n = 0;
  n = rio_readlineb(&m_fdbuf, buf, Message::MAX_LEN);
  if (n < 0) {
    m_last_result = EOF_OR_ERROR;
    return false;
  }
  buf[n] = '\0';
  std::string delimiter = ":";
  std::string text = buf;
  int pos = text.find(delimiter);
  msg.tag = text.substr(0, pos);
  msg.data = text.substr(pos + 1);
  m_last_result = SUCCESS;
  return true;
}
