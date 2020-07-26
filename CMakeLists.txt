
cmake_minimum_required(VERSION 3.5)

project (hello)


set(CMAKE_CXX_COMPILER "clang++-10") # change version if needed
set(CMAKE_C_COMPILER "clang-10")
set(CMAKE_BUILD_TYPE "Debug") # remove it will build release version 



find_package(Threads)

add_executable(hello src/hello.cpp)
target_link_libraries(hello ${CMAKE_THREAD_LIBS_INIT})

