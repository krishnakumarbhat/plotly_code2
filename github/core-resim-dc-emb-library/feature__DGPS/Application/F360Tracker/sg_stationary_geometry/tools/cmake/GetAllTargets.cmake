# Function: get_included_dirs
# Description: 
#   list all directories that are added via add_subdirectory() (recursively). 
# Syntax:
#   get_included_dirs_out -> all gathered directories in form of list
#   starting_dir_path -> path to starting directory
function(get_included_dirs get_included_dirs_out starting_dir_path)
    set(output_dirs "")
    get_directory_property(sub_dirs DIRECTORY ${starting_dir_path} SUBDIRECTORIES)
    list(APPEND output_dirs ${sub_dirs})
    
    foreach(sub_dir ${sub_dirs})      
        get_included_dirs(tmp ${sub_dir})    
        list(APPEND output_dirs ${tmp})
    endforeach()
    
    list(REMOVE_DUPLICATES output_dirs)
    set(${get_included_dirs_out} ${output_dirs})
    set(${get_included_dirs_out} ${${get_included_dirs_out}} PARENT_SCOPE)
endfunction()

# Function: get_all_targets
# Description: 
#   list all targets that are added via add_subdirectory() (recursively). 
# Syntax:
#   get_all_targets_out -> all gathered targets
#   starting_dir_path -> path to starting directory
function(get_all_targets get_all_targets_out starting_dir_path)
    set(all_targets "")
    get_included_dirs(all_dirs ${starting_dir_path})

    foreach(dir ${all_dirs})
        get_property(targets DIRECTORY ${dir} PROPERTY BUILDSYSTEM_TARGETS)
        list(APPEND all_targets ${targets})
    endforeach()
    
    list(REMOVE_DUPLICATES all_targets)
    set(${get_all_targets_out} ${all_targets})
    set(${get_all_targets_out} ${${get_all_targets_out}} PARENT_SCOPE)
endfunction()