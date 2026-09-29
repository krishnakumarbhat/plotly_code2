#ifndef LCDA_LANE_MODEL_H
#define LCDA_LANE_MODEL_H

/**
 * @file lane_model.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Lane model based on camera and navigation data.
 *
 * @copyright Copyright (C) 2019 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_macros.h"
#include "fbk_vehicle_data_t.h"
#include "lcda_core_calibration_t.h"
#include "lcda_input_t.h"
#include "pa_reuse.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

typedef enum
{
   COUNTRY_TYPE_DEFAULT = 0u,
   COUNTRY_TYPE_US      = 2u,
   COUNTRY_TYPE_GERMANY = 3u,
   COUNTRY_TYPE_JAPAN   = 5u,
   COUNTRY_TYPE_KOREA   = 7u,
   COUNTRY_TYPE_CHINA   = 22u
} Country_Type_T;

typedef enum
{
   CALC_METHOD_DEFAULT_VALUE    = 0,
   CALC_METHOD_NAVIGATION_DATA  = 1,
   CALC_METHOD_DRIVING_DYNAMICS = 2,
   CALC_METHOD_CAMERA_DATA      = 3
} Lane_Model_Calc_Method_T;

typedef struct
{
   float32_T lane_width;
   float32_T lane_center_offset;
   float32_T lane_lateral_speed[FBK_NUMBER_OF_SIDES];
   Lane_Model_Calc_Method_T calculation_method;
} Lane_Model_Output_T;

/*===========================================================================*\
* Global Function Prototypess
\*===========================================================================*/

/**
 * @brief This function initializes the lane model - clears the outputs and initializes the persistent variables. This function
 * should be called once during initialization.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-6888}
 * @verification{}
 */
void Lcda_Initialize_Lane_Model(const Lcda_Core_Calibration_T *p_cals /**< Lcda calibration data */,
                                Lane_Model_Output_T *p_lane_model_output /**< Lcda lane model output */);

/**
 * @brief The main function that decides whether the navigation data is enabled or driving (vehicle) dynamics is enabled and
 * processes these and then populates the final lane model output.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2780}
 * @SDD{SF-6887}
 * @verification{}
 */
void Lcda_Process_Lane_Model(const Lcda_Input_T *p_lcda_input /**< Lcda input */,
                             const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                             const Lcda_Core_Calibration_T *p_cals /**< Lcda calibration data */,
                             Lane_Model_Output_T *p_lane_model_output /**< Lcda lane model output */);

#endif /* LCDA_LANE_MODEL_H */
