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
#include "sf-worker.h"

//
// FIXME: Do all the memory management stuff properly.
//

int main()
{
    //  Prepare our context and sockets
    gZMQContextp = new zmq::context_t(1);

    //zmq::socket_t clients(*gZMQContextp, ZMQ_ROUTER);
    //clients.bind("tcp://*:5555");
    //zmq::socket_t worker_sockets(*gZMQContextp, ZMQ_DEALER);
    //worker_sockets.bind("inproc://workers");

    // Spawn our log output worker
    SFLogOutput log_output = SFLogOutput();

    SFLogger logger;
    while (1)
    {
      sleep(1);    
      logger.log();      
    }
    
    //std::vector<SFWorker *> workers;
    ////  Launch pool of worker threads, same as number of CPUs
    //for (int thread_nbr = 0; thread_nbr != 5; thread_nbr++) {
    //  workers.push_back(new SFWorker());
    //}
    ////  Connect work threads to client threads via a queue
    //zmq::device(ZMQ_QUEUE, clients, worker_sockets);
    return 0;
}