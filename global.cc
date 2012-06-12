#include <sstream>
#include <zmq.hpp>
#include "logger.h"
#include "global.h"

namespace ShardFree
{

// Global initialization order isn't guaranteed across compilation units
// Manually initialize this at the start of the app.
zmq::context_t *gZMQContextp = NULL;

zmq::context_t &getZMQContext()
{
  // FIXME: Make this thread safe
  if (gZMQContextp == NULL)
  {
    gZMQContextp = new zmq::context_t(1);
  }
  return *gZMQContextp;
}

std::string getZMQErrorString()
{
 // Not efficient, but it shouldn't need to be
 std::ostringstream oss;
  oss << "Errno:" << errno;
  switch(errno)
  {
    case EINVAL:
      oss << "EINVAL";
      break;
    case EPROTONOSUPPORT:
      oss << "EPROTONOSUPPORT";
      break;
    case ENOCOMPATPROTO:
      oss << "ENOCOMPATPROTO";
      break;
    case ETERM:
      oss << "ETERM";
      break;
    case ENOTSOCK:
      oss << "ENOTSOCK";
      break;
    case EMTHREAD:
      oss << "EMTHREAD";
      break;
    default:
      oss << "UNKNOWN ERROR";
  }
  return oss.str();
}

}
