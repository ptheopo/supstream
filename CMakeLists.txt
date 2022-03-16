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

include_directories(${GLIB_INCLUDE_DIRS})
include_directories(${GST_INCLUDE_DIRS})
include_directories(${YAML_INCLUDE_DIRS})
include_directories(${UUID_INCLUDE_DIRS})
include_directories(${LIBZMQ_INCLUDE_DIRS})

link_libraries(${GLIB_LIBRARIES})
link_libraries(${GST_LIBRARIES})
link_libraries(${YAML_LIBRARIES})
link_libraries(${UUID_LIBRARIES})
link_libraries(${LIBZMQ_LIBRARIES})
link_libraries(-lpthread)
link_libraries(-lcjson)

# Sources

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.c)
file(GLOB SOURCES_REQUEST ${PROJECT_SOURCE_DIR}/src/request/*.c)
set(SOURCES ${SOURCES} ${SOURCES_REQUEST})

set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/bin/)

add_compile_options(-Wall -Wextra)

add_executable(${PROJECT_NAME} ${SOURCES})
