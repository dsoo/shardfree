#ifndef SHARDFREE_SF_WORKER_H_
#define SHARDFREE_SF_WORKER_H_

#include <string>

class SFLogger;

#define SFLOG Log(logger()).get()

class SFWorker
{
  public:
    SFWorker(const std::string &id);
    virtual ~SFWorker();

    void start();
    virtual void run() = 0;
    
    const SFLogger &logger();
  private:
    void init();
    
    static void *runWorker(void *argp);
  private:
    std::string mID;
    SFLogger *mLoggerp;
};

#endif //SHARDFREE_SF_WORKER_H_