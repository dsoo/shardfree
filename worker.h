#ifndef SHARDFREE_WORKER_H_
#define SHARDFREE_WORKER_H_

#include <string>

namespace ShardFree
{

class Logger;

class Worker
{
  public:
    Worker(const std::string &id, bool logging = true);
    virtual ~Worker();

    void start();
    virtual void run() = 0;

    const ShardFree::Logger &logger() const;
    const std::string &id() const;
  protected:
    zmq::context_t &context() const;
  private:
    void init();

    static void *runWorker(void *argp);
  private:
    std::string mID;
    zmq::context_t *mContextp;
    bool mLogging;
    ShardFree::Logger *mLoggerp;
};

}

#endif //SHARDFREE_WORKER_H_