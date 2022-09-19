# Infos

cmake_minimum_required (VERSION 3.16)
project (supstream)

# Flags

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "Set C Compiler Flags" FORCE)

# Libraries

find_package(PkgConfig REQUIRED)

pkg_search_module(GLIB REQUIRED glib-2.0)
pkg_search_module(GST REQUIRED gstreamer-1.0)
pkg_search_module(YAML REQUIRED yaml-0.1)
pkg_search_module(UUID REQUIRED uuid)
pkg_search_module(LIBZMQ REQUIRED libzmq)
pkg_search_module(LIBCURL REQUIRED libcurl)

include_directories(${GLIB_INCLUDE_DIRS})
include_directories(${GST_INCLUDE_DIRS})
include_directories(${YAML_INCLUDE_DIRS})
include_directories(${UUID_INCLUDE_DIRS})
include_directories(${LIBZMQ_INCLUDE_DIRS})
include_directories(${LIBCURL_INCLUDE_DIRS})

link_libraries(${GLIB_LIBRARIES})
link_libraries(${GST_LIBRARIES})
link_libraries(${YAML_LIBRARIES})
link_libraries(${UUID_LIBRARIES})
link_libraries(${LIBZMQ_LIBRARIES})
link_libraries(${LIBCURL_LIBRARIES})
link_libraries(-lpthread)
link_libraries(-lcjson)
link_libraries(-lm)

# Sources

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.c)
file(GLOB SOURCES_AST ${PROJECT_SOURCE_DIR}/src/ast/*.c)
file(GLOB SOURCES_AST_ACTION ${PROJECT_SOURCE_DIR}/src/ast/action/*.c)
file(GLOB SOURCES_REQUEST ${PROJECT_SOURCE_DIR}/src/request/*.c)
file(GLOB SOURCES_LINK ${PROJECT_SOURCE_DIR}/src/link/*.c)
file(GLOB SOURCES_SEMANTIC ${PROJECT_SOURCE_DIR}/src/semantic/*.c)
file(GLOB SOURCES_SEMANTIC_BLOCK ${PROJECT_SOURCE_DIR}/src/semantic/block/*.c)
file(GLOB SOURCES_SEMANTIC_LINE ${PROJECT_SOURCE_DIR}/src/semantic/line/*.c)
set(SOURCES ${SOURCES} ${SOURCES_AST})
set(SOURCES ${SOURCES} ${SOURCES_AST_ACTION})
set(SOURCES ${SOURCES} ${SOURCES_REQUEST})
set(SOURCES ${SOURCES} ${SOURCES_LINK})
set(SOURCES ${SOURCES} ${SOURCES_SEMANTIC})
set(SOURCES ${SOURCES} ${SOURCES_SEMANTIC_BLOCK})
set(SOURCES ${SOURCES} ${SOURCES_SEMANTIC_LINE})

set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/bin/)

add_compile_options(-Wall -Wextra)

add_executable(${PROJECT_NAME} ${SOURCES})

# Unit tests

link_libraries(-lcunit)

file(GLOB SOURCES_UT ${PROJECT_SOURCE_DIR}/src/unit_tests/*.c)
file(GLOB SOURCES_LIST_UT ${PROJECT_SOURCE_DIR}/src/list.c)
file(GLOB SOURCES_AST_UT ${PROJECT_SOURCE_DIR}/src/ast/*.c)
set(SOURCES_UT ${SOURCES_UT} ${SOURCES_LIST_UT})
set(SOURCES_UT ${SOURCES_UT} ${SOURCES_AST_UT})

add_executable(unit_tests ${SOURCES_UT})
