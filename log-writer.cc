#include <pthread.h>
#include <iostream>

#include "global.h"
#include "log-writer.h"

namespace ShardFree
{

LogWriter::LogWriter(const std::string &publisher_name) :
  Thread("LogWriter"),
  mPublisherName(publisher_name),
  mPublisherp(NULL)
{
}

LogWriter::~LogWriter()
{
  delete mPublisherp;
  mPublisherp = NULL;
}

void LogWriter::init()
{
  mPublisherp = new zmq::socket_t(getZMQContext(), ZMQ_SUB);
  mPublisherp->setsockopt(ZMQ_SUBSCRIBE, "", 0);

  bool connected = false;
  while (!connected)
  {
    try
    {
      mPublisherp->connect(mPublisherName.c_str());
      connected = true;
    }
    catch(...)
    {
      // FIXME: If the logwriter can't talk to the publisher
      // we need to be more aggressive about fixing this or dying, since
      // this means that logging won't be working.
      // Note that we're using cerr, as we assume that the standard
      // logger won't display any useful output since we're the writer.
      std::cerr << getZMQErrorString() << std::endl;
      sleep(1);
    }
  }
}

void LogWriter::run()
{
  while (1)
  {
    zmq::message_t message;
    mPublisherp->recv(&message);

    // FIXME: This is totally not safe and likely to break.
    // PubSub socket, which will then allow me to send it to a websocket proxy
    std::cout << std::string((char *)message.data(), message.size());
    // FIXME: Should terminate on shutdown message from parent
  }
}

}
