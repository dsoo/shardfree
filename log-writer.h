#ifndef SHARDFREE_LOG_WRITER_H_
#define SHARDFREE_LOG_WRITER_H_

#include <string>
#include "zmq.hpp"

#include "thread.h"

namespace ShardFree
{
//
// Outputs logs from the logger asynchronously on a separate thread.
//
class LogWriter : public Thread
{
  public:
    LogWriter(const std::string &publisher_name = "inproc://logpub");
    virtual ~LogWriter();

  protected:
    /*virtual*/ void init();
    /*virtual*/ void run();

  private:
    std::string mPublisherName;
    zmq::socket_t *mPublisherp;
};
}
#endif // SHARDFREE_LOG_WRITER_H