#ifndef SHARDFREE_LOG_PUBLISHER_H_
#define SHARDFREE_LOG_PUBLISHER_H_

#include <string>
#include <zmq.hpp>

namespace ShardFree
{

class LogPublisher
{
  public:
    LogPublisher(const std::string &collector_name = "inproc://logger",
                   const std::string &publisher_name = "inproc://logpub");
    virtual ~LogPublisher();

    void run();

  private:
    static void *runWorker(void *argp);

  private:
    std::string mCollectorName;
    std::string mPublisherName;
    zmq::socket_t *mCollectorp;
    zmq::socket_t *mPublisherp;
};

}

#endif // SHARDFREE_LOG_PUBLISHER_H_