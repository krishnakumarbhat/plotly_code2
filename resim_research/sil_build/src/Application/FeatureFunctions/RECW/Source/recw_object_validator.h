#ifndef RECW_OBJECT_VALIDATOR_H
#define RECW_OBJECT_VALIDATOR_H

/**
 * @file recw_object_validator.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Export of Recw submodule for the validity check of objects.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
 * Includes
\*===========================================================================*/

#include "fbk_vehicle_data_t.h"
#include "pa_reuse.h"
#include "recw_core_calibration_t.h"
#include "recw_persistent_t.h"

#include "recw_types.h"

/*===========================================================================*\
 * Global Function Prototypess
\*===========================================================================*/

/**
 * @brief Checks whether the object is relevant for Recw.
 * In case of an object which was relevant before an hysteresis shall be applied
 *
 * @return True when object is relevant an object attributes shall be calculated
 *
 * @SRS{SF-1695,SF-1700,SF-1716,SF-1703}
 * @SAE{SF-2959}
 * @SDD{SF-7925}
 * @verification{}
 */
boolean_T Recw_Is_Object_Relevant(Recw_Object_T *p_recw_object /**< RECW object */,
                                  Recw_Persistent_T *p_persistent /**< RECW persistent data */,
                                  const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                  const Recw_Core_Calibration_T *p_cals /**< RECW calibrations */,
                                  Car_Wash_Scenario_Flags_T *p_car_wash_scenario_flags /**< possible car wash scenario flags */);

#endif /* RECW_OBJECT_VALIDATOR_H */
