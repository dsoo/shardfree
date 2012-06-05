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

#include "sf-global.h"
#include "sf-logger.h"
#include "sf-log-publisher.h"
#include "sf-log-writer.h"
#include "sf-presence.h"
#include "sf-simulator.h"

//
// FIXME: Do all the memory management stuff properly.
//

int main()
{
  //  Prepare our context and sockets
  gZMQContextp = new zmq::context_t(1);

  // Spawn our log output worker
  SFLogPublisher log_publisher("inproc://logger", "tcp://*:5555");
  SFLogWriter log_writer("tcp://localhost:5555");
  SFLogger logger;

  // Spawn the presence server
  SFPresence presence;
  presence.start();
  
  // Test talking to the presence service
  SFPresenceClient presence_client;
  
  presenceClient->sendRequest();
  
  
  
  std::vector<SFWorker *> workers;
  //  Launch pool of worker threads, same as number of CPUs
  for (int thread_nbr = 0; thread_nbr != 1; thread_nbr++) {
    std::ostringstream ss;
    ss << thread_nbr;
    SFSimulator *simulatorp = new SFSimulator(ss.str());
    simulatorp->start();
    workers.push_back(simulatorp);
  }

  int counter = 0;
  while (1)
  {
    sleep(1);
    Log(logger).get() << "Waity waity " << counter;
    ++counter;
  }
  //
  ////  Connect work threads to client threads via a queue
  //zmq::device(ZMQ_QUEUE, clients, worker_sockets);
  return 0;
}