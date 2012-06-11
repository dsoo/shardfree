#include <iostream>

#include "global.h"
#include "logger.h"
#include "presence.h"

#include <vector>

namespace ShardFree
{
//
// Instance methods
//
Presence::Presence(const std::string &id) : Worker(id)
{
}

Presence::~Presence()
{
  delete mRequestSocketp;
  mRequestSocketp = NULL;
}

void Presence::run()
{
  mRequestSocketp = new zmq::socket_t(context(), ZMQ_REP);
  mRequestSocketp->bind(std::string("inproc://presence").c_str());

  SFLOG << "Presence server waiting for requests";
  while(1)
  {
    // Listen for requests
    // add - add a new simulator
    // remove - remove an existing simulator
    // list - get a list of all simulators

    zmq::message_t message;
    mRequestSocketp->recv(&message);
    SFLOG << "Message size:" << message.size();
    SFLOG << "Presence Message:" << std::string((char *)message.data(), message.size());
    //mRequestSocketp->recv(&message);
    //SFLOG << "Body:" << std::string((char *)message.data(), message.size());

    std::string response_str("presence_response");
    zmq::message_t response(response_str.size());
    memcpy(response.data(), response_str.data(), response_str.size());
    mRequestSocketp->send(response);
  }
}

PresenceClient::PresenceClient(const std::string &endpoint) : mPresenceReqp(NULL)
{
  SFLOG << "Presence client connecting to " << endpoint;
  mPresenceReqp = new zmq::socket_t(*gZMQContextp, ZMQ_REQ);
  bool connected = false;
  while (!connected)
  {
    try
    {
      mPresenceReqp->connect(endpoint.c_str());
      connected = true;
    }
    catch(...)
    {
      SFLOG << "Errno:" << errno;
      switch(errno)
      {
        case EINVAL:
          SFLOG << "EINVAL";
          break;
        case EPROTONOSUPPORT:
          SFLOG << "EPROTONOSUPPORT";
          break;
        case ENOCOMPATPROTO:
          SFLOG << "ENOCOMPATPROTO";
          break;
        case ETERM:
          SFLOG << "ETERM";
          break;
        case ENOTSOCK:
          SFLOG << "ENOTSOCK";
          break;
        case EMTHREAD:
          SFLOG << "EMTHREAD";
          break;
        default:
          SFLOG << "UNKNOWN ERROR";
      }
      sleep(1);
    }
  }
}

PresenceClient::~PresenceClient()
{
  delete mPresenceReqp;
  mPresenceReqp = NULL;
}

void PresenceClient::sendRequest()
{
  SFLOG << "Sending presence request";
  std::string request_str = "presence";
  zmq::message_t message(request_str.size());
  memcpy(message.data(), request_str.data(), request_str.size());
  mPresenceReqp->send(message);

  zmq::message_t response;
  mPresenceReqp->recv(&response);
  SFLOG << "Presence response:" << std::string((char*)message.data(), message.size());
}

}
