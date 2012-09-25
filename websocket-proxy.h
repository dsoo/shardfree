#ifndef SHARDFREE_WEBSOCKET_PROXY_H_
#define SHARDFREE_WEBSOCKET_PROXY_H_

// Writes logs out to a websocket server.

#include <string>
#include <vector>
#include "zmq.hpp"

#include "thread.h"

namespace ShardFree
{

//
// Outputs logs from the logger asynchronously on a separate thread.
//
class WebsocketProxy : public Thread
{
  public:
    WebsocketProxy(const int port = 7890);
    virtual ~WebsocketProxy();

    void addPublisher(const std::string &name);
  protected:
    /*virtual*/ void init();
    /*virtual*/ void run();

  private:

    std::vector<zmq::socket_t *> mPublishers;

    //std::string mPublisherName;
    //zmq::socket_t *mPublisherp;
    int mPort;
};

}

#endif // SHARDFREE_LOG_WRITER_WEBSOCKET_H_