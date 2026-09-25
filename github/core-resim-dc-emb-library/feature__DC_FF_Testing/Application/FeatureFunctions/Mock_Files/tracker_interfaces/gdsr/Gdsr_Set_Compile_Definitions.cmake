function(Gdsr_Set_Compile_Definitions customer variant)
    if("${customer}" STREQUAL "<customer>")
        add_compile_definitions("GDSR_TRACKER_PROJECT_VARIANT_<customer>")
    else()
        # just take the default by doing nothing
    endif()
endfunction()
