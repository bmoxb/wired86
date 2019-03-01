cmake_minimum_required(VERSION 3.12)
project(Wired86
    LANGUAGES CXX
    DESCRIPTION "Fully-documented emulator for the Intel 8086 CPU, the 8253 PIT and the 8259 PIC.")

# The emulator code is initially built as a library so that multiple builds (such as a CLI build, GUI build, etc) can
# be made without having to compile the same files multiple times.
set(LIB_NAME "common-${PROJECT_NAME}")

set(SRC_FILES # Define all source files (execept main).
    src/common/conversion
    src/common/emu/cpu/registerindexes
)
set(CLI_SRC_FILES src/cli/main)
set(GUI_SRC_FILES src/gui/main)
set(TEST_SRC_FILES src/test/main)

add_library(${LIB_NAME} STATIC ${SRC_FILES}) # Create common library.

target_compile_features(${LIB_NAME} PRIVATE cxx_std_17) # Library uses C++ 17 standard.
target_compile_options(${LIB_NAME} PRIVATE -Wall -Wextra -pedantic -Werror) # Enable strict errors and warnings (GCC).
target_include_directories(${LIB_NAME} PUBLIC include) # All builds using this library share the dependency on the
                                                       # include/ directory.

option(CLI "Create command-line interface build?" ON)
option(GUI "Create graphical user interface build?" OFF)
option(TEST "Create testing build?" OFF)

# COMMAND-LINE INTERFACE
if(CLI)
    set(CLI_NAME "cli-${PROJECT_NAME}")
    message("Creating command-line interface build: ${CLI_NAME}")

    add_executable(${CLI_NAME} ${CLI_SRC_FILES})
    target_link_libraries(${CLI_NAME} PRIVATE ${LIB_NAME})
endif()

# GRAPHICAL USER INTERFACE
if(GUI)
    set(GUI_NAME "gui-${PROJECT_NAME}")
    message("Creating graphical user interface build: ${GUI_NAME}")

    find_package(FLTK REQUIRED)

    message("FLTK found: ${FLTK_FOUND}")
    message("FLTK include directory: ${FLTK_INCLUDE_DIR}")

    add_executable(${GUI_NAME} ${GUI_SRC_FILES})
    target_link_libraries(${GUI_NAME} PRIVATE fltk)
    target_include_directories(${GUI_NAME} PRIVATE ${FLTK_INCLUDE_DIR})
    target_link_libraries(${GUI_NAME} PRIVATE ${LIB_NAME})
endif()

# TESTING
if(TEST)
    set(TEST_NAME "test-${PROJECT_NAME}")
    message("Creating testing build: ${TEST_NAME}")

    add_executable(${TEST_NAME} ${TEST_SRC_FILES})
    target_link_libraries(${TEST_NAME} PRIVATE ${LIB_NAME})
endif()