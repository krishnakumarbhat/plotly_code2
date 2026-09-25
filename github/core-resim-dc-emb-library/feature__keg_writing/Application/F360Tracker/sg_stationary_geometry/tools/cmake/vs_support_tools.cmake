# Support debugging visualization of EmbeddedList as an array
target_sources(stationary_geometries PRIVATE tools/cmake/vs_files/embedded_list.natvis)

# Set debugging arguments
if (TARGET COMPONENT_RESIM_EXECUTABLE)
    set_target_properties(COMPONENT_RESIM_EXECUTABLE PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "-input_file <log_path> -lib_name stationary_geometries_wrapper -lib_include_folder $(SolutionDir)components/resim/resim_wrapper/$(Configuration) -logging_folder logging -data_injection/fill_missing true -data_injection/remove_excessing true")
endif()