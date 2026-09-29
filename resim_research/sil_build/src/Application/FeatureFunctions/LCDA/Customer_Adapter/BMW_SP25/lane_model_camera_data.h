#ifndef LCDA_LANE_MODEL_CAMERA_DATA_H
#define LCDA_LANE_MODEL_CAMERA_DATA_H

/**
 * @file lane_model_camera_data.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Processes the BMW SRR5 camera data.
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
   LM_CAMERA_STATUS_INIT /* Initializing. Lane outputs not yet available */,
   LM_CAMERA_STATUS_AVAILABLE /* Lane model output quality is good and can be used */,
   LM_CAMERA_STATUS_DEGRADED /* Not all data is available to calculate the output with confidence */
} Lane_Model_Camera_Status_T;

typedef enum
{
   CAMERA_CALC_METHOD_DEFAULT_VALUE = 0,
   CAMERA_CALC_METHOD_LANE_BORDERS  = 1,
   CAMERA_CALC_METHOD_EGO_LANE_INFO = 2,
   CAMERA_CALC_METHOD_HOLD_OUTPUT   = 3
} Lane_Model_Camera_Calc_Method_T;

typedef struct
{
   float32_T lane_width;
   float32_T lane_center_offset;
   float32_T lane_lateral_speed[FBK_NUMBER_OF_SIDES];
   Lane_Model_Camera_Status_T status;
   Lane_Model_Camera_Calc_Method_T calculation_method;
} Lane_Model_Output_Camera_T;

/*===========================================================================*\
* Global Function Prototypess
\*===========================================================================*/

/**
 * @brief Initializes the camera lane model persistent data.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2780}
 * @SDD{SF-6907}
 * @verification{}
 */
void Lcda_Init_Lane_Model_Camera(void);

/**
 * @brief Processes the camera data - checks the relevant signals for confidence and increments the counts.
 * It populates the output of the camera lane model in the following priority:
 * 1. Lane width and lane center offset calculated based on the distance to the first left and right lane borders
 * 2. Ego lane width provided by the camera signal
 * 3. Holds the previous values for a few cycles before clearing to initial default state
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2780}
 * @SDD{SF-6906}
 * @verification{}
 */
void Lcda_Get_Camera_Data_Lane_Model(const Lcda_Input_T *p_lcda_input /**< Lcda input */,
                                     const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                     const Lcda_Core_Calibration_T *p_cals /**< Lcda calibration data */,
                                     Lane_Model_Output_Camera_T *p_output /**< Lcda lane model camera output */);

/**
 * @brief This function initializes the given output variable to default values.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2780}
 * @SDD{SF-6911}
 * @verification{}
 */
void Lcda_Init_Lane_Output(Lane_Model_Output_Camera_T *p_output /**< Lcda lane model camera output */);

#endif /* LCDA_LANE_MODEL_CAMERA_DATA_H */
