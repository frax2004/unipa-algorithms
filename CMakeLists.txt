cmake_minimum_required(VERSION 3.30.0)

project(testing LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

find_package(raylib REQUIRED)


if(raylib_FOUND)
  message(STATUS "Raylib found at ${raylib_DIR}")
else()
  message(FATAL_ERROR "Raylib could not be located. Please install raylib")
endif()

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(testing)

target_include_directories(testing PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(testing PUBLIC stdc++exp raylib opengl32 gdi32 winmm)
target_compile_options(testing PUBLIC "-O0" "-Wall" "-Wextra" "-Wpedantic")
target_precompile_headers(testing PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/Common.hpp)
target_sources(testing PUBLIC ${SOURCES})
