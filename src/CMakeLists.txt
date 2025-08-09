cmake_minimum_required(VERSION 3.16)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(lcd_hello_world)

# Use custom partition table
set(PARTITION_TABLE_CUSTOM_FILENAME ${CMAKE_SOURCE_DIR}/partitions.csv)
set(PARTITION_TABLE_CUSTOM 1)
