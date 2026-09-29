#ifndef LCDA_INPUT_GENERATOR_H
#define LCDA_INPUT_GENERATOR_H
/**
 * @file lcda_input_generator.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file for LCDA test input generator.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

#include "fbk_field_of_interest.h"
#include "fbk_object_data_t.h"
#include "lcda_persistent_t.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"

void Lcda_Create_Valid_Bsw_Cvw_Track(Fbk_Object_Data_T *p_tracker_object, const uint8_t id);
void Lcda_Create_Zone(Vector_2d_T point0, float32_T length, float32_T width, Fbk_Field_Of_Interest_T *p_zone);
void Lcda_Create_Bsw_Track(Fbk_Object_Data_T *p_tracker_object, const uint8_t index, float32_T lon_pos, float32_T lat_pos);
void Lcda_Create_Cvw_Alert(uint8_t side, uint8_t obj_idx, Lcda_Cvw_Persistent_T *p_cvw_persistent);

#endif /* LCDA_INPUT_GENERATOR_H */
