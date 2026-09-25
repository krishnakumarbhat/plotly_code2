cmake_minimum_required(VERSION 3.14)
project(LogicModel2 VERSION 1.0 LANGUAGES CXX)

set(FMU_NAME "LogicModel2")

# ── Platform detection ─────────────────────────────────────────────────────────
if(WIN32)
    set(FMU_PLATFORM "win64")
    set(FMU_LIB_EXT  ".dll")
elseif(UNIX)
    set(FMU_PLATFORM "linux64")
    set(FMU_LIB_EXT  ".so")
else()
    message(FATAL_ERROR "Unsupported platform — only Windows and Linux are supported.")
endif()

# ── Paths ──────────────────────────────────────────────────────────────────────
set(SRC_DIR       "${CMAKE_CURRENT_SOURCE_DIR}")
set(SCRIPTS_DIR   "${CMAKE_CURRENT_SOURCE_DIR}/../Scripts")
set(BUILD_OUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../Build")
set(GEN_CPP       "${SRC_DIR}/generated_signal_map.cpp")
set(GEN_H         "${SRC_DIR}/generated_structs.h")
set(MODEL_DESC    "${SRC_DIR}/modelDescription.xml")
set(FMU_OUT       "${BUILD_OUT_DIR}/${FMU_NAME}.fmu")
set(STAGING_DIR   "${CMAKE_BINARY_DIR}/staging")
set(EXTERNAL_LIB_DIR       "${CMAKE_CURRENT_SOURCE_DIR}/../ExternalLibs/Libs")
set(EXTERNAL_LIB_INCLUDE_DIR       "${CMAKE_CURRENT_SOURCE_DIR}/../ExternalLibs/Includes")

# ── Verify generated files exist ──────────────────────────────────────────────
# These are produced by dbc_to_fmi_generator.py and are NOT auto-regenerated
# here. Run build.bat --generate <dbc_path> (or build.sh --generate) first.
foreach(_f "${GEN_CPP}" "${GEN_H}" "${MODEL_DESC}")
    if(NOT EXISTS "${_f}")
        get_filename_component(_name "${_f}" NAME)
        message(FATAL_ERROR
            "Missing generated file: ${_name}\n"
            "Run the build script with --generate <dbc_path> to produce it first.")
    endif()
endforeach()

# ── Compile shared library ────────────────────────────────────────────────────
add_library(${FMU_NAME} SHARED
    "${GEN_CPP}"
    "${SRC_DIR}/LogicModel2.cpp"
    "${SRC_DIR}/Version.cpp"
    "${SRC_DIR}/autogen_LRCF_message_decoding.cpp"
)

target_include_directories(${FMU_NAME} PRIVATE "${SRC_DIR}" "${EXTERNAL_LIB_INCLUDE_DIR}")

set_target_properties(${FMU_NAME} PROPERTIES
    CXX_STANDARD            14
    CXX_STANDARD_REQUIRED   ON
    POSITION_INDEPENDENT_CODE ON
    PREFIX                  ""
    OUTPUT_NAME             "${FMU_NAME}"
)

if(MSVC)
    target_compile_options(${FMU_NAME} PRIVATE /W3 /O2)
    set_target_properties(${FMU_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
else()
    target_compile_options(${FMU_NAME} PRIVATE -Wall -Wno-unused-parameter -O2)
endif()

if(UNIX)
    # $ORIGIN makes LogicModel2.so resolve its own dependencies from its directory
    set_target_properties(${FMU_NAME} PROPERTIES
        INSTALL_RPATH            "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH TRUE
    )
    find_program(PATCHELF patchelf REQUIRED)
endif()

# ── Package FMU zip ───────────────────────────────────────────────────────────
add_custom_command(
    TARGET "${FMU_NAME}" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
            "${STAGING_DIR}/binaries/${FMU_PLATFORM}"
    COMMAND ${CMAKE_COMMAND} -E copy
            "${MODEL_DESC}"
            "${STAGING_DIR}/modelDescription.xml"
    COMMAND ${CMAKE_COMMAND} -E copy
            "$<TARGET_FILE:${FMU_NAME}>"
            "${STAGING_DIR}/binaries/${FMU_PLATFORM}/${FMU_NAME}${FMU_LIB_EXT}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${EXTERNAL_LIB_DIR}"
            "${STAGING_DIR}/binaries/${FMU_PLATFORM}"
)

# ── Patch RUNPATH=$ORIGIN on all staged .so files (Linux only) ────────────────
# This must run AFTER copy_directory so external libs are already in staging,
# but BEFORE the zip step so the patched versions are what goes into the FMU.
# Uses a cmake -P script to avoid all bash/make/CMake shell-escaping issues
# ($$lib, $ORIGIN quoting etc. are error-prone inside bash -c "...").
if(UNIX)
    add_custom_command(
        TARGET "${FMU_NAME}" POST_BUILD
        COMMAND ${CMAKE_COMMAND}
            "-DBIN_DIR=${STAGING_DIR}/binaries/${FMU_PLATFORM}"
            "-DPATCHELF=${PATCHELF}"
            -P "${CMAKE_CURRENT_SOURCE_DIR}/patch_rpath.cmake"
        COMMENT "Patching RUNPATH=\$ORIGIN for all .so in staging…"
    )
endif()

# ── Create FMU zip (runs last: after copy + RPATH patching) ──────────────────
add_custom_command(
    TARGET "${FMU_NAME}" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${BUILD_OUT_DIR}"
    COMMAND ${CMAKE_COMMAND} -E tar "cf" "${FMU_OUT}" --format=zip "."
    WORKING_DIRECTORY "${STAGING_DIR}"
    COMMENT "Packaging ${FMU_NAME}.fmu  [${FMU_PLATFORM}]…"
)

add_custom_target(package_fmu ALL DEPENDS "${FMU_OUT}")

# ── Validate target ───────────────────────────────────────────────────────────
add_custom_target(validate
    COMMAND python3 "${SCRIPTS_DIR}/validate_lrcf_fmu.py" "${FMU_OUT}"
    WORKING_DIRECTORY "${SCRIPTS_DIR}"
    DEPENDS package_fmu
    COMMENT "Running FMPy validation suite…"
)

# ── Summary ───────────────────────────────────────────────────────────────────
message(STATUS "FMU name  : ${FMU_NAME}")
message(STATUS "Platform  : ${FMU_PLATFORM}")
message(STATUS "Output    : ${FMU_OUT}")
message(STATUS "")
message(STATUS "Targets:")
message(STATUS "  cmake --build .              → compile + package")
message(STATUS "  cmake --build . -t validate  → + run FMPy validation")
