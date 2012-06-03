#include <iostream>

#include "sf-global.h"
#include "sf-logger.h"
#include "sf-simulator.h"

#include <vector>

//
// Instance methods
//
SFSimulator::SFSimulator(const std::string &id) : SFWorker(id), mCounter(0)
{
  // Start up SUB socket to listen to trusted neighbors
  // Start up SUB socket to listen to clients
  // Start up PUB socket to push updates to listeners
  // Find neighbors
  // Connect to neighbors
}

SFSimulator::~SFSimulator()
{
  delete mPubSocketp;
  mPubSocketp = NULL;
}

void SFSimulator::run()
{
  // Set up the PUB socket where you push all your output
  mPubSocketp = new zmq::socket_t(context(), ZMQ_PUB);
  mPubSocketp->bind((std::string("inproc://pub") + id()).c_str());
  // Find all of your neighbors
  // Subscribe to all of your neighbors

  while(1) {
    collect();
    simulate();
    resolveAndPush();
  }
}

void SFSimulator::collect()
{
    // Listen to all of your neighbor's subs until you get all of the updates for the
    // current frame.

    ////  Connect to weather server
    //zmq::socket_t subscriber(context, ZMQ_SUB);
    //subscriber.connect("tcp://localhost:5556");
    //subscriber.setsockopt(ZMQ_SUBSCRIBE, "10001 ", 6);
    //
    ////  Initialize poll set
    //zmq::pollitem_t items [] = {
    //    { receiver, 0, ZMQ_POLLIN, 0 },
    //    { subscriber, 0, ZMQ_POLLIN, 0 }
    //};
    ////  Process messages from both sockets
    //while (1) {
    //    zmq::message_t message;
    //    zmq::poll (&items [0], 2, -1);
    //    
    //    if (items [0].revents & ZMQ_POLLIN) {
    //        receiver.recv(&message);
    //        //  Process task
    //    }
    //    if (items [1].revents & ZMQ_POLLIN) {
    //        subscriber.recv(&message);
    //        //  Process weather update
    //    }
    //}
    return;
}

void SFSimulator::simulate()
{
    SFLOG << "Testy testy " << mCounter;
    // Push all updates as a result of simulation into update queue/data structure
    mCounter += 1;
    return;
}

void SFSimulator::resolveAndPush()
{
    // Iterate through all updates, resolving conflicts and applying to data structure
    // Push updates out to anybody that's listening
}
