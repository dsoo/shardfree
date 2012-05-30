//
//  Hello World server in C++
//  Binds REP socket to tcp://*:5555
//  Expects "Hello" from client, replies with "World"
//
#include <pthread.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <zmq.hpp>

void init()
{
    // Start up SUB socket to listen to trusted neighbors
    // Start up SUB socket to listen to clients
    // Start up PUB socket to push updates to listeners
    // Find neighbors
    // Connect to neighbors
}

void collect()
{
    // Collect updates from neighbors
    std::cout << "Collecting" << endl;

    // Pull messages until all updates for current frame received from neighbors or we time out.
    return;
}

void simulate()
{
    std::cout << "Simulating" << endl;

    // Push all updates as a result of simulation into update queue/data structure
    return;
}

void resolve_and_push_updates()
{
    // Iterate through all updates, resolving conflicts and applying to data structure
    // Push updates out to anybody that's listening
}

int main()
{
    //  Prepare our context and sockets
    zmq::context_t context(1);
    zmq::socket_t clients(context, ZMQ_ROUTER);
    clients.bind("tcp://*:5555");
    zmq::socket_t workers(context, ZMQ_DEALER);
    workers.bind("inproc://workers");

    //  Launch pool of worker threads, same as number of CPUs
    for (int thread_nbr = 0; thread_nbr != 5; thread_nbr++) {
        pthread_t worker;
        pthread_create(&worker, NULL, worker_routine, (void *) &context);
    }
    //  Connect work threads to client threads via a queue
    zmq::device(ZMQ_QUEUE, clients, workers);
    return 0;
}