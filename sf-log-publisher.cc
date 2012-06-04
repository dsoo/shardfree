#include <pthread.h>
#include "sf-global.h"
#include "sf-log-publisher.h"

SFLogPublisher::SFLogPublisher(const std::string &collector_name, const std::string &publisher_name) :
  mCollectorName(collector_name),
  mPublisherName(publisher_name),
  mCollectorp(NULL),
  mPublisherp(NULL)
{
  // Create a worker thread that listens to the logger socket and prints the output
  pthread_t worker;

  zmq::socket_t ready_socket(*gZMQContextp, ZMQ_PULL);
  ready_socket.bind("inproc://logpubready");

  //std::cout << "Creating worker" << std::endl;
  pthread_create (&worker, NULL, runWorker, this);
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
  //std::cout << "Starting up publisher" << std::endl;
  mCollectorp = new zmq::socket_t(*gZMQContextp, ZMQ_PULL);
  mCollectorp->bind(mCollectorName.c_str());
  mPublisherp = new zmq::socket_t(*gZMQContextp, ZMQ_PUB);
  mPublisherp->bind(mPublisherName.c_str());

  // Now that we're bound, tell the main thread that we're ready for use
  {
    zmq::socket_t sender(*gZMQContextp, ZMQ_PUSH);
    sender.connect("inproc://logpubready");
    zmq::message_t message;
    sender.send(message);
  }

  //std::cout << "Collecting logs" << std::endl;
  while (1) {
    zmq::message_t message;
    mCollectorp->recv(&message);

    //// FIXME: This is totally not safe and likely to break.
    //// PubSub socket, which will then allow me to send it to a websocket proxy
    //std::cout << std::string((char *)message.data(), message.size());
    // FIXME: Should terminate on shutdown message from parent
    mPublisherp->send(message);
  }
}

void *SFLogPublisher::runWorker(void *argp)
{
  auto log_publisherp = (SFLogPublisher *)argp;
  log_publisherp->run();
  return 0;
}
