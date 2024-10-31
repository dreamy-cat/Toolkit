cmake_minimum_required(VERSION 3.5)
project(Toolkit)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
# Other version of GCC...
#set(CMAKE_CXX_COMPILER "/usr/local/bin/g++")
#set(CMAKE_LIBRARY_PATH "/usr/local/lib64")
set(CMAKE_CXX_FLAGS "-std=c++14")
#set(CMAKE_CXX_FLAGS "-g2")

aux_source_directory(. SRC_LIST)

#include_directories(${inlclude})
#set(PROJECT_NAME main.cpp simple_client.cpp ${include}/range/v3/all.hpp)

add_executable(${PROJECT_NAME} ${SRC_LIST})

# later
#target_link_libraries(${PROJECT_NAME} -lpthread)
