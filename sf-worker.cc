#include <pthread.h>
#include <iostream>
#include <zmq.hpp>

#include "sf-worker.h"
#include "sf-simulator.h"

void *worker_routine (void *arg)
{
  std::cout << "Started worker" << std::endl;

  //zmq::socket_t socket (*context, ZMQ_REP);
  //socket.connect ("inproc://workers");

  SFSimulator *simulatorp = SFSimulator::create();
  simulatorp->run();
  return NULL;
}

SFWorker::SFWorker()
{
  pthread_t worker;
  pthread_create(&worker, NULL, worker_routine, NULL);
}

SFWorker::~SFWorker()
{
  
}