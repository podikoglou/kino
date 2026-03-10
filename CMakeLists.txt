cmake_minimum_required(VERSION 3.15)

project(kino VERSION 1.0
             LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 20)

add_library(libkino 
  src/vm.hpp src/isa.hpp src/value.hpp 
  src/vm.cpp src/isa.cpp)

add_executable(kino src/bin/kino.cpp)
target_link_libraries(kino PUBLIC libkino)

add_executable(kino-as src/bin/kino-as.cpp)
target_link_libraries(kino-as PUBLIC libkino)

