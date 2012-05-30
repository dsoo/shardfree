# Super-basic Makefile. To be made more awesome once I have stuff up and running
# and it matters.

CXXFILES:=$(shell find . -type f -name '*.cc' -print)
OUTPUTS = build/sf-server build/sf-client

SERVER_OBJECTS = sf-server.o
CLIENT_OBJECTS = sf-client.o
OBJECTS = $(SERVER_OBJECTS) $(CLIENT_OBJECTS)

WARNINGS = -Wall
LFLAGS = -lzmq
CPPFLAGS = $(WARNINGS)

all: $(OUTPUTS)

clean:
	rm -rf $(OUTPUTS) $(OBJECTS)

build/sf-server: $(SERVER_OBJECTS)
	g++ $(CPPFLAGS) $(LFLAGS) $^ -o $@

build/sf-client: $(CLIENT_OBJECTS)
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
		B=`basename -s .cpp $$F`; \
		$(CXX) $(CPPFLAGS) -MM -MT $$D/$$B.o -MG $$F \
		 >> dependencies.mk; \
	done