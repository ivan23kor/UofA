cmake_minimum_required(VERSION 3.8)
project(MyLoopPass)

# Add LLVM to build.
find_package(LLVM REQUIRED CONFIG)
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)
include_directories(${LLVM_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include)
add_definitions(${LLVM_DEFINITIONS})

# Export compile commands db.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable or disable clang tidy.
if (NOT DISABLE_CLANG_TIDY)
  set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*,llvm*)
endif()

# Set CXX standard.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect MyLoopPass src.
set(srcs
  "${CMAKE_CURRENT_SOURCE_DIR}/MyLoopPass.cpp"
)

# Create the MyLoopPass library.
add_llvm_library(LLVMMyLoopPass MODULE BUILDTREE_ONLY
  MyLoopPass.cpp

  DEPENDS
  intrinsics_gen
  PLUGIN_TOOL
  opt
)
