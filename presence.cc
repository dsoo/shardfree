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
  std::cout << "Presence client connecting to " << endpoint << std::endl;
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
      std::cout << "Errno:" << errno << std::endl;
      switch(errno) {
        case EINVAL:
          std::cout << "EINVAL" << std::endl;
          break;
        case EPROTONOSUPPORT:
          std::cout << "EPROTONOSUPPORT" << std::endl;
          break;
        case ENOCOMPATPROTO:
          std::cout << "ENOCOMPATPROTO" << std::endl;
          break;
        case ETERM:
          std::cout << "ETERM" << std::endl;
          break;
        case ENOTSOCK:
          std::cout << "ENOTSOCK" << std::endl;
          break;
        case EMTHREAD:
          std::cout << "EMTHREAD" << std::endl;
          break;
        default:
          std::cout << "UNKNOWN ERROR" << std::endl;
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
  std::cout << "Sending presence request" << std::endl;
  std::string request_str = "presence";
  zmq::message_t message(request_str.size());
  memcpy(message.data(), request_str.data(), request_str.size());
  mPresenceReqp->send(message);

  zmq::message_t response;
  mPresenceReqp->recv(&response);
  std::cout << "Presence response:" << std::string((char*)message.data(), message.size());
}

}
