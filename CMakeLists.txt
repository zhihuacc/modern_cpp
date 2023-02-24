cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Modern-Cpp)

set(CMAKE_BUILD_TYPE "Release")

set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_C_FLAGS_DEBUG "-O0")

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")

set(CMAKE_CXX_FLAGS "-g -fPIC -Wall -Werror=return-type -Werror=return-stack-address -Werror=uninitialized -Werror=format -Wno-unused-parameter -fno-omit-frame-pointer")
set(CMAKE_C_FLAGS "-g -fPIC -Wall -Werror=return-type -Werror=return-stack-address -Werror=uninitialized -Werror=format -Wno-unused-parameter -fno-omit-frame-pointer")

add_subdirectory(grammar)