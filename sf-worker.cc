#include <pthread.h>
#include <iostream>
#include <zmq.hpp>

#include "sf-global.h"
#include "sf-worker.h"
#include "sf-simulator.h"
#include "sf-logger.h"

void *SFWorker::runWorker(void *argp)
{
  SFWorker *workerp = (SFWorker *)(argp);
  std::cout << workerp->id() << ":runWorker" << std::endl;

  workerp->init();
  workerp->run();
  return NULL;
}

SFWorker::SFWorker(const std::string &id) : mID(id), mContextp(gZMQContextp), mLoggerp(NULL)
{
  // Assumes that we always have a viable ZeroMQ context to work with.
  // FIXME: Get context from a singleton instead of a global
}

SFWorker::~SFWorker()
{
  mContextp = NULL;
  delete mLoggerp;
  mLoggerp = NULL;
}

void SFWorker::init()
{
  // Start up the logger
  mLoggerp = new SFLogger();
  mLoggerp->setPrefix(mID + ":");
}

void SFWorker::start()
{
  pthread_t worker;
  pthread_create(&worker, NULL, SFWorker::runWorker, this);
}

zmq::context_t &SFWorker::context() const
{
  return *mContextp;
}

const SFLogger &SFWorker::logger() const
{
  // FIXME: Throw an exception if this is uninitialized
  return *mLoggerp;
}

const std::string &SFWorker::id() const
{
  return mID;
}
