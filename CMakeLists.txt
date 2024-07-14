cmake_minimum_required(VERSION 3.10)
project(gravity C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set optimization level based on build type
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O2")

# Compiler flags
add_compile_definitions(
        BUILD_GRAVITY_API
        GRAVITY_INCLUDE_MATH
        GRAVITY_INCLUDE_JSON
        GRAVITY_INCLUDE_ENV
        GRAVITY_INCLUDE_FILE
#        GRAVITY_INCLUDE_TSORT
)

# Directories
set(COMPILER_DIR src/compiler/)
set(RUNTIME_DIR src/runtime/)
set(SHARED_DIR src/shared/)
set(UTILS_DIR src/utils/)
set(OPT_DIR src/optionals/)

# Source files
file(GLOB SRC
        ${COMPILER_DIR}*.c
        ${RUNTIME_DIR}*.c
        ${SHARED_DIR}*.c
        ${UTILS_DIR}*.c
        ${OPT_DIR}*.c
)

# Include directories
include_directories(
        ${COMPILER_DIR}
        ${RUNTIME_DIR}
        ${SHARED_DIR}
        ${UTILS_DIR}
        ${OPT_DIR}
)

add_library(gravity_static STATIC ${SRC})
set_target_properties(gravity_static PROPERTIES OUTPUT_NAME gravity)

# Main executable
add_executable(gravity ${SRC} src/cli/gravity.c)

# Example executable
add_executable(example ${SRC} examples/example.c)



















##############################


## Shared library
#add_library(gravity_shared SHARED ${SRC})
#set_target_properties(gravity_shared PROPERTIES OUTPUT_NAME gravity)
#
## Link libraries
#if(WIN32)
#    target_link_libraries(gravity m Shlwapi)
#    target_link_libraries(example m Shlwapi)
#    target_link_libraries(gravity_shared m Shlwapi)
#elseif(APPLE)
#    target_link_libraries(gravity m)
#    target_link_libraries(example m)
#    target_link_libraries(gravity_shared m)
#elseif(CMAKE_SYSTEM_NAME MATCHES "^(OpenBSD|FreeBSD|NetBSD|DragonFly)$")
#    target_link_libraries(gravity m)
#    target_link_libraries(example m)
#    target_link_libraries(gravity_shared m)
#else()
#    target_link_libraries(gravity m rt)
#    target_link_libraries(example m rt)
#    target_link_libraries(gravity_shared m rt)
#endif()

# Installation rules
#install(TARGETS gravity gravity_shared gravity_static
#    RUNTIME DESTINATION bin
#    LIBRARY DESTINATION lib
#    ARCHIVE DESTINATION lib
#)
# Old CMakeLists.txt
# cmake_minimum_required(VERSION 3.0)
# project(gravity VERSION 1.0 LANGUAGES C)
#
# set(CMAKE_C_STANDARD 99)
#
# option(BUILD_CLI "Build the command line interface" ON)
#
# # ----------------------------------------------------------------
# # Library
# add_subdirectory(src)
#
# # ----------------------------------------------------------------
# # Command Line Interface
# if(BUILD_CLI)
#     add_subdirectory(src/cli)
# endif()
