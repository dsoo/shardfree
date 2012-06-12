#include <iostream>

#include "global.h"
#include "logger.h"
#include "simulator.h"

#include <vector>

//class SFPollset
//{
//  public:
//    SFPollset();
//    virtual ~SFPollset();
//
//    void addSocket(zmq::socket_t &socket)
//    poll(long timeout_usecs);
//  private:
//    std::vector<zmq::socket_t&> mSockets;
//};

namespace ShardFree
{

//
// Instance methods
//
Simulator::Simulator(const std::string &id) : Thread(id), mCounter(0)
{
  // Start up SUB socket to listen to trusted neighbors
  // Start up SUB socket to listen to clients
  // Start up PUB socket to push updates to listeners
  // Find neighbors
  // Connect to neighbors
}

Simulator::~Simulator()
{
  delete mPubSocketp;
  mPubSocketp = NULL;
}

void Simulator::init()
{
}

void Simulator::run()
{
  // Set up the PUB socket where you push all your output
  mPubSocketp = new zmq::socket_t(context(), ZMQ_PUB);
  mPubSocketp->bind((std::string("inproc://pub") + id()).c_str());

  // Tell the presence server that you're available


  // Find all of your neighbors (from the presence server)


  //// Subscribe to all of your neighbors
  //for(int i = 0; i < 5; ++i) {
  //  std::ostringstream ss;
  //  ss << i;
  //  if (id() != ss.str()) {
  //    // Add the neighbor
  //    SFLOG << "Neighbor: " << i;
  //    // Connect to a neighbor
  //    zmq::socket_t *neighborp = new zmq::socket_t(context(), ZMQ_SUB);
  //    neighborp->setsockopt(ZMQ_SUBSCRIBE, "", 0);
  //    neighborp->connect((std::string("inproc://pub") + ss.str()).c_str());
  //    mNeighbors.push_back(neighborp);
  //  }
  //}

  while(1) {
    collect();
    simulate();
    resolveAndPush();
  }
}

void Simulator::collect()
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

void Simulator::simulate()
{
  sleep(1);
  SFLOG << "Testy testy " << mCounter;
  // Push all updates as a result of simulation into update queue/data structure
  mCounter += 1;
  return;
}

void Simulator::resolveAndPush()
{
  // Iterate through all updates, resolving conflicts and applying to data structure
  // Push updates out to anybody that's listening
}

}
