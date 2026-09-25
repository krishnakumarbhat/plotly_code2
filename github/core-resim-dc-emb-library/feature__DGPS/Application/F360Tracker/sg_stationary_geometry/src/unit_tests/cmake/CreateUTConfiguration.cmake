# Function: create_ut_wrapper_configuration
# Description:
#   creates target named create_ut_configuration that holds compiler UT configuration
function(create_ut_configuration)
   add_library(sg_ut_config INTERFACE)

   target_link_libraries(sg_ut_config
      INTERFACE
         sg_compiler_config
         wrapper_gtest
   )

   target_compile_options(sg_ut_config
      INTERFACE
         "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/EHa>"
   )
endfunction()