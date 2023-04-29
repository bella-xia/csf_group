#include "guard.h"
#include "message.h"
#include "message_queue.h"
#include "user.h"
#include "room.h"

#include <iostream>

Room::Room(const std::string &room_name)
    : room_name(room_name.substr(0, room_name.length() - 1))
{
  members = UserSet();
  pthread_mutex_init(&lock, NULL);
  // TODO: initialize the mutex
}

Room::~Room()
{
  pthread_mutex_destroy(&lock);
  // TODO: destroy the mutex
}

void Room::add_member(User *user)
{
  pthread_mutex_lock(&lock);
  members.insert(user);
  pthread_mutex_unlock(&lock);
  // TODO: add User to the room
}

void Room::remove_member(User *user)
{
  pthread_mutex_lock(&lock);
  members.erase(user);
  pthread_mutex_unlock(&lock);
  // TODO: remove User from the room
}

void Room::broadcast_message(const std::string &sender_username, const std::string &message_text)
{
  pthread_mutex_lock(&lock);
  for (std::set<User *>::iterator it = members.begin(); it != members.end(); it++)
  {
    Message *send_message = new Message(TAG_DELIVERY, room_name + ":" + sender_username.substr(0, sender_username.length() - 1) + ":" + message_text);
    (*it)->mqueue.enqueue(send_message);
    // free(send_message);
  }
  pthread_mutex_unlock(&lock);
  // TODO: send a message to every (receiver) User in the room
}
