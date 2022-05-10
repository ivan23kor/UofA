cmake_minimum_required(VERSION 3.15)
project(V)

# Add LLVM to build.
find_package(LLVM REQUIRED CONFIG)
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)
include_directories(${LLVM_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include)
add_definitions(${LLVM_DEFINITIONS})

# Create the VUOQPass library.
add_llvm_library(LLVMVUOQPass MODULE #BUILDTREE_ONLY
  VUOQ.cpp

  PLUGIN_TOOL
  opt
)
