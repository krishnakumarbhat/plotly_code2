# Macro: setup_streams
# Description:
#   setup streams numbers
macro(setup_streams)
    if(SG_VARIANT STREQUAL SCANIA_TRATON)
        set(SG_STREAM_OUTPUT_MINOR          8)
        set(SG_STREAM_REDUCED_OUTPUT_MINOR  8)
        set(SG_STREAM_INTERNALS_MINOR       8)
    else()
        set(SG_STREAM_OUTPUT_MINOR          7)
        set(SG_STREAM_REDUCED_OUTPUT_MINOR  7)
        set(SG_STREAM_INTERNALS_MINOR       7)
    endif()

    set(SG_STREAM_OUTPUT_MAJOR          180)
    set(SG_STREAM_TIMING_MAJOR          181)
    set(SG_STREAM_INTERNALS_MAJOR       182)
    set(SG_STREAM_REDUCED_OUTPUT_MAJOR  183)

    set(SG_STREAM_TIMING_MINOR 2)
endmacro()
