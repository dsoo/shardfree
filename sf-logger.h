#ifndef SHARDFREE_SF_LOGGER_H_
#define SHARDFREE_SF_LOGGER_H_

#include <sstream>

//
// ZeroMQ-based logging library for fast, functional multithreaded debugging.
//
//

class SFLogger;

class Log
{
public:
  Log(const SFLogger &logger);
  virtual ~Log();
  std::ostringstream& get();
private:
  const SFLogger &logger;
  std::ostringstream mOSS;
private:
  Log(const Log&);
  Log& operator =(const Log&);
};

// Usage

class SFLogger
{
  public:
    SFLogger(const std::string &collector_name = "inproc://logger");
    virtual ~SFLogger();

    void setPrefix(const std::string &prefix);
    void output(const std::string &str) const;
  private:
    zmq::socket_t *mCollectorp;
    std::string mPrefix;
};

#endif // SHARDFREE_SF_LOGGER_H_