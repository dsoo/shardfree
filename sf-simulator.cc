#include <iostream>

#include "sf-global.h"
#include "sf-logger.h"
#include "sf-simulator.h"

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

}

void SFSimulator::run()
{
  // Set up the PUB socket where you push all your output
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
