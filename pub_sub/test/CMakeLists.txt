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


set(KOI_PUB_SUB_TEST KoiPubSubTest)

add_executable(${KOI_PUB_SUB_TEST}
        koi_pub_sub_test.cpp
)

target_link_libraries(${KOI_PUB_SUB_TEST} PRIVATE
        KoiPubSub
        Catch2::Catch2
)
