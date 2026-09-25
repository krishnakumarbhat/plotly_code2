## Based on ROT QNX toolchain

## CMake toolchain file for IPNext Q++ v8.3.0 compiler
# QCC manual where all options, flags description and etc. can be found here:
# https://www.qnx.com/developers/docs/7.1/#com.qnx.doc.neutrino.utilities/topic/q/qcc.html
# like https://ddad.artifactory.cc.bmwgroup.net/artifactory/list/ipnext-sdk-snapshots/nightly/20221212/master/ipnext-sdk/blackbox-sdk/

## Notes:
# All flags come from ipnext_blackbox_sdk from like:
# https://ddad.artifactory.cc.bmwgroup.net/artifactory/list/ipnext-sdk-snapshots/nightly/20221212/master/ipnext-sdk/blackbox-sdk/

## System setup
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR ipnext)

#########################################################
## C++ setup
#########################################################
set(CMAKE_CXX14_STANDARD_COMPILE_OPTION "-std=c++14")
set(CMAKE_CXX14_EXTENSION_COMPILE_OPTION "-std=gnu++14")
list(APPEND CMAKE_CXX_COMPILE_FEATURES cxx_std_14)

## Commands setup
if(DEFINED ENV{QNX_COMPILER_DIR_PATH})
    set(QNX_COMPILER_DIR_PATH $ENV{QNX_COMPILER_DIR_PATH} CACHE PATH "Path to the qnx compiler directory")
endif()

if(NOT QNX_COMPILER_DIR_PATH)
    find_program(
        QNX_Q++_PATH
        q++
    )

    message("QNX compiler path is not set. Trying to find ...")
    get_filename_component(QNX_DIR_PATH ${QNX_Q++_PATH} DIRECTORY)
    set(QNX_COMPILER_DIR_PATH ${QNX_DIR_PATH} CACHE PATH "Path to the QNX compiler directory")

    if(NOT QNX_Q++_PATH)
        message(FATAL_ERROR  "QNX compiler is not found. Set QNX_COMPILER_DIR_PATH")
    endif()
    message("QNX compiler is found: ${QNX_COMPILER_DIR_PATH}")
endif()

set(QNX_VERSION 8)
set(QNX_GCC_VERSION 12.2.0)
set(QNX_TARGET_VERSION 8.0.0)

if(UNIX)
   set(CMAKE_C_COMPILER  ${QNX_COMPILER_DIR_PATH}/qcc)
   set(CMAKE_CXX_COMPILER  ${QNX_COMPILER_DIR_PATH}/q++)
   set(CMAKE_AR ${QNX_COMPILER_DIR_PATH}/aarch64-unknown-nto-qnx8.0.0-ar)
   set(CMAKE_ASM_COMPILER  ${QNX_COMPILER_DIR_PATH}/aarch64-unknown-nto-qnx8.0.0-as)
   set(CMAKE_LINKER  ${QNX_COMPILER_DIR_PATH}/q++)
else()
   set(CMAKE_C_COMPILER  ${QNX_COMPILER_DIR_PATH}/qcc.exe)
   set(CMAKE_CXX_COMPILER  ${QNX_COMPILER_DIR_PATH}/q++.exe)
   set(CMAKE_AR ${QNX_COMPILER_DIR_PATH}/aarch64-unknown-nto-qnx8.0.0-ar.exe)
   set(CMAKE_ASM_COMPILER  ${QNX_COMPILER_DIR_PATH}/aarch64-unknown-nto-qnx8.0.0-as.exe)
   set(CMAKE_LINKER  ${QNX_COMPILER_DIR_PATH}/q++.exe)
endif()

set(CMAKE_C_COMPILER_ID QNX_QCC)
## Common flags
set(common_flags "")
list(APPEND common_flags "-fPIC")                           # Compile a library as position-independent code (PIC).
list(APPEND common_flags "-fstack-protector-strong")        # Inserts a stack cookie onto the stack frame for vulnerable functions, to protect against stack buffer overflow
list(APPEND common_flags "-fdiagnostics-color=always")
list(APPEND common_flags "-fno-omit-frame-pointer")         # Generates functions without a frame
list(APPEND common_flags "-fno-canonical-system-headers")
list(APPEND common_flags "-feliminate-unused-debug-types")
list(APPEND common_flags "-fstack-clash-protection")
list(APPEND common_flags "-O2")                             # Optimization

# Macro definition
list(APPEND common_flags "-D_FORTIFY_SOURCE=2")
list(APPEND common_flags "-D_QNX_SOURCE")                   #We can't compile application without defining _QNX_SOURCE on global level
                                                            # for vector libraries, due to leaking sybmols
                                                            # See SPPAD-69623

# Target options
list(APPEND common_flags "-V12.2.0,gcc_ntoaarch64le")        # The compiler name, version number, and the target name
list(APPEND common_flags "-march=armv8.1-a")

# Warning options
list(APPEND common_flags "-Wall")                           # Enable additional warnings
list(APPEND common_flags "-Werror")                         # Treat warnings as errors.
list(APPEND common_flags "-DNDEBUG")                        # Turn off asserts
list(APPEND common_flags "-Wno-builtin-macro-redefined")

## C compilation flags setup
set(c_flags "")
list(APPEND c_flags ${common_flags})
string(REPLACE ";" " " c_flags "${c_flags}")
set(CMAKE_C_FLAGS ${c_flags})

## C++ compilation flags setup
set(cxx_flags "")
list(APPEND cxx_flags ${common_flags})
list(APPEND cxx_flags "-std=c++14")
string(REPLACE ";" " " cxx_flags "${cxx_flags}")
set(CMAKE_CXX_FLAGS ${cxx_flags})

## Assembler flags setup
set(asm_flags "")
string(REPLACE ";" " " asm_flags "${asm_flags}")
set(CMAKE_ASM_FLAGS ${asm_flags})

## Archiver flags setup
set(ar_create_flags "")
list(APPEND ar_create_flags "-r")       # r -> Replace or add an object module.
#list(APPEND ar_create_flags "-n")      # n -> Create a new library from scratch.
string(REPLACE ";" " " ar_create_flags "${ar_create_flags}")

set(ar_append_flags "")
list(APPEND ar_append_flags "-r")       # r -> Replace or add an object module .

## Linker flags
set(linker_flags "")
list(APPEND linker_flags "-V12.2.0,gcc_ntoaarch64le") #
list(APPEND linker_flags "-Wl,-z,relro") #
list(APPEND linker_flags "-Wl,-z,now") #
list(APPEND linker_flags "-Wl,-z,noexecstack") #
# Only link libs as needed
list(APPEND linker_flags "-Wl,--push-state") #
list(APPEND linker_flags "-Wl,--as-needed") #
list(APPEND linker_flags "-lc++") #
list(APPEND linker_flags "-lm") #
list(APPEND linker_flags "-Wl,--pop-state") #


string(REPLACE ";" " " linker_flags "${linker_flags}")
set(CMAKE_EXE_LINKER_FLAGS ${linker_flags})

## Archiver commands setup
set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> ${ar_create_flags} <TARGET> <LINK_FLAGS> <OBJECTS>")
set(CMAKE_CXX_ARCHIVE_APPEND "<CMAKE_AR> ${ar_append_flags} <TARGET> <LINK_FLAGS> <OBJECTS>")
set(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> ${ar_create_flags} <TARGET> <LINK_FLAGS> <OBJECTS>")
set(CMAKE_C_ARCHIVE_APPEND "<CMAKE_AR> ${ar_append_flags} <TARGET> <LINK_FLAGS> <OBJECTS>")

## Trick CMake into not testing the compiler
set(CMAKE_C_COMPILER_ID_RUN TRUE)
set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_ID_RUN TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)