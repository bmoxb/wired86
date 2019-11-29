cmake_minimum_required(VERSION 3.12)
project(Wired86 LANGUAGES CXX)

# The emulator code is initially built as a library so that multiple builds (such as a CLI build, GUI build, etc) can
# be made without having to compile the same files multiple times.
set(LIB_NAME common)
add_library(${LIB_NAME} src/emu/cpu/intel8086)
target_include_directories(${LIB_NAME} PUBLIC include) # All builds using this library share the dependency on the
                                                       # include/ directory.

set(CLI_NAME cli)
add_executable(${CLI_NAME} src/main)
target_link_libraries(${CLI_NAME} ${LIB_NAME})
