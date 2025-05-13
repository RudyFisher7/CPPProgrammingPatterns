cmake_minimum_required(VERSION 3.13)
project(KoiPubSubTest)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.4.0 # or a later release
)

FetchContent_MakeAvailable(Catch2)


set(KOI_REFLECTION_TEST KoiReflectionTest)

add_executable(${KOI_REFLECTION_TEST}
        test.cpp
)

target_link_libraries(${KOI_REFLECTION_TEST} PUBLIC
        KoiReflection
        Catch2::Catch2
)
