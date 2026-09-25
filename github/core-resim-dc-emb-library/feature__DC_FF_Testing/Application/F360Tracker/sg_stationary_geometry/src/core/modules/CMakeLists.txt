set(tmp_cmake_folder ${CMAKE_FOLDER})

set(CMAKE_FOLDER ${tmp_cmake_folder}/modules/support)
add_subdirectory(safety)
add_subdirectory(host)
add_subdirectory(math)

set(CMAKE_FOLDER ${tmp_cmake_folder}/modules/drivability_classification)
add_subdirectory(drivability_classification)

set(CMAKE_FOLDER ${tmp_cmake_folder}/modules/kernel)
add_subdirectory(calibrations)
add_subdirectory(common)
add_subdirectory(contour_downselection)
add_subdirectory(contour_initialization)
add_subdirectory(detection_clustering)
add_subdirectory(detection_processing)
add_subdirectory(measurement_association)
add_subdirectory(measurement_update)
add_subdirectory(postprocessing)
add_subdirectory(time_update)

set(CMAKE_FOLDER ${tmp_cmake_folder})
