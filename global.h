#ifndef SHARDFREE_GLOBAL_H_
#define SHARDFREE_GLOBAL_H_

#include <zmq.hpp>

namespace ShardFree
{
  zmq::context_t &getZMQContext();
}

#endif // SHARDFREE_GLOBAL_H