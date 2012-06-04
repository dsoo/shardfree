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
#include "sf-log-writer.h"

//
// FIXME: Do all the memory management stuff properly.
//

int main(int argc, char *argv[])
{
  //  Prepare our context and sockets
  gZMQContextp = new zmq::context_t(1);

  // Spawn our log output worker
  SFLogWriter log_writer(argv[1]);
  while (1)
  {
    sleep(1);
  }
  return 0;
}