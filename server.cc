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
  ShardFree::getZMQContext(); // Do this while context initialization isn't thread safe

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

  std::vector<ShardFree::Thread *> workers;
  //  Launch pool of worker threads
  for (int worker_id = 0; worker_id < 4; ++worker_id) {
    std::ostringstream id_str;
    id_str << worker_id;

    // These are specifically "simulation" threads, although actually, they should
    // just be generic workers that do whatever we ask them to do
    ShardFree::Simulator *simulatorp = new ShardFree::Simulator(id_str.str());
    simulatorp->start();
    workers.push_back(simulatorp);
  }

  int counter = 0;
  while (1)
  {
    // The main thread doesn't actually do any work.
    sleep(1);
    ++counter;
  }

  // FIXME: Clean up threads and globals
  return 0;
}