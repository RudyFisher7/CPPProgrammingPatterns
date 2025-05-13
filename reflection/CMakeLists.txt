cmake_minimum_required(VERSION 3.13)
project(KoiReflection LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "Configured for compiler ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Configured for compiler architecture ${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}")
message(STATUS "Configured for compiler abi ${CMAKE_CXX_COMPILER_ABI}")
message(STATUS "Configured for system ${CMAKE_SYSTEM_NAME}")

set(SOURCES
        source/string_name.cpp
        source/object.cpp
        source/variant_reference.cpp
)

set(HEADERS
        include/koi_object/string_name.hpp
        include/koi_object/object.hpp
        include/koi_object/variant_reference.hpp
)

add_library(${PROJECT_NAME} STATIC
        ${SOURCES}
        ${HEADERS}
)

target_include_directories(${PROJECT_NAME} PUBLIC
        include
)


add_subdirectory(test)