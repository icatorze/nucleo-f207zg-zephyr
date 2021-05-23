# Find Zephyr. This also loads Zephyr's build system.
cmake_minimum_required(VERSION 3.13.1)

set(BOARD nucleo_f207zg)

list(APPEND DTS_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(displaytest1)

#list(APPEND ZEPHYR_EXTRA_MODULES ${THS_DIR}/drivers/zephyr)

FILE(GLOB app_sources src/*.c)
FILE(GLOB ihm_sources src/ihm/*.c)


target_sources(app PRIVATE ${app_sources})
target_sources(app PRIVATE ${ihm_sources})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../drivers)

