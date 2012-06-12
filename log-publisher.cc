#include <pthread.h>
#include "global.h"
#include "log-publisher.h"

namespace ShardFree
{

LogPublisher::LogPublisher(const std::string &collector_name, const std::string &publisher_name) :
  mCollectorName(collector_name),
  mPublisherName(publisher_name),
  mCollectorp(NULL),
  mPublisherp(NULL)
{
  // Create a worker thread that listens to the logger socket and prints the output
  pthread_t worker;

  zmq::socket_t ready_socket(getZMQContext(), ZMQ_PULL);
  ready_socket.bind("inproc://logpubready");

  pthread_create (&worker, NULL, runWorker, this);
  zmq::message_t message;

  // Waits until ZMQ sockets are abound before returning.
  ready_socket.recv(&message);
}

LogPublisher::~LogPublisher()
{
  delete mCollectorp;
  mCollectorp = NULL;
  delete mPublisherp;
  mPublisherp = NULL;
}

void LogPublisher::run()
{
  mCollectorp = new zmq::socket_t(getZMQContext(), ZMQ_PULL);
  mCollectorp->bind(mCollectorName.c_str());
  mPublisherp = new zmq::socket_t(getZMQContext(), ZMQ_PUB);
  mPublisherp->bind(mPublisherName.c_str());

  // Now that we're bound, tell the main thread that we're ready for use
  {
    zmq::socket_t sender(getZMQContext(), ZMQ_PUSH);
    sender.connect("inproc://logpubready");
    zmq::message_t message;
    sender.send(message);
  }

  while (1) {
    zmq::message_t message;
    mCollectorp->recv(&message);

    //// FIXME: This is totally not safe and likely to break.
    //// PubSub socket, which will then allow me to send it to a websocket proxy
    // FIXME: Should terminate on shutdown message from parent
    mPublisherp->send(message);
  }
}

void *LogPublisher::runWorker(void *argp)
{
  LogPublisher *log_publisherp = (LogPublisher *)argp;
  log_publisherp->run();
  return 0;
}

}
