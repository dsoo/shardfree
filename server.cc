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

  //
  // Initialize our logging
  //
  ShardFree::LogPublisher log_publisher("inproc://logger", "inproc://logpub");
  log_publisher.start();

  ShardFree::LogWriter log_writer("inproc://logpub");
  log_writer.start();

  ShardFree::LogWriterWebsocket log_writer_websocket("inproc://logpub");
  log_writer_websocket.start();

  //
  // Spawn the presence server
  //
  ShardFree::Presence presence;
  presence.start();

  //
  // Test talking to the presence service
  //
  ShardFree::PresenceClient presence_client;
  presence_client.sendRequest();

  //
  // Spawn simulation threads
  //
  std::vector<ShardFree::Thread *> simulators;

  for (int simulator_id = 0; simulator_id < 4; ++simulator_id) {
    std::ostringstream id_str;
    id_str << "sim" << simulator_id;

    // These are specifically "simulation" threads, although actually, they should
    // just be generic workers that do whatever we ask them to do
    ShardFree::Simulator *simulatorp = new ShardFree::Simulator(id_str.str());
    simulatorp->start();
    simulators.push_back(simulatorp);
  }

  int counter = 0;
  while (1)
  {
    // The main thread doesn't actually do any work.
    sleep(1);
    ++counter;
  }

  // FIXME: Clean up everything

  return 0;
}