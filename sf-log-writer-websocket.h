#ifndef SHARDFREE_SF_LOG_WRITER_WEBSOCKET_H_
#define SHARDFREE_SF_LOG_WRITER_WEBSOCKET_H_

// Writes logs out to a websocket server.

#include <string>
#include <zmq.hpp>

//
// Outputs logs from the logger asynchronously on a separate thread.
//
class SFLogWriterWebsocket
{
  public:
    SFLogWriterWebsocket(const std::string &publisher_name = "inproc://logpub");
    virtual ~SFLogWriterWebsocket();

    void run();
  private:
    static void *runWorker(void *argp);

  private:
    std::string mPublisherName;
    zmq::socket_t *mPublisherp;
};

#endif // SHARDFREE_SF_LOG_WRITER_WEBSOCKET_H_