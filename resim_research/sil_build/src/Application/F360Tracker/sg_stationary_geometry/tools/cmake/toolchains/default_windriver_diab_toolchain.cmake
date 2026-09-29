## ## CMake toolchain file for Windriver compiler. Preapread based on:
#  -> GDSR Tracker (repo: OT_ObjectTracking) - setup
#  -> NISSAN_SWEET400_SRR6 - initial flag list

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER_ID WindRiver)
set(CMAKE_C_COMPILER_ID_RUN TRUE)
set(CMAKE_CXX_COMPILER_ID WindRiver)
set(CMAKE_CXX_COMPILER_ID_RUN TRUE)

## Overwrite default compiler flags
# The "CACHE STRING "" FORCE" option is required in order
# to overwrite otherwise the set command will be ignored
unset(CMAKE_C_FLAGS CACHE)
unset(CMAKE_CXX_FLAGS CACHE)

set(WINDRIVER_TARGET -tPPCE200Z7260N3VEF:simple)
string(JOIN " "
   WINDRIVER_COMPILE_FLAGS
      ${WINDRIVER_TARGET}
      # Source for compiler options:
      # http://sdt52.aptiv.com/eng_ops_web/software/ctools/diab/5.9.6/
      #    wind_river_diab_compiler_options_reference_5.9.6_edition_5.pdf
      -g3                  # Generate symbolic debugger information and do most optimizations.
                           # Highly optimized code can be difficult to debug
      -D__PPC_VLE__
      -DNDEBUG
      -Xkill-opt=0x80000   # Disable target-dependent optimizations.
                           # This option is reserved for internal Wind River use. It should be used
                           # only on the advice of Wind River Customer Support. mask is a bit mask
                           # with one bit for each optimization type. mask may be given in hex, e.g.,
                           # -Xkillopt=0x12. Multiple optimizations can be disabled by OR-ing their
                           # mask bits. Undefined mask bits are ignored. -Xkill-opt=0xffffffff has
                           # a similar (but not exactly the same) effect as not using the - O option
                           # at all.
                           # Minor Transformations to Simplify Code Generation (0x80000)
      -Xsection-split      # Generate a separate section class for each function or variable.
      -XO                  # Enable extra optimizations.
      -Xclib-optim-off     # Disregard ANSI C library functions.
      -Xstmw-fast          # Specifies whether the stmw and the lmw instructions should be used
                           # to save/restore registers at function entry/exit. -Xstmw-slow means
                           # that they should never be used. -Xstmw-ok means that they can be
                           # used in leaf functions. -Xstmw-fast means that they should always
                           # be used instead of calling a library function to perform the operation.
      -Xcode-factor        # Find common code sequences at link time and share them,
                           # reducing code size at the cost of inserting some additional branches.
      -Xsize-opt           # Optimize for size rather than speed when there is a choice.
      -Xaddr-sconst=0x41   # Set addressing mode for sconst sections.
                           # Specify addressing for constant static and global variables with size
                           # less than or equal to -Xsmall-const
      -Xlicense-wait       # Wait for license.
      -Xkeywords=0x1F      # Enable extended keywords.
                           # Recognize new keywords according to mask, a bit mask specifying which keywords to add.
                           # 0x10 interrupt (C only)
      -Xforce-declarations # Generate warnings on undeclared functions.
      -Xforce-prototypes   # Generate warnings on functions without previous prototype
      -Xpass-source        # The compiler can add metadata to output object files
      -Xnested-interrupts  # Disable nested interrupts in interrupt functions
      -Xsmall-data=0          # Place small non-constant static and global variables with a size in
                              # bytes less than or equal to n inthe SDATA section class.
      -Xsmall-const=0         # Place small const static and global variables with a size in bytes
                              # less than or equal to n in the SCONST section class.
      -Xinline=0              # Inline functions with fewer than n nodes.
      -Xinline-explicit-force # Allow inlining of recursive function calls.
                              # -Xinline-explicit-force -Xinline-explicit-force=n -X163 -X163=n
                              # Inline recursive function calls up to n times. The default is 50.
                              # If this option is not used, the compiler inlines a function at most
                              # once. If this option is combined with -Xinline=0, the compiler inlines
                              # only functions declared within a C++ class or with inline, inline,
                              # or #pragma inline.
      -Xlocal-struct=0        # Allocate static and global variables to local data area.
      -Xmin-align=4           # Specify minimum alignment for single memory access to multi-byte values.
      -Xopt-count=5           # Execute the compiler's optimizing stage n times.
      -Xparse-size=10000000   # -Xparse-count replaces -Xparse-size, which has been deprecated.
      -Xstrings-in-text       # -Xconst-in-data and -Xstrings-in-text are shortcuts for locating all
                              # “constants” (CONST,SCONST, and STRING classes, not just “const” or
                              # string data) in “data” sections (mask=0) or“text” sections (mask=0xff)
                              # respectively.
      -Xtest-at-both          # Loop tests at top and bottom.
      -Xunroll=4              # -Xunroll=n -X15=n Unroll small loops n times.
      -Xunroll-size=100       # Specify the maximum number of nodes a loop can contain to be
                              # considered for loop unrolling. Each operator and each operand
                              # counts as one node, so the expression a = b - c; contains 5 nodes.
      -Xno-common             # Disable use of the “COMMON” feature so that the compiler or assembler
                              # will allocate each uninitialized public variable in the .bss section
                              # for the module defining it, and the linker will require exactly one
                              # definition of each public variable.
      -Xdialect-c++14
      # -e Change diagnostic severity level.
      # -esn[,n...]
      # For each of one or more diagnostic message numbers n in the comma-separated list, change the
      # severity level of the message to s where s is one of:
      # i: Information, equivalent to ignore.
      # w: Warning.
      # e: Error (continue compilation).
      # f: Fatal error (terminate immediately).
      # Source for error codes:
      # http://sdt52.aptiv.com/eng_ops_web/software/ctools/diab/5.9.6/
      #    wind_river_diab_compiler_error_messages_reference_5.9.6_edition_6.pdf
      -ei1606                 # conditional expression or part of it is always true/false
      -ei1824                 # explicit cast from 'type1 ' to 'type2 ' discards volatile qualifier
      -ei1827                 # array element size (array_size ) incompatible with requested element
                              # alignment (base_size ) - elements will only be element_aligned -byte aligned.
      -Xintc-eoir=0           # Write end of interrupt register.
)

## Linker flags
# include standard library
string(CONCAT WINDRIVER_LINKER_FLAGS "-lstl")

# Set compiler and linker flags
set(CMAKE_C_FLAGS        "${WINDRIVER_COMPILE_FLAGS}" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS      "${WINDRIVER_COMPILE_FLAGS}" CACHE STRING "" FORCE)
set(CMAKE_C_LINK_FLAGS   "${WINDRIVER_LINKER_FLAGS}" CACHE STRING "" FORCE)
set(CMAKE_C_COMPILER    "dcc" CACHE PATH "")
set(CMAKE_CXX_COMPILER  "dplus" CACHE PATH "")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
set(CMAKE_C_BYTE_ORDER LITTLE_ENDIAN)
set(CMAKE_CXX_BYTE_ORDER LITTLE_ENDIAN)
set(CMAKE_C_COMPILE_FEATURES "")
list(APPEND CMAKE_C_COMPILE_FEATURES
    c_std_90
    c_std_99
    c_std_11
)
set(CMAKE_CXX_COMPILE_FEATURES "")
list(APPEND CMAKE_CXX_COMPILE_FEATURES
    cxx_std_98
    cxx_std_03
    cxx_std_11
    cxx_std_14
    cxx_std_17
)

## Trick CMake into not testing the compiler
set(CMAKE_C_COMPILER_ID_RUN TRUE)
set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_ID_RUN TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)
