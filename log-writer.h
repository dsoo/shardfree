#ifndef SHARDFREE_LOG_WRITER_H_
#define SHARDFREE_LOG_WRITER_H_

#include <string>
#include <zmq.hpp>

namespace ShardFree
{
//
// Outputs logs from the logger asynchronously on a separate thread.
//
class LogWriter
{
  public:
    LogWriter(const std::string &publisher_name = "inproc://logpub");
    virtual ~LogWriter();

    void run();
  private:
    static void *runThread(void *argp);

  private:
    std::string mPublisherName;
    zmq::socket_t *mPublisherp;
};
}
#endif // SHARDFREE_LOG_WRITER_H