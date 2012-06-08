#ifndef SHARDFREE_SF_WORKER_H_
#define SHARDFREE_SF_WORKER_H_

#include <string>

class SFLogger;

#define SFLOG Log(logger()).get()

class SFWorker
{
  public:
    SFWorker(const std::string &id, bool logging = true);
    virtual ~SFWorker();

    void start();
    virtual void run() = 0;
    
    const SFLogger &logger() const;
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
    SFLogger *mLoggerp;
};

#endif //SHARDFREE_SF_WORKER_H_