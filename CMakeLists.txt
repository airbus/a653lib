# SPDX-FileCopyrightText: 2025 wucke13
#
# SPDX-License-Identifier: Apache-2.0 OR MIT
#
# Note: formatted using gersemi

#
### Global project setup
#

cmake_minimum_required(VERSION 3.23)
project(a653lib C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#
### Utility and helper functions
#

# Make the compiler as strict as possible
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wextra -Wpedantic) # TODO add -Werror eventually
endif()

#
# Generated ARINC headers
#

set(
    ARINC653_ZIP
    ""
    CACHE FILEPATH
          "Path to a pre-fetched arinc653.h.zip archive. If set, CMake copies it into the build tree instead of downloading it."
)

set(ARINC_URL
    "https://brx-content.fullsight.org/site/binaries/content/assets/itc/content/support-files/arinc653.h.zip"
)

set(ARINC_GEN_DIR "${CMAKE_CURRENT_BINARY_DIR}/download")
set(ARINC_ZIP "${ARINC_GEN_DIR}/arinc653.h.zip")
set(ARINC_RAW_HEADER "${ARINC_GEN_DIR}/ARINC653.h")
set(ARINC_PATCHED_HEADER "${ARINC_GEN_DIR}/ARINC653_patched.h")

set(ARINC_SPLIT_HEADERS
    "${ARINC_GEN_DIR}/a653Blackboard.h"
    "${ARINC_GEN_DIR}/a653Buffer.h"
    "${ARINC_GEN_DIR}/a653Event.h"
    "${ARINC_GEN_DIR}/a653Mutex.h"
)

file(MAKE_DIRECTORY "${ARINC_GEN_DIR}")

# If an external archive path is provided (e.g. by Nix), copy it into the
# location expected by the normal build flow.
if(ARINC653_ZIP)
    if(NOT EXISTS "${ARINC653_ZIP}")
        message(
            FATAL_ERROR
            "ARINC653_ZIP was set, but the file does not exist: ${ARINC653_ZIP}"
        )
    endif()

    message(STATUS "Using pre-fetched ARINC archive: ${ARINC653_ZIP}")
    configure_file("${ARINC653_ZIP}" "${ARINC_ZIP}" COPYONLY)
endif()

# Optional but strongly recommended:
# set(ARINC_SHA256 "<fill in the real sha256>")

if(NOT EXISTS "${ARINC_ZIP}")
    message(STATUS "Downloading ${ARINC_URL}")
    file(
        DOWNLOAD
        "${ARINC_URL}"
        "${ARINC_ZIP}"
        TLS_VERIFY ON
        # EXPECTED_HASH SHA256=${ARINC_SHA256}
        STATUS ARINC_DOWNLOAD_STATUS
        SHOW_PROGRESS
    )
    list(GET ARINC_DOWNLOAD_STATUS 0 ARINC_DOWNLOAD_CODE)
    if(NOT ARINC_DOWNLOAD_CODE EQUAL 0)
        list(GET ARINC_DOWNLOAD_STATUS 1 ARINC_DOWNLOAD_MSG)
        message(FATAL_ERROR "Failed to download ARINC headers: ${ARINC_DOWNLOAD_MSG}")
    endif()
endif()

if(NOT EXISTS "${ARINC_RAW_HEADER}")
    file(ARCHIVE_EXTRACT
        INPUT "${ARINC_ZIP}"
        DESTINATION "${ARINC_GEN_DIR}"
    )
endif()

find_program(AWK_EXECUTABLE awk REQUIRED)

add_custom_command(
    OUTPUT "${ARINC_PATCHED_HEADER}"
    COMMAND "${AWK_EXECUTABLE}"
            -f "${CMAKE_CURRENT_SOURCE_DIR}/scripts/process-arinc-header.awk"
            "${ARINC_RAW_HEADER}"
            > "${ARINC_PATCHED_HEADER}"
    DEPENDS
        "${ARINC_RAW_HEADER}"
        "${CMAKE_CURRENT_SOURCE_DIR}/scripts/process-arinc-header.awk"
    VERBATIM
    COMMENT "Generating patched ARINC653 header"
)

add_custom_command(
    OUTPUT ${ARINC_SPLIT_HEADERS}
    COMMAND "${CMAKE_COMMAND}" -E chdir "${ARINC_GEN_DIR}"
            "${AWK_EXECUTABLE}"
            -f "${CMAKE_CURRENT_SOURCE_DIR}/scripts/split-arinc-header.awk"
            "${ARINC_PATCHED_HEADER}"
    DEPENDS
        "${ARINC_PATCHED_HEADER}"
        "${CMAKE_CURRENT_SOURCE_DIR}/scripts/split-arinc-header.awk"
    BYPRODUCTS ${ARINC_SPLIT_HEADERS}
    VERBATIM
    COMMENT "Splitting patched ARINC653 header into per-interface headers"
)

add_custom_target(arinc_headers DEPENDS ${ARINC_SPLIT_HEADERS})

add_library(arinc_generated_headers INTERFACE)
add_dependencies(arinc_generated_headers arinc_headers)
target_include_directories(arinc_generated_headers INTERFACE "${ARINC_GEN_DIR}")

#
# Existing helper macro
#

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
        a653_lib/a653_i_blackboard.c
        a653_lib/a653_i_buffer.c
        a653_lib/a653_i_error.c
        a653_lib/a653_i_event.c
        a653_lib/a653_i_partition.c
        a653_lib/a653_i_process.c
        a653_lib/a653_i_queuing.c
        a653_lib/a653_i_sampling.c
        a653_lib/a653_i_semaphore.c
        a653_lib/a653_i_shm_if.c
        a653_lib/a653_i_sync.c
        a653_lib/a653_i_time.c
        a653_lib/a653_i_time_lib.c
        a653_lib/a653_i_mutex.c
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

target_link_libraries(a653lib PUBLIC arinc_generated_headers)

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
