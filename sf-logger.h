#ifndef SHARDFREE_SF_LOGGER_H_
#define SHARDFREE_SF_LOGGER_H_

//
// ZeroMQ-based logging library for fast, functional multithreaded debugging.
// 
//

namespace zmq
{
  class socket_t;
}

class SFLogger
{
  public:
    SFLogger();
    virtual ~SFLogger();
    
    void log();
  private:
    zmq::socket_t *socketp;
};

//
// Outputs logs from the logger asynchronously on a separate thread.
//
class SFLogOutput
{
  public:
    SFLogOutput();
    virtual ~SFLogOutput();
};

#endif // SHARDFREE_SF_LOGGER_H_