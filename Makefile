# Super-basic Makefile. To be made more awesome once I have stuff up and running
# and it matters.

CXXFILES:=$(shell find . -type f -name '*.cc' -print)

OUTPUT_ROOT=$(abspath ./build)
OBJ_DIR = $(OUTPUT_ROOT)/obj
3P_DIR = $(OUTPUT_ROOT)/3p
OUTPUTS := server log-client
OUTPUTS := $(addprefix $(OUTPUT_ROOT)/, $(OUTPUTS))

SERVER_OBJECTS := server.o thread.o simulator.o global.o logger.o log-publisher.o log-writer.o presence.o log-writer-websocket.o
SERVER_OBJECTS := $(addprefix $(OBJ_DIR)/, $(SERVER_OBJECTS))
CLIENT_OBJECTS = log-client.o global.o log-writer.o logger.o thread.o
OBJECTS = $(SERVER_OBJECTS)

WARNINGS = -Wall
LFLAGS = -lz -lpthread
CPPFLAGS = $(WARNINGS) -g -I $(3P_DIR)/include

.PHONY: all server submodules libwebsockets zeromq3-x libs

all: $(OBJ_DIR) $(OUTPUTS)
server: $(OUTPUT_ROOT) $(OBJ_DIR) $(OUTPUT_ROOT)/server
vulcan:
	vulcan build -v -s . -p $(OUTPUT_ROOT) -c "make libs && make depend && make server" -o ./shardfree.tgz

$(OUTPUT_ROOT) $(OBJ_DIR):
	mkdir -p $@


submodules:
	git submodule init
	git submodule update

libs: libwebsockets zeromq3-x

libwebsockets:
	mkdir -p $(3P_DIR);
	cd libwebsockets; \
	chmod a+x ./install-sh; \
	./configure --prefix=$(3P_DIR); \
	make install

zeromq3-x:
	mkdir -p $(3P_DIR);
	cd zeromq3-x; \
	./autogen.sh; \
	./configure --prefix=$(3P_DIR); \
	make install

libclean:
	cd libwebsockets; \
	make clean
	cd zeromq3-x; \
	make clean

clean:
	rm -rf $(OBJECTS)

$(OUTPUT_ROOT)/server: $(SERVER_OBJECTS)
	g++ $(CPPFLAGS) $(LFLAGS) $^ $(3P_DIR)/lib/libwebsockets.a $(3P_DIR)/lib/libzmq.a -o $@

$(OUTPUT_ROOT)/log-client: $(CLIENT_OBJECTS)
	g++ $(CPPFLAGS) $(LFLAGS) $^ $(3P_DIR)/lib/libzmq.a -o $@

depend:
	cp /dev/null dependencies.mk
	for F in $(CFILES); do \
		D=`dirname $$F`; \
		B=`basename $$F .c`; \
		$(CC) $(CPPFLAGS) -MM -MT $(OBJ_DIR)/$$B.o -MG $$F \
		 >> dependencies.mk; \
	done
	for F in $(CXXFILES); do \
		D=`dirname $$F`; \
		B=`basename $$F .cc`; \
		$(CXX) $(CPPFLAGS) -MM -MT $(OBJ_DIR)/$$B.o -MG $$F \
		 >> dependencies.mk; \
	done

$(OBJ_DIR)/%.o: %.cc
	$(CXX) $(CPPFLAGS) -c -o $@ $<

-include dependencies.mk
