# Function: generate_variant_dependent_files
# Description:
#   generates variant dependent file and add them to iface target
function(generate_variant_dependent_files)
    set(generation_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

    # sg_variant_selector.h
    set(variant_selector_header_path "${generation_dir}/sg_variant_selector.h")
    string(TOUPPER "SG_VARIANT_${SG_VARIANT}" variant_macro_inplace_str)
    configure_file("variants/sg_variant_selector.h.in" ${variant_selector_header_path})

    # sg_output.h
    set(standard_output_header_path "${generation_dir}/sg_output.h")
    set(major_stream_number ${SG_STREAM_OUTPUT_MAJOR})
    set(minor_stream_number ${SG_STREAM_OUTPUT_MINOR})
    configure_file("types/sg_output.h.in" ${standard_output_header_path})

    # sg_reduced_output.h
    set(standard_reduced_output_header_path "${generation_dir}/sg_reduced_output.h")
    set(major_stream_number ${SG_STREAM_REDUCED_OUTPUT_MAJOR})
    set(minor_stream_number ${SG_STREAM_REDUCED_OUTPUT_MINOR})
    configure_file("types/sg_reduced_output.h.in" ${standard_reduced_output_header_path})

    # sg_timing_dump.h
    set(standard_timing_header_path "${generation_dir}/sg_timing_dump.h")
    set(major_stream_number ${SG_STREAM_TIMING_MAJOR})
    set(minor_stream_number ${SG_STREAM_TIMING_MINOR})
    configure_file("types/dump/sg_timing_dump.h.in" ${standard_timing_header_path})

    # sg_internals_dump.h
    set(standard_internals_header_path "${generation_dir}/sg_internals_dump.h")
    set(major_stream_number ${SG_STREAM_INTERNALS_MAJOR})
    set(minor_stream_number ${SG_STREAM_INTERNALS_MINOR})
    configure_file("types/dump/sg_internals_dump.h.in" ${standard_internals_header_path})

    set(generated_files
            ${variant_selector_header_path}
            ${standard_output_header_path}
            ${standard_reduced_output_header_path}
            ${standard_timing_header_path}
            ${standard_internals_header_path}
    )

    if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.22.0") 
        target_sources(stationary_geometries_iface PRIVATE ${generated_files})
        source_group("generated" FILES ${generated_files})
    endif()
    
    set_source_files_properties(${generated_files} PROPERTIES GENERATED TRUE)
endfunction()