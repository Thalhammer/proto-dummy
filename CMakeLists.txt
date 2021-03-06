cmake_minimum_required(VERSION 3.10)

project(ProtoDummy)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(Protobuf_DEBUG False)
set(PROTOBUF_GENERATE_CPP_APPEND_PATH False)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

function(PROTOBUF_GENERATE_PB_CPP SRCS HDRS)
  if(NOT ARGN)
    message(SEND_ERROR "Error: PROTOBUF_GENERATE_PB_CPP() called without any proto files")
    return()
  endif()
  
  set(_protobuf_include_path -I ${CMAKE_CURRENT_SOURCE_DIR})

  if(DEFINED PROTOBUF_IMPORT_DIRS)
    foreach(DIR ${Protobuf_IMPORT_DIRS})
      get_filename_component(ABS_PATH ${DIR} ABSOLUTE)
      list(FIND _protobuf_include_path ${ABS_PATH} _contains_already)
      if(${_contains_already} EQUAL -1)
          list(APPEND _protobuf_include_path -I ${ABS_PATH})
      endif()
    endforeach()
  endif()

  set(${SRCS})
  set(${HDRS})
  foreach(FIL ${ARGN})
    get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
    get_filename_component(FIL_DIR ${FIL} DIRECTORY)
    get_filename_component(FIL_WE ${FIL} NAME_WE)

    list(APPEND ${SRCS} "${CMAKE_CURRENT_BINARY_DIR}/${FIL_DIR}/${FIL_WE}.pb.cc")
    list(APPEND ${HDRS} "${CMAKE_CURRENT_BINARY_DIR}/${FIL_DIR}/${FIL_WE}.pb.h")

    add_custom_command(
      OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${FIL_DIR}/${FIL_WE}.pb.cc"
             "${CMAKE_CURRENT_BINARY_DIR}/${FIL_DIR}/${FIL_WE}.pb.h"
      COMMAND  ${Protobuf_PROTOC_EXECUTABLE}
      ARGS --cpp_out=${CMAKE_CURRENT_BINARY_DIR}
           ${_protobuf_include_path} ${ABS_FIL}
      DEPENDS ${ABS_FIL} ${Protobuf_PROTOC_EXECUTABLE}
      COMMENT "Running C++ protocol buffer compiler on ${FIL}"
      VERBATIM)
  endforeach()

  set_source_files_properties(${${SRCS}} ${${HDRS}} PROPERTIES GENERATED TRUE)
  set(${SRCS} ${${SRCS}} PARENT_SCOPE)
  set(${HDRS} ${${HDRS}} PARENT_SCOPE)
endfunction()

PROTOBUF_GENERATE_PB_CPP(PROTO_SRCS PROTO_HDRS
    options.proto
)

add_executable(proto-dummy
  ${PROTO_SRCS}
  ${PROTO_HDRS}
  main.cpp
  dummy_code_generator.cpp
)
target_link_libraries(proto-dummy
  libprotobuf.a
  libprotoc.a
)
target_include_directories(proto-dummy
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)