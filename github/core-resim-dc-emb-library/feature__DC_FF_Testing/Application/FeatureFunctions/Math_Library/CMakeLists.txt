cmake_minimum_required(VERSION 3.20)
project(ML_MathLibrary)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/deps/ST_SharedTools/cmake_modules)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/development/cmake_modules)

set(CMAKE_EXPORT_COMPILE_COMMANDS On)

#include(AS_coverage)

enable_testing()

include_directories(deps/ST_SharedTools/mock_header/reuse)
include_directories(development/dependencies)

#include(FetchGoogletest)
#fetch_googletest(version "1.11.0")

set(MLMathLibrary_fast_math_exp_table "use_table" CACHE STRING "")
set(MLMathLibrary_fast_math_trig_table "use_table" CACHE STRING "")
add_subdirectory(ml_core ml_core)

target_compile_options(MLMathLibrary PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -Wall>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4 /wd4214>)

add_subdirectory(development development)
