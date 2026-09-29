/**
 * @file lane_model_camera_data.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Processes the BMW SRR5 camera data.
 *
 * @copyright Copyright (C) 2019 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "lane_model_camera_data.h"
#include "camera_data_t.h"
#include "fbk_macros.h"
#include "fbk_vehicle_data_t.h"
#include "ml_saturated_math.h"
#include "ml_trigonometry.h"

/*===========================================================================*\
* Local Defines
\*===========================================================================*/

typedef struct
{
   uint16_t count_valid_lane_left;
   uint16_t count_valid_lane_right;
   uint16_t count_valid_ego_lane_width;
   uint16_t count_valid_left_adjacent_lane_width;
   uint16_t count_valid_right_adjacent_lane_width;
   uint16_t count_hold_prev_valid_output;
   uint16_t count_holding_lateral_speed_left;
   uint16_t count_holding_lateral_speed_right;
   float32_T prev_lane_lateral_speed[FBK_NUMBER_OF_SIDES];
   Lane_Model_Output_Camera_T prev_lm_output_camera;

} Lane_Model_Camera_Persistent_T;

#define LCDA_EGO_AVERAGE_QUALITY_LANE_WIDTH (1u)
#define LCDA_EGO_NORMAL_QUALITY_LANE_WIDTH (2u)
#define LCDA_ADJACENT_AVERAGE_QUALITY_LANE_WIDTH (1u)
#define LCDA_ADJACENT_NORMAL_QUALITY_LANE_WIDTH (2u)

#define LCDA_LANE_CENTER_OFFSET_UNKNOWN (0.0f)
#define LCDA_LANE_WIDTH_UNKNOWN (0.0f)
#define LCDA_LANE_MAX_LATERAL_SPEED (10.0f)
#define LCDA_LANE_LATERAL_SPEED_MAX_HOLDING_COUNTER (8u)

#define LCDA_MAX_VALID_LANE_EXISTENCE_PROBABILITY (100.0f)


/*===========================================================================*\
* File Scope variables
\*===========================================================================*/

static Lane_Model_Camera_Persistent_T Lane_Model_Camera_Persistent;

/*===========================================================================*\
* Local Function Prototypes
\*===========================================================================*/

/**
 * @brief This function checks the existence probability and quality factors and increments the counts in the persistent variables.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2780}
 * @SDD{SF-6910}
 * @verification{}
 */
static void Lcda_Process_Camera_Data(const Camera_Data_T *p_cam_data /**< Camera data */,
                                     const Lcda_Core_Calibration_T *p_cals /**< Lcda calibration data */);

/**
 * @brief This function returns true of the lane width and lane center offset are within reasonable values.
 *
 * @return true if lane data is plausible, false otherwise
 *
 * @SRS{}
 * @SAE{SF-2780}
 * @SDD{SF-6917}
 * @verification{}
 */
static boolean_T Lcda_Is_Lane_Data_Plausible(const float32_T lane_width /**< Lane width */,
                                             const float32_T lc_offset /**< Lane center offset */,
                                             const Lcda_Core_Calibration_T *p_cals /**< Lcda calibration data */);

/**
 * @brief This function calculates the lane width and lane center offset in VCS co-ordinates from the distances to the right and
 * left lane borders. If the lane borders satisfy the quality criteria for a minimum number of cycles, then the lane width and
 * offset are calculated. Lane center offset is defined as position of the center of the lane center with respect to the ego
 * longitudinal axis. If the ego is closer to left edge of the lane the lane center offset will be positive. This function assumes
 * the following with respect to the distances to the left and right lane borders:
 * 1. lane_dist_left will have a positive sign and lane_dist_right will have a negative sign.
 * 2. lane_dist_left and lane_dist_right are provided with reference to the ego center line in longitudinal direction.
 *
 * If the lane width and lane center offset can be calculated and have a plausible value, the output status is set to
 * LM_CAMERA_STATUS_AVAILABLE, otherwise the outputs are initialized to default values.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2780}
 * @SDD{SF-6916}
 * @verification{}
 */
static void Lcda_Get_Output_Lane_Borders(const Lcda_Input_T *p_lcda_input /**< Lcda input */,
                                         const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                         const Lcda_Core_Calibration_T *p_cals /**< Lcda calibration data */,
                                         Lane_Model_Output_Camera_T *p_output /**< Lcda lane model camera output */);

/**
 * @brief This function checks if the quality of the ego lane width as provided by the camera is good enough for a sufficient
 * amount of time. If the quality check is passed, then it populates the output with the ego lane width from the camera data and a
 * default lane center offset. The status is set to LM_CAMERA_STATUS_DEGRADED.
 *
 * If the quality checks are not passed, or if the calculated outputs are not plausible, then the output is set to default values.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2780}
 * @SDD{SF-6915}
 * @verification{}
 */
static void Lcda_Get_Camera_Ego_Lane_Info(const Lcda_Input_T *p_lcda_input /**< Lcda input */,
                                          const Lcda_Core_Calibration_T *p_cals /**< Lcda calibration data */,
                                          Lane_Model_Output_Camera_T *p_output /**< Lcda lane model camera output */);

/**
 * @brief This function populates the output with the data from the previous cycle if the output in the previous cycles was valid.
 * This previous output is held for a certain number of cycles and then the outputs are cleared to the default values.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2780}
 * @SDD{SF-6914}
 * @verification{}
 */
static void Lcda_Hold_Previous_Output(Lane_Model_Output_Camera_T *p_output /**< Lcda lane model camera output */,
                                      const Lcda_Core_Calibration_T *p_cals /**< Lcda calibration data */);

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

void Lcda_Init_Lane_Model_Camera(void)
{
   /* Initialize the persistent variables */
   Lane_Model_Camera_Persistent.count_valid_lane_left                   = 0u;
   Lane_Model_Camera_Persistent.count_valid_lane_right                  = 0u;
   Lane_Model_Camera_Persistent.count_valid_ego_lane_width              = 0u;
   Lane_Model_Camera_Persistent.count_valid_left_adjacent_lane_width    = 0u;
   Lane_Model_Camera_Persistent.count_valid_right_adjacent_lane_width   = 0u;
   Lane_Model_Camera_Persistent.count_hold_prev_valid_output            = 0u;
   Lane_Model_Camera_Persistent.count_holding_lateral_speed_left        = LCDA_LANE_LATERAL_SPEED_MAX_HOLDING_COUNTER;
   Lane_Model_Camera_Persistent.count_holding_lateral_speed_right       = LCDA_LANE_LATERAL_SPEED_MAX_HOLDING_COUNTER;
   Lane_Model_Camera_Persistent.prev_lane_lateral_speed[FBK_SIDE_LEFT]  = FBK_ZERO_F;
   Lane_Model_Camera_Persistent.prev_lane_lateral_speed[FBK_SIDE_RIGHT] = FBK_ZERO_F;

   Lcda_Init_Lane_Output(&Lane_Model_Camera_Persistent.prev_lm_output_camera);
}

void Lcda_Get_Camera_Data_Lane_Model(const Lcda_Input_T *p_lcda_input,
                                     const Fbk_Vehicle_Data_T *p_vehicle_data,
                                     const Lcda_Core_Calibration_T *p_cals,
                                     Lane_Model_Output_Camera_T *p_output)
{

   /* Check the lane data and increment the qualification counts */
   Lcda_Process_Camera_Data(p_lcda_input->camera_data, p_cals);

   /* Calculate the lane model output using the lane borders */
   Lcda_Get_Output_Lane_Borders(p_lcda_input, p_vehicle_data, p_cals, p_output);

   /* If we could not calculate based on the lane borders, see if we can use the ego lane width output directly from the camera */
   if (LM_CAMERA_STATUS_AVAILABLE != p_output->status)
   {
      Lcda_Get_Camera_Ego_Lane_Info(p_lcda_input, p_cals, p_output);
   }


   /* If we are not able to calculate the output from the above methods, then hold the previous output value */
   if (LM_CAMERA_STATUS_INIT == p_output->status)
   {
      Lcda_Hold_Previous_Output(p_output, p_cals);
   }
   else
   {
      Lane_Model_Camera_Persistent.count_hold_prev_valid_output = 0u;
   }

   /* Save the current output in persistence */
   Lane_Model_Camera_Persistent.prev_lm_output_camera.lane_width         = p_output->lane_width;
   Lane_Model_Camera_Persistent.prev_lm_output_camera.lane_center_offset = p_output->lane_center_offset;
   Lane_Model_Camera_Persistent.prev_lm_output_camera.status             = p_output->status;
}

void Lcda_Init_Lane_Output(Lane_Model_Output_Camera_T *p_output)
{
   p_output->lane_width         = LCDA_LANE_WIDTH_UNKNOWN;
   p_output->lane_center_offset = LCDA_LANE_CENTER_OFFSET_UNKNOWN;
   p_output->status             = LM_CAMERA_STATUS_INIT;
   p_output->calculation_method = CAMERA_CALC_METHOD_DEFAULT_VALUE;
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static void Lcda_Process_Camera_Data(const Camera_Data_T *p_cam_data, const Lcda_Core_Calibration_T *p_cals)
{
   /* Increment counts if lane existence prob is high enough */
   if (p_cam_data->lane_existance_probability_first_left > p_cals->k_lm_min_lane_exist_prob_percent)
   {
      Sat_Inc_Uint16(&Lane_Model_Camera_Persistent.count_valid_lane_left);
   }
   else
   {
      Lane_Model_Camera_Persistent.count_valid_lane_left = 0u;
   }

   if (p_cam_data->lane_existance_probability_first_right > p_cals->k_lm_min_lane_exist_prob_percent)
   {
      Sat_Inc_Uint16(&Lane_Model_Camera_Persistent.count_valid_lane_right);
   }
   else
   {
      Lane_Model_Camera_Persistent.count_valid_lane_right = 0u;
   }

   /* Increment count if the ego lane width quality is good enough */
   if ((LCDA_EGO_NORMAL_QUALITY_LANE_WIDTH == (uint8_t) p_cam_data->quality_lane_width_ego)
       || (LCDA_EGO_AVERAGE_QUALITY_LANE_WIDTH == (uint8_t) p_cam_data->quality_lane_width_ego))
   {
      Sat_Inc_Uint16(&Lane_Model_Camera_Persistent.count_valid_ego_lane_width);
   }
   else
   {
      Lane_Model_Camera_Persistent.count_valid_ego_lane_width = 0u;
   }


   /* Increment count if the quality of the left adjacent lane width is good enough */
   if ((LCDA_ADJACENT_NORMAL_QUALITY_LANE_WIDTH == (uint8_t) p_cam_data->quality_lane_width_left)
       || (LCDA_ADJACENT_AVERAGE_QUALITY_LANE_WIDTH == (uint8_t) p_cam_data->quality_lane_width_left))
   {
      Sat_Inc_Uint16(&Lane_Model_Camera_Persistent.count_valid_left_adjacent_lane_width);
   }
   else
   {
      Lane_Model_Camera_Persistent.count_valid_left_adjacent_lane_width = 0u;
   }

   /* Increment count if the quality of the right adjacent lane width is good enough */
   if ((LCDA_ADJACENT_NORMAL_QUALITY_LANE_WIDTH == (uint8_t) p_cam_data->quality_lane_width_right)
       || (LCDA_ADJACENT_AVERAGE_QUALITY_LANE_WIDTH == (uint8_t) p_cam_data->quality_lane_width_right))
   {
      Sat_Inc_Uint16(&Lane_Model_Camera_Persistent.count_valid_right_adjacent_lane_width);
   }
   else
   {
      Lane_Model_Camera_Persistent.count_valid_right_adjacent_lane_width = 0u;
   }
}

static boolean_T Lcda_Is_Lane_Data_Plausible(const float32_T lane_width, const float32_T lc_offset, const Lcda_Core_Calibration_T *p_cals)
{
   return (boolean_T) ((lane_width > p_cals->k_lm_min_plausible_lane_width) && (lane_width < p_cals->k_lm_max_plausible_lane_width)
                       && (Fbk_Abs_F(lc_offset) < (p_cals->k_lm_max_plausible_lc_offset_factor * lane_width)));
}

static void Lcda_Get_Output_Lane_Borders(const Lcda_Input_T *p_lcda_input,
                                         const Fbk_Vehicle_Data_T *p_vehicle_data,
                                         const Lcda_Core_Calibration_T *p_cals,
                                         Lane_Model_Output_Camera_T *p_output)
{
   float32_T lane_width;
   float32_T lane_center;
   float32_T lc_offset;


   /* If lane data is valid for a certain number of cycles, then use it calculate the lane width*/
   if ((Lane_Model_Camera_Persistent.count_valid_lane_left > p_cals->k_lm_min_qualification_cycle_count)
       && (Lane_Model_Camera_Persistent.count_valid_lane_right > p_cals->k_lm_min_qualification_cycle_count))
   {
      lane_width = p_lcda_input->camera_data->lane_distance_first_left - p_lcda_input->camera_data->lane_distance_first_right;

      lane_center = Fbk_Half(lane_width);

      /* Get lane center offset in VCS co-ordinates */
      lc_offset = -p_lcda_input->camera_data->lane_distance_first_right - lane_center;


      /* If the data is plausible then populate the output */
      if (Lcda_Is_Lane_Data_Plausible(lane_width, lc_offset, p_cals))
      {
         p_output->lane_width         = lane_width;
         p_output->lane_center_offset = lc_offset;
         p_output->status             = LM_CAMERA_STATUS_AVAILABLE;
         p_output->calculation_method = CAMERA_CALC_METHOD_LANE_BORDERS;
      }
      else
      {
         Lcda_Init_Lane_Output(p_output);
      }
   }
   else
   {
      /* The lane borders are not yet qualified so we cannot calculate the output yet */
      Lcda_Init_Lane_Output(p_output);
   }

   /* If lane data is valid for a certain number of cycles, then use it calculate the lane width*/
   if ((Lane_Model_Camera_Persistent.count_valid_lane_left > p_cals->k_lm_min_qualification_cycle_count)
       && (p_lcda_input->camera_data->lane_existance_probability_first_left <= LCDA_MAX_VALID_LANE_EXISTENCE_PROBABILITY))
   {
      /* Calculate the lateral speed */
      p_output->lane_lateral_speed[FBK_SIDE_LEFT] =
         Fast_Sin(p_lcda_input->camera_data->lane_angle_first_left) * p_vehicle_data->host_speed;
      /*Set persistent data*/
      Lane_Model_Camera_Persistent.prev_lane_lateral_speed[FBK_SIDE_LEFT] = p_output->lane_lateral_speed[FBK_SIDE_LEFT];
      Lane_Model_Camera_Persistent.count_holding_lateral_speed_left       = FBK_ZERO_UINT;
   }
   else if (Lane_Model_Camera_Persistent.count_holding_lateral_speed_left < LCDA_LANE_LATERAL_SPEED_MAX_HOLDING_COUNTER)
   {
      p_output->lane_lateral_speed[FBK_SIDE_LEFT] = Lane_Model_Camera_Persistent.prev_lane_lateral_speed[FBK_SIDE_LEFT];
      Sat_Inc_Uint16(&Lane_Model_Camera_Persistent.count_holding_lateral_speed_left);
   }
   else
   {
      p_output->lane_lateral_speed[FBK_SIDE_LEFT] = FBK_ZERO_F;
   }

   /* If lane data is valid for a certain number of cycles, then use it calculate the lane width*/
   if ((Lane_Model_Camera_Persistent.count_valid_lane_right > p_cals->k_lm_min_qualification_cycle_count)
       && (p_lcda_input->camera_data->lane_existance_probability_first_right <= LCDA_MAX_VALID_LANE_EXISTENCE_PROBABILITY))
   {
      /* Calculate the lateral speed */
      p_output->lane_lateral_speed[FBK_SIDE_RIGHT] =
         Fast_Sin(p_lcda_input->camera_data->lane_angle_first_right) * p_vehicle_data->host_speed;
      /*Set persistent data*/
      Lane_Model_Camera_Persistent.prev_lane_lateral_speed[FBK_SIDE_RIGHT] = p_output->lane_lateral_speed[FBK_SIDE_RIGHT];
      Lane_Model_Camera_Persistent.count_holding_lateral_speed_right       = FBK_ZERO_UINT;
   }
   else if (Lane_Model_Camera_Persistent.count_holding_lateral_speed_right < LCDA_LANE_LATERAL_SPEED_MAX_HOLDING_COUNTER)
   {
      p_output->lane_lateral_speed[FBK_SIDE_RIGHT] = Lane_Model_Camera_Persistent.prev_lane_lateral_speed[FBK_SIDE_RIGHT];
      Sat_Inc_Uint16(&Lane_Model_Camera_Persistent.count_holding_lateral_speed_right);
   }
   else
   {
      p_output->lane_lateral_speed[FBK_SIDE_RIGHT] = FBK_ZERO_F;
   }

   /*Plausibilization of lateral Speed*/
   if (Fbk_Abs_F(p_output->lane_lateral_speed[FBK_SIDE_RIGHT]) > LCDA_LANE_MAX_LATERAL_SPEED)
   {
      p_output->lane_lateral_speed[FBK_SIDE_RIGHT] = FBK_ZERO_F;
   }
   if (Fbk_Abs_F(p_output->lane_lateral_speed[FBK_SIDE_LEFT]) > LCDA_LANE_MAX_LATERAL_SPEED)
   {
      p_output->lane_lateral_speed[FBK_SIDE_LEFT] = FBK_ZERO_F;
   }
}

static void Lcda_Get_Camera_Ego_Lane_Info(const Lcda_Input_T *p_lcda_input,
                                          const Lcda_Core_Calibration_T *p_cals,
                                          Lane_Model_Output_Camera_T *p_output)
{
   if (Lane_Model_Camera_Persistent.count_valid_ego_lane_width > p_cals->k_lm_min_qualification_cycle_count)
   {
      p_output->lane_width         = p_lcda_input->camera_data->lane_width_ego;
      p_output->lane_center_offset = LCDA_LANE_CENTER_OFFSET_UNKNOWN;
      p_output->status             = LM_CAMERA_STATUS_DEGRADED;
      p_output->calculation_method = CAMERA_CALC_METHOD_EGO_LANE_INFO;

      /* Plausibility check of the output data */
      if (Lcda_Is_Lane_Data_Plausible(p_output->lane_width, p_output->lane_center_offset, p_cals))
      {
         /* Keep the output as it is*/
      }
      else
      {
         Lcda_Init_Lane_Output(p_output);
      }
   }
   else
   {
      Lcda_Init_Lane_Output(p_output);
   }
}

static void Lcda_Hold_Previous_Output(Lane_Model_Output_Camera_T *p_output, const Lcda_Core_Calibration_T *p_cals)
{
   if ((Lane_Model_Camera_Persistent.count_hold_prev_valid_output < p_cals->k_lm_min_output_hold_cycles)
       && ((LM_CAMERA_STATUS_DEGRADED == Lane_Model_Camera_Persistent.prev_lm_output_camera.status)
           || (LM_CAMERA_STATUS_AVAILABLE == Lane_Model_Camera_Persistent.prev_lm_output_camera.status)))
   {
      p_output->lane_width         = Lane_Model_Camera_Persistent.prev_lm_output_camera.lane_width;
      p_output->lane_center_offset = LCDA_LANE_CENTER_OFFSET_UNKNOWN;
      p_output->status             = LM_CAMERA_STATUS_DEGRADED;
      p_output->calculation_method = CAMERA_CALC_METHOD_HOLD_OUTPUT;

      Lane_Model_Camera_Persistent.count_hold_prev_valid_output++;
   }
   else
   {
      Lcda_Init_Lane_Output(p_output);
      Lane_Model_Camera_Persistent.count_hold_prev_valid_output = 0u;
   }
}
