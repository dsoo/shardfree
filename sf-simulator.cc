#include <iostream>

#include "sf-global.h"
#include "sf-logger.h"
#include "sf-simulator.h"

//
// Instance methods
//
SFSimulator::SFSimulator() : SFWorker(), mCounter(0)
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
  while(1) {
    collect();
    simulate();
    resolveAndPush();
  }
}

void SFSimulator::collect()
{
    // Collect updates from neighbors
    //std::cout << "Collecting" << std::endl;

    // Pull messages until all updates for current frame received from neighbors or we time out.
    return;
}

void SFSimulator::simulate()
{
    SFLOG << "Testy testy " << mCounter;
    sleep(1);
    // Push all updates as a result of simulation into update queue/data structure
    mCounter += 1;
    return;
}

void SFSimulator::resolveAndPush()
{
    // Iterate through all updates, resolving conflicts and applying to data structure
    // Push updates out to anybody that's listening
}
