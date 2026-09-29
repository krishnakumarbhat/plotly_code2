# Function: setup_gcov_coverage
# Description: 
#   Extern current module setup by adding needed compile flags and generating target that creates coverage report
function(setup_gcov_coverage)
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
        message(FATAL_ERROR "Coverage requires GNU or Clang compiler")
    endif()
    
    # ===============================================================
    # Coverage interface library (setup)
    # ===============================================================
    add_library(sg_gcov_coverage_setup INTERFACE)
    
    # compiler flags
    set(compiler_flags "")
    list(APPEND compiler_flags "--coverage")
    list(APPEND compiler_flags "-fprofile-arcs")
    list(APPEND compiler_flags "-ftest-coverage")
    list(APPEND compiler_flags "-fno-exceptions")
    list(APPEND compiler_flags "-fno-inline")                  # Do not expand any functions inline apart from those marked with the always_inline attribute.
    list(APPEND compiler_flags "-fno-threadsafe-statics")      # Do not emit the extra code to use the routines specified in the C++ ABI for thread-safe initialization of local statics. You can use this option to reduce code size slightly in code that doesn’t need to be thread-safe.
    
    target_compile_options(
        sg_gcov_coverage_setup 
            INTERFACE
                "$<$<COMPILE_LANG_AND_ID:CXX,Clang,GNU>:${compiler_flags}>"
    )
    
    target_link_libraries(
        sg_gcov_coverage_setup
            INTERFACE
                "$<$<CXX_COMPILER_ID:GNU>:gcov>"
                "$<$<CXX_COMPILER_ID:Clang>:${clang_link_libraries}>"
    )
    
    # linking libraries
    set(clang_link_libraries "")
    find_library(
        CLANG_RT_LIBRARY_PATH clang_rt.profile-x86_64 
        HINTS "/usr/lib/llvm-15/lib/clang/15.0.7/lib/linux"
    )
    if(CLANG_RT_LIBRARY_PATH)
        list(APPEND clang_link_libraries "clang_rt.profile-x86_64")
        get_filename_component(CLANG_RT_LIBRARY_DIR_PATH ${CLANG_RT_LIBRARY_PATH} DIRECTORY)
        link_directories(${CLANG_RT_LIBRARY_DIR_PATH})
    else()
        message(WARNING "clang_rt library is not found. 'undefined reference' issue may occur") 
    endif()
    
    # ===============================================================
    # Coverage generation command
    # ===============================================================        
    include(ProcessorCount)
    ProcessorCount(nb_processors)
    set(UnitTests_root "${CMAKE_CURRENT_SOURCE_DIR}")
    set(UnitTests_output_dir "${CMAKE_BINARY_DIR}/coverage_results")
    add_custom_target(
        cmd_build_and_run_tests
            COMMENT "Build and run tests"
            COMMAND find ${CMAKE_BINARY_DIR} -name "*.gcda" -type f -delete
            COMMAND ${CMAKE_COMMAND} -E make_directory ${UnitTests_output_dir}
            COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} -j${nb_processors} && ctest --test-dir ${CMAKE_BINARY_DIR} --output-on-failure -j${nb_processors}
    )
    
    # === GCOVR ==========================================================
    find_program(GCOVR gcovr)
    if(NOT GCOVR)
        message(FATAL_ERROR "gcovr is not found.")
    endif()
    
    set(cmd_target_comment "Generate coverity report\nRoot: ${UnitTests_root}\nOutput: ${UnitTests_output_dir}")
    set(cmd_coverage_generation_generic ${GCOVR} ${CMAKE_BINARY_DIR} --root ${UnitTests_root} --html-details --output ${UnitTests_output_dir}/coverage.html --exclude ${UnitTests_root}/unit_tests)
    
    # === GNU ==========================================================
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")  
        add_custom_target(
            cmd_generate_coverage_report
                COMMENT ${cmd_target_comment}
                DEPENDS cmd_build_and_run_tests
                COMMAND ${cmd_coverage_generation_generic}
        )
    endif()
           
    # === Clang ==========================================================           
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        find_program(LLVM_COV NAMES llvm-cov llvm-cov-15)
        if(NOT LLVM_COV)
            message(FATAL_ERROR "llvm-cov is not found.")
        endif()
        
        add_custom_target(
            cmd_generate_coverage_report
                COMMENT ${cmd_target_comment}
                DEPENDS cmd_build_and_run_tests
                COMMAND ${cmd_coverage_generation_generic} --gcov-executable "${LLVM_COV} gcov"
        )
    endif()
endfunction()