#ifndef SHARDFREE_SF_PRESENCE_H_
#define SHARDFREE_SF_PRESENCE_H_

#include "sf-worker.h"

class SFPresence : public SFWorker
{
  public:
    SFPresence(const std::string &id = "presence");
    virtual ~SFPresence();
    
    /*virtual*/ void run();
  private:
    zmq::socket_t *mRequestSocketp;
    //std::vector<std::string> mSimulators;
};

class SFPresenceClient
{
  public:
    SFPresenceClient(const std::string &endpoint);
    virtual ~SFPresenceClient();
    
    void sendRequest();
  private:
    zmq::socket_t *mPresenceReqp;
}

#endif // SHARDFREE_SF_PRESENCE_H_