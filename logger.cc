#include <zmq.hpp>
#include "logger.h"
#include "global.h"

namespace ShardFree
{
// Logging implementation cribbed (and simplified) from here: http://www.drdobbs.com/cpp/201804215

Log::Log(const ShardFree::Logger &logger) : logger(logger)
{
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
    logger.output(mOSS.str());
  }
}

Logger::Logger(const std::string &collector_name)
{
  mCollectorp = new zmq::socket_t(*gZMQContextp, ZMQ_PUSH);
  mCollectorp->connect(collector_name.c_str());

  //try
  //{
  //  mCollectorp->connect(collector_name.c_str());
  //}
  //catch(...)
  //{
  //  std::cout << "Errno:" << errno << std::endl;
  //  switch(errno) {
  //    case EINVAL:
  //      std::cout << "EINVAL" << std::endl;
  //      break;
  //    case EPROTONOSUPPORT:
  //      std::cout << "EPROTONOSUPPORT" << std::endl;
  //      break;
  //    case ENOCOMPATPROTO:
  //      std::cout << "ENOCOMPATPROTO" << std::endl;
  //      break;
  //    case ETERM:
  //      std::cout << "ETERM" << std::endl;
  //      break;
  //    case ENOTSOCK:
  //      std::cout << "ENOTSOCK" << std::endl;
  //      break;
  //    case EMTHREAD:
  //      std::cout << "EMTHREAD" << std::endl;
  //      break;
  //    default:
  //      std::cout << "UNKNOWN ERROR" << std::endl;
  //  }
  //  exit(-1);
  //}
}

Logger::~Logger()
{
  delete mCollectorp;
  mCollectorp = NULL;
}

void Logger::setPrefix(const std::string &prefix)
{
  mPrefix = prefix;
}

void Logger::output(const std::string &str) const
{
  zmq::message_t line(str.size() + mPrefix.size());
  memcpy((void *)line.data(), mPrefix.data(), mPrefix.size());
  memcpy((void *)((char *)line.data() + mPrefix.size()), str.data(), str.size());
  mCollectorp->send(line);
}

}
