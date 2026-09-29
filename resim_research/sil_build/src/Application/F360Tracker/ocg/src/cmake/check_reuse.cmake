function(check_if_reuse_h_available)
   # Check if reuse file is available in project include directories
   get_directory_property(INC_DIRS INCLUDE_DIRECTORIES)
   message(DEBUG "Scanning for reuse.h file in: '${INC_DIRS}'")
   find_file(OCG_REUSE_HEADER_PATH "reuse.h" PATHS ${INC_DIRS} NO_DEFAULT_PATH)
   if(NOT OCG_REUSE_HEADER_PATH)
      message(FATAL_ERROR "reuse.h header file not found. OCG requires this file to be supplied externaly for each platform. "
                          "reuse.h file should contain definitions of standard integer types accorging to "
                          "Aptiv C Coding Standard rule C67 (required also by Aptiv C++ Coding Standard). "
                          "Either supply the file by adding appropraite include_directories() in parent project "
                          "or activate OCG_USE_INTERNAL_INCLUDES option to mock with internal reuse.h file.")
   else()
      message(DEBUG "Found reuse.h file in: '${REUSE_HEADER_PATH}'")
   endif()
   
   unset(OCG_REUSE_HEADER_PATH CACHE)
   
endfunction()