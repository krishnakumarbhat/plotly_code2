target_include_directories(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_sources(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/fbk_macros.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/fbk_functions.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/fbk_functions.c)

target_include_directories(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/zone)
target_sources(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/zone/fbk_field_of_interest.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/zone/fbk_field_of_interest_factory.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/zone/fbk_field_of_interest_factory.c)

target_include_directories(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/reference_point)
target_sources(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/reference_point/fbk_ref_point.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/reference_point/fbk_ref_point_calc.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/reference_point/fbk_ref_point_calc.c)

target_include_directories(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/interpolation)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/interpolation/fbk_array_interpolation.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/interpolation/fbk_array_interpolation.c)

target_include_directories(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/validation)
target_sources(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/validation/fbk_guardrail_data_t.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/validation/fbk_guardrail_validation.c)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/validation/fbk_guardrail_validation.h)

target_sources(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/validation/fbk_object_data_t.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/validation/fbk_object_validation.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/validation/fbk_object_validation.c)

target_sources(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/validation/fbk_vehicle_data_t.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/validation/fbk_vehicle_validation.c)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/validation/fbk_vehicle_validation.h)

target_include_directories(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/trajectory_prediction)
target_sources(Feature_Building_Kit
               PRIVATE ${CMAKE_CURRENT_LIST_DIR}/trajectory_prediction/fbk_circular_shape_calculator.h)
target_sources(Feature_Building_Kit
               PRIVATE ${CMAKE_CURRENT_LIST_DIR}/trajectory_prediction/fbk_circular_shape_calculator.c)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/trajectory_prediction/fbk_ego_traj_predictor.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/trajectory_prediction/fbk_ego_traj_predictor.c)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/trajectory_prediction/fbk_obj_traj_predictor.h)
target_sources(Feature_Building_Kit PRIVATE ${CMAKE_CURRENT_LIST_DIR}/trajectory_prediction/fbk_obj_traj_predictor.c)
target_sources(Feature_Building_Kit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/trajectory_prediction/fbk_traj_predictor_t.h)
