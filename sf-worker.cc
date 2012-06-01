#include <pthread.h>
#include <iostream>
#include <zmq.hpp>

#include "sf-worker.h"
#include "sf-simulator.h"
#include "sf-logger.h"

void *SFWorker::runWorker(void *argp)
{
  SFWorker *workerp = (SFWorker *)(argp);

  workerp->init();
  workerp->run();
  return NULL;
}

SFWorker::SFWorker() : mLoggerp(NULL)
{
}

SFWorker::~SFWorker()
{
  delete mLoggerp;
  mLoggerp = NULL;
}

void SFWorker::init()
{
  // Start up the logger
  mLoggerp = new SFLogger();
}

void SFWorker::start()
{
  pthread_t worker;
  pthread_create(&worker, NULL, SFWorker::runWorker, this);
}

const SFLogger &SFWorker::logger()
{
  // FIXME: Throw an exception if this is uninitialized
  return *mLoggerp;
}