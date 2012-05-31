#include <pthread.h>
#include <iostream>
#include <zmq.hpp>
#include "sf-logger.h"
#include "sf-global.h"

SFLogger::SFLogger()
{
  socketp = new zmq::socket_t(*gZMQContextp, ZMQ_PUSH);
  try
  {
    std::cout << "Connecting logger" << std::endl;
    socketp->connect("inproc://logger");
    std::cout << "Logger connected!" << std::endl;
  }
  catch(...)
  {
    std::cout << "Errno:" << errno << std::endl;
    switch(errno) {
      case EINVAL:
        std::cout << "EINVAL" << std::endl;
        break;
      case EPROTONOSUPPORT:
        std::cout << "EPROTONOSUPPORT" << std::endl;
        break;
      case ENOCOMPATPROTO:
        std::cout << "ENOCOMPATPROTO" << std::endl;
        break;
      case ETERM:
        std::cout << "ETERM" << std::endl;
        break;
      case ENOTSOCK:
        std::cout << "ENOTSOCK" << std::endl;
        break;
      case EMTHREAD:
        std::cout << "EMTHREAD" << std::endl;
        break;
      default:
        std::cout << "UNKNOWN ERROR" << std::endl;
    }
    exit(-1);
  }
}

SFLogger::~SFLogger()
{
  delete socketp;
  socketp = NULL;
}

void SFLogger::log()
{
  // Sends a message to the socket
  zmq::message_t log(6);
  memcpy ((void *) log.data (), "Hello", 5);
  socketp->send(log);
}

void *log_output_routine(void *arg)
{
  std::cout << "Started log worker" << std::endl;

  zmq::socket_t receiver(*gZMQContextp, ZMQ_PULL);
  receiver.bind("inproc://logger");

  // Now that we're bound, tell the main thread that we're ready for use
  {
    zmq::socket_t sender(*gZMQContextp, ZMQ_PUSH);
    sender.connect("inproc://logready");
    zmq::message_t message;
    sender.send(message);
  }

  while (1) {
    zmq::message_t message;
    receiver.recv(&message);
    
    // FIXME: This is totally not safe and likely to break.
    std::cout << std::string((char *)message.data(), message.size()) << std::endl;
    // FIXME: Should terminate on shutdown message from parent
  }
}

SFLogOutput::SFLogOutput()
{
  // Create a worker thread that listens to the logger socket and prints the output
  pthread_t worker;
  
  zmq::socket_t receiver(*gZMQContextp, ZMQ_PULL);
  receiver.bind("inproc://logready");
  
  // FIXME: This needs to be made to block until the logger thread is created and initialized.
  pthread_create (&worker, NULL, log_output_routine, NULL);
  zmq::message_t message;
  
  // Waits until log output thread has sent a message back before returning.
  receiver.recv(&message);  
}


SFLogOutput::~SFLogOutput()
{
  
}