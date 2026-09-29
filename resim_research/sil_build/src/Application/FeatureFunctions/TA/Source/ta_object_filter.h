#ifndef TA_OBJECT_FILTER_H
#define TA_OBJECT_FILTER_H

/**
 * @file ta_object_filter.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Implements functions related to filter valid objects.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_vehicle_data_t.h"
#include "pa_reuse.h"
#include "ta_core_calibration_t.h"
#include "ta_core_input_t.h"
#include "ta_types.h"

/*============================================================================*\
 * EXPORTED FUNCTIONS PROTOTYPES
\*============================================================================*/

/**
 * @brief Returns if the object raised an alert for FTA or RTA, depending on the ta_alert_mode.
 *
 * @return True if object raised an alert, false otherwise
 *
 * @SRS{SF-2347,SF-2348}
 * @SAE{SF-3238}
 * @SDD{SF-8791}
 * @verification{Create the following input combinations: 1.) TA alert mode equals the objects one. 2.) Objects alert mode is set
 * to none and Ta alert mode is set to both sides. Both cases shall return true other cases shall return false.}
 */
boolean_T Ta_Is_Obj_Active(const Ta_Object_T *p_ta_object, const Ta_Alert_Mode_T ta_alert_mode);

/**
 * @brief Updates the relevance flags for the given object.
 *
 * @return void
 *
 * @SRS{SF-2352,SF-2278}
 * @SAE{SF-3238}
 * @SDD{SF-8735}
 * @verification{Create an object which is relevant for FTA and also placed within the danger zone. In case of RTA create an object
 * which is either in the info or warn zone. Only in these three cases f_obj_ta_relevant flag shall be set to True.}
 */
void Ta_Update_Object_Relevance(Ta_Object_T *p_ta_object /**< TA Object */,
                                const Ta_Core_Input_T *p_ta_core_input /**< TA Core Input */,
                                const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */);

#endif /* TA_OBJECT_FILTER_H */
