#ifndef SHARDFREE_SF_SIMULATOR_H_
#define SHARDFREE_SF_SIMULATOR_H_

#include <vector>
#include "sf-worker.h"

class SFLogger;

class SFSimulator : public SFWorker
{
  public:    
    SFSimulator(const std::string &id);
    virtual ~SFSimulator();

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

#endif // SHARDFREE_SF_SIMULATOR_H_