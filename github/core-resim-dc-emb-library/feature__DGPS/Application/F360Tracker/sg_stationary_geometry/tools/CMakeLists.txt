set(original_cmake_folder ${CMAKE_FOLDER})

option(SG_UT "Unittest framework" OFF)
if(SG_UT)
    set(CMAKE_FOLDER ${original_cmake_folder}/gtest) 
    add_subdirectory(googletest-1.13.0)
    add_subdirectory(ut_framework_wrapper)
    if(DEFINED ENV{GTEST_DISABLE_PTHREADS} AND NOT $ENV{GTEST_DISABLE_PTHREADS})
        message("If pthreads are enabled for gtest, it is neccessary to link pthread library to gtest target")
        target_link_libraries(
            gtest
            PRIVATE "$<$<CXX_COMPILER_ID:Clang,GNU>:pthread>"
        )
    elseif(NOT DEFINED ENV{GTEST_DISABLE_PTHREADS})
        message(WARNING "GTEST_DISABLE_PTHREADS env variable not set, pthreads for gtest are disabled, it can cause memory leaks")
    endif()
   
    include(cmake/SetupUnitTestsReportTarget.cmake)
    setup_unit_tests_report_target()
endif()

if(SG_DEVELOPER_MODE)
    include(cmake/SetupDevInstallation.cmake)
    setup_dev_installation()
    
    option(SG_DEPENDENCY_GRAPH "Add target that generates dependency graph" OFF)
    mark_as_advanced(SG_DEPENDENCY_GRAPH)
    if(SG_DEPENDENCY_GRAPH)
       set(CMAKE_FOLDER ${original_cmake_folder}) 
       include(cmake/SetupDependencyGraph.cmake)
       setup_dependency_graph()
    endif()
endif()

if(MSVC)
   include(cmake/vs_support_tools.cmake)
endif()

set(CMAKE_FOLDER ${original_cmake_folder})