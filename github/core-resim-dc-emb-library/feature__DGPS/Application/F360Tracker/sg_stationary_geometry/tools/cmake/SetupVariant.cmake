# Function: setup_variant
# Description:
#   setup variant
macro(setup_variant)
    ###################################################
    # Variant configuration
    ###################################################
    set(SG_VARIANT "" CACHE STRING "Core variant")
    set(SUPPORTED_VARIANTS
            PLATFORM_UNLIMITED
            PLATFORM_HIGH
            PLATFORM_MEDIUM
            PLATFORM_LOW
            PLATFORM_REDUCED
            BMW_SP25_MRR
            SCANIA_TRATON
            NORTH_STAR_FRONT
            NORTH_STAR_CORNER
            NORTH_STAR_REAR
    )
    set_property(CACHE SG_VARIANT PROPERTY STRINGS ${SUPPORTED_VARIANTS})

    if(NOT SG_VARIANT IN_LIST SUPPORTED_VARIANTS)
       message( FATAL_ERROR "Variant is not provided or not supported ('${SG_VARIANT}'). Set SG_VARIANT to one of possible variants ${SUPPORTED_VARIANTS}" )
    endif()

    ###################################################
    # DC on/off
    ###################################################
    set(DC_SUPPORTED_VARIANTS
            PLATFORM_UNLIMITED
            PLATFORM_HIGH
            PLATFORM_MEDIUM
            PLATFORM_LOW
            PLATFORM_REDUCED
            BMW_SP25_MRR
            NORTH_STAR_FRONT
            NORTH_STAR_CORNER
            NORTH_STAR_REAR
    )

    if(SG_VARIANT IN_LIST DC_SUPPORTED_VARIANTS)
        set(SG_DC_ENABLED ON)
    else()
        set(SG_DC_ENABLED OFF)
    endif()
endmacro()