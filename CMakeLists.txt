cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(ps1pp VERSION "1.0.0" LANGUAGES CXX)

add_library(tpl INTERFACE)
option(STATIC "Attempt to statically link everything" OFF)
set(EXTRA_STATIC_LINK_FLAGS
    ""
    CACHE STRING
          "Extra flags passed to linker when attempting to statically link")

if(STATIC)
  foreach(arg ${EXTRA_STATIC_LINK_FLAGS})
    target_link_options(tpl INTERFACE ${arg})
  endforeach()

  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_link_options(tpl INTERFACE -static-libgcc -static-libstdc++)
  else()
    message(
      STATUS
        "Could not statically link the C++ standard library shipped with your "
        "compiler, falling back to dynamic.")
  endif()
endif()

add_subdirectory(tpl)
add_subdirectory(src)
