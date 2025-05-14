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


cmake_minimum_required(VERSION 3.13)
project(CPPProgrammingPatterns)

# Detect number of available cores on the system
if (UNIX)
    execute_process(COMMAND nproc OUTPUT_VARIABLE NUM_CORES OUTPUT_STRIP_TRAILING_WHITESPACE)
elseif (WIN32)
    execute_process(COMMAND cmd /C "echo %NUMBER_OF_PROCESSORS%" OUTPUT_VARIABLE NUM_CORES OUTPUT_STRIP_TRAILING_WHITESPACE)
elseif (APPLE)
    execute_process(COMMAND sysctl -n hw.ncpu OUTPUT_VARIABLE NUM_CORES OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

math(EXPR JOBS "${NUM_CORES} - 2")
set(CMAKE_BUILD_PARALLEL_LEVEL ${JOBS})
message(STATUS "Detected ${NUM_CORES} cores, set ${JOBS} parallel jobs for build.")


add_subdirectory(pub_sub)
add_subdirectory(reflection)
