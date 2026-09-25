# Function: setup_dev_installation
# Description: 
#   setup installation for development purpose i.e. all libraries/executables are copied to 'dev_files' directory 
function(setup_dev_installation)
    include(cmake/GetAllTargets.cmake)
    get_all_targets(targets_to_install ${CMAKE_CURRENT_SOURCE_DIR})

    install(
        TARGETS ${targets_to_install}
        DESTINATION dev_files
    )
endfunction()