#ifndef FBK_OBJECT_VALIDATION_H
#define FBK_OBJECT_VALIDATION_H

/**
 * @file fbk_object_validation.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file with functions for object validation.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_object_data_t.h"
#include "pa_context.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"

/*===========================================================================*\
* Global Function Definition
\*===========================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif
   /**
    * @brief Checks whether a given object status is implausible. This is the case when the object is determined as coasted but the
    * object is not in any sensor field of view
    *
    * @return true in case that coasted status is invalid
    *
    * @SRS{SF-263}
    * @SAE{SF-2552}
    * @SDD{SF-4131}
    * @verification{}
    */
   boolean_T Fbk_Is_Obj_Coasted_Status_Implausible(const Fbk_Object_Data_T *p_object_data);
   /**
    * @brief Checks whether a given object is in any sensor field of view.
    *
    * @return true in case
    *
    * @SRS{SF-262}
    * @SAE{SF-2552}
    * @SDD{SF-4132}
    * @verification{}
    */
   boolean_T Fbk_Is_Obj_In_Any_Sensor_Fov(const Fbk_Object_Data_T *p_object_data);


   /**
    * @brief Resets the object data structure in Fbk.
    *
    * @return void
    *
    * @SRS{}
    * @SAE{}
    * @SDD{SF-4175}
    * @verification{}
    */
   void Fbk_Reset_Object_Data(Fbk_Object_Data_T *p_fbk_object_data /**< internal fbk object structure*/);

   /**
    * @brief Fills object data from tracker output.
    *
    * @return void
    *
    * @SRS{}
    * @SAE{}
    * @SDD{SF-4176}
    * @verification{}
    */
   void Fbk_Fill_Object_Information(Fbk_Object_Data_T *p_fbk_object_data /**< internal fbk object structure*/,
                                    const Pa_Context_T *p_context /**< context data*/,
                                    const uint8_t obj_index /**< index of the passed object*/);
   /**
    * @brief Check if the object data is in within the defined range
    *
    * @return void
    *
    * @SRS{}
    * @SAE{}
    * @SDD{CSCSA-307402}
    * @verification{Check if the object data is within the defined range}
    */
   boolean_T Fbk_Verify_Object_Data_Range(const Fbk_Object_Data_T *p_object_data /**< internal fbk object structure*/);

   /**
    * @brief Checks whether an object is valid.
    *
    * @return void
    *
    * @SRS{}
    * @SAE{}
    * @SDD{SF-4177}
    * @verification{}
    */
   boolean_T Fbk_Is_Obj_State_Valid(Pa_Obj_Status_T obj_state);

   /**
    * @brief Checks object side.
    *
    * @return 0 for left, 1 for right
    *
    * @SRS{}
    * @SAE{}
    * @SDD{CSCSA-31267}
    * @verification{}
    */
   uint8_t Fbk_Get_Obj_Side(const float32_T position);

   /**
    * @brief Checks object side depending on used coordinate system.
    *
    * @return 0 for left, 1 for right
    *
    * @SRS{}
    * @SAE{}
    * @SDD{CSCSA-31268}
    * @verification{}
    */
   uint8_t Fbk_Get_Obj_Side_Coord_Sys(const Fbk_Object_Data_T *p_tracker_data, const boolean_T f_use_curvi_coordinates);

   /**
    * @brief Checks object side sign.
    *
    * @return -1 for negative relative position, 1 for 0/positive relative position
    *
    * @SRS{}
    * @SAE{}
    * @SDD{CSCSA-31269}
    * @verification{}
    */
   float32_T Fbk_Get_Obj_Side_Sign(const float32_T position);

   /**
    * @brief Converts object side to sign.
    *
    * @return -1 for left side, 1 for right side
    *
    * @SRS{}
    * @SAE{}
    * @SDD{CSCSA-31270}
    * @verification{}
    */
   float32_T Fbk_Convert_Obj_Side_To_Sign(const uint8_t side);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FBK_OBJECT_VALIDATION_H*/
