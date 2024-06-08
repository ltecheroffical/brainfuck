cmake_minimum_required(VERSION 3.14)
project(brainfuck VERSION 0.1.0 LANGUAGES C)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_executable(brainfuck ${SOURCES})
