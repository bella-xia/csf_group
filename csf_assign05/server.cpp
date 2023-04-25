#include <pthread.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <set>
#include <vector>
#include <cctype>
#include <cassert>
#include "message.h"
#include "connection.h"
#include "user.h"
#include "room.h"
#include "guard.h"
#include "server.h"

////////////////////////////////////////////////////////////////////////
// Server implementation data types
////////////////////////////////////////////////////////////////////////

struct SharedData
{
  Connection *conn;
  Server *m_server;
  SharedData(Connection *connection, Server *ser)
  {
    conn = connection;
    m_server = ser;
  }
};
// TODO: add any additional data types that might be helpful
//       for implementing the Server member functions

////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////

namespace
{
  int chat_with_sender(SharedData *, std::string);
  int chat_with_receiver(SharedData *, User *);
  void *worker(void *arg)
  {
    SharedData *shared_data = (SharedData *)arg;
    Connection *connection = shared_data->conn;
    Server *ser = shared_data->m_server;
    if (pthread_detach(pthread_self()) != 0)
    {
      std::cerr << "Error: fail to detach the pthread." << std::endl;
      pthread_exit(nullptr);
    }
    // TODO: use a static cast to convert arg from a void* to
    //       whatever pointer type describes the object(s) needed
    //       to communicate with a client (sender or receiver)
    Message login_message;
    Message ok_message(TAG_OK, "\n");
    Message room_message;
    if (!connection->receive(login_message))
    {
      std::cerr << "Error: fail to receive" << std::endl;
      pthread_exit(nullptr);
      // TODO: err
    }
    if (login_message.tag == TAG_RLOGIN ||
        login_message.tag == TAG_SLOGIN)
    {
      if (!connection->send(ok_message))
      {
        std::cerr << "Error: fail to send message" << std::endl;
        pthread_exit(nullptr);
        // TODO err
      }
    }
    else
    {
      std::cerr << "Error: invalid message" << std::endl;
      pthread_exit(nullptr);
      // TODO err
    }

    if (login_message.tag == TAG_RLOGIN)
    {
      if (!connection->receive(room_message))
      {
        std::cerr << "Error: fail to receive message" << std::endl;
        pthread_exit(nullptr);
        // TODO: err
      }
      if (room_message.tag == TAG_JOIN)
      {
        if (!connection->send(ok_message))
        {
          std::cerr << "Error: fail to send message" << std::endl;
          pthread_exit(nullptr);
          // TODO: err
        }
        else
        {
          std::string room_name = room_message.data;
          User *new_user = new User(login_message.data);
          Room *rm = ser->find_or_create_room(room_name);
          rm->add_member(new_user);
          if (chat_with_receiver(shared_data, new_user) == 0)
          {
            delete (new_user);
            return nullptr;
          }
          else
          {
            delete (new_user);
            pthread_exit(nullptr);
          }
        }
      }
      else
      {
        std::cerr << "Error: invalid message." << std::endl;
        pthread_exit(nullptr);
        // TODO: err
      }

      // TODO: read login message (should be tagged either with
      //       TAG_SLOGIN or TAG_RLOGIN), send response

      // TODO: depending on whether the client logged in as a sender or
      //       receiver, communicate with the client (implementing
      //       separate helper functions for each of these possibilities
      //       is a good idea)
      return nullptr;
    }
    else
    {
      if (chat_with_sender(shared_data, login_message.data) == 0)
      {
        return nullptr;
      }
    }
    return nullptr;
  }
  int chat_with_sender(SharedData *shared_data, std::string user_name)
  {
    Connection *connection = shared_data->conn;
    Server *ser = shared_data->m_server;
    Message ok_message = Message(TAG_OK, "\n");
    Message err_message = Message(TAG_ERR, "\n");
    Room *rm = nullptr;
    while (1)
    {
      Message *input = new Message();
      bool err = false;
      if (!connection->receive(*input))
      {
        std::cerr << "Error: fail to receive message." << std::endl;
        pthread_exit(nullptr);
        // TODO err
      }
      if (input->data.length() > Message::MAX_LEN)
      {
        err = true;
        err_message.data = "the message length exceeds the maximum limit.\n";
      }
      else if (input->data.length() == 0)
      {
        err = true;
        err_message.data = "cannot parse the message.\n";
      }
      else if (input->tag == TAG_JOIN)
      {
        /*
        if (rm != nullptr)
        {
          err = true;
          err_message.data = "already in a room.\n";
          // TODO err
        }
        */
        std::string room_name = input->data;
        rm = ser->find_or_create_room(room_name);
      }
      else if (input->tag == TAG_LEAVE)
      {
        if (rm == nullptr)
        {
          err = true;
          err_message.data = "not in a room.\n";
          // TODO err
        }
        else
        {
          rm = nullptr;
        }
      }
      else if (input->tag == TAG_QUIT)
      {
        connection->send(ok_message);
        return 0;
      }
      else
      {
        if (rm == nullptr)
        {
          err = true;
          err_message.data = "not in a room.\n";
        }
        else
        {
          std::string msg = input->data;
          rm->broadcast_message(user_name, msg);
        }
      }
      if (err)
      {
        if (!connection->send(err_message))
        {
          std::cerr << "Error: fail to send message." << std::endl;
          pthread_exit(nullptr);
        }
      }
      else
      {
        if (!connection->send(ok_message))
        {
          std::cerr << "Error: fail to send message." << std::endl;
          pthread_exit(nullptr);
        }
      }
      delete (input);
    }
    return 0;
  }

  int chat_with_receiver(SharedData *shared_data, User *user)
  {
    Connection *connection = shared_data->conn;
    while (1)
    {
      Message *message = user->mqueue.dequeue();
      if (message == nullptr)
      {
        continue;
      }
      if (!connection->send(*message))
      {
        std::cerr << "Error: fail to send message." << std::endl;
        delete (message);
        pthread_exit(nullptr);
        // return -1;
        // TODO: err
      }
      delete (message);
    }
    return 0;
  }
}

////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

Server::Server(int port)
    : m_port(port), m_ssock(-1)
{
  m_rooms = RoomMap();
  pthread_mutex_init(&m_lock, NULL);
}

Server::~Server()
{
  pthread_mutex_destroy(&m_lock);
  for (std::map<std::string, Room *>::iterator it = m_rooms.begin(); it != m_rooms.end(); it++)
  {
    delete (it->second);
  }
}

bool Server::listen()
{
  int listenfd = open_listenfd(std::to_string(m_port).c_str());
  if (listenfd < 0)
  {
    return false;
  }
  m_ssock = listenfd;
  return true;
  // TODO: use open_listenfd to create the server socket, return true
  //       if successful, false if not
}

void Server::handle_client_requests()
{
  while (1)
  {
    int clientfd = accept(m_ssock, NULL, NULL);
    if (clientfd < 0)
    {
      std::cerr << "Error: fail to establish connection." << std::endl;
      exit(1);
      // TODO:err
    }
    Connection *connection = new Connection(clientfd);
    SharedData *shared_data = new SharedData(connection, this);
    pthread_t thr_id;
    if (pthread_create(&thr_id, NULL, worker, shared_data) != 0)
    {
      std::cerr << "Error: fail to create a pthread." << std::endl;
      delete (connection);
      delete (shared_data);
      pthread_exit(nullptr);
      // TODO:error
    }
  }

  // TODO: infinite loop calling accept or Accept, starting a new
  //       pthread for each connected client
}

Room *Server::find_or_create_room(const std::string &room_name)
{
  // TODO: return a pointer to the unique Room object representing
  //       the named chat room, creating a new one if necessary
  auto it = m_rooms.find(room_name);
  if (it != m_rooms.end())
  {
    return it->second;
  }
  Room *new_room = new Room(room_name);

  m_rooms.insert({room_name, new_room});
  return new_room;
}