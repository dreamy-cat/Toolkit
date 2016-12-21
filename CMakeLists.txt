project(SimpleClient)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
# Other version of GCC, fix later.
set(CMAKE_CXX_COMPILER "/usr/local/bin/g++")
set(CMAKE_LIBRARY_PATH "/usr/local/lib64")
set(CMAKE_CXX_FLAGS "-std=c++14")

aux_source_directory(. SRC_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST})

# later
target_link_libraries(${PROJECT_NAME} -lpthread)
