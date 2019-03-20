cmake_minimum_required(VERSION 3.12)
project(Wired86
    LANGUAGES CXX
    DESCRIPTION "Fully-documented emulator for the Intel 8086 CPU, the 8253 PIT and the 8259 PIC.")

# The emulator code is initially built as a library so that multiple builds (such as a CLI build, GUI build, etc) can
# be made without having to compile the same files multiple times.
set(LIB_NAME "common")

set(SRC_FILES # Define all common library source files.
    src/common/logging
    src/common/convert
    src/common/emu/cpu/intel8086
    src/common/emu/cpu/reg/registers8086
    src/common/emu/cpu/instr/opcode
    src/common/emu/cpu/instr/modregrm
    src/common/emu/cpu/instr/immediate
    src/common/emu/cpu/instr/instruction
    src/common/emu/cpu/instr/stack
)
set(CLI_SRC_FILES src/cli/main)
set(GUI_SRC_FILES src/gui/main)
set(TEST_SRC_FILES src/test/main)

add_library(${LIB_NAME} STATIC ${SRC_FILES}) # Create common library.

target_compile_features(${LIB_NAME} PUBLIC cxx_std_17) # Common library uses C++ 17.
target_compile_features(${LIB_NAME} INTERFACE cxx_std_11) # Uses of the common library require minimum C++ 11.
target_include_directories(${LIB_NAME} PUBLIC include/common) # All builds using this library share a dependency on the
                                                              # include/common directory.

target_compile_options(${LIB_NAME} PRIVATE -Werror # Warnings are treated as errors.
                                           -Wall # All standard warnings.
                                           -Wextra # Additional warnings.
                                           -Wsuggest-override # Require the explicit use of the 'override' keyword.
                                           -Wsuggest-final-methods # Require the explicit use of the 'final' keyword.
                                           -Wcast-align # Prevent casts of pointers to target values of greater width.
                                           -Wcast-qual # Prevent casting to remove type qualifier (e.g. attempt to remove 'const').
                                           -Wuseless-cast # Prevent unnecessary casts.
                                           -Wzero-as-null-pointer-constant # Prevent use of '0' literal in place of 'nullptr'.
                                           -Wredundant-decls # Prevent multiple identical declarations within the same scope.
                                           -Wshadow # Prevent shadowing.
                                           -pedantic) # Obey ISO C++.



# COMMAND-LINE INTERFACE
set(CLI_NAME "cli")
message("Creating command-line interface build: ${CLI_NAME}")

add_executable(${CLI_NAME} ${CLI_SRC_FILES})

target_include_directories(${CLI_NAME} PRIVATE include/cli)
target_link_libraries(${CLI_NAME} PRIVATE ${LIB_NAME})



# GRAPHICAL USER INTERFACE
set(GUI_NAME "gui")
message("Creating graphical user interface build: ${GUI_NAME}")

find_package(FLTK REQUIRED)

message("FLTK found: ${FLTK_FOUND}")
message("FLTK include directory: ${FLTK_INCLUDE_DIR}")

add_executable(${GUI_NAME} ${GUI_SRC_FILES})

target_include_directories(${GUI_NAME} PRIVATE include/gui)
target_include_directories(${GUI_NAME} PRIVATE ${FLTK_INCLUDE_DIR})
target_link_libraries(${GUI_NAME} PRIVATE fltk)
target_link_libraries(${GUI_NAME} PRIVATE ${LIB_NAME})



# TESTING
set(TEST_NAME "test")
message("Creating testing build: ${TEST_NAME}")

add_executable(${TEST_NAME} ${TEST_SRC_FILES})

target_include_directories(${TEST_NAME} PRIVATE include/test)
target_link_libraries(${TEST_NAME} PRIVATE ${LIB_NAME})