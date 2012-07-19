#include "zmq.hpp"
#include "logger.h"
#include "global.h"

namespace ShardFree
{
// Logging implementation cribbed (and simplified) from here: http://www.drdobbs.com/cpp/201804215

__thread Logger *tLoggerp = NULL;

Log::Log()
{
  if (!tLoggerp)
  {
    // Initialize the per-thread logger
    // FIXME: Need to clean this up when the thread goes away, somehow.
    tLoggerp = new ShardFree::Logger;
    // FIXME: Would be nice to set the prefix to something useful.
  }
}

std::ostringstream& Log::get()
{
   return mOSS;
}

Log::~Log()
{
  if (mOSS.str().size())
  {
    mOSS << std::endl;
    tLoggerp->output(mOSS.str());
  }
}

Logger::Logger(const std::string &collector_name)
{
  mCollectorp = new zmq::socket_t(getZMQContext(), ZMQ_PUSH);

  // FIXME: Properly handle errors for this connect
  mCollectorp->connect(collector_name.c_str());
}

Logger::~Logger()
{
  delete mCollectorp;
  mCollectorp = NULL;
}

/*static*/ Logger &Logger::get()
{
  if (!tLoggerp)
  {
    // FIXME: Need to clean this up when the thread goes away, somehow.
    tLoggerp = new Logger;
  }
  return *tLoggerp;
}

void Logger::setPrefix(const std::string &prefix)
{
  mPrefix = prefix;
}

void Logger::output(const std::string &str) const
{
  // FIXME: These memcpys are sketchy and ugly
  zmq::message_t line(str.size() + mPrefix.size());
  memcpy((void *)line.data(), mPrefix.data(), mPrefix.size());
  memcpy((void *)((char *)line.data() + mPrefix.size()), str.data(), str.size());
  mCollectorp->send(line);
}

}
