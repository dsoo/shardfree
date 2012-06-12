# Super-basic Makefile. To be made more awesome once I have stuff up and running
# and it matters.

CXXFILES:=$(shell find . -type f -name '*.cc' -print)
OUTPUTS = server log-client

SERVER_OBJECTS = server.o thread.o simulator.o global.o logger.o log-publisher.o log-writer.o presence.o log-writer-websocket.o
CLIENT_OBJECTS = log-client.o global.o log-writer.o logger.o thread.o
OBJECTS = $(SERVER_OBJECTS)
BUILD_DIR = build

WARNINGS = -Wall
LFLAGS = -lzmq -lz ./libwebsockets/lib/.libs/libwebsockets.a
CPPFLAGS = $(WARNINGS) -g -I ./libwebsockets/lib

all: $(BUILD_DIR) $(OUTPUTS)

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(OBJECTS)

server: $(SERVER_OBJECTS)
	g++ $(CPPFLAGS) $(LFLAGS) $^ -o $@

log-client: $(CLIENT_OBJECTS)
	g++ $(CPPFLAGS) $(LFLAGS) $^ -o $@

depend:
	cp /dev/null dependencies.mk
	for F in $(CFILES); do \
		D=`dirname $$F`; \
		B=`basename -s .c $$F`; \
		$(CC) $(CPPFLAGS) -MM -MT $$D/$$B.o -MG $$F \
		 >> dependencies.mk; \
	done
	for F in $(CXXFILES); do \
		D=`dirname $$F`; \
		B=`basename -s .cc $$F`; \
		$(CXX) $(CPPFLAGS) -MM -MT $$D/$$B.o -MG $$F \
		 >> dependencies.mk; \
	done

include dependencies.mk
