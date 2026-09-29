#ifndef SCW_INPUT_GENERATOR_H
#define SCW_INPUT_GENERATOR_H

#include "scw_core_output_t.h"
#include "scw_persistent_t.h"

#include "fbk_object_data_t.h"
#include "ml_vector_2d_t.h"
#include "pa_shared_types.h"

void Scw_Create_Valid_Tracker_Object(Scw_Object_T *p_tracker_object, const uint8_t index);
void Scw_Set_In_Range_Persistents(Scw_Persistent_T *p_scw_persistent, const uint8_t index);

#endif
