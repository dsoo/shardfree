#include <pthread.h>
#include <iostream>
#include "global.h"
#include "log-publisher.h"

namespace ShardFree
{

LogPublisher::LogPublisher(const std::string &collector_name, const std::string &publisher_name) :
  Thread("LogPublisher"),
  mCollectorName(collector_name),
  mPublisherName(publisher_name),
  mCollectorp(NULL),
  mPublisherp(NULL)
{
}

LogPublisher::~LogPublisher()
{
  delete mCollectorp;
  mCollectorp = NULL;
  delete mPublisherp;
  mPublisherp = NULL;
}

void LogPublisher::init()
{
  mCollectorp = new zmq::socket_t(getZMQContext(), ZMQ_PULL);
  try
  {
    mCollectorp->bind(mCollectorName.c_str());
  }
  catch (...)
  {
    std::cerr << getZMQErrorString() << std::endl;
  }
  mPublisherp = new zmq::socket_t(getZMQContext(), ZMQ_PUB);
  try
  {
    mPublisherp->bind(mPublisherName.c_str());
  }
  catch (...)
  {
    std::cerr << getZMQErrorString() << std::endl;
  }
}

void LogPublisher::run()
{
  while (1)
  {
    zmq::message_t message;
    mCollectorp->recv(&message);
    // FIXME: Should gracefully terminate on shutdown message from parent
    mPublisherp->send(message);
  }
}

}
