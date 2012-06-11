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
  //std::cout << workerp->id() << ":runWorker" << std::endl;

  workerp->init();
  workerp->run();
  return NULL;
}

Worker::Worker(const std::string &id, bool logging) : mID(id), mContextp(gZMQContextp), mLogging(logging), mLoggerp(NULL)
{
  // FIXME: Initialize the ZeroMQ context in a better way than a global.
}

Worker::~Worker()
{
  mContextp = NULL;
  delete mLoggerp;
  mLoggerp = NULL;
}

void Worker::init()
{
  if (mLogging)
  {
    // Start up the logger
    mLoggerp = new ShardFree::Logger();
    mLoggerp->setPrefix(mID + ":");
  }
}

void Worker::start()
{
  pthread_t worker;
  pthread_create(&worker, NULL, Worker::runWorker, this);
}

zmq::context_t &Worker::context() const
{
  return *mContextp;
}

const ShardFree::Logger &Worker::logger() const
{
  // FIXME: Throw an exception if this is uninitialized
  return *mLoggerp;
}

const std::string &Worker::id() const
{
  return mID;
}

}
