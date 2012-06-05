#ifndef SHARDFREE_SF_ZMQ_H_
#define SHARDFREE_SF_ZMQ_H_

//
// Better bindings for ZeroMQ for C++ than the default ones, which are not
// very C++-like.
//

class ZMQContext
{
  public:
    static ZMQContext *context(); // Global context
    ZMQSocket *getSocket() const;
}

class ZMQSocket
{
  public:
    bind(const std::string &endpoint);
    connect(const std::string &endpoint);
    send();
    recv();
};

class ZMQMessage
{
};

#endif // SHARDFREE_ZMQ_H_