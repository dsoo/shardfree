#ifndef SHARDFREE_SIMULATOR_H_
#define SHARDFREE_SIMULATOR_H_

#include <vector>
#include "worker.h"

namespace ShardFree
{

class Simulator : public Worker
{
  public:
    Simulator(const std::string &id);
    virtual ~Simulator();

    /*virtual*/ void run();
  private:
    void collect();
    void simulate();
    void resolveAndPush();

  private:
    int mCounter;
    zmq::socket_t *mPubSocketp;
    std::vector<zmq::socket_t *> mNeighbors;
};

}

#endif // SHARDFREE_SIMULATOR_H_