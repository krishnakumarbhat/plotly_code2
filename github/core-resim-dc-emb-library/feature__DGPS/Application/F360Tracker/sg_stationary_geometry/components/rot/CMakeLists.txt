cmake_minimum_required(VERSION 3.16)

project("ROT_interface")

set(tmp_cmake_folder ${CMAKE_FOLDER})

add_subdirectory(rot_10_42_00)

#############################################################
# Extension w.r.t. to ROT CMake file
#############################################################
set(iface_generated_dir ${CMAKE_BINARY_DIR}/iface_variant_${F360_TRACKER_VARIANT})
target_include_directories(
    F360-Tracker SYSTEM
        INTERFACE 
            "${iface_generated_dir}/core"
            "${iface_generated_dir}/Logging"
            "${iface_generated_dir}/Types"
)

# It is not recommended to use GLOB/GLOB_RECURSE (https://cmake.org/cmake/help/latest/command/file.html#filesystem)
# GLOB_RECURSE is used due to interface files are generated from Enterprise Architect 
file(GLOB_RECURSE interface_files CONFIGURE_DEPENDS "${iface_generated_dir}/*.h")
target_sources(
    F360-Tracker
        PRIVATE
            ${interface_files}
)

set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(
    F360-Tracker
        INTERFACE
            "$<${msvc_cxx}:/FI${CMAKE_CURRENT_SOURCE_DIR}/rot_fix.h>" # turn off assert for structure size checking
)

message(DEBUG "SG|DEBUG MSG: ROT interface is integrated")

set(CMAKE_FOLDER ${tmp_cmake_folder})
