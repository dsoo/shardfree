#ifndef SHARDFREE_LOGGER_H_
#define SHARDFREE_LOGGER_H_

#include <sstream>

//
// ZeroMQ-based logging library for fast, functional multithreaded debugging.
//
//

#define SFLOG ShardFree::Log().get()

namespace ShardFree
{
class Logger;

class Log
{
public:
  Log();
  virtual ~Log();
  std::ostringstream& get();
private:
  std::ostringstream mOSS;
private:
  Log(const Log&);
  Log& operator =(const Log&);
};

// Usage

class Logger
{
  public:
    Logger(const std::string &collector_name = "inproc://logger");
    virtual ~Logger();

    static Logger &get();

    void setPrefix(const std::string &prefix);
    void output(const std::string &str) const;
  private:
    zmq::socket_t *mCollectorp;
    std::string mPrefix;
};
}

#endif // SHARDFREE_LOGGER_H_