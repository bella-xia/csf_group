#include <cassert>
#include <ctime>
#include "message_queue.h"
#include "message.h"
#include <iostream>

MessageQueue::MessageQueue() {
  m_messages = std::deque<Message*>();
  pthread_mutex_init(&m_lock, NULL);
  sem_init(&m_avail, 0, 0);
  // TODO: initialize the mutex and the semaphore
}

MessageQueue::~MessageQueue() {
  for(std::deque<Message*>::iterator it = m_messages.begin(); it != m_messages.end(); it++) {
    delete(*it);
  }
  sem_destroy(&m_avail);
  pthread_mutex_destroy(&m_lock);
  // TODO: destroy the mutex and the semaphore
}

void MessageQueue::enqueue(Message *msg) {
  sem_post(&m_avail);
  pthread_mutex_lock(&m_lock);
  m_messages.insert(m_messages.begin(), msg);
  pthread_mutex_unlock(&m_lock);
  sem_wait(&m_avail);
  // TODO: put the specified message on the queue

  // be sure to notify any thread waiting for a message to be
  // available by calling sem_post
}

Message *MessageQueue::dequeue() {
  struct timespec ts;
  // get the current time using clock_gettime:
  // we don't check the return value because the only reason
  // this call would fail is if we specify a clock that doesn't
  // exist
  clock_gettime(CLOCK_REALTIME, &ts);

  // compute a time one second in the future
  ts.tv_sec += 1;
  
  while(sem_timedwait(&m_avail, &ts) == -1) {
    continue;
    //TODO: erro
  };
  // TODO: call sem_timedwait to wait up to 1 second for a message
  //       to be available, return nullptr if no message is available

  // TODO: remove the next message from the queue, return it
  pthread_mutex_lock(&m_lock);
  Message *msg = nullptr;
  if(!m_messages.empty()) {
    msg = m_messages.back();
    m_messages.pop_back();
  }
  pthread_mutex_unlock(&m_lock);
  sem_post(&m_avail);
  return msg;
}
