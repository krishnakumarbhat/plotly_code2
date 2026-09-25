# SetupSGOCGOptions.cmake
#
# Macro to set up and validate ENABLE_SG and ENABLE_OCG component options.
# Provides consistent defaults and mutual-exclusion checking.
#
# Usage: setup_sg_ocg_options(DEFAULT_SG DEFAULT_OCG)
#   DEFAULT_SG:  Default value for ENABLE_SG (ON/OFF)
#   DEFAULT_OCG: Default value for ENABLE_OCG (ON/OFF)
#
# Usage: apply_sg_ocg_compile_definitions(TARGET_NAME [VISIBILITY])
#   TARGET_NAME: Target that should receive ENABLE_SG or ENABLE_OCG
#   VISIBILITY:  PRIVATE, PUBLIC, or INTERFACE. Defaults to PRIVATE.
#

macro(setup_sg_ocg_options default_sg default_ocg)
    # Define options only if not already set by a parent/caller
    if(NOT DEFINED ENABLE_SG)
        option(ENABLE_SG "Enable SG and disable OCG component" ${default_sg})
    endif()

    if(NOT DEFINED ENABLE_OCG)
        option(ENABLE_OCG "Enable OCG and disable SG component" ${default_ocg})
    endif()

    # Validate mutual exclusion
    if(ENABLE_SG AND ENABLE_OCG)
        message(FATAL_ERROR "Only one of ENABLE_SG or ENABLE_OCG can be ON.")
    endif()
endmacro()

macro(apply_sg_ocg_compile_definitions target_name)
    set(_sg_ocg_visibility PRIVATE)
    if(${ARGC} GREATER 1)
        set(_sg_ocg_visibility ${ARGV1})
    endif()

    if(ENABLE_SG)
        target_compile_definitions(${target_name} ${_sg_ocg_visibility} ENABLE_SG)
    elseif(ENABLE_OCG)
        target_compile_definitions(${target_name} ${_sg_ocg_visibility} ENABLE_OCG)
    endif()
endmacro()
