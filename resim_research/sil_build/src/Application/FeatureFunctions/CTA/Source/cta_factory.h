#ifndef CTA_FACTORY_H
#define CTA_FACTORY_H

/**
 * @file cta_factory.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Exports initialization procedures of Cta.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "cta_core_calibration_t.h"
#include "cta_core_input_t.h"
#include "cta_instance.h"
#include "cta_types.h"
#include "fbk_vehicle_data_t.h"
#include "pa_const_macros.h"
#include "pa_reuse.h"


/*===========================================================================*\
* Global Functions	Definition
\*===========================================================================*/

/**
 * @brief Fills the cta internal object with tracker data
 *
 * @return void
 *
 * @SRS{SF-192,SF-193}
 * @SAE{SF-2459}
 * @SDD{SF-3820}
 * @verification{Check that the objects tracker data is filled correctly.}
 */
void Cta_Fill_Current_Object(Cta_Object_Data_T *p_cta_object_to_be_filled /**<object to be filled*/,
                             Cta_Instance_T *p_cta_instance /** Cta Instance pointer */,
                             const Fbk_Vehicle_Data_T *p_vehicle_data /**< host vehicle data*/,
                             const uint8_t obj_index /**< obj index whose tracker output shall be considered*/);

/**
 * @brief Initializes the cta object persistent data struct for a specific object.
 *
 * @return void
 *
 * @SRS{SF-192,SF-193}
 * @SAE{SF-2459}
 * @SDD{SF-3824}
 * @verification{Check that the persistent data is reset.}
 */
void Cta_Reset_Single_Obj_Persistent(Cta_Object_Persistent_T *p_obj_persistent);


/**
 * @brief Initializes the data to which p_cta_object_data points to and sets its internal pointers
 * to NULL.
 *
 * @return void
 *
 * @SRS{SF-192,SF-193}
 * @SAE{SF-2459}
 * @SDD{SF-4027}
 * @verification{Check that the comparison data is initialized correctly.}
 */
void Cta_Init_Comparison_Data(Cta_Comparison_Data_T *p_cta_comparison_data /**<comparison data of CTA*/,
                              Cta_Instance_T *p_cta_instance /**< pointer to tracker object data*/);

/**
 * @brief Initializes the criticality level calibrations such that in this separate struct calibrations can be modified
 * additionally.
 *
 * @return void
 *
 * @SRS{SF-236,SF-198,SF-170}
 * @SAE{SF-2459}
 * @SDD{SF-4057}
 * @verification{Check that the initialization routine is correct.}
 */
void Cta_Init_Crit_Level_Cals(Cta_Crit_Level_Calibration_T *p_cta_crit_level_cals,
                              const Cta_Core_Input_T *p_cta_core_input,
                              const Cta_Core_Calibration_T *p_cta_cal,
                              const Fbk_Vehicle_Data_T *p_vehicle_data);

/**
 * @brief Resets the persistent array for every object.
 *
 * @return void
 *
 * @SRS{SF-193}
 * @SAE{SF-2459}
 * @SDD{SF-3823}
 * @verification{Check that the persistent object array is reset.}
 */
void Cta_Reset_Obj_Persistent_Array(Cta_Object_Persistent_T obj_persistent_array[CTA_OBJ_MAX_ARRAY_SIZE]);


/**
 * @brief Resets every entry of target attributes array.
 *
 * @return void
 *
 * @SRS{SF-192,SF-193}
 * @SAE{SF-2459}
 * @SDD{SF-3822}
 * @verification{Check that the attributes array is reset.}
 */
void Cta_Reset_Obj_Attribute_Array(Cta_Object_Attributes_T obj_attributes_array[PA_OBJ_NUMBER_OF_OBJECTS]);

#endif /*CTA_FACTORY_H*/
