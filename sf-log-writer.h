#ifndef SHARDFREE_SF_LOG_WRITER_H_
#define SHARDFREE_SF_LOG_WRITER_H_

#include <string>
#include <zmq.hpp>

//
// Outputs logs from the logger asynchronously on a separate thread.
//
class SFLogWriter
{
  public:
    SFLogWriter(const std::string &publisher_name = "inproc://logpub");
    virtual ~SFLogWriter();

    void run();    
  private:
    static void *runWorker(void *argp);    

  private:
    std::string mPublisherName;
    zmq::socket_t *mPublisherp;
};

#endif // SHARDFREE_SF_LOG_WRITER_H