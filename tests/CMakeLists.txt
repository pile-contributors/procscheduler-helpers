# ============================================================================
#
# This file hosts all tests.
#
# If INCLUDE_PILE_SOURCES_DIRECTLY is ON then PROCSCHEDULER_SOURCES and
# PROCSCHEDULER_HEADERS are defined we add them to the executable. PROCSCHEDULER_LIBRARY
# is empty.
#
# On the other hand if INCLUDE_PILE_SOURCES_DIRECTLY is
# OFF then PROCSCHEDULER_SOURCES and PROCSCHEDULER_HEADERS are empty and we link
# against PROCSCHEDULER_LIBRARY that is now the name of the target that
# builds the library.
#
# ============================================================================

find_package(Qt5 QUIET COMPONENTS Core Widgets)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (NOT INCLUDE_PILE_SOURCES_DIRECTLY)
    set (PROCSCHEDULER_LIBRARY procscheduler)
endif()

add_executable(test_basic
    basic.cc)

if (UNIX)
    find_package(Threads REQUIRED)
    list(APPEND GTEST_BOTH_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})
    add_definitions(-fPIC)
endif()

target_link_libraries(test_basic
    ${GTEST_BOTH_LIBRARIES}
    ${PROCSCHEDULER_LIBRARY})

add_test(
    NAME test_basic
    COMMAND $<TARGET_FILE:test_basic>)
