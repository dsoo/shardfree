#include <iostream>

#include "sf-global.h"
#include "sf-logger.h"
#include "sf-simulator.h"

//
// Class methods
//
SFSimulator *SFSimulator::create()
{
  return new SFSimulator;
}

//
// Instance methods
//
SFSimulator::SFSimulator() : counter(0), loggerp(NULL)
{
  loggerp = new SFLogger();
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
    Log(*loggerp).get() << "Testy testy " << counter;
    sleep(1);
    // Push all updates as a result of simulation into update queue/data structure
    counter += 1;
    return;
}

void SFSimulator::resolveAndPush()
{
    // Iterate through all updates, resolving conflicts and applying to data structure
    // Push updates out to anybody that's listening
}
