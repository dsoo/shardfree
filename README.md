zero-sim
========

ZeroMQ-based server for real-time distributed simulation and clients

## Mac build instructions

Install zeromq using brew

Build libwebsockets as a submodule

git submodule init
cd libwebsockets
./configure;make # Make install doesn't work
