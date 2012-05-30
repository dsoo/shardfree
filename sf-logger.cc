#include <pthread.h>
#include <iostream>
#include <zmq.hpp>
#include "sf-logger.h"
#include "sf-global.h"

SFLogger::SFLogger()
{
  socketp = new zmq::socket_t(*gZMQContextp, ZMQ_PUSH);
  socketp->connect("inproc://logger");
}

SFLogger::~SFLogger()
{
  delete socketp;
  socketp = NULL;
}

void SFLogger::log()
{
  //std::cout << "Start sending" << std::endl;
  //// Sends a message to the socket
  //zmq::message_t log(6);
  //memcpy ((void *) log.data (), "Hello", 5);
  //socketp->send(log);
  //std::cout << "Done sending" << std::endl;
}

void *log_output_routine(void *arg)
{
  std::cout << "Started log worker" << std::endl;

  zmq::socket_t receiver(*gZMQContextp, ZMQ_PULL);
  receiver.bind("inproc://logger");
  while (1) {
    zmq::message_t message;
    std::cout << "Waiting for log" << std::endl;
    receiver.recv(&message);
    std::cout << "Got log" << std::endl;
    // FIXME: Should terminate on shutdown message from parent
  }
}

SFLogOutput::SFLogOutput()
{
  // Create a worker thread that listens to the logger socket and prints the output
  pthread_t worker;
  pthread_create (&worker, NULL, log_output_routine, NULL);
}


SFLogOutput::~SFLogOutput()
{
  
}