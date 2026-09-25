function(Srf_Set_Compile_Settings target_name target_project_variant)

   # Copy compile_definitions from fbk to input target such that same definitions are seen from feature and fbk
   # perspective, in case that a conditional define or conditional include is used.
   get_target_property(_compile_defs Feature_Building_Kit COMPILE_DEFINITIONS)
   target_compile_definitions(${target_name} PRIVATE ${_compile_defs})

   # If not embedded build then define ct_activate_cal_print
   if(NOT CMAKE_CROSSCOMPILING)
      target_compile_definitions(${target_name} PUBLIC CT_ACTIVATE_CAL_PRINT)

      if(${target_name}_UPDATE_DEFAULT_CALS)
         target_compile_definitions(${target_name} PUBLIC SRF_UPDATE_DEFAULT_CALIBRATION)
      endif()

      if(${target_name}_FULL_DEBUG_MODE)
         message(STATUS "Set full debug mode for ${target_name}")
         target_compile_definitions(${target_name} PUBLIC SRF_FULL_DEBUG_MODE)
      endif()

   endif(NOT CMAKE_CROSSCOMPILING)

   # Set Customer dependent macro. Keep in mind that this is not needed in case of generic customer. Inherit this option
   # e.g. for Unit test build for sake of consistency
   if(NOT ${target_project_variant} MATCHES Generic)
      target_compile_definitions(${target_name} PUBLIC "CUSTOMER_${target_project_variant}")
   endif()

   if(COMPONENT_RESIM_FF_${target_name})
      target_compile_definitions(${target_name} PRIVATE SRF_COMPONENT_RESIM)
   endif()

   if(MSVC)
      # Debug settings
      target_compile_definitions(${target_name} PRIVATE "$<$<CONFIG:Debug>:_DEBUG>")
      target_compile_definitions(${target_name} PRIVATE "$<$<CONFIG:Release>:NDEBUG>")
   else()
      # Debug settings
      target_compile_options(${target_name} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")

      if(TARGET COMPONENT_RESIM_CORE)
         target_compile_options(${target_name} PRIVATE "-fPIC")
      endif()
      if(BUILD_FOR_32_BIT)
         set_target_properties(${target_name} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
      endif()
   endif()

   # Set target path of target
   if(${target_project_variant})
      if(FEATURE_FUNCTION_FOLDER_NAME)
         set(TARGET_PATH ${FEATURE_FUNCTION_FOLDER_NAME})
      else()
         set(TARGET_PATH "${target_project_variant}_SRR/Feature_Functions")
      endif()
      set_target_properties(${target_name} PROPERTIES FOLDER "${TARGET_PATH}")
   else()
      set_target_properties(${target_name} PROPERTIES FOLDER "Feature Functions")
   endif()

endfunction()
