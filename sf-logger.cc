#include <pthread.h>
#include <iostream>
#include <zmq.hpp>
#include "sf-logger.h"
#include "sf-global.h"


// Logging implementation cribbed (and simplified) from here: http://www.drdobbs.com/cpp/201804215

Log::Log(const SFLogger &logger) : logger(logger)
{
}

std::ostringstream& Log::get()
{
   return mOSS;
}

Log::~Log()
{
  if (mOSS.str().size())
  {
    mOSS << std::endl;
    logger.output(mOSS.str());
  }
}


SFLogger::SFLogger()
{
  socketp = new zmq::socket_t(*gZMQContextp, ZMQ_PUSH);
  try
  {
    socketp->connect("inproc://logger");
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

void SFLogger::output(const std::string &str) const
{
  zmq::message_t line(str.size());
  memcpy((void *)line.data(), str.data(), str.size());
  socketp->send(line);
}

void *log_output_routine(void *arg);

SFLogOutput::SFLogOutput()
{
  // Create a worker thread that listens to the logger socket and prints the output
  pthread_t worker;

  zmq::socket_t receiver(*gZMQContextp, ZMQ_PULL);
  receiver.bind("inproc://logready");

  pthread_create (&worker, NULL, log_output_routine, NULL);
  zmq::message_t message;

  // Waits until log output thread is fully initialized before returning.
  receiver.recv(&message);
}


SFLogOutput::~SFLogOutput()
{
}


void *log_output_routine(void *arg)
{
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
    // FIXME: This should aggregate the stream and publish it on a ZeroMQ
    // PubSub socket, which will then allow me to send it to a websocket proxy
    std::cout << std::string((char *)message.data(), message.size());
    // FIXME: Should terminate on shutdown message from parent
  }
}
