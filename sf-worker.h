#ifndef SHARDFREE_SF_WORKER_H_
#define SHARDFREE_SF_WORKER_H_

class SFWorker
{
  public:
    SFWorker();
    virtual ~SFWorker();
};

void spawn_worker(zmq::context_t *contextp);

#endif //SHARDFREE_SF_WORKER_H_