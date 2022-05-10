cmake_minimum_required(VERSION 3.16)

project(Othello VERSION 1.1)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")
add_executable(othello ${SOURCES})
target_link_libraries(othello PUBLIC "-lsfml-graphics -lsfml-window -lsfml-system")
