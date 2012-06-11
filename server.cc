//
//  Hello World server in C++
//  Binds REP socket to tcp://*:5555
//  Expects "Hello" from client, replies with "World"
//
#include <string>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <zmq.hpp>

#include "global.h"
#include "logger.h"
#include "log-publisher.h"
#include "log-writer.h"
#include "log-writer-websocket.h"
#include "presence.h"
#include "simulator.h"

//
// FIXME: Do all the memory management stuff properly.
//

int main()
{
  //  Prepare our context and sockets
  gZMQContextp = new zmq::context_t(1);

  // Spawn our log output worker
  ShardFree::LogPublisher log_publisher("inproc://logger", "inproc://logpub");
  ShardFree::LogWriter log_writer("inproc://logpub");
  ShardFree::LogWriterWebsocket log_writer_websocket("inproc://logpub");

  // Spawn the presence server
  ShardFree::Presence presence;
  presence.start();

  // Test talking to the presence service
  sleep(1);
  ShardFree::PresenceClient presence_client;
  presence_client.sendRequest();



  std::vector<ShardFree::Worker *> workers;
  //  Launch pool of worker threads, same as number of CPUs
  for (int thread_nbr = 0; thread_nbr != 3; thread_nbr++) {
    std::ostringstream ss;
    ss << thread_nbr;
    ShardFree::Simulator *simulatorp = new ShardFree::Simulator(ss.str());
    simulatorp->start();
    workers.push_back(simulatorp);
  }

  int counter = 0;
  while (1)
  {
    sleep(1);
    ShardFree::Log().get() << "Waity waity " << counter;
    ++counter;
  }
  //
  ////  Connect work threads to client threads via a queue
  //zmq::device(ZMQ_QUEUE, clients, worker_sockets);
  return 0;
}