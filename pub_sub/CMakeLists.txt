cmake_minimum_required(VERSION 3.13)
project(KoiPubSub LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "Configured for compiler ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Configured for compiler architecture ${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}")
message(STATUS "Configured for compiler abi ${CMAKE_CXX_COMPILER_ABI}")
message(STATUS "Configured for system ${CMAKE_SYSTEM_NAME}")

set(SOURCES
        source/server.cpp
        source/callable.cpp
)

set(HEADERS
        include/koi_pub_sub/server.hpp
        include/koi_pub_sub/callable.hpp
        include/koi_pub_sub/serialization/serialization.hpp
        include/koi_pub_sub/models/data.hpp
)

add_library(${PROJECT_NAME} STATIC
        ${SOURCES}
        ${HEADERS}
)

target_include_directories(${PROJECT_NAME} PUBLIC
        include
)


add_subdirectory(test)