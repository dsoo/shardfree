#ifndef SHARDFREE_PRESENCE_H_
#define SHARDFREE_PRESENCE_H_

#include "thread.h"

namespace ShardFree
{

class Presence : public Thread
{
  public:
    Presence(const std::string &id = "presence");
    virtual ~Presence();

  protected:
    /*virtual*/ void init();
    /*virtual*/ void run();
  private:
    zmq::socket_t *mRequestSocketp;
    //std::vector<std::string> mSimulators;
};

class PresenceClient
{
  public:
    PresenceClient(const std::string &endpoint = "inproc://presence");
    virtual ~PresenceClient();

    void sendRequest();
  private:
    zmq::socket_t *mPresenceReqp;
};

}
#endif // SHARDFREE_PRESENCE_H_