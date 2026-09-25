set(tmp_cmake_folder ${CMAKE_FOLDER})

###########################################################################
# Resimulation framework
###########################################################################
option(SG_RESIM "Resimulation framework" OFF)
if(SG_RESIM)
   set(COMPONENT_RESIM_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Use the same buildtype for the component resim.") # Otherwise the CR would force to "Release".
   set(MODULE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Use the same buildtype for the component resim.") # Otherwise there might be internal wrong values.
   add_subdirectory(resim)

   set_target_properties(
      COMPONENT_RESIM_CORE
      COMPONENT_RESIM_EXECUTABLE
         PROPERTIES
            FOLDER ${tmp_cmake_folder}/resim/component_resim
   )
   set_target_properties(
      EXTERN_CMP
      EXTERN_PUGI_XML
         PROPERTIES
            FOLDER ${tmp_cmake_folder}/resim/component_resim/extern
   )
   set_target_properties(
      stationary_geometries_wrapper
         PROPERTIES
            FOLDER ${tmp_cmake_folder}/resim
   )
   
   if (SG_DRIVABILITY_CLASSIFICATION_TOOLING_WRAPPER)
      set_target_properties(
         dc_tooling_wrapper
            PROPERTIES
               FOLDER ${tmp_cmake_folder}/resim
   )
   endif()
   
   message(DEBUG "SG|DEBUG MSG: Resim framework is integrated")
endif()

###########################################################################
# Timer for resimm framwewrok
###########################################################################
add_subdirectory(timer_resim)

##############################################################################
# External components like RSPP (Radar Sensor Preproccessing)
##############################################################################
option(SG_MOCK_EXTERNAL_INCLUDES "Mock required headers using internal directory" OFF)
if(SG_MOCK_EXTERNAL_INCLUDES)
    add_subdirectory(rot)
endif()


set(CMAKE_FOLDER ${tmp_cmake_folder})