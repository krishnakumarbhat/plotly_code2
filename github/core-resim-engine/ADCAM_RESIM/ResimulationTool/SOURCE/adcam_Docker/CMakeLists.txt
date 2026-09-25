cmake_minimum_required(VERSION 3.20)
project(ifv600_emb_lib C CXX)

# ---------------------------------------------------------------------------
# Global settings
# ---------------------------------------------------------------------------
set(CMAKE_C_STANDARD   11)
set(CMAKE_CXX_STANDARD 17)

# Produce a shared library (.dll / .so)
set(BUILD_SHARED_LIBS ON)

# All static libraries linked into the shared .so must be compiled as PIC.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Put all outputs in a predictable place
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(SWC_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/..")

# ---------------------------------------------------------------------------
# Global compile definitions (visible to all targets in this tree)
# ---------------------------------------------------------------------------
add_compile_definitions(
    EMB_SIL        # SIL-specific guards in application code
    PC_BUILD       # Generic "not AURIX" guard
    NO_HARDWARE    # Suppress direct HW register access
    UNIT_TEST      # Expose internal APIs for testing if needed
)

# Symbol visibility for GCC/Clang shared libraries
if(NOT MSVC)
    add_compile_options(-fvisibility=hidden)
    # RTE-generated headers contain legacy C naming patterns where a struct
    # member can reuse the same identifier as a typedef. GCC rejects this in
    # C++ mode unless permissive parsing is enabled.
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-fpermissive>)
    # Match Tasking ECU behaviour: --align=0 makes all enums use minimum size.
    # Without this, GCC plain C enums default to 4 bytes while C++ enums with
    # an explicit underlying type (e.g. sata_enums.h ':uint8_T') are 1 byte,
    # causing layout mismatches for any struct shared across C/C++ boundaries
    # (SATA_Fusion_Track_T, Fusion_Object_Track_T, FUS_Stream, etc.).
    add_compile_options(-fshort-enums)
    #add_compile_options(-fpack-struct=4)
endif()

option(XTRK_ENABLE "Enable XTRK tracker source updates during configure" OFF)

find_package(Python3 REQUIRED COMPONENTS Interpreter)

if(XTRK_ENABLE)
    # file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Output/XTRK")
    set(PYTHON_SCRIPT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/update.py")
    set(XTRK_STATUS_MSG "[XTRK]: TrackerLib Files Updated")
    set(XTRK_ERROR_MSG "XTRK Python Script Failed to Update")
else()
    set(PYTHON_SCRIPT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/trackercodereset.py")
    set(XTRK_STATUS_MSG "[GIT]: TrackerLib Files Reset")
    set(XTRK_ERROR_MSG "Tracker Reset Python Script Failed to Reset")
endif()

#execute_process(
#    COMMAND "${Python3_EXECUTABLE}" "${PYTHON_SCRIPT_PATH}"
#    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
#    RESULT_VARIABLE SCRIPT_RETURN_CODE
#)

#if(NOT SCRIPT_RETURN_CODE EQUAL 0)
#    message(FATAL_ERROR "${XTRK_ERROR_MSG}")
#else()
#    message(STATUS "${XTRK_STATUS_MSG}")
#endif()

# ---------------------------------------------------------------------------
# Hardware abstraction stubs (MCAL no-ops, OS no-ops, AUTOSAR types)
# ---------------------------------------------------------------------------
add_subdirectory(hw_stubs)

# ---------------------------------------------------------------------------
# RTE stub layer (global signal buffers + per-SWC glue implementations)
# Must be added BEFORE the platform_sw module subdirs.
# ---------------------------------------------------------------------------
add_subdirectory(rte_stubs)

# ---------------------------------------------------------------------------
# Platform-SW SWC modules
# Each subdirectory builds a static library that wraps one SWC from
# Appl/platform_sw/src/ and pulls in the rte_stubs include paths.
# ---------------------------------------------------------------------------
add_subdirectory(modules/objFusion)
add_subdirectory(modules/objTracker_flr_wrapper)
add_subdirectory(modules/objTracker_mrr64det_wrapper)
# Enforce strict sequential build order for the two tracker variants:
#   objTracker(N) → objTracker_flr_sil → objTracker(M) → objTracker_mrr64det_sil
#
# Both F360TrackerLib ExternalProject builds write f360_variant_definition.h
# into the same shared SOURCE_DIR. Running them in parallel corrupts that file.
# Making M's external builds depend on objTracker_flr_sil (which itself depends
# on N's external builds) gives the exact sequential chain needed.
# Full sequential chain:
#   rspp_N → f360_N → objTracker_flr_sil → rspp_M → f360_M → objTracker_mrr64det_sil
add_dependencies(rspp_variant_M_ext objTracker_flr_sil)
add_dependencies(f360_variant_M_ext rspp_variant_M_ext)
add_subdirectory(modules/vse)
add_subdirectory(modules/olp)
add_subdirectory(modules/features)
add_subdirectory(modules/sata)
add_subdirectory(modules/eyeqif_scale1)
add_subdirectory(modules/cdd_featureif)
add_subdirectory(modules/sip)
add_subdirectory(modules/spp_otp)

# ---------------------------------------------------------------------------
# yaml-cpp (static, even though BUILD_SHARED_LIBS is ON globally)
# ---------------------------------------------------------------------------
set(YAML_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TESTS   OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TOOLS   OFF CACHE BOOL "" FORCE)
set(YAML_CPP_INSTALL       OFF CACHE BOOL "" FORCE)
add_subdirectory(external_libs/yaml-cpp)

add_library(ConfigLoader STATIC 
            sil_wrappers/sil_config.cpp
            sil_wrappers/sil_config.h)
target_include_directories(ConfigLoader PRIVATE 
            external_libs/lib/include)
target_link_libraries(ConfigLoader PRIVATE yaml-cpp)
target_compile_options(ConfigLoader PRIVATE -fno-pack-struct)
target_compile_definitions(ConfigLoader PRIVATE YAML_CPP_STATIC_DEFINE)
# ---------------------------------------------------------------------------
# Main SIL shared library
# ---------------------------------------------------------------------------
add_library(ifv600_emb_lib SHARED
    sil_wrappers/sil_main.cpp
    sil_wrappers/sil_input_wrapper.cpp
    sil_wrappers/sil_output_wrapper.cpp
    sil_wrappers/sil_mudp.cpp
    sil_wrappers/sil_udp_log_edrstatus.cpp
    sil_wrappers/sil_udp_log_ff_10ms_output.cpp
    sil_wrappers/sil_udp_log_ff_20ms_output.cpp
    sil_wrappers/sil_udp_log_objectfusion.cpp
    sil_wrappers/sil_udp_log_olp_data_logging.cpp
    sil_wrappers/sil_udp_log_proxi_and_car_cfg_param.cpp
    sil_wrappers/sil_udp_log_radarif_output.cpp
    sil_wrappers/sil_udp_log_sata_spp_otp_veh.cpp
    sil_wrappers/sil_udp_log_trackerflr_output.cpp
    sil_wrappers/sil_udp_log_trackermrr_output.cpp
    sil_wrappers/sil_udp_log_vehstream_calibration.cpp
    sil_wrappers/sil_udp_log_vse_output.cpp
    sil_wrappers/sil_udp_log_artm_measurement.cpp
    sil_wrappers/sil_udp_log_avi_output.cpp
    sil_wrappers/sil_udp_log_debugstream.cpp
    sil_wrappers/sil_tracker_wrapper.cpp
    sil_wrappers/sil_wrapper_core_utility.cpp
    sil_wrappers/sil_objfusion_wrapper.cpp
    sil_wrappers/autogen_LRCF_encode_messages.cpp
    sil_wrappers/sil_vcan_wrapper.cpp
    sil_wrappers/sil_wrapper_features.cpp
    sil_wrappers/sil_calibration_wrapper.cpp
    sil_wrappers/sil_nvm_wrapper.cpp;
    sil_wrappers/sil_eyeqif_wrapper.cpp
    sil_wrappers/sil_vse_wrapper.cpp
)


target_include_directories(ifv600_emb_lib BEFORE PUBLIC 
        rte_stubs
        #${SWC_ROOT}/platform_sw/src/swbb_asr_swc_objFusion/test/CtCdd_ObjectFusion_test/stub
        )


file(GLOB_RECURSE SATA_SWC_HEADER_FILES "${SWC_ROOT}/platform_sw/src/swbb_asr_swc_sata/protected/src/aptiv/*.h")
set(SATA_SWC_INCLUDE_DIRS "")
foreach(hdr IN LISTS SATA_SWC_HEADER_FILES)
    get_filename_component(dir "${hdr}" DIRECTORY)
    if(NOT dir MATCHES "[/\\\\]Testing([/\\\\]|$)"
       AND NOT dir MATCHES "[/\\\\]tests?([/\\\\]|$)")
        list(APPEND SATA_SWC_INCLUDE_DIRS "${dir}")
    endif()
endforeach()
list(REMOVE_DUPLICATES SATA_SWC_INCLUDE_DIRS)

target_include_directories(ifv600_emb_lib PRIVATE 
        rte_stubs
        sil_wrappers
		sil_wrappers/SIL_Engine_Headers
        sil_wrappers/sil_logging_headers
        sil_wrappers/autogen_headers
        ${SWC_ROOT}/platform_sw/src/swbb_asr_cdd_featureif/protected/src/aptiv/algo_cfg
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_sata/protected/src/aptiv/bb_cpp_core_lib/sata_wrapper/inc/
        ${SWC_ROOT}/project_sw/swc/udp/protected/aptiv
        ${SWC_ROOT}/project_sw/swc/edr/inc
        ${SWC_ROOT}/project_sw/swc/swbb_asr_swc_radarIf_flr/protected/generated/flr_if
        ${SWC_ROOT}/project_sw/swc/swbb_asr_swc_radarIf_mrr/protected/generated/mrr_if
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_objTracker_flr/protected/src/aptiv/tracker_wrapper/inc
        # rot_iface dirs: f360_log_types.h places types inside namespace f360_variant_N
        # under #ifdef __cplusplus — these dirs must be on the path for C++ TUs.
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_objTracker_flr/protected/src/aptiv/tracker_core/rot_iface/Logging
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_objTracker_flr/protected/src/aptiv/tracker_core/rot_iface/Types
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_objTracker_flr/protected/src/aptiv/tracker_core/rot_iface/core
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_objTracker_flr/protected/src/aptiv/tracker_core/rspp_iface/core
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_radarIf_mrr/protected/src/aptiv/tracker_wrapper/inc
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_objFusion/protected/src/aptiv/of_wrapper/inc
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_sata/protected/src/aptiv/sata_wrapper/inc
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_sata/protected/src/aptiv/bb_cpp_core_lib/sata_building_block/tsc/inc
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_sata/protected/src/aptiv/bb_cpp_core_lib/sata_building_block/esata/inc
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_sata/protected/src/aptiv/bb_cpp_core_lib/sata_building_block/fazta/inc
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_sata/protected/src/aptiv/bb_cpp_core_lib/sata_building_block/frme/inc
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_sata/protected/src/aptiv/bb_cpp_core_lib/sata_building_block/ftsel/inc
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_sata/protected/src/aptiv/bb_cpp_core_lib/sata_building_block/ita/inc
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_sata/protected/src/aptiv/bb_cpp_core_lib/sata_building_block/lcta/inc
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_sata/protected/src/aptiv/bb_cpp_core_lib/sata_building_block/rrme/inc
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_sata/protected/src/aptiv/bb_cpp_core_lib/sata_building_block/rtsel/inc
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_sata/protected/src/aptiv/bb_cpp_core_lib/sata_building_block/sata_common/inc
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_sata/protected/src/aptiv/bb_cpp_core_lib/sata_building_block/tmp/inc
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_sata/protected/src/aptiv/bb_cpp_core_lib/sata_building_block/ula/inc
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_sata/protected/src/aptiv/bb_cpp_core_lib/sata_building_block/vruta/inc
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_sata/protected/src/aptiv/bb_cpp_core_lib/interface/helper
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_sata/protected/src/aptiv/bb_cpp_core_lib/interface
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_spp_otp/protected/src/aptiv/bb_cpp_core_lib/stream
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_spp_otp/protected/src/aptiv/bb_cpp_core_lib/TJA_wrapper/inc
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_objTracker_mrr64det/protected/src/aptiv/tracker_core/rot_iface/Logging
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_objTracker_mrr64det/protected/src/aptiv/tracker_core/rot_iface/Types
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_objTracker_mrr64det/protected/src/aptiv/tracker_core/rot_iface/core
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_objTracker_mrr64det/protected/src/aptiv/tracker_wrapper/inc
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_objTracker_mrr64det/protected/src/aptiv/tracker_core/rspp_iface
        ${SWC_ROOT}/platform_sw/src/swbb_asr_cdd_featureif/protected/src/aptiv/utils/aptiv/inc
        ${SWC_ROOT}/platform_sw/src/swbb_asr_cdd_featureif/protected/src/aptiv/utils/generated/inc
        ${SWC_ROOT}/platform_sw/src/eyeqif_scale1/protected/src/aptiv/AVI/_inc
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_Vse/protected/aptiv/header/
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_Vse/protected/aptiv/integration/
        ${SWC_ROOT}/project_sw/swc/sw_version/_inc/
        ${SWC_ROOT}/platform_sw/src/swbb_asr_swc_olp/protected/src/aptiv/olp_wrapper/inc
    )

# SATA type headers (sata_enums.h, sata_interface_input.h) gate all definitions
# behind #if defined(SATA_PROJECT_NORTHSTAR) || CADM || RENAULT.
# SATA_PROJECT_CADM is defined here to select the CADM code paths for the SIL build.
target_compile_definitions(ifv600_emb_lib PRIVATE 
        SW_VAR_SCALE1 
        _ALLOW_KEYWORD_MACROS 
        EMB_LIB_SIL_WRAPPER_SHARED_EXPORT 
        SATA_PROJECT_CADM 
        UDP_UNIT_TEST
        AVI_HANDCODE=1 
        INCLUDE_SPP_OTP 
        INCLUDE_SATA_L2P 
        INCLUDE_SATA_ESATA 
        ENABLE_AVI_MAPPING
        IFV600_LIBRARY_NAME="ifv600_emb_lib"
    )

target_link_libraries(ifv600_emb_lib
    PRIVATE
        hw_stubs
        rte_stubs
        objFusion_sil
        objTracker_flr_sil
        objTracker_mrr64det_sil
        vse_sil
        olp_sil
        # features_sil
        sata_sil
        eyeqif_scale1_sil
        cdd_featureif_sil
        sip_sil
        spp_otp_sil
        EEV_sil
        HBC_sil
        IM_sil
        IW_sil
        LSF_sil
        MC_sil
        MMM_sil
        ODD1_sil
        OW_sil
        SFL_sil
        TSI_sil
        UTP_sil
        ConfigLoader
)

if(MSVC)
    # Windows / Visual Studio — /Zp4 matches embedded 4-byte packing
    target_compile_options(ifv600_emb_lib PRIVATE
        "/FI${CMAKE_CURRENT_SOURCE_DIR}/rte_stubs/sil_suppress.h"
        "/FI${CMAKE_CURRENT_SOURCE_DIR}/rte_stubs/udp_record.h"
        "/FI${CMAKE_CURRENT_SOURCE_DIR}/rte_stubs/Rte_Type.h"
        "/FI${SWC_ROOT}/platform_sw/src/swbb_asr_swc_objTracker_flr/protected/src/aptiv/tracker_wrapper/inc/tracker_namespace_def_flr.h"
        "/FI${SWC_ROOT}/platform_sw/src/swbb_asr_swc_objTracker_mrr64det/protected/src/aptiv/tracker_wrapper/inc/tracker_namespace_def_srr.h"
	# Same MRR force-includes as GCC: FLR's rot_iface/Logging appears earlier
        # in the -I search path, so f360_log_types.h must be forced by full path.
        "/FI${SWC_ROOT}/platform_sw/src/swbb_asr_swc_objTracker_mrr64det/protected/src/aptiv/tracker_core/rot_iface/core/f360_constants.h"
        "/FI${SWC_ROOT}/platform_sw/src/swbb_asr_swc_objTracker_mrr64det/protected/src/aptiv/tracker_core/rot_iface/Logging/f360_log_types.h"
    )
else()
    # Linux / GCC / Clang — -fpack-struct=4 matches embedded 4-byte packing
    target_compile_options(ifv600_emb_lib PRIVATE
        "-fpack-struct=4"
        "-include${CMAKE_CURRENT_SOURCE_DIR}/rte_stubs/sil_suppress.h"
        "-include${CMAKE_CURRENT_SOURCE_DIR}/rte_stubs/udp_record.h"
        "-include${CMAKE_CURRENT_SOURCE_DIR}/rte_stubs/Rte_Type.h"
        # Declares f360_variant_N / f360_variant_M namespaces before
        # tracker_f360_internal_logging_flr.h and radar_ecu_CORE1*.h are parsed
        # in C++ mode (their #ifdef __cplusplus branches use these namespaces).
        "-include${SWC_ROOT}/platform_sw/src/swbb_asr_swc_objTracker_flr/protected/src/aptiv/tracker_wrapper/inc/tracker_namespace_def_flr.h"
        "-include${SWC_ROOT}/platform_sw/src/swbb_asr_swc_objTracker_mrr64det/protected/src/aptiv/tracker_wrapper/inc/tracker_namespace_def_srr.h"
        # MRR f360_log_types.h must be force-included because FLR's rot_iface/Logging
        # appears earlier in the -I search path, so plain #include "f360_log_types.h"
        # always resolves to FLR's variant-N version.  The force-include uses the full
        # path to guarantee MRR's variant-M types (F360_Object_Log_Output_T etc.) are
        # defined in namespace f360_variant_M before radar_ecu_CORE1.h is parsed.
        # f360_constants.h must be force-included first because it defines the
        # f360_variant_M::MAX_NUMBER_OF_SENSORS constant used by f360_rot_object_log.h.
        "-include${SWC_ROOT}/platform_sw/src/swbb_asr_swc_objTracker_mrr64det/protected/src/aptiv/tracker_core/rot_iface/core/f360_constants.h"
        "-include${SWC_ROOT}/platform_sw/src/swbb_asr_swc_objTracker_mrr64det/protected/src/aptiv/tracker_core/rot_iface/Logging/f360_log_types.h"
   )
endif()


# Export ALL symbols on Windows (equivalent to Bazel --features=fully_static_link)
if(MSVC)
    set_target_properties(ifv600_emb_lib PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS ON
    )
endif()

# Human-readable output name: no "lib" prefix on MinGW (Windows DLL convention),
# keep the default "lib" prefix on Linux.
set_target_properties(ifv600_emb_lib PROPERTIES OUTPUT_NAME "ifv600_emb_lib")
set_target_properties(ifv600_emb_lib PROPERTIES PREFIX "")


# ---------------------------------------------------------------------------
# Copy MinGW runtime DLLs to bin/ so the DLL can be loaded without PATH setup
# ---------------------------------------------------------------------------
if(MINGW)
    get_filename_component(_gcc_bin_dir "${CMAKE_C_COMPILER}" DIRECTORY)
    set(_mingw_runtime_dlls
        "${_gcc_bin_dir}/libgcc_s_seh-1.dll"
        "${_gcc_bin_dir}/libstdc++-6.dll"
        "${_gcc_bin_dir}/libwinpthread-1.dll"
    )
    foreach(_dll IN LISTS _mingw_runtime_dlls)
        if(EXISTS "${_dll}")
            add_custom_command(TARGET ifv600_emb_lib POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_dll}" "$<TARGET_FILE_DIR:ifv600_emb_lib>"
                COMMENT "Copying ${_dll} to bin/"
            )
        endif()
    endforeach()

endif()

add_custom_command(TARGET ifv600_emb_lib POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/emb_sil_config.yaml" "$<TARGET_FILE_DIR:ifv600_emb_lib>"
    COMMENT "Copying emb_sil_config.yaml to bin/"
)
# ---------------------------------------------------------------------------
# Optional: simple standalone test runner
# ---------------------------------------------------------------------------
option(BUILD_TESTS "Build the SIL self-test harness" OFF)
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

# ---------------------------------------------------------------------------
# Install rules (optional)
# ---------------------------------------------------------------------------
install(TARGETS ifv600_emb_lib
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)
# ---------------------------------------------------------------------------
# Docker image + Singularity image – POST_BUILD steps on ifv600_emb_lib
#
# Enable with:  cmake -DBUILD_DOCKER_IMAGE=ON <source_dir>
#
# Build sequence (each step depends on the previous):
#   1. docker build   → image in local Docker cache (ifv600-resim:latest)
#   2. docker save    → docker/build/ifv600-resim.tar.gz
#   3. singularity build (if singularity is on PATH)
#                     → docker/build/ifv600-resim.simg
#
# Requires Docker daemon to be running on the build machine.
# Singularity is optional; the step is skipped if not found.
# ---------------------------------------------------------------------------
option(BUILD_DOCKER_IMAGE "Build the ifv600-resim Docker/Singularity images as a post-build step" OFF)

if(BUILD_DOCKER_IMAGE)
    message(WARNING  
    
            "Note: while creating docker image ifv600-resim, copy APT_SRR_RESIM executables and its dependencies to bin folder.First verify the executables are working fine in host machine and then build the docker image. If the executables are not working fine in host machine, then it will not work in docker image as well.")

    find_program(DOCKER_EXECUTABLE docker)
    if(NOT DOCKER_EXECUTABLE)
        message(FATAL_ERROR
            "BUILD_DOCKER_IMAGE=ON but 'docker' executable was not found. "
            "Install Docker and ensure it is on PATH.")
    endif()

    set(_docker_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/docker/build")
    set(_docker_tar        "${_docker_output_dir}/ifv600-resim.tar")
    set(_singularity_simg  "${_docker_output_dir}/ifv600-resim.simg")

    # ------------------------------------------------------------------
    # Step 1: docker build → local image cache
    # ------------------------------------------------------------------
    add_custom_command(TARGET ifv600_emb_lib POST_BUILD
        COMMAND ${DOCKER_EXECUTABLE} build
                --no-cache
                --tag  ifv600-resim:latest
                --file "${CMAKE_CURRENT_SOURCE_DIR}/Dockerfile"
                "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMENT "POST_BUILD [1/3]: Building Docker image ifv600-resim:latest"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )

    # ------------------------------------------------------------------
    # Step 2: docker save | gzip → docker/build/ifv600-resim.tar.gz
    # Uses bash -c so the shell pipe is evaluated correctly.
    # ------------------------------------------------------------------
    add_custom_command(TARGET ifv600_emb_lib POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${_docker_output_dir}"
        COMMAND ${DOCKER_EXECUTABLE} save --output=${_docker_tar} ifv600-resim:latest
        COMMENT "POST_BUILD [2/3]: Exporting image to docker/build/ifv600-resim.tar"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        VERBATIM
    )

    # ------------------------------------------------------------------
    # Step 3 (optional): singularity build → docker/build/ifv600-resim.simg
    # Reads the tar produced in step 2 via the docker-archive:// URI.
    # ------------------------------------------------------------------
    set(SINGULARITYCE_PREFERRED_PREFIX "$ENV{HOME}/.local/singularityce-4.1.4" CACHE PATH
        "Preferred SingularityCE installation prefix")
    find_program(SINGULARITY_EXECUTABLE
        NAMES singularity
        HINTS "${SINGULARITYCE_PREFERRED_PREFIX}/bin"
        DOC "SingularityCE executable"
    )
    if(SINGULARITY_EXECUTABLE)
        add_custom_command(TARGET ifv600_emb_lib POST_BUILD
            COMMAND ${SINGULARITY_EXECUTABLE} build
                    --force
                    "${_singularity_simg}"
                    "docker-archive://${_docker_tar}"
            COMMENT "POST_BUILD [3/3]: Building Singularity image docker/build/ifv600-resim.simg"
            WORKING_DIRECTORY "${_docker_output_dir}"
        )
        message(STATUS
            "Singularity found (${SINGULARITY_EXECUTABLE}): "
            "ifv600-resim.simg will be built in docker/build/")
    else()
        message(STATUS
            "Singularity not found on PATH: step 3 (Singularity image) will be skipped. "
            "Install Singularity/Apptainer and re-run cmake to enable it.")
    endif()

    message(STATUS
        "BUILD_DOCKER_IMAGE=ON: POST_BUILD pipeline attached to ifv600_emb_lib – "
        "docker build → docker/build/ifv600-resim.tar.gz"
        "${SINGULARITY_EXECUTABLE}:+ → docker/build/ifv600-resim.simg}")
endif()

