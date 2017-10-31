cmake_minimum_required(VERSION 2.8.4)
project(LSEat)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

set(SOURCE_FILES data/data.c picard/picard.c enterprise/enterprise.c picard/managers/shellManager.c modules/fitxers.c modules/shell.c modules/utils.c modules/types.h modules/socketUtils.c)

add_executable(dbsg ${SOURCE_FILES})