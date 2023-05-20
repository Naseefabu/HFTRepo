cmake_minimum_required(VERSION 3.16)
project(coinbase_feed_handler)

#set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "-O3 -pthread -DNDEBUG -lssl -lcrypto")


include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz) # nlohmann json
FetchContent_MakeAvailable(json)

find_package(Boost 1.79.0 REQUIRED COMPONENTS system thread filesystem container)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

set(URLIB_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/external/url/include")
set(URLIB_HEADERS ${URLIB_DIRECTORY}/boost/url.hpp ${URLIB_DIRECTORY}/boost/url/src.hpp)

add_executable(coinbase main.cpp)
target_include_directories(coinbase SYSTEM PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(coinbase PUBLIC Boost::boost Threads::Threads)
target_link_libraries(coinbase PUBLIC ssl crypto)
target_link_libraries(coinbase PRIVATE nlohmann_json::nlohmann_json)