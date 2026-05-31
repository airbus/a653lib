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
### Options
#

option(A653LIB_BUILD_WASM "Build wasm guest partitions and runtime launchers" OFF)
option(A653LIB_FETCH_C_ABI_LENS "Clone and build c-abi-lens automatically" ON)

set(
    WASI_SYSROOT
    "/usr/share/wasi-sysroot"
    CACHE PATH
         "WASI sysroot used for wasm32-wasip1 guest compilation"
)

set(
    WASM_CLANG
    "clang"
    CACHE FILEPATH
          "Clang executable used for wasm32-wasip1 guest compilation"
)

set(
    IWASM_LIBRARY
    ""
    CACHE FILEPATH
          "Optional full path to libiwasm.a or shared lib for the WAMR launcher"
)

set(
    WASMTIME_LIBRARY
    ""
    CACHE FILEPATH
          "Optional full path to libwasmtime for the Wasmtime launcher"
)

set(
    C_ABI_LENS_REPO_DIR
    "${CMAKE_CURRENT_BINARY_DIR}/c-abi-lens-src"
    CACHE PATH
          "Location of the arinc653-wasm/c-abi-lens checkout"
)

set(
    C_ABI_LENS_EXECUTABLE
    ""
    CACHE FILEPATH
          "Optional path to a prebuilt c-abi-lens executable"
)

set(
    ARINC653_ZIP
    ""
    CACHE FILEPATH
          "Path to a pre-fetched arinc653.h.zip archive. If set, CMake copies it into the build tree instead of downloading it."
)

#
### Generated ARINC headers
#

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
find_program(SED_EXECUTABLE sed REQUIRED)

set(PROCESS_ARINC_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/process_arinc_header.sh")
file(WRITE "${PROCESS_ARINC_SCRIPT}" [=[
#!/bin/sh
set -eu
exec awk -f "$1" "$2" > "$3"
]=])
execute_process(COMMAND chmod +x "${PROCESS_ARINC_SCRIPT}")

add_custom_command(
    OUTPUT "${ARINC_PATCHED_HEADER}"
    COMMAND "${PROCESS_ARINC_SCRIPT}"
            "${CMAKE_CURRENT_SOURCE_DIR}/scripts/process-arinc-header.awk"
            "${ARINC_RAW_HEADER}"
            "${ARINC_PATCHED_HEADER}"
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
### Existing helper macro
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

target_include_directories(
    a653lib
    PUBLIC
        a653_inc
    PRIVATE
        ${PRIVATE_INCLUDE_DIRS}
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
### Wasm support
#

if(A653LIB_BUILD_WASM)
    set(A653_WASM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/a653_lib_wasm32")
    set(A653_WASM_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/a653_inc")
    set(CAMW32_GETSET_HEADER "${CMAKE_CURRENT_BINARY_DIR}/camw32_getset.h")

    find_program(CLANG_EXECUTABLE NAMES "${WASM_CLANG}" clang REQUIRED)
    find_program(GIT_EXECUTABLE git)
    find_program(CARGO_EXECUTABLE cargo)

    find_library(IWASM_LIBRARY_AUTO NAMES iwasm)
    find_library(WASMTIME_LIBRARY_AUTO NAMES wasmtime
      HINTS /usr/local/lib
    )

    if(IWASM_LIBRARY)
        set(IWASM_LINK "${IWASM_LIBRARY}")
    elseif(IWASM_LIBRARY_AUTO)
        set(IWASM_LINK "${IWASM_LIBRARY_AUTO}")
    else()
        set(IWASM_LINK iwasm)
    endif()

    if(WASMTIME_LIBRARY)
        set(WASMTIME_LINK "${WASMTIME_LIBRARY}")
    elseif(WASMTIME_LIBRARY_AUTO)
        set(WASMTIME_LINK "${WASMTIME_LIBRARY_AUTO}")
    else()
        set(WASMTIME_LINK wasmtime)
    endif()

    add_custom_target(
        wasm_headers
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${A653_WASM_INCLUDE_DIR}"
        COMMAND "${CMAKE_COMMAND}" -E copy_directory
                "${CMAKE_CURRENT_SOURCE_DIR}/a653_inc"
                "${A653_WASM_INCLUDE_DIR}"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                ${ARINC_SPLIT_HEADERS}
                "${A653_WASM_INCLUDE_DIR}"
        DEPENDS arinc_headers
        COMMENT "Preparing merged include tree for wasm guest compilation"
    )

    if(C_ABI_LENS_EXECUTABLE)
        set(C_ABI_LENS_BIN "${C_ABI_LENS_EXECUTABLE}")
        add_custom_target(c_abi_lens_tool DEPENDS "${C_ABI_LENS_BIN}")
    else()
        set(C_ABI_LENS_SRC_DIR "${C_ABI_LENS_REPO_DIR}")
        set(C_ABI_LENS_BIN "${C_ABI_LENS_SRC_DIR}/pkgs/c-abi-lens/target/debug/c-abi-lens")

        if(A653LIB_FETCH_C_ABI_LENS)
            if(NOT GIT_EXECUTABLE)
                message(FATAL_ERROR "git is required to fetch c-abi-lens")
            endif()
            if(NOT CARGO_EXECUTABLE)
                message(FATAL_ERROR "cargo is required to build c-abi-lens")
            endif()

            add_custom_command(
                OUTPUT "${C_ABI_LENS_SRC_DIR}/pkgs/c-abi-lens/Cargo.toml"
                COMMAND "${GIT_EXECUTABLE}" clone
                        "https://github.com/psiegl/arinc653-wasm.git"
                        --branch psiegl-old
                        "${C_ABI_LENS_SRC_DIR}"
                VERBATIM
                COMMENT "Cloning arinc653-wasm repository"
            )

            add_custom_command(
                OUTPUT "${C_ABI_LENS_BIN}"
                COMMAND "${CARGO_EXECUTABLE}" build
                WORKING_DIRECTORY "${C_ABI_LENS_SRC_DIR}/pkgs/c-abi-lens"
                DEPENDS "${C_ABI_LENS_SRC_DIR}/pkgs/c-abi-lens/Cargo.toml"
                VERBATIM
                COMMENT "Building c-abi-lens"
            )

            add_custom_target(c_abi_lens_tool DEPENDS "${C_ABI_LENS_BIN}")
        else()
            if(NOT EXISTS "${C_ABI_LENS_BIN}")
                message(
                    FATAL_ERROR
                    "Set C_ABI_LENS_EXECUTABLE or enable A653LIB_FETCH_C_ABI_LENS"
                )
            endif()
            add_custom_target(c_abi_lens_tool DEPENDS "${C_ABI_LENS_BIN}")
        endif()
    endif()

    set(GEN_CAMW32_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/gen_camw32_getset.sh")
    file(WRITE "${GEN_CAMW32_SCRIPT}" [=[
#!/bin/sh
set -eu
tool="$1"
header="$2"
out="$3"
tmp="${out}.tmp"
"$tool" "$header" -- --target=wasm32-wasip1 > "$tmp"
sed 's|camw|camw32|g' "$tmp" > "$out"
rm -f "$tmp"
]=])
    execute_process(COMMAND chmod +x "${GEN_CAMW32_SCRIPT}")

    add_custom_command(
        OUTPUT "${CAMW32_GETSET_HEADER}"
        COMMAND "${GEN_CAMW32_SCRIPT}"
                "${C_ABI_LENS_BIN}"
                "${A653_WASM_INCLUDE_DIR}/a653Lib.h"
                "${CAMW32_GETSET_HEADER}"
        DEPENDS
            wasm_headers
            c_abi_lens_tool
            "${A653_WASM_INCLUDE_DIR}/a653Lib.h"
        VERBATIM
        COMMENT "Generating camw32_getset.h using c-abi-lens"
    )

    add_custom_target(camw32_getset DEPENDS "${CAMW32_GETSET_HEADER}")

    set(A653_WASM_GUESTS
        partition_a
        partition_b
    )

    set(A653_WASM_RUNTIME_COMMON_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/init.c"
        "${A653_WASM_DIR}/generic_helper.c"
    )

    set(A653_WASM_RUNTIME_SPECIFIC_SOURCES
        "${A653_WASM_DIR}/apex_host_fncs_wasm32.c"
        "${A653_WASM_DIR}/arinc653_part1_apex_time_wasm32.c"
        "${A653_WASM_DIR}/arinc653_part1_apex_process_wasm32.c"
        "${A653_WASM_DIR}/arinc653_part1_apex_partition_wasm32.c"
        "${A653_WASM_DIR}/arinc653_part1_apex_sampling_port_wasm32.c"
        "${A653_WASM_DIR}/arinc653_part2_apex_sampling_port_extension_wasm32.c"
        "${A653_WASM_DIR}/arinc653_part1_apex_queuing_port_wasm32.c"
        "${A653_WASM_DIR}/arinc653_part1_apex_semaphore_wasm32.c"
        "${A653_WASM_DIR}/arinc653_part1_apex_error_wasm32.c"
        "${A653_WASM_DIR}/arinc653_part1_apex_buffer_wasm32.c"
        "${A653_WASM_DIR}/arinc653_part1_apex_event_wasm32.c"
        "${A653_WASM_DIR}/arinc653_part1_apex_blackboard_wasm32.c"
        "${A653_WASM_DIR}/arinc653_part1_apex_mutex_wasm32.c"
        "${A653_WASM_DIR}/wasm32_main.c"
    )

    add_executable(a653lib_main_wasm main.c)
    target_link_libraries(a653lib_main_wasm PUBLIC a653lib)
    target_include_directories(
        a653lib_main_wasm
        PRIVATE
            ${PRIVATE_INCLUDE_DIRS}
            "${A653_WASM_DIR}"
            "${A653_WASM_INCLUDE_DIR}"
    )
    target_compile_definitions(a653lib_main_wasm PRIVATE __WASM_RT__)
    set_target_properties(a653lib_main_wasm PROPERTIES OUTPUT_NAME "a653_main_wasm")
    add_dependencies(a653lib_main_wasm wasm_headers)

    add_executable(
        p_wamr
        ${A653_WASM_RUNTIME_COMMON_SOURCES}
        ${A653_WASM_RUNTIME_SPECIFIC_SOURCES}
        "${A653_WASM_DIR}/a653_wamr.c"
    )
    target_link_libraries(p_wamr PRIVATE a653lib "${IWASM_LINK}" m)
    target_include_directories(
        p_wamr
        PRIVATE
            a653_lib
            "${A653_WASM_DIR}"
            "${A653_WASM_INCLUDE_DIR}"
            "${CMAKE_CURRENT_BINARY_DIR}"
    )
    target_compile_definitions(p_wamr PRIVATE __WAMR__)
    add_dependencies(p_wamr wasm_headers camw32_getset)

    add_executable(
        p_wasmtime
        ${A653_WASM_RUNTIME_COMMON_SOURCES}
        ${A653_WASM_RUNTIME_SPECIFIC_SOURCES}
        "${A653_WASM_DIR}/a653_wasmtime.c"
    )
    target_link_libraries(p_wasmtime PRIVATE a653lib "${WASMTIME_LINK}" m wasmtime)
    target_include_directories(
        p_wasmtime
        PRIVATE
            a653_lib
            "${A653_WASM_DIR}"
            "${A653_WASM_INCLUDE_DIR}"
            "${CMAKE_CURRENT_BINARY_DIR}"
    )
    target_compile_definitions(p_wasmtime PRIVATE __WASMTIME__)
    add_dependencies(p_wasmtime wasm_headers camw32_getset)

    set_target_properties(p_wamr p_wasmtime PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED YES
    )

    foreach(partition IN LISTS A653_WASM_GUESTS)
        set(src "${CMAKE_CURRENT_SOURCE_DIR}/${partition}.c")
        set(out "${CMAKE_CURRENT_BINARY_DIR}/${partition}.wasm")

        add_custom_command(
            OUTPUT "${out}" "${layout}"
            COMMAND
                "${CLANG_EXECUTABLE}"
                -I${A653_WASM_INCLUDE_DIR}
                --target=wasm32-wasip1
                -Wl,--export-table
                -Wl,--allow-undefined
                --sysroot=${WASI_SYSROOT}
                -o "${out}"
                "${src}"
            DEPENDS
                wasm_headers
                "${src}"
            VERBATIM
            COMMENT "Building wasm guest ${partition}.wasm"
        )

        add_custom_target("${partition}_wasm" DEPENDS "${out}")
        list(APPEND WASM_TARGETS "${partition}_wasm")
        install(FILES "${out}" DESTINATION bin)
    endforeach()

    add_custom_target(
        wasm
        DEPENDS
            a653lib_main_wasm
            p_wamr
            p_wasmtime
            ${WASM_TARGETS}
    )
endif()

#
### Declare installables
#

# install the executables
install(TARGETS ${EXECUTABLES} DESTINATION bin)

# install the wasm runtime tools when enabled
if(A653LIB_BUILD_WASM)
    install(TARGETS a653lib_main_wasm p_wamr p_wasmtime DESTINATION bin)
endif()

# install the base and the partition init library, and the public header files
install(TARGETS a653lib a653lib_partition_init FILE_SET a653lib_headers)
