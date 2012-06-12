zero-sim
========

ZeroMQ-based server for real-time distributed simulation

Note that this is currently HIGHLY experimental right now. And doesn't actually do
very much.

In fact, all it can do is remote log publishing via WebSocket, which I guess is sort of
neat, but not particularly useful yet.

There's virtually no error handling.
There's no tests.
Expect and assume all the APIs to change or disappear.

## Mac build instructions

I haven't done anything beyond the minimum to get things up and running here. Once
I decide to deploy this on something other than one of my development Macs I'll put
some real work into the build tools. In any case:

Install zeromq using brew

Build libwebsockets as a submodule

    git submodule init
    cd libwebsockets
    ./configure;make # Make install doesn't work

To build, run:
    make depend
    make

To clean, run:
    make clean

## Running
To run the server:

    ./server # Runs the server

All it does is run some (very) fake simulation threads which output logs.

You can view the logs by connecting to http://localhost:7890