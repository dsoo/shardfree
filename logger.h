#ifndef SHARDFREE_LOGGER_H_
#define SHARDFREE_LOGGER_H_

#include <sstream>

//
// ZeroMQ-based logging library for fast, functional multithreaded debugging.
//
//

namespace ShardFree
{
class Logger;

class Log
{
public:
  Log(const ShardFree::Logger &logger);
  virtual ~Log();
  std::ostringstream& get();
private:
  const ShardFree::Logger &logger;
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

    void setPrefix(const std::string &prefix);
    void output(const std::string &str) const;
  private:
    zmq::socket_t *mCollectorp;
    std::string mPrefix;
};
}

#endif // SHARDFREE_LOGGER_H_