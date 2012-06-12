#include <pthread.h>
#include <iostream>
#include <zmq.hpp>

#include "global.h"
#include "worker.h"
#include "simulator.h"
#include "logger.h"

namespace ShardFree
{

void *Worker::runWorker(void *argp)
{
  Worker *workerp = (Worker *)(argp);
  Logger::get().setPrefix(workerp->mID + ":");

  workerp->init();
  // We're initialized, we can let the main thread go.
  {
    zmq::socket_t sender(getZMQContext(), ZMQ_PUSH);
    sender.connect((std::string("inproc://ready") + workerp->mID).c_str());
    zmq::message_t message;
    sender.send(message);
  }
  workerp->run();
  return NULL;
}

Worker::Worker(const std::string &id, bool logging) : mID(id), mContext(getZMQContext()), mLogging(logging), mLoggerp(NULL)
{
  // FIXME: Verify the the id is unique
}

Worker::~Worker()
{
  delete mLoggerp;
  mLoggerp = NULL;
}

void Worker::init()
{
}

void Worker::start()
{
  // Returns once worker initialization is done, so the
  // main thread can be certain that it can use the object
  // safely.

  zmq::socket_t ready_socket(getZMQContext(), ZMQ_PULL);
  ready_socket.bind((std::string("inproc://ready") + mID).c_str());

  pthread_t worker;
  pthread_create(&worker, NULL, Worker::runWorker, this);

  // FIXME: We should be prepared to time out or do
  // something else useful in the case that the worker fails to
  // initialize properly.
  zmq::message_t message;
  ready_socket.recv(&message);
}

zmq::context_t &Worker::context() const
{
  return mContext;
}

const std::string &Worker::id() const
{
  return mID;
}

}
