#ifndef SHARDFREE_LOG_WRITER_WEBSOCKET_H_
#define SHARDFREE_LOG_WRITER_WEBSOCKET_H_

// Writes logs out to a websocket server.

#include <string>
#include <zmq.hpp>

namespace ShardFree
{

//
// Outputs logs from the logger asynchronously on a separate thread.
//
class LogWriterWebsocket
{
  public:
    LogWriterWebsocket(const std::string &publisher_name = "inproc://logpub");
    virtual ~LogWriterWebsocket();

    void run();
  private:
    static void *runWorker(void *argp);

  private:
    std::string mPublisherName;
    zmq::socket_t *mPublisherp;
};

}

#endif // SHARDFREE_LOG_WRITER_WEBSOCKET_H_