# Super-basic Makefile. To be made more awesome once I have stuff up and running
# and it matters.

CXXFILES:=$(shell find . -type f -name '*.cc' -print)

OUTPUT_ROOT=build
OBJ_DIR = $(OUTPUT_ROOT)/obj
OUTPUTS := server log-client
OUTPUTS := $(addprefix $(OUTPUT_ROOT)/, $(OUTPUTS))

SERVER_OBJECTS := server.o thread.o simulator.o global.o logger.o log-publisher.o log-writer.o presence.o log-writer-websocket.o
SERVER_OBJECTS := $(addprefix $(OBJ_DIR)/, $(SERVER_OBJECTS))
CLIENT_OBJECTS = log-client.o global.o log-writer.o logger.o thread.o
OBJECTS = $(SERVER_OBJECTS)

WARNINGS = -Wall
LFLAGS = -Bstatic -lzmq -lz ./libwebsockets/lib/.libs/libwebsockets.a
CPPFLAGS = $(WARNINGS) -g -I ./libwebsockets/lib

all: $(OUTPUT_ROOT) $(OBJ_DIR) $(OUTPUTS)

$(BUILD_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJECTS)

$(OUTPUT_ROOT)/server: $(SERVER_OBJECTS)
	g++ $(CPPFLAGS) $(LFLAGS) $^ -o $@

$(OUTPUT_ROOT)/log-client: $(CLIENT_OBJECTS)
	g++ $(CPPFLAGS) $(LFLAGS) $^ -o $@

depend:
	cp /dev/null dependencies.mk
	for F in $(CFILES); do \
		D=`dirname $$F`; \
		B=`basename -s .c $$F`; \
		$(CC) $(CPPFLAGS) -MM -MT $(OBJ_DIR)/$$B.o -MG $$F \
		 >> dependencies.mk; \
	done
	for F in $(CXXFILES); do \
		D=`dirname $$F`; \
		B=`basename -s .cc $$F`; \
		$(CXX) $(CPPFLAGS) -MM -MT $(OBJ_DIR)/$$B.o -MG $$F \
		 >> dependencies.mk; \
	done

$(OBJ_DIR)/%.o: %.cc
	$(CXX) $(CPPFLAGS) -c -o $@ $<

-include dependencies.mk
