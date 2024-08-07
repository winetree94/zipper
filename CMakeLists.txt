cmake_minimum_required(VERSION 3.22)
project(zipper C)

set(CMAKE_C_STANDARD 11)

add_executable(
        zipper
        src/main.c
        src/zipper.c
        src/zipper.h
)
