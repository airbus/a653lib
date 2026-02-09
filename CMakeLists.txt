# SPDX-FileCopyrightText: 2025 wucke13
#
# SPDX-License-Identifier: Apache-2.0 OR MIT
#
# Note: formatted using gersemi

#
### Global project setup
#

cmake_minimum_required(VERSION 3.23)
project(a653lib)
set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

#
### Utility and helper functions
#

# Make the compiler as strict as possible
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wextra -Wpedantic) # TODO add -Werror eventually
endif()

# Function to add a new ARINC 653 Partition executable
#
# Arguments:
#
# - Name of the CMake executable to be declared
# - (optional) Main source file containing the entry point. Defaults to the name from the first
#   argument with the suffix `.c`
macro(add_653_partition)
    # check number of args to be one or 2, fail otherwise
    if(${ARGC} EQUAL 1)
        set(ARGV1 ${ARGV0}.c)
    elseif(NOT ${ARGC} EQUAL 2)
        message(
            FATAL_ERROR
            "add_653_partition must be called with one or two arguments."
        )
    endif()

    add_executable(${ARGV0} ${ARGV1})
    target_link_libraries(${ARGV0} PRIVATE a653lib a653lib_partition_init)
    list(APPEND EXECUTABLES ${ARGV0})
endmacro()

#
### Declare the a653lib related libraries
#

set(PRIVATE_INCLUDE_DIRS a653_lib)

# ARINC 653 library to be used by partitions developers. Comes with public interface via header
# files.
add_library(a653lib)
target_sources(
    a653lib
    PRIVATE
        a653_lib/a653_i_error.c
        a653_lib/a653_i_partition.c
        a653_lib/a653_i_process.c
        a653_lib/a653_i_queuing.c
        a653_lib/a653_i_sampling.c
        a653_lib/a653_i_semaphore.c
        a653_lib/a653_i_shm_if.c
        a653_lib/a653_i_sync.c
        a653_lib/a653_i_time.c
        a653_lib/a653_i_time_lib.c
        a653_lib/a653Init.c
    PUBLIC
        FILE_SET a653lib_headers
        TYPE HEADERS
        BASE_DIRS a653_inc
        FILES
            a653_inc/a653Error.h
            a653_inc/a653Init.h
            a653_inc/a653Lib.h
            a653_inc/a653Partition.h
            a653_inc/a653Process.h
            a653_inc/a653Queuing.h
            a653_inc/a653Sampling.h
            a653_inc/a653Semaphore.h
            a653_inc/a653Time.h
            a653_inc/a653Type.h
)

# Implementation dependent initialization code, to be linked into partitions. This lib has no
# public interface.
add_library(a653lib_partition_init)
target_sources(a653lib_partition_init PRIVATE init.c)
target_include_directories(
    a653lib_partition_init
    PRIVATE ${PRIVATE_INCLUDE_DIRS}
)
target_link_libraries(a653lib_partition_init PUBLIC a653lib)

#
### Main executable
#

add_executable(a653lib_main main.c)
target_link_libraries(a653lib_main PUBLIC a653lib)
target_include_directories(a653lib_main PRIVATE ${PRIVATE_INCLUDE_DIRS})
list(APPEND EXECUTABLES a653lib_main)

#
## Partitions
#

add_653_partition(partition_a)
add_653_partition(partition_b partition_b.c)

#
### Declare installables
#

# install the executables
install(TARGETS ${EXECUTABLES} DESTINATION bin)

# install the base and the partition init library, and the public header files
install(TARGETS a653lib a653lib_partition_init FILE_SET a653lib_headers)
