cmake_minimum_required(VERSION 3.22)
project(rb-tree-cxx)

set(CMAKE_CXX_STANDARD 14)

add_library(rb-tree-cxx OBJECT ./src/rb_tree.cpp)
target_include_directories(rb-tree-cxx PUBLIC ./include)