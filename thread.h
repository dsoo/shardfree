#ifndef SHARDFREE_THREAD_H_
#define SHARDFREE_THREAD_H_

#include <string>

namespace ShardFree
{

class Thread
{
  public:
    Thread(const std::string &id);
    virtual ~Thread();

    void start();

    const std::string &id() const;

  protected:
    // Do not ever directly call these yourself.
    virtual void init() = 0;
    virtual void run() = 0;

  private:
    Thread(const Thread &); // No copy constructor


  protected:
    zmq::context_t &context() const;

    static void *runThread(void *argp);
  private:
    std::string mID;
    zmq::context_t &mContext;
};

}

#endif //SHARDFREE_WORKER_H_