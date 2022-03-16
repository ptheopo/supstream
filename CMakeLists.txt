# Infos

cmake_minimum_required (VERSION 3.18)
project (supstream)

# Flags

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -lcjson" CACHE STRING "Set C Compiler Flags" FORCE)

# Libraries

find_package(PkgConfig REQUIRED)

pkg_search_module(GLIB REQUIRED glib-2.0)
pkg_search_module(GST REQUIRED gstreamer-1.0)
pkg_search_module(YAML REQUIRED yaml-0.1)
pkg_search_module(UUID REQUIRED uuid)
pkg_search_module(LIBZMQ REQUIRED libzmq)
pkg_search_module(PTHREAD REQUIRED pthread-stubs)

include_directories(${GLIB_INCLUDE_DIRS})
include_directories(${GST_INCLUDE_DIRS})
include_directories(${YAML_INCLUDE_DIRS})
include_directories(${UUID_INCLUDE_DIRS})
include_directories(${LIBZMQ_INCLUDE_DIRS})
include_directories(${PTHREAD_INCLUDE_DIRS})

link_libraries(${GLIB_LD_FLAGS})
link_libraries(${GST_LD_FLAGS})
link_libraries(${YAML_LD_FLAGS})
link_libraries(${UUID_LD_FLAGS})
link_libraries(${LIBZMQ_LD_FLAGS})
link_libraries(${PTHREAD_LD_FLAGS})

# Sources

set(SOURCES
    src/ast.c
    src/config.c
    src/debug.c
    src/fifo.c
    src/gateway.c
    src/linked.c
    src/list.c
    src/main.c
    src/pipeline.c
    src/request.c
    src/semantic.c
    src/verify.c
)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} ${SOURCES})
