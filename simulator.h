#ifndef SHARDFREE_SIMULATOR_H_
#define SHARDFREE_SIMULATOR_H_

#include <vector>
#include "thread.h"

namespace ShardFree
{

class Simulator : public Thread
{
  public:
    Simulator(const std::string &id);
    virtual ~Simulator();

  protected:
    /*virtual*/ void init();
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