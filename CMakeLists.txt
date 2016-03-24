# ============================================================================
#
# Introduction
# ------------
#
# This file is the main file for the procscheduler-helpers. The project follows
# the pattern laid out at http://pile-contributors.github.io/
# for a `pile-helpers` repository.
#
# Usage
# -----
#
# You can use the resources in this project to understand this pile,
# to use it as a model for your own piles or to improve and test this pile.
#
# The project has an optional dependency on Qt5 library. If found,
# the procscheduler pile will use the definitions for shared libraries.
#
# ============================================================================

set (PROJECT_NAME "procscheduler-helpers")

project(${PROJECT_NAME})
cmake_minimum_required(VERSION 2.8.11)
enable_testing()

option( INCLUDE_PILE_SOURCES_DIRECTLY
        "Use internal procscheduler as a pile (ON) or as a library (OFF)"
        OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(INCLUDE_OUTPUT_PATH
    "${PROJECT_BINARY_DIR}/build/include")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    "${PROJECT_BINARY_DIR}/build/bin")
set(EXECUTABLE_OUTPUT_PATH
    "${PROJECT_BINARY_DIR}/build/bin")
set(LIBRARY_OUTPUT_PATH
    "${PROJECT_BINARY_DIR}/build/lib")

include_directories(
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${INCLUDE_OUTPUT_PATH}")

find_package(Qt5 COMPONENTS Core)
if(Qt5Core_FOUND)
    include_directories(
         ${Qt5Core_INCLUDE_DIRS})
endif()

if(INCLUDE_PILE_SOURCES_DIRECTLY)
    include(pile_support)
    pileInclude (ProcScheduler)
    procschedulerInit(PILE)
    pileInclude (ProcRun)
    procrunInit(PILE)
    pileInclude (AppLib)
    applibInit(PILE)
    pileInclude (Translate)
    translateInit(PILE)
else()
    set (PROCSCHEDULER_BUILD_MODE SHARED)
    add_subdirectory(procrun)
    add_subdirectory(procscheduler)
    add_subdirectory(applib)
    add_subdirectory(translate)
    file(GLOB_RECURSE PROCSCHEDULER_HEADERS
        "${PROJECT_SOURCE_DIR}/procscheduler/*.h")
endif()

# copy the headers under build/include/procscheduler
# so that source files find them when using
#include <procscheduler/...>
pileCreateCopyTarget (
    "copy_headers"
    "headers are being copied"
    "${INCLUDE_OUTPUT_PATH}/procscheduler"
    "${PROCSCHEDULER_HEADERS}")

add_subdirectory(tests)

# ============================================================================
