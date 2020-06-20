proto-dummy
=========
Protobuf generator example plugin.
This is designed as a base to start new plugins of and is thus designed as a minimum example.

Build
-----
Standard cmake build, i.e.

mkdir build && cd build

cmake .. && make

Dependencies
------------
libprotobuf-dev

libprotoc-dev

Usage
-----
Example given:

protoc --plugin=protoc-gen-dummy="./build/proto-dummy" --proto_path=proto --dummy_out=out  test.proto 
