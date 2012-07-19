#include <pthread.h>
#include <iostream>
#include "zmq.hpp"

#include "global.h"
#include "thread.h"
#include "simulator.h"
#include "logger.h"

namespace ShardFree
{

void *Thread::runThread(void *argp)
{
  Thread *threadp = (Thread *)(argp);


  threadp->init();
  // We're initialized, we can let the parent thread go.
  {
    zmq::socket_t sender(getZMQContext(), ZMQ_PUSH);
    try
    {
      sender.connect((std::string("inproc://ready") + threadp->mID).c_str());
    }
    catch (...)
    {
      std::cerr << getZMQErrorString() << std::endl;
    }
    zmq::message_t message;
    sender.send(message);
  }

  // FIXME: Log prefix initialization should happen earlier, but since
  // we use this to initialize the log publisher, it causes problems.
  Logger::get().setPrefix(threadp->mID + ":");
  threadp->run();
  return NULL;
}

Thread::Thread(const std::string &id) : mID(id), mContext(getZMQContext())
{
  // FIXME: Verify the the id is unique

}

Thread::~Thread()
{
}

void Thread::start()
{
  // Needs to be separate because we can't call virtual functions
  // during constructors.

  // Returns once worker initialization is done, so the
  // main thread can be certain that it can use the object
  // safely.

  zmq::socket_t ready_socket(getZMQContext(), ZMQ_PULL);
  ready_socket.bind((std::string("inproc://ready") + mID).c_str());

  pthread_t thread;
  pthread_create(&thread, NULL, Thread::runThread, this);

  // FIXME: We should be prepared to time out or do
  // something else useful in the case that the worker fails to
  // initialize properly.
  zmq::message_t message;
  ready_socket.recv(&message);
}

zmq::context_t &Thread::context() const
{
  return mContext;
}

const std::string &Thread::id() const
{
  return mID;
}

}
