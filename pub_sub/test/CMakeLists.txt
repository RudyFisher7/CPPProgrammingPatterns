# MIT License
#
# Copyright (c) 2025 Rudy Fisher (kiyasui-hito)
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.


cmake_minimum_required(VERSION 3.26)
project(KoiPubSubTest)


Include(FetchContent)

if(NOT catch2_POPULATED)#fixme:: still fetches in each sibling test project
    message(STATUS "${PROJECT_NAME}: fetch Catch2.")
    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG        v3.4.0 # or a later release
            GIT_PROGRESS TRUE
            GIT_SHALLOW TRUE
    )

    FetchContent_MakeAvailable(Catch2)

    FetchContent_GetProperties(Catch2)
#    FetchContent_Populate(Catch2)
    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
    message(STATUS "${CMAKE_MODULE_PATH}")
else()
    message(STATUS "${PROJECT_NAME}: Catch2 already available.")
endif()

if(catch2_POPULATED)
    set(catch2_POPULATED TRUE PARENT_SCOPE)
    message(STATUS "Catch2 populated: ${catch2_POPULATED}")
endif()


add_executable(${PROJECT_NAME}
        test.cpp
        mock_object.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        KoiPubSub
        Catch2::Catch2
)

include(CTest)
include(Catch)
catch_discover_tests(${PROJECT_NAME})
add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}>)
