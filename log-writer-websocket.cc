#include <pthread.h>
#include <iostream>

#include "libwebsockets.h"

#include "global.h"
#include "logger.h"
#include "log-writer-websocket.h"

namespace ShardFree
{

static int callback_http(struct libwebsocket_context * context,
                         struct libwebsocket *wsi,
                         enum libwebsocket_callback_reasons reason, void *user,
                         void *in, size_t len)
{
	char client_name[128];
	char client_ip[128];

	switch (reason) {
	case LWS_CALLBACK_HTTP:
		fprintf(stderr, "serving HTTP URI %s\n", (char *)in);

		if (in && strcmp((char *)in, "/favicon.ico") == 0) {
			if (libwebsockets_serve_http_file(wsi, "./favicon.ico", "image/x-icon"))
      {
				fprintf(stderr, "Failed to send favicon\n");
			}
			break;
		}

    // Talking HTTP to the port will always return the logger html file.

		/* send the script... when it runs it'll start websockets */
		if (libwebsockets_serve_http_file(wsi, "./log.html", "text/html")) {
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

static int
callback_log(struct libwebsocket_context * context,
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
  mPublisherName(publisher_name),
  mPublisherp(NULL)
{
  // Create a worker thread that listens to the logger socket and prints the output
  pthread_t worker;

  // FIXME: This should be uniquely named
  zmq::socket_t ready_socket(getZMQContext(), ZMQ_PULL);
  ready_socket.bind("inproc://writerwebsocketready");

  pthread_create (&worker, NULL, runWorker, this);
  zmq::message_t message;

  // Waits until ZMQ sockets are abound before returning.
  ready_socket.recv(&message);
}

LogWriterWebsocket::~LogWriterWebsocket()
{
  delete mPublisherp;
  mPublisherp = NULL;
}

void LogWriterWebsocket::run()
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
      SFLOG << "Errno:" << errno;
      switch(errno)
      {
        case EINVAL:
          SFLOG << "EINVAL";
          break;
        case EPROTONOSUPPORT:
          SFLOG << "EPROTONOSUPPORT";
          break;
        case ENOCOMPATPROTO:
          SFLOG << "ENOCOMPATPROTO";
          break;
        case ETERM:
          SFLOG << "ETERM";
          break;
        case ENOTSOCK:
          SFLOG << "ENOTSOCK";
          break;
        case EMTHREAD:
          SFLOG << "EMTHREAD";
          break;
        default:
          SFLOG << "UNKNOWN ERROR";
      }
      sleep(1);
    }
  }

  int port = 7681;
  int opts = 0;
  const char *interface = NULL;
  struct libwebsocket_protocols protocols[] = {
    /* first protocol must always be HTTP handler */

    {
      "http-only",		/* name */
      callback_http,		/* callback */
      0			/* per_session_data_size */
    },
    {
      "log",
      callback_log,
      0, // Size of per session data
    },
    {
      NULL, NULL, 0		/* End of list */
    }
  };

  // Initialize the Websocket context
  struct libwebsocket_context *context = NULL;
  context = libwebsocket_create_context(port, interface, protocols,
                                        libwebsocket_internal_extensions,
                                        NULL, NULL, NULL, -1, -1, opts);

  // Now that we're bound, tell the main thread that we're ready for use
  {
    zmq::socket_t sender(getZMQContext(), ZMQ_PUSH);
    sender.connect("inproc://writerwebsocketready");
    zmq::message_t message;
    sender.send(message);
  }

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

void *LogWriterWebsocket::runWorker(void *argp)
{
  auto log_writerp = (LogWriterWebsocket *)argp;
  log_writerp->run();
  return 0;
}

}
