set(_TRACKER_VARIANT_VALUES A B C D E F G H I J K L M N O P Q R S)

function(setup_f360_tracker_variant trackerlib_dir namespace_var default_variant_var)
   set(F360_TRACKER_VARIANT "A" CACHE STRING "Tracker variant")
   set_property(CACHE F360_TRACKER_VARIANT PROPERTY STRINGS ${_TRACKER_VARIANT_VALUES})

   string(TOUPPER "${F360_TRACKER_VARIANT}" _f360_tracker_variant)
   if(NOT _f360_tracker_variant IN_LIST _TRACKER_VARIANT_VALUES)
      message(FATAL_ERROR "Unsupported F360 tracker variant '${F360_TRACKER_VARIANT}'. Expected one of: ${_TRACKER_VARIANT_VALUES}")
   endif()

   message("F360 Tracker: Used variant -> ${_f360_tracker_variant}")

   string(TOLOWER "${_f360_tracker_variant}" _f360_tracker_variant_lower)
   configure_file(
      ${trackerlib_dir}/SharedTrackerAPI/core/variants/f360_variant_definition_${_f360_tracker_variant_lower}.h
      ${trackerlib_dir}/SharedTrackerAPI/core/f360_variant_definition.h
      COPYONLY)

   set(${namespace_var} "f360_variant_${_f360_tracker_variant}" PARENT_SCOPE)

   if("${_f360_tracker_variant}" STREQUAL "A")
      set(${default_variant_var} ON PARENT_SCOPE)
   else()
      set(${default_variant_var} OFF PARENT_SCOPE)
   endif()
endfunction()

function(setup_rspp_variant rspp_include_dir namespace_var)
   set(RSPP_VARIANT "A" CACHE STRING "Tracker variant")
   set_property(CACHE RSPP_VARIANT PROPERTY STRINGS ${_TRACKER_VARIANT_VALUES})

   string(TOUPPER "${RSPP_VARIANT}" _rspp_variant)
   if(NOT _rspp_variant IN_LIST _TRACKER_VARIANT_VALUES)
      message(FATAL_ERROR "Unsupported RSPP variant '${RSPP_VARIANT}'. Expected one of: ${_TRACKER_VARIANT_VALUES}")
   endif()

   message("RSPP: Used variant -> ${_rspp_variant}")

   string(TOLOWER "${_rspp_variant}" _rspp_variant_lower)
   configure_file(
      ${rspp_include_dir}/variants/rspp_variant_definition_${_rspp_variant_lower}.h
      ${rspp_include_dir}/rspp_variant_definition.h
      COPYONLY)

   set(${namespace_var} "rspp_variant_${_rspp_variant}" PARENT_SCOPE)
endfunction()