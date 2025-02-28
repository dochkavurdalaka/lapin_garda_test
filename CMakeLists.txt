cmake_minimum_required(VERSION 3.6.0)
project(lapin_garda_test VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR}/include)

list(APPEND CMAKE_CXX_FLAGS "-Wall -Werror -Wsign-compare")

set(CMAKE_CXX_FLAGS_SHADREL "-O2 -DNDEBUG"
    CACHE STRING "Compiler flags for the release version"
    FORCE)

message("General compiler flags: " ${CMAKE_CXX_FLAGS})
message("Debug build compiler flags: " ${CMAKE_CXX_FLAGS_DEBUG})
message("ShadRel build compiler flags: " ${CMAKE_CXX_FLAGS_SHADREL})

add_executable(Server Server.cpp)
add_executable(Client Client.cpp)
