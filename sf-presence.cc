#include <iostream>

#include "sf-global.h"
#include "sf-logger.h"
#include "sf-presence.h"

#include <vector>

//
// Instance methods
//
SFPresence::SFPresence(const std::string &id) : SFWorker(id)
{
}

SFPresence::~SFPresence()
{
  delete mRequestSocketp;
  mRequestSocketp = NULL;
}

void SFPresence::run()
{
  mRequestSocketp = new zmq::socket_t(context(), ZMQ_REP);
  mRequestSocketp->bind((std::string("inproc://presence") + id()).c_str());

  SFLOG << "Presence server waiting for requests";
  while(1)
  {
    // Listen for requests
    // add - add a new simulator
    // remove - remove an existing simulator
    // list - get a list of all simulators
    
    zmq::message_t message;
    mRequestSocketp->recv(&message);
    SFLOG << "Type:" << std::string((char *)message.data(), message.size());
    mRequestSocketp->recv(&message);
    SFLOG << "Body:" << std::string((char *)message.data(), message.size());
  }
}
//
//SFPresenceClient::SFPresenceClient(const std::string &endpoint) : mPresenceReqp(NULL)
//{
//  mPresenceReqp = new zmq::socket_t(gZMQContextp, ZMQ_REQ);
//  mPresenceReqp->connect(endpoint.c_str());
//}
//
//SFPresenceClient::~SFPresenceClient()
//{
//  delete mPresenceReqp;
//  mPresenceReqp = NULL;
//}
//
//void SFPresenceClient::sendRequest()
//{
//  zmq::message_t message()
//}