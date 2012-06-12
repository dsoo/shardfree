#include <pthread.h>
#include <iostream>

#include "libwebsockets.h"

#include "global.h"
#include "logger.h"
#include "log-writer-websocket.h"

namespace ShardFree
{

//
// FIXME: Major refactoring of all kinds needed here.
// This is cribbed from the libwebsockets sample server code, which is frankly
// sort of terrible. I'd like to refactor libwebsockets at some point to clean it
// up, at a minimum putting a C++ wrapper around it to hide the gory implementation.
//

static int callback_http(struct libwebsocket_context * context,
                         struct libwebsocket *wsi,
                         enum libwebsocket_callback_reasons reason, void *user,
                         void *in, size_t len)
{
  char client_name[128];
  char client_ip[128];

  switch (reason)
  {
  case LWS_CALLBACK_HTTP:
    fprintf(stderr, "serving HTTP URI %s\n", (char *)in);

    if (in && strcmp((char *)in, "/favicon.ico") == 0)
    {
      if (libwebsockets_serve_http_file(wsi, "./favicon.ico", "image/x-icon"))
      {
        fprintf(stderr, "Failed to send favicon\n");
      }
      break;
    }

    // Talking HTTP to the port will always return the logger html file.

    /* send the script... when it runs it'll start websockets */
    if (libwebsockets_serve_http_file(wsi, "./log.html", "text/html"))
    {
      fprintf(stderr, "Failed to send HTTP file\n");
    }
    break;

    /*
     * callback for confirming to continue with client IP appear in
     * protocol 0 callback since no websocket protocol has been agreed
     * yet.  You can just ignore this if you won't filter on client IP
     * since the default uhandled callback return is 0 meaning let the
     * connection continue.
     */

  case LWS_CALLBACK_FILTER_NETWORK_CONNECTION:
    libwebsockets_get_peer_addresses((int)(long)user, client_name,
                 sizeof(client_name), client_ip, sizeof(client_ip));

    fprintf(stderr, "Received network connect from %s (%s)\n",
                                            client_name, client_ip);

    /* if we returned non-zero from here, we kill the connection */
    break;

  default:
    break;
  }

  return 0;
}

static int callback_log(struct libwebsocket_context * context,
                        struct libwebsocket *wsi,
                        enum libwebsocket_callback_reasons reason,
                        void *user, void *in, size_t len)
{
  switch (reason)
  {
  case LWS_CALLBACK_ESTABLISHED:
    fprintf(stderr, "LWS_CALLBACK_ESTABLISHED\n");
    break;

  case LWS_CALLBACK_SERVER_WRITEABLE:
    break;

  case LWS_CALLBACK_BROADCAST:
    {
      // FIXME: This should probably use binary, not text?
      int n = libwebsocket_write(wsi, (unsigned char *)in, len, LWS_WRITE_TEXT);
      if (n < 0)
      {
        fprintf(stderr, "broadcast write failed\n");
      }
    }
    break;

  case LWS_CALLBACK_RECEIVE:
    // FIXME: Should validate payload, then route
    // and do the appropriate thing based on the type of
    // connection.
    break;

  case LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION:
    break;

  default:
    break;
  }

  return 0;
}


LogWriterWebsocket::LogWriterWebsocket(const std::string &publisher_name) :
  Thread("LogWriterWebsocket"),
  mPublisherName(publisher_name),
  mPublisherp(NULL)
{
}

LogWriterWebsocket::~LogWriterWebsocket()
{
  delete mPublisherp;
  mPublisherp = NULL;
}

void LogWriterWebsocket::init()
{
  mPublisherp = new zmq::socket_t(getZMQContext(), ZMQ_SUB);
  mPublisherp->setsockopt(ZMQ_SUBSCRIBE, "", 0);

  bool connected = false;
  while (!connected)
  {
    try
    {
      mPublisherp->connect(mPublisherName.c_str());
      connected = true;
    }
    catch(...)
    {
      // FIXME: Catch JUST the exception that the C++ ZeroMQ bindings throw
      // FIXME: Makes an assumption here about logging working, which
      // is probably okay but might not be.
      SFLOG << getZMQErrorString();
      sleep(1);
    }
  }
}

void LogWriterWebsocket::run()
{

  int port = 7681;
  int opts = 0;
  const char *interface = NULL;
  struct libwebsocket_protocols protocols[] = {
    /* first protocol must always be HTTP handler */

    {
      "http-only",              /* name */
      callback_http,            /* callback */
      0                 /* per_session_data_size */
    },
    {
      "log",
      callback_log,
      0, // Size of per session data
    },
    {
      NULL, NULL, 0             /* End of list */
    }
  };

  // Initialize the Websocket context
  struct libwebsocket_context *context = NULL;
  context = libwebsocket_create_context(port, interface, protocols,
                                        libwebsocket_internal_extensions,
                                        NULL, NULL, NULL, -1, -1, opts);
  while (1)
  {
    // Poll for incoming connections from websocket listeners and
    // simultaneously publish logs.
    libwebsocket_service(context, 15); // Assuming a 60 Hz frame rate, try to handle at least once a frame

    zmq::message_t message;
    // Now, pull all waiting messages from the queue and push them to any waiting clients
    while (mPublisherp->recv(&message, ZMQ_NOBLOCK))
    {
      // FIXME: Validate lengths and buffer sizes
      unsigned char buf[LWS_SEND_BUFFER_PRE_PADDING + 1024 + LWS_SEND_BUFFER_POST_PADDING];
      unsigned char *p = &buf[LWS_SEND_BUFFER_PRE_PADDING];
      memcpy(p, message.data(), message.size());
      libwebsockets_broadcast(&protocols[1], p, message.size());
    }
  }
}

}
