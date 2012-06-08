# Super-basic Makefile. To be made more awesome once I have stuff up and running
# and it matters.

CXXFILES:=$(shell find . -type f -name '*.cc' -print)
OUTPUTS = build/sf-server build/sf-log-client

SERVER_OBJECTS = sf-server.o sf-worker.o sf-simulator.o sf-global.o sf-logger.o sf-log-publisher.o sf-log-writer.o sf-presence.o sf-log-writer-websocket.o
CLIENT_OBJECTS = sf-log-client.o sf-global.o sf-log-writer.o
OBJECTS = $(SERVER_OBJECTS)
BUILD_DIR = build

WARNINGS = -Wall
LFLAGS = -L./websockets/lib -lzmq ./websockets/lib/libwebsockets.a -lz
CPPFLAGS = $(WARNINGS) -g

all: $(BUILD_DIR) $(OUTPUTS)

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(OBJECTS)

build/sf-server: $(SERVER_OBJECTS)
	g++ $(CPPFLAGS) $(LFLAGS) $^ -o $@

build/sf-log-client: $(CLIENT_OBJECTS)
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
