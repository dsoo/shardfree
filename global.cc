#include <zmq.hpp>
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
}
