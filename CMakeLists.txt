cmake_minimum_required (VERSION 3.18)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(ps1pp CXX)

add_library(tpl INTERFACE)
add_subdirectory(tpl)
add_subdirectory(src)
