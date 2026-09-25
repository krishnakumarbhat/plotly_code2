# Function: setup_dependency_graph
# Description: 
#   setup dependency graph
function(setup_dependency_graph)
    set(sg_original_cmake_folder ${CMAKE_FOLDER})

    find_program(GRAPHVIZ_DOT_PROGRAM dot)
    
    if(GRAPHVIZ_DOT_PROGRAM)
        add_custom_target(
            cmd_generate_dependency_graph ALL
                COMMAND ${CMAKE_COMMAND} "--graphviz=graph_files/Dependency_Graph.dot" .
                COMMAND ${GRAPHVIZ_DOT_PROGRAM} -Tpng -o Dependency_Graph.png graph_files/Dependency_Graph.dot -Nfontcolor=blue
                WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        )
    else()
        message(FATAL_ERROR "Graphviz is not installed")
    endif()

    configure_file(cmake/CMakeGraphVizOptions.cmake ${CMAKE_BINARY_DIR}/CMakeGraphVizOptions.cmake COPYONLY)
    
    set(CMAKE_FOLDER ${sg_original_cmake_folder})
endfunction()