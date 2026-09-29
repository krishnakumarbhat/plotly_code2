## CMake toolchain file for Tasking Tricore compiler
# Tricore manual where all options, flags description and etc. can be found here: https://www.tasking.com/support/tricore/
# Example: https://www.tasking.com/support/tricore/ctc_user_guide_v6.2r2.pdf

## System setup
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR aurix)
set(CMAKE_C_COMPILER_ID Tasking_Tricore)
set(CMAKE_CXX_COMPILER_ID Tasking_Tricore)

## Commands setup
if(DEFINED ENV{TRICORE_COMPILER_DIR_PATH})
    set(TRICORE_COMPILER_DIR_PATH $ENV{TRICORE_COMPILER_DIR_PATH} CACHE PATH "Path to the tricore compiler directory")
endif()

if(NOT TRICORE_COMPILER_DIR_PATH)
    find_program(
        TRICORE_CCTC_PATH
        cctc
    )

    message("TriCore compiler path is not set. Trying to find ...")
    get_filename_component(TRICORE_DIR_PATH ${TRICORE_CCTC_PATH} DIRECTORY)
    set(TRICORE_COMPILER_DIR_PATH ${TRICORE_DIR_PATH} CACHE PATH "Path to the tricore compiler directory")

    if(NOT TRICORE_CCTC_PATH)
        message(FATAL_ERROR  "TriCore compiler is not found. Set TRICORE_COMPILER_DIR_PATH")
    endif()
    message("TriCore compiler is found: ${TRICORE_COMPILER_DIR_PATH}")
endif()

set(CMAKE_C_COMPILER_ID TASKING_TriCore)
set(CMAKE_C_COMPILER  ${TRICORE_COMPILER_DIR_PATH}/ctc.exe)
set(CMAKE_CXX_COMPILER  ${TRICORE_COMPILER_DIR_PATH}/cctc.exe)
set(CMAKE_AR ${TRICORE_COMPILER_DIR_PATH}/artc.exe)
set(CMAKE_ASM_COMPILER  ${TRICORE_COMPILER_DIR_PATH}/astc.exe)
set(CMAKE_LINKER  ${TRICORE_COMPILER_DIR_PATH}/ltc.exe)

## C++ compilation flags setup
# Below set of flags in a minimal set that allow us to build a library.
set(CMAKE_CXX_FLAGS  "")
set(cxx_flags "")
list(APPEND cxx_flags "--g++")    # Enable GNU C++ compiler language extensions.
list(APPEND cxx_flags "--c++14")    # Comply to C++ standard.
list(APPEND cxx_flags "--instantiate=used")    # Instantiate those template entities that were used in the compilation.This will include all static data members for which there are template definitions
list(APPEND cxx_flags "--warnings-as-errors")    # treat warnings as errors.
list(APPEND cxx_flags "--exceptions")    # Enable support for C++ exceptions handling. Once "--c++11" is set then by default "--exceptions" is set.
list(APPEND cxx_flags "--rtti")    # Enable support for C++ RTTI (run-time type information)
list(APPEND cxx_flags "--io-streams=cpp11")  # Support for C++ I/O streams -> c++11 - use C++ library/use LLVM library (LLVM allows to use e.g. std::array).
list(APPEND cxx_flags "--gnu-version=40800")  
list(APPEND cxx_flags "--define=NDEBUG")    # define NDEBUG macro.
string(REPLACE ";" " " cxx_flags "${cxx_flags}")
set(CMAKE_CXX_FLAGS ${cxx_flags})

## Archiver flags setup
set(ar_create_flags "")
list(APPEND ar_create_flags "-r")    # r -> Replace or add an object module.
list(APPEND ar_create_flags "-n")    # n -> Create a new library from scratch.
string(REPLACE ";" " " ar_create_flags "${ar_create_flags}")

set(ar_append_flags "")
list(APPEND ar_append_flags "-r")    # r -> Replace or add an object module .

string(REPLACE ";" " " ar_append_flags "${ar_append_flags}")

## Archiver commands setup
set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> ${ar_create_flags} <TARGET> <LINK_FLAGS> <OBJECTS>")
set(CMAKE_CXX_ARCHIVE_APPEND "<CMAKE_AR> ${ar_append_flags} <TARGET> <LINK_FLAGS> <OBJECTS>")
set(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> ${ar_create_flags} <TARGET> <LINK_FLAGS> <OBJECTS>") 
set(CMAKE_C_ARCHIVE_APPEND "<CMAKE_AR> ${ar_append_flags} <TARGET> <LINK_FLAGS> <OBJECTS>")

## Linker flags
set(linker_flags "")
list(APPEND linker_flags "-lcw_fpu")  # link against libcw_fpu.a library
list(APPEND linker_flags "-lcpx_fpu")  # link against libcpx_fpu.a library
list(APPEND linker_flags "-lcxxx_fpu")  # link against libcxxx_fpu.a library
list(APPEND linker_flags "--optimize=2")  # All optimizations. Alias for -Ocltxy.
list(APPEND linker_flags "--munch")  # with this option you tell the linker to activate the muncher in the pre-locate phase.
string(REPLACE ";" " " linker_flags "${linker_flags}")
set(CMAKE_EXE_LINKER_FLAGS ${linker_flags})

## Other flags setup
set(CMAKE_C_FLAGS  "")
set(CMAKE_ASM_FLAGS "")
  
## Trick CMake into not testing the compiler
set(CMAKE_C_COMPILER_ID_RUN TRUE)
set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_ID_RUN TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)
