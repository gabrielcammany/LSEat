cmake_minimum_required(VERSION 2.8.4)
project(LSEat)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

set(SOURCE_FILES main.c fitxers.c shell.c utils.c types.h)
add_executable(dbsg ${SOURCE_FILES})