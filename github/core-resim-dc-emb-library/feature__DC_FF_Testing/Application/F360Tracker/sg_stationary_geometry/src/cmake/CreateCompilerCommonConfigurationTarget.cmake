# Function: setup_compiler_common_configuration
# Description: 
#   creates target named sg_compiler_config that holds compiler common configuration
function(create_compiler_common_configuration_target)
    add_library(sg_compiler_config INTERFACE)
    
    target_compile_features(
        sg_compiler_config
            INTERFACE cxx_std_14
    )    

    ##############################################################################
    # Compile flags
    ##############################################################################
    set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
    set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

    set(gcc_like_cxx_flags "")
    list(APPEND gcc_like_cxx_flags "-Wall")
    list(APPEND gcc_like_cxx_flags "-Wextra")
    list(APPEND gcc_like_cxx_flags "-Wshadow")
    list(APPEND gcc_like_cxx_flags "-pedantic")
    #list(APPEND gcc_like_cxx_flags "-Werror")

    if(SG_RESIM)
       list(APPEND gcc_like_cxx_flags "-fPIC") # required by dynamic library (Component Resim)
    endif()

    set(msvc_cxx_flags "")
    list(APPEND msvc_cxx_flags "/Wall")  
    list(APPEND msvc_cxx_flags "/wd4514") # TODO (FZD-1677) -> Count: 3104 ('function' : unreferenced inline function has been removed)
    list(APPEND msvc_cxx_flags "/wd4626") # TODO (FZD-1678) -> Count: 199  ('type': assignment operator was implicitly defined as deleted)
    list(APPEND msvc_cxx_flags "/wd4820") # TODO (FZD-1679) -> Count: 1132 ('type': 'number' bytes padding added after type 'type')
    list(APPEND msvc_cxx_flags "/wd5027") # TODO (FZD-1680) -> Count: 164  ('type': move assignment operator was implicitly defined as deleted)
    list(APPEND msvc_cxx_flags "/wd5045") # TODO (FZD-1681) -> Count: 307  (Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified)
    
    target_compile_options(
        sg_compiler_config
            INTERFACE
                "$<${gcc_like_cxx}:${gcc_like_cxx_flags}>"
                "$<${msvc_cxx}:${msvc_cxx_flags}>"
    )

    ##############################################################################
    # Compile definitions
    ##############################################################################
    if(MSVC_TOOLSET_VERSION EQUAL 141)
        target_compile_definitions(
            sg_compiler_config
                INTERFACE
                    _STL_WARNING_LEVEL=3
        )
    endif()
	
    ##############################################################################
    # Saving data for Decision Tree training
    ##############################################################################
    option(SG_SAVE_ASSIGNED_DETS_TO_SUBSEGMENTS "Enable saving assigned detections to subsegments" OFF)
    if(SG_SAVE_ASSIGNED_DETS_TO_SUBSEGMENTS)
        target_compile_definitions(
            sg_compiler_config
                INTERFACE
                    SG_SAVE_DETECTIONS_ASSIGNED_TO_SUBSEGMENTS
        )
    endif()
endfunction()