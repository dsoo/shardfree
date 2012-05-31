#ifndef SHARDFREE_SF_LOGGER_H_
#define SHARDFREE_SF_LOGGER_H_

#include <sstream>

//
// ZeroMQ-based logging library for fast, functional multithreaded debugging.
//
//

namespace zmq
{
  class socket_t;
}

class SFLogger;

class Log
{
public:
  Log(SFLogger &logger);
  virtual ~Log();
  std::ostringstream& get();
private:
  SFLogger &logger;
  std::ostringstream os;
private:
  Log(const Log&);
  Log& operator =(const Log&);
};

// Usage

class SFLogger
{
  public:
    SFLogger();
    virtual ~SFLogger();

    void output(const std::string &str);
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