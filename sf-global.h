#ifndef SHARDFREE_SF_GLOBAL_H_
#define SHARDFREE_SF_GLOBAL_H_

#include <zmq.hpp>

// FIXME: This should be a singleton instead of a global.
extern zmq::context_t *gZMQContextp;

#endif // SHARDFREE_SF_GLOBAL_H