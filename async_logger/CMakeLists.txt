cmake_minimum_required(VERSION 3.16)
project(async_logger)

set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "-O3 -pthread -lssl -lcrypto")

add_executable(log sample.cpp)

target_include_directories(log PUBLIC ${PROJECT_SOURCE_DIR}/include)
add_subdirectory(tests)
