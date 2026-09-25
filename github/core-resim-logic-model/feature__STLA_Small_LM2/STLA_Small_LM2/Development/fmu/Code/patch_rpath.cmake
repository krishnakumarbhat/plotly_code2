# patch_rpath.cmake  —  called by CMakeLists.txt POST_BUILD via cmake -P
# Patches RUNPATH=$ORIGIN into every .so in BIN_DIR so the dynamic linker
# automatically searches the same directory for sibling shared libraries.
#
# Variables expected (passed with -D from add_custom_command):
#   BIN_DIR   — absolute path to the staging binaries directory
#   PATCHELF  — absolute path to the patchelf executable

if(NOT PATCHELF)
    message(FATAL_ERROR "patch_rpath.cmake: PATCHELF variable not set")
endif()
if(NOT BIN_DIR)
    message(FATAL_ERROR "patch_rpath.cmake: BIN_DIR variable not set")
endif()

file(GLOB _libs LIST_DIRECTORIES false "${BIN_DIR}/*.so")

if(NOT _libs)
    message(WARNING "[patchelf] No .so files found in ${BIN_DIR}")
endif()

foreach(_lib IN LISTS _libs)
    execute_process(
        COMMAND "${PATCHELF}" --set-rpath "$ORIGIN" "${_lib}"
        RESULT_VARIABLE _rc
        ERROR_VARIABLE  _err
    )
    if(_rc EQUAL 0)
        message(STATUS "[patchelf] \$ORIGIN set on ${_lib}")
    else()
        message(WARNING "[patchelf] Failed on ${_lib}: ${_err}")
    endif()
endforeach()
