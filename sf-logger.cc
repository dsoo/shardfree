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


SFLogger::SFLogger(const std::string &collector_name)
{
  mCollectorp = new zmq::socket_t(*gZMQContextp, ZMQ_PUSH);
  try
  {
    mCollectorp->connect(collector_name.c_str());
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
  delete mCollectorp;
  mCollectorp = NULL;
}

void SFLogger::setPrefix(const std::string &prefix)
{
  mPrefix = prefix;
}

void SFLogger::output(const std::string &str) const
{
  zmq::message_t line(str.size() + mPrefix.size());
  memcpy((void *)line.data(), mPrefix.data(), mPrefix.size());
  memcpy((void *)((char *)line.data() + mPrefix.size()), str.data(), str.size());
  mCollectorp->send(line);
}

void *log_output_routine(void *arg);

SFLogPublisher::SFLogPublisher(const std::string &collector_name, const std::string &publisher_name) :
  mCollectorName(collector_name),
  mPublisherName(publisher_name),
  mCollectorp(NULL),
  mPublisherp(NULL)
{
  // Create a worker thread that listens to the logger socket and prints the output
  pthread_t worker;

  zmq::socket_t ready_socket(*gZMQContextp, ZMQ_PULL);
  ready_socket.bind("inproc://logready");

  pthread_create (&worker, NULL, log_output_routine, this);
  zmq::message_t message;

  // Waits until ZMQ sockets are abound before returning.
  ready_socket.recv(&message);
}

SFLogPublisher::~SFLogPublisher()
{
  delete mCollectorp;
  mCollectorp = NULL;
  delete mPublisherp;
  mPublisherp = NULL;
}

void SFLogPublisher::run()
{
  mCollectorp = new zmq::socket_t(*gZMQContextp, ZMQ_PULL);
  mCollectorp->bind(mCollectorName.c_str());
  mPublisherp = new zmq::socket_t(*gZMQContextp, ZMQ_PUB);
  mPublisherp->bind(mPublisherName.c_str());

  // Now that we're bound, tell the main thread that we're ready for use
  {
    zmq::socket_t sender(*gZMQContextp, ZMQ_PUSH);
    sender.connect("inproc://logready");
    zmq::message_t message;
    sender.send(message);
  }

  while (1) {
    zmq::message_t message;
    mCollectorp->recv(&message);

    // FIXME: This is totally not safe and likely to break.
    // PubSub socket, which will then allow me to send it to a websocket proxy
    std::cout << std::string((char *)message.data(), message.size());
    // FIXME: Should terminate on shutdown message from parent
    mPublisherp->send(message);
  }
}

void *log_output_routine(void *arg)
{
  auto log_publisherp = (SFLogPublisher *)arg;
  log_publisherp->run(); 
}
