function(Srf_Generate_Lcf_Files target_name target_root_path module_variant)
   if(${target_name}_GENERATE_LCF_FILES)

      # Get basic directories for lnk file generation
      get_target_property(target_include_dirs ${target_name} INCLUDE_DIRECTORIES)
      list(FILTER target_include_dirs EXCLUDE REGEX "Unit_Test")
      list(FILTER target_include_dirs EXCLUDE REGEX "Mock_Files")
      list(REMOVE_DUPLICATES target_include_dirs)

      # Group directories
      set(ff_cal_file_pattern "")
      set(ff_other_file_pattern "")

      foreach(dir ${target_include_dirs})
         if(${dir} MATCHES "Calibration.*")
            if(${dir} MATCHES ".*${module_variant}")
               list(APPEND ff_cal_file_pattern "${dir}/*.c")
            endif()
         else()
            list(APPEND ff_other_file_pattern "${dir}/*.c")
         endif()
      endforeach()

      # Get source files and rename to object files
      file(GLOB ff_cal_files ${ff_cal_file_pattern})
      list(TRANSFORM ff_cal_files REPLACE "\\.c" ".o")
      file(GLOB ff_other_files ${ff_other_file_pattern})
      list(TRANSFORM ff_other_files REPLACE "\\.c" ".o")

      # message(STATUS "ff_cal_files: ${ff_cal_files}") message(STATUS "ff_other_files: ${ff_other_files}")

      # Write lnk files
      get_filename_component(target_root_path ${target_root_path} ABSOLUTE)
      string(TOLOWER ${target_name} target_name_lc)
      set(lnk_file_path_begin "${target_root_path}/_lnk/${module_variant}/core2_${target_name_lc}")

      set(comment_header "")
      list(APPEND comment_header "/*****************************************************************************\n")
      list(APPEND comment_header "* COPYRIGHT, 2021, Aptiv All Rights reserved\n")
      list(APPEND comment_header "*****************************************************************************/\n")
      list(APPEND comment_header "\n")

      # Write file _bss.lcf #######
      set(file_path "${lnk_file_path_begin}_bss.lcf")
      set(file_content ${comment_header})

      foreach(file_arg IN LISTS ff_cal_files ff_other_files)
         get_filename_component(file_name "${file_arg}" NAME)
         list(APPEND file_content "${file_name} (.bss)\n")
      endforeach()
      file(WRITE ${file_path} ${file_content})
      message(STATUS "Created linker file ${file_path}")

      # Write file _data.lcf #######
      set(file_path "${lnk_file_path_begin}_data.lcf")
      set(file_content ${comment_header})

      foreach(file_arg IN LISTS ff_other_files)
         get_filename_component(file_name "${file_arg}" NAME)
         list(APPEND file_content "${file_name} (.data)\n")
      endforeach()
      file(WRITE ${file_path} ${file_content})
      message(STATUS "Created linker file ${file_path}")

      # Write file _cal_data.lcf #######
      set(file_path "${lnk_file_path_begin}_cal_data.lcf")
      set(file_content ${comment_header})

      foreach(file_arg IN LISTS ff_cal_files)
         get_filename_component(file_name "${file_arg}" NAME)
         list(APPEND file_content "${file_name} (.data)\n")
      endforeach()
      file(WRITE ${file_path} ${file_content})
      message(STATUS "Created linker file ${file_path}")

      # Write file _txt.lcf #######
      set(file_path "${lnk_file_path_begin}_txt.lcf")
      set(file_content ${comment_header})

      foreach(file_arg IN LISTS ff_cal_files ff_other_files)
         get_filename_component(file_name "${file_arg}" NAME)
         list(APPEND file_content "${file_name} (.text_vle)\n")
      endforeach()
      file(WRITE ${file_path} ${file_content})
      message(STATUS "Created linker file ${file_path}")

   endif()
endfunction()
