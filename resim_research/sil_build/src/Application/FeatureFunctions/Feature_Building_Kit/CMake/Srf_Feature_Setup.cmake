function(Srf_Feature_Setup target_name target_root_path fbk_root_path all_valid_customers)

   # ###################################################################################################################
   # Set some initial settings
   # ###################################################################################################################

   if(NOT CI_ADDITIONAL_SETUP)
      # Set default build settings
      set(${target_name}_PROJECT_VARIANT "Generic" CACHE STRING "Project variant to be used.")
   else()
      # Set special case settings for CI/CD Jenkins build
      set(${target_name}_PROJECT_VARIANT "${CUSTOMER}" CACHE STRING "Project variant to be used.")
   endif()

   set_property(CACHE ${target_name}_PROJECT_VARIANT PROPERTY STRINGS ${all_valid_customers})

   option(${target_name}_UNIT_TESTS "Include unit tests into build" OFF)
   option(${target_name}_UPDATE_DEFAULT_CALS "Set default calibration values in feature init" ON)
   option(${target_name}_FULL_DEBUG_MODE "Set debug mode to full debug information. Could cause large bin files." OFF)

   # ###################################################################################################################
   # Check for valid customer
   # ###################################################################################################################
   if(NOT ${${target_name}_PROJECT_VARIANT} IN_LIST all_valid_customers)
      message(
         FATAL_ERROR
            "${target_name}_PROJECT_VARIANT ${${target_name}_PROJECT_VARIANT} is not in the list of ${target_name}s valid customers. Please check the ${target_name}_PROJECT_VARIANT definiton or update the list in  ${target_root_path}/CMakeLists.txt"
      )
   endif()

   # ###################################################################################################################
   # Check for cross compilation
   # ###################################################################################################################
   if(CMAKE_CROSSCOMPILING)
      if(RR_COMMON_INC OR RR_COMMON_SRC)
         if(EXISTS ${RR_COMMON_INC})
            include_directories(${RR_COMMON_INC})
         else(EXISTS ${RR_COMMON_INC})
            include_directories(${RR_COMMON_SRC})
         endif(EXISTS ${RR_COMMON_INC})
      else(RR_COMMON_INC OR RR_COMMON_SRC)
         set(REQUIRE_TOOLBOX_UNIT_TST_INCLUDES ON)
      endif(RR_COMMON_INC OR RR_COMMON_SRC)
   endif(CMAKE_CROSSCOMPILING)

   # ###################################################################################################################
   # Linking to required libs
   # ###################################################################################################################
   if(TARGET MLMathLibrary)
      target_link_libraries(${target_name} MLMathLibrary)
   else()
      message(
         FATAL_ERROR "Features rely on shared functions from the math library. Thus MathLibrary needs to be integrated."
      )
   endif()

   if(TARGET CT_CALTOOL)
      target_link_libraries(${target_name} CT_CALTOOL)
   else()
      message(
         FATAL_ERROR
            "For definition of Cal header as well as endian swap method, the c sources of Calibration Tool needs to be integrated."
      )
   endif()

   if(TARGET AS-bin-writer-lib)
      target_link_libraries(${target_name} AS-bin-writer-lib)
   endif()

   # Link Feature_Building_Kit target for abstraction layer and shared feature function libs to given target
   if(TARGET Feature_Building_Kit)
      target_link_libraries(${target_name} Feature_Building_Kit)
   else()
      message(FATAL_ERROR "For Access to Tracker Output API the Feature Kit needs no be integrated.")
   endif()

   # ###################################################################################################################
   # Include necessary c and h files
   # ###################################################################################################################
   include(${target_root_path}/Source/${target_name}_Source.cmake)
   include(${target_root_path}/Customer_Adapter/${target_name}_Customer_Adapter.cmake)
   include(${target_root_path}/Calibration/${target_name}_Calibration.cmake)

   # ###################################################################################################################
   # Targets for further Analysis
   # ###################################################################################################################
   if(${${target_name}_UNIT_TESTS})
      if(TARGET gtest)
         add_subdirectory("${target_root_path}/Testing/SWE4_Unit_Tests" "${CMAKE_CURRENT_BINARY_DIR}/UnitTest")
      else()
         message(WARNING "-- ${target_name}/CMake/CMakeLists.txt: Can not target GTEST since GTEST_PATH is not defined")
      endif()
   endif()

   # ###################################################################################################################
   # Source Grouping and Set folder structure
   # ###################################################################################################################
   get_target_property(${target_name}_SOURCES ${target_name} SOURCES)
   source_group(TREE ${target_root_path} FILES ${${target_name}_SOURCES})
   set_property(GLOBAL PROPERTY USE_FOLDERS ON)

   # ###################################################################################################################
   # Create options to call automated Makefile and Linker File generation
   # ###################################################################################################################
   include(${fbk_root_path}/CMake/Srf_Generate_Mak_Files.cmake)
   include(${fbk_root_path}/CMake/Srf_Generate_Lcf_Files.cmake)

   option(${target_name}_GENERATE_MAK_FILES "Generate *.mak for this feature" OFF)
   option(${target_name}_GENERATE_LCF_FILES "Generate *.lcf for this feature" OFF)

   # We do not require those files for customer Generic
   if(NOT ${${target_name}_PROJECT_VARIANT} STREQUAL "Generic")
      # Automatic generation of mak files
      if(${target_name}_GENERATE_MAK_FILES)
         if(NOT ${target_name}_make_alias)
            set(${target_name}_make_alias "")
         endif()
         srf_generate_mak_files(${target_name} "${${target_name}_make_alias}" ${target_root_path}
                                ${${target_name}_PROJECT_VARIANT} ${fbk_root_path})
      endif()

      # Automatic generation of lcf files
      if(${target_name}_GENERATE_LCF_FILES)
         srf_generate_lcf_files(${target_name} ${target_root_path} ${${target_name}_PROJECT_VARIANT})
      endif()
   endif()

endfunction()
