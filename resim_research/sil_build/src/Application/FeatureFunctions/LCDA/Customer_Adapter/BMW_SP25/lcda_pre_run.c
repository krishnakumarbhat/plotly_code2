/**
 * @file lcda_pre_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the BMW SRR5 pre run logic for LCDA.
 *
 * @copyright Copyright (C) 2018 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "lcda_pre_run.h"
#include "camera_data_t.h"
#include "fbk_guardrail_data_t.h"
#include "fbk_macros.h"
#include "fbk_vehicle_data_t.h"
#include "lane_model.h"
#include "lcda_bmw_sp25_debug_interface.h"
#include "lcda_bmw_sp25_types.h"
#include "lcda_core_calibration_t.h"
#include "lcda_core_input_t.h"
#include "lcda_customer_calibration_t.h"
#include "lcda_state_machine.h"
#include "lcda_types.h"
#include "ml_interval.h"
#include "ml_math.h"
#include "ml_saturated_math.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"

#include <assert.h>

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/
static LCDA_FF_State_T Lcda_current_state = LCDA_STATE_NOT_AVAILABLE;

/**
 * @brief Getter function for LCDA states
 *
 */
LCDA_FF_State_T *Lcda_Get_State_Output_Ptr(void)
{
   return &Lcda_current_state;
}

/*===========================================================================*\
* File Scope typedefs
\*===========================================================================*/

/**
 * @brief Lcda_Guardrail_Persistent_T summarizes possible guardrail related persistent information.
 */
typedef struct
{
   Lcda_Guardrail_Sources_T guardrail_data[FBK_NUMBER_OF_SIDES]; /**< Struct for guardrail data */
   uint8_t bad_guardrail_holding_counter[FBK_NUMBER_OF_SIDES];
   uint8_t stage_age[FBK_NUMBER_OF_SIDES]; /**< number of scans RADAR guardail has existed */
} Lcda_Guardrail_Persistent_T;

/* Lane type values according to BMW SRR5 board net specifications. */
typedef enum
{
   LCDA_CAMERA_LANE_TYPE_UNKNOWN                    = 0x00,
   LCDA_CAMERA_LANE_TYPE_DASHED                     = 0x01,
   LCDA_CAMERA_LANE_TYPE_SOLID                      = 0x02,
   LCDA_CAMERA_LANE_TYPE_DOTTED                     = 0x03,
   LCDA_CAMERA_LANE_TYPE_ROAD_EDGE                  = 0x04,
   LCDA_CAMERA_LANE_TYPE_DOUBLE_LINE_CROSSABLE      = 0x05,
   LCDA_CAMERA_LANE_TYPE_DOUBLE_LINE_UNCROSSABLE    = 0x06,
   LCDA_CAMERA_LANE_TYPE_MULTIPLE_LINES_CROSSABLE   = 0x07,
   LCDA_CAMERA_LANE_TYPE_MULTIPLE_LINES_UNCROSSABLE = 0x08,
   LCDA_CAMERA_LANE_TYPE_CURB                       = 0x09,
   LCDA_CAMERA_LANE_TYPE_SNOW_EDGE                  = 0x0a,
   LCDA_CAMERA_LANE_TYPE_STRUCTURED                 = 0x0b,
   LCDA_CAMERA_LANE_TYPE_UNDEFINED                  = 0x0c,
   LCDA_CAMERA_LANE_TYPE_INTERFACE_NOT_AVAILABLE    = 0x0d,
   LCDA_CAMERA_LANE_TYPE_RESERVED_ERROR             = 0x0e,
   LCDA_CAMERA_LANE_TYPE_SIGNAL_NOT_FILLED          = 0x0f
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} LCDA_CAMERA_LANE_TYPE_T;

/* Defines the HMI warntrigger switches so that warn settings can be set according to the respective situation for the core. */
typedef enum
{
   LCDA_BMW_WARNTRIGGER_LATE       = (0),
   LCDA_BMW_WARNTRIGGER_NORMAL     = (1),
   LCDA_BMW_WARNTRIGGER_EARLY      = (2),
   LCDA_BMW_WARNTRIGGER_VERY_EARLY = (3)
} Lcda_Bmw_Hmi_Warntrigger_T;

/*===========================================================================*\
* File Scope variables
\*===========================================================================*/

static Lane_Model_Output_T Lane_Model_Output;
/* coverity[misra_c_2012_rule_8_9_violation][Defined static variable in dedicated section for overview of memory consumption] */
static Camera_Data_T Dummy_Camera_Data;
static Lcda_Guardrail_Persistent_T Guardrail_Persistent;
/* coverity[misra_c_2012_rule_8_9_violation][Defined static variable in dedicated section for overview of memory consumption] */
static uint8_t Lane_Change_Counter[FBK_NUMBER_OF_SIDES];
/* coverity[misra_c_2012_rule_8_9_violation][Defined static variable in dedicated section for overview of memory consumption] */
static uint8_t Camera_Lane_Plausibilisation_Counter[FBK_NUMBER_OF_SIDES];


/*===========================================================================*\
* Local Function Prototypes
\*===========================================================================*/

/**
 * @brief Check if the appropriate conditions have been met for the guardrail data to be valid
 *
 * @return True if the guardrail data shall be suppressed.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 */
static boolean_T Lcda_Is_Guardrail_Invalid(uint8_t side,
                                           const Lcda_Guardrail_Persistent_T *p_guardrail_persistent,
                                           const Lcda_Customer_Calibration_T *p_cust_cals);

/**
 * @brief Sets radar based guardrail information.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2780}
 * @SDD{SF-6963}
 */
static void Lcda_Set_Radar_Based_Guardrail(Lcda_Core_Input_T *p_core_input,
                                           Lcda_Guardrail_Persistent_T *p_guardrail_persistent,
                                           const Lcda_Input_T *p_lcda_input,
                                           const Lcda_Core_Calibration_T *p_cals,
                                           const Lcda_Customer_Calibration_T *p_cust_cals);

/**
 * @brief Computes lateral position and confidence of camera.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2780}
 * @SDD{SF-6964}
 */
static void Lcda_Compute_Camera_Lat_Pos_And_Confidence(float32_T *p_camera_lateral_position,
                                                       float32_T *p_camera_confidence,
                                                       const uint8_t lane_type_first,
                                                       const float32_T lane_distance_first,
                                                       const float32_T lane_existance_probability_first,
                                                       const uint8_t lane_type_second,
                                                       const float32_T lane_distance_second,
                                                       const float32_T lane_existance_probability_second);

/**
 * @brief Maps guardrail sources like radar guardrail and camera guardrail to core input.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2780}
 * @SDD{SF-6847}
 * @verification{Check whether the mapping of the guardrail data to the expected core input is correctly.}
 */
static void Lcda_Set_Guardrail_Data(Lcda_Core_Input_T *p_core_input /**< Core input of Lcda */,
                                    Lcda_Guardrail_Persistent_T *p_guardrail_persistent /**< Lcda persistent data */,
                                    const Lcda_Input_T *p_lcda_input /**< Lcda feature input */,
                                    const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
                                    const Lcda_Customer_Calibration_T *p_cust_cals /**< Lcda customer calibrations */);

/**
 * @brief Maps the hmi flag of the feature set from external to the warn settings of the core.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2780}
 * @SDD{SF-6850}
 * @verification{Check whether the warn settings are set correctly for different hmi settings.}
 */
static void Lcda_Set_Hmi_Warn_Settings(Lcda_Core_Input_T *p_core_input /**< Core input of Lcda */,
                                       const Lcda_Input_T *p_lcda_input /**< Lcda feature input */,
                                       const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */);

/**
 * @brief Increases or decreases plausibilisation counter for camera lanes.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2780}
 * @SDD{SF-6931}
 * @verification{}
 */
static void Lcda_Camera_Lane_Plausibilisation(const Lcda_Input_T *p_lcda_input /**< Lcda feature input */,
                                              const Lcda_Customer_Calibration_T *p_cals /**< Lcda calibrations */);

/**
 * @brief Set lane change counter to check for lane changes.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2780}
 * @SDD{SF-6966}
 * @verification{}
 */
static void
Lcda_Set_Lane_Change_Counter(const Lcda_Core_Input_T *p_core_input /**< Core input of Lcda */,
                             const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
                             const Lcda_Customer_Calibration_T *p_custom_cals /**< Lcda customer calibrations */,
                             const float32_T lane_dist_first /**< Distance to the first lane marking */,
                             const float32_T lane_width_raw /**< Raw lane width */,
                             const uint8_t side /**< Side on which we check for a lane change */,
                             const uint8_t opposite_side /**< Opposite side of where we check for a lane change */,
                             const boolean_T f_host_drives_over_laneline /**< Flag indicating if the host crossed the lane line */,
                             const boolean_T f_lane_plausible /**< Are lane indications plausible */);

/**
 * @brief Lane change detection based on camera lanes and vehicle movement across them.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2780}
 * @SDD{SF-6930}
 * @verification{}
 */
static void Lcda_Lane_Change_Detection(Lcda_Core_Input_T *p_core_input /**< Core input of Lcda */,
                                       const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                       const Lcda_Input_T *p_lcda_input /**< Lcda feature input */,
                                       const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
                                       const Lcda_Customer_Calibration_T *p_custom_cals /**< Lcda customer calibrations */);


const Lane_Model_Output_T *Lcda_Get_Lane_Model_Output(void);

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

const Lane_Model_Output_T *Lcda_Get_Lane_Model_Output(void)
{
   return &Lane_Model_Output;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
void Lcda_Init_Input(Lcda_Input_T *p_lcda_input)
{
   assert(NULL != p_lcda_input);
   if (p_lcda_input->camera_data == NULL)
   {
      p_lcda_input->camera_data = &Dummy_Camera_Data;
   }
   /* Set flags to default */
   p_lcda_input->f_lcda_enable                              = FBK_ZERO_UINT;
   p_lcda_input->f_lcda_enable_bsw                          = FBK_ZERO_UINT;
   p_lcda_input->f_lcda_enable_cvw                          = FBK_ZERO_UINT;
   p_lcda_input->f_lcda_enable_slc                          = FBK_ZERO_UINT;
   p_lcda_input->f_lcda_enable_awa                          = FBK_ZERO_UINT;
   p_lcda_input->f_lcda_enable_dropback                     = FBK_ZERO_UINT;
   p_lcda_input->f_lcda_trailer_mode                        = FBK_ZERO_UINT;
   p_lcda_input->f_lcda_trailer_connected                   = FBK_ZERO_UINT;
   p_lcda_input->f_lcda_enable_basic_lane_model             = FBK_ZERO_UINT;
   p_lcda_input->f_lcda_enable_extended_lane_model          = FBK_ZERO_UINT;
   p_lcda_input->lcda_coding_parameters.c_f_lcda_enable_bsw = FBK_ZERO_UINT;
   p_lcda_input->lcda_coding_parameters.c_f_lcda_enable_slc = FBK_ZERO_UINT;
   p_lcda_input->lcda_coding_parameters.c_f_lcda_enable_cvw = FBK_ZERO_UINT;
   p_lcda_input->lcda_coding_parameters.c_f_lcda_enable_awa = FBK_ZERO_UINT;


   p_lcda_input->lcda_input_signals.vehicle_condition              = PARKENBN_NIO;
   p_lcda_input->lcda_input_signals.vehicle_driving_direction      = VEHICLE_STANDSTILL;
   p_lcda_input->lcda_input_signals.curve_radii                    = FBK_ZERO_F;
   p_lcda_input->lcda_coding_parameters.c_lcda_min_vel_lower_limit = FBK_ZERO_F;
   p_lcda_input->lcda_coding_parameters.c_lcda_max_vel_upper_limit = FBK_ZERO_F;
   p_lcda_input->lcda_coding_parameters.c_f_lcda_enabled           = FBK_FALSE;
   p_lcda_input->lcda_coding_parameters.c_min_curve_radii          = FBK_ZERO_F;
   p_lcda_input->lcda_input_signals.lcda_function_error            = LEVEL0;
   /* Enable fallback handling by default */
   p_lcda_input->f_lcda_enable_fallback = FBK_ONE_UINT;

   /* Enable guardrails by default */
   p_lcda_input->f_lcda_enable_environment_plausibilization = FBK_ONE_UINT;

   /* Disable China-specific modes by default */
   p_lcda_input->f_lcda_enable_cvw_limit_zone = FBK_ZERO_UINT;
   p_lcda_input->f_lcda_enable_bsw_GBT        = FBK_ZERO_UINT;

   /* Set country_type to default */
   p_lcda_input->country_type = (uint8_t) COUNTRY_TYPE_DEFAULT;
}

/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_lcda_instance" points to a non-constant type.] */
void Lcda_Pre_Run_Init(Lcda_Instance_T *p_lcda_instance)
{
   const Lcda_Core_Calibration_T *p_cals;

   /* Assert */
   assert(NULL != p_lcda_instance);
   p_cals = &p_lcda_instance->calibration;

   /* Initialize the lane model */
   Lcda_Initialize_Lane_Model(p_cals, &Lane_Model_Output);

   /*Init guardrail holding counter*/
   Guardrail_Persistent.bad_guardrail_holding_counter[FBK_SIDE_LEFT]  = FBK_ZERO_UINT;
   Guardrail_Persistent.bad_guardrail_holding_counter[FBK_SIDE_RIGHT] = FBK_ZERO_UINT;
   Guardrail_Persistent.stage_age[FBK_SIDE_LEFT]                      = FBK_ZERO_UINT;
   Guardrail_Persistent.stage_age[FBK_SIDE_RIGHT]                     = FBK_ZERO_UINT;
}
void Lcda_Pre_Run(Lcda_Instance_T *p_lcda_instance /**< Lcda core input */,
                  const Lcda_Input_T *p_lcda_input /**< Lcda input */,
                  const Fbk_Output_T *p_fbk_output)
{
   Lcda_Core_Input_T *p_core_input;
   const Fbk_Vehicle_Data_T *p_vehicle_data;
   const Lcda_Core_Calibration_T *p_cals;
   const Lcda_Customer_Calibration_T *p_custom_cals;

   /* Asserts */
   assert(NULL != p_lcda_instance);
   assert(NULL != p_lcda_input);
   assert(NULL != p_fbk_output);

   /* Fill vehicle data */
   p_core_input   = &p_lcda_instance->core_input;
   p_vehicle_data = &p_fbk_output->p_pa_data->vehicle_data;
   p_cals         = &p_lcda_instance->calibration;
   p_custom_cals  = &p_lcda_instance->customer_calibration;

   p_core_input->p_pa_data = p_fbk_output->p_pa_data;
   /* Call lane model processing */
   Lcda_Process_Lane_Model(p_lcda_input, p_vehicle_data, p_cals, &Lane_Model_Output);

   Lcda_Set_Current_Lcda_Functional_State(p_lcda_input, &Lcda_current_state, p_vehicle_data);

   if (Lcda_current_state == LCDA_STATE_ACTIVE)
   {
      p_core_input->enabled_flags.f_lcda_enabled = (boolean_T) (Fbk_Is_True(p_lcda_input->f_lcda_enable));
   }
   /* Independent flags */
   p_core_input->enabled_flags.f_dropback_enabled = (boolean_T) (Fbk_Is_True(p_lcda_input->f_lcda_enable_dropback));
   p_core_input->enabled_flags.f_fallback_enabled = (boolean_T) (Fbk_Is_True(p_lcda_input->f_lcda_enable_fallback));

   /* Flags set depending on the trailer */
   if (Fbk_Is_True(p_lcda_input->f_lcda_trailer_mode) && Fbk_Is_True(p_lcda_input->f_lcda_trailer_connected))
   {
      p_core_input->enabled_flags.f_bsw_enabled = (boolean_T) (Fbk_Is_True(p_lcda_input->f_lcda_enable_bsw));
      p_core_input->enabled_flags.f_cvw_enabled = FBK_FALSE;
      p_core_input->enabled_flags.f_slc_enabled = FBK_FALSE;
      p_core_input->enabled_flags.f_elc_enabled = FBK_FALSE;
   }
   else
   {
      p_core_input->enabled_flags.f_bsw_enabled = (boolean_T) (Fbk_Is_True(p_lcda_input->f_lcda_enable_bsw));
      p_core_input->enabled_flags.f_cvw_enabled = (boolean_T) (Fbk_Is_True(p_lcda_input->f_lcda_enable_cvw));
      p_core_input->enabled_flags.f_slc_enabled = (boolean_T) (Fbk_Is_True(p_lcda_input->f_lcda_enable_slc));
      p_core_input->enabled_flags.f_elc_enabled = (boolean_T) (Fbk_Is_True(p_lcda_input->f_lcda_enable_awa));
   }

   /* Set BSW zone calculation mode based on GBT/China flag */
   if (Fbk_Is_True(p_lcda_input->f_lcda_enable_bsw_GBT))
   {
      p_core_input->bsw_zone_calculation_mode = BSW_ZONE_CALC_FIXED_ZONE_VCS;
   }
   else
   {
      p_core_input->bsw_zone_calculation_mode = BSW_ZONE_CALC_VL_LW;
   }

   /* Set warn settings dependent on the given Hmi */
   Lcda_Set_Hmi_Warn_Settings(p_core_input, p_lcda_input, p_cals);

   /* Set lane width from lane model  */
   p_core_input->lane_width                         = Lane_Model_Output.lane_width;
   p_core_input->lane_center_offset                 = Lane_Model_Output.lane_center_offset;
   p_core_input->lane_lateral_speed[FBK_SIDE_LEFT]  = Lane_Model_Output.lane_lateral_speed[FBK_SIDE_LEFT];
   p_core_input->lane_lateral_speed[FBK_SIDE_RIGHT] = Lane_Model_Output.lane_lateral_speed[FBK_SIDE_RIGHT];

   Lcda_Set_Guardrail_Data(p_core_input, &Guardrail_Persistent, p_lcda_input, p_cals, &p_lcda_instance->customer_calibration);

   Lcda_Camera_Lane_Plausibilisation(p_lcda_input, p_custom_cals);

   Lcda_Lane_Change_Detection(p_core_input, p_vehicle_data, p_lcda_input, p_cals, p_custom_cals);

   /* Write LCDA inputs to debug output */
   Binary_Pass_Lcda_Debug_Bmw_Sp25_Pre_Run(Lane_Change_Counter, Camera_Lane_Plausibilisation_Counter);
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static boolean_T Lcda_Is_Guardrail_Invalid(uint8_t side,
                                           const Lcda_Guardrail_Persistent_T *p_guardrail_persistent,
                                           const Lcda_Customer_Calibration_T *p_cust_cals)
{
   boolean_T f_valid = FBK_FALSE;
   if (p_guardrail_persistent->stage_age[side] < p_cust_cals->k_bmw_sp25_guardrail_age_stage_thresh)
   {
      f_valid = FBK_TRUE;
   }

   return f_valid;
}

static void Lcda_Set_Radar_Based_Guardrail(Lcda_Core_Input_T *p_core_input,
                                           Lcda_Guardrail_Persistent_T *p_guardrail_persistent,
                                           const Lcda_Input_T *p_lcda_input,
                                           const Lcda_Core_Calibration_T *p_cals,
                                           const Lcda_Customer_Calibration_T *p_cust_cals)
{
   uint8_t side;
   const Fbk_Guardrail_Data_T *p_guardrail_data;
   float32_T lat_pos_rel_diff;
   float32_T guardrail_lat_pos;
   float32_T guardrail_lat_pos_prev;

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      p_guardrail_data = &p_core_input->p_pa_data->guardrail_data[side];

      if ((Fbk_Is_True(p_lcda_input->f_lcda_enable_environment_plausibilization)) && (Fbk_Is_True(p_guardrail_data->f_present)))
      {
         float32_T exist_prob_mock = LCDA_GUARDRAIL_LOW_EXIST_PROB;

         if (PA_OBJ_STATUS_MATURE == p_guardrail_data->status)
         {
            exist_prob_mock = LCDA_GUARDRAIL_HIGH_EXIST_PROB;
         }

         p_core_input->guardrail_data[side].radar.lateral_position = p_guardrail_data->lat_pos;
         p_core_input->guardrail_data[side].radar.confidence       = exist_prob_mock;
      }
      else
      {
         p_core_input->guardrail_data[side].radar.lateral_position = FBK_ZERO_F;
         p_core_input->guardrail_data[side].radar.confidence       = FBK_ZERO_F;
      }
      /*Set guardrail status based on existence probability and guardrail lateral position relative difference */
      guardrail_lat_pos      = Fbk_Abs_F(p_core_input->guardrail_data[side].radar.lateral_position);
      guardrail_lat_pos_prev = Fbk_Abs_F(p_guardrail_persistent->guardrail_data[side].radar.lateral_position);

      lat_pos_rel_diff = Fbk_Abs_F(guardrail_lat_pos_prev - guardrail_lat_pos) / (Fbk_Max(guardrail_lat_pos_prev, EPSILON));


      if ((p_core_input->guardrail_data[side].radar.confidence >= p_cals->k_min_exist_prob_radar_guardrail)
          && (lat_pos_rel_diff <= p_cust_cals->k_bmw_sp25_guardrail_rel_diff_thresh))
      {
         p_core_input->guardrail_data[side].radar.status = LCDA_GUARDRAIL_VALID;
         Sat_Inc_Uint8(&p_guardrail_persistent->stage_age[side]);
      }
      else
      {
         p_core_input->guardrail_data[side].radar.status = LCDA_GUARDRAIL_INVALID;
         p_guardrail_persistent->stage_age[side]         = FBK_ZERO_UINT;
      }

      /* Suppres radar guardrail data when the conditions were not met */
      if (Lcda_Is_Guardrail_Invalid(side, p_guardrail_persistent, p_cust_cals))
      {
         p_core_input->guardrail_data[side].radar.status = LCDA_GUARDRAIL_INVALID;
      }
   }
}

static void Lcda_Compute_Camera_Lat_Pos_And_Confidence(float32_T *p_camera_lateral_position,
                                                       float32_T *p_camera_confidence,
                                                       const uint8_t lane_type_first,
                                                       const float32_T lane_distance_first,
                                                       const float32_T lane_existance_probability_first,
                                                       const uint8_t lane_type_second,
                                                       const float32_T lane_distance_second,
                                                       const float32_T lane_existance_probability_second)
{
   if (((uint8_t) LCDA_CAMERA_LANE_TYPE_STRUCTURED == lane_type_first) || ((uint8_t) LCDA_CAMERA_LANE_TYPE_CURB == lane_type_first)
       || ((uint8_t) LCDA_CAMERA_LANE_TYPE_ROAD_EDGE == lane_type_first))
   {
      (*p_camera_lateral_position) = -lane_distance_first;
      (*p_camera_confidence)       = LCDA_CONVERT_FROM_PERCENTAGE(lane_existance_probability_first);
   }
   else if (((uint8_t) LCDA_CAMERA_LANE_TYPE_STRUCTURED == lane_type_second)
            || ((uint8_t) LCDA_CAMERA_LANE_TYPE_CURB == lane_type_second)
            || ((uint8_t) LCDA_CAMERA_LANE_TYPE_ROAD_EDGE == lane_type_second))
   {
      (*p_camera_lateral_position) = -lane_distance_second;
      (*p_camera_confidence)       = LCDA_CONVERT_FROM_PERCENTAGE(lane_existance_probability_second);
   }
   else
   {
      (*p_camera_lateral_position) = FBK_ZERO_F;
      (*p_camera_confidence)       = FBK_ZERO_F;
   }

   /*Set corrupt confidence values (out of bounds) to 0*/
   if (((*p_camera_confidence) > FBK_ONE_F) || ((*p_camera_confidence) < FBK_ZERO_F))
   {
      (*p_camera_confidence) = FBK_ZERO_F;
   }
}

static void Lcda_Set_Guardrail_Data(Lcda_Core_Input_T *p_core_input,
                                    Lcda_Guardrail_Persistent_T *p_guardrail_persistent,
                                    const Lcda_Input_T *p_lcda_input,
                                    const Lcda_Core_Calibration_T *p_cals,
                                    const Lcda_Customer_Calibration_T *p_cust_cals)
{
   uint8_t side;
   float32_T camera_lateral_position[FBK_NUMBER_OF_SIDES];
   float32_T camera_confidence[FBK_NUMBER_OF_SIDES];
   float32_T lateral_position_step;
   float32_T confidence_step;
   float32_T step_size;

   float32_T min_exist_prob_camera_guardrail;

   /*Check CAL dependency*/
   if (p_cust_cals->k_bmw_sp25_lowest_probabilty_percentage_cal_for_adjustment > p_cals->k_min_exist_prob_camera_guardrail)
   {
      min_exist_prob_camera_guardrail = p_cust_cals->k_bmw_sp25_lowest_probabilty_percentage_cal_for_adjustment;
   }
   else
   {
      min_exist_prob_camera_guardrail = p_cals->k_min_exist_prob_camera_guardrail;
   }

   /* Set all radar based core guardrail inputs */
   Lcda_Set_Radar_Based_Guardrail(p_core_input, p_guardrail_persistent, p_lcda_input, p_cals, p_cust_cals);

   if (Fbk_Is_True(p_lcda_input->f_lcda_enable_environment_plausibilization)
       && (Fbk_Is_True(p_cals->k_lcda_f_enable_camera_based_guardrail)))
   {
      Lcda_Compute_Camera_Lat_Pos_And_Confidence(
         &camera_lateral_position[FBK_SIDE_LEFT], &camera_confidence[FBK_SIDE_LEFT],
         p_lcda_input->camera_data->lane_type_first_left, p_lcda_input->camera_data->lane_distance_first_left,
         p_lcda_input->camera_data->lane_existance_probability_first_left, p_lcda_input->camera_data->lane_type_second_left,
         p_lcda_input->camera_data->lane_distance_second_left, p_lcda_input->camera_data->lane_existance_probability_second_left);

      Lcda_Compute_Camera_Lat_Pos_And_Confidence(
         &camera_lateral_position[FBK_SIDE_RIGHT], &camera_confidence[FBK_SIDE_RIGHT],
         p_lcda_input->camera_data->lane_type_first_right, p_lcda_input->camera_data->lane_distance_first_right,
         p_lcda_input->camera_data->lane_existance_probability_first_right, p_lcda_input->camera_data->lane_type_second_right,
         p_lcda_input->camera_data->lane_distance_second_right, p_lcda_input->camera_data->lane_existance_probability_second_right);

      /*Calculate camera input based guardrail*/
      for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
      {
         /*Update holding counter for bad guardrails*/
         if (camera_confidence[side] < min_exist_prob_camera_guardrail)
         {
            Sat_Inc_Uint8(&p_guardrail_persistent->bad_guardrail_holding_counter[side]);
         }
         else
         {
            p_guardrail_persistent->bad_guardrail_holding_counter[side] = FBK_ZERO_UINT;
         }

         /*Calculate Step Size*/
         if (Fbk_Abs_F(FBK_ONE_F - p_cust_cals->k_bmw_sp25_lowest_probabilty_percentage_cal_for_adjustment) > EPSILON)
         {
            step_size = FBK_ONE_F / (FBK_ONE_F - p_cust_cals->k_bmw_sp25_lowest_probabilty_percentage_cal_for_adjustment)
                        * (camera_confidence[side] - p_cust_cals->k_bmw_sp25_lowest_probabilty_percentage_cal_for_adjustment);
            step_size = Fbk_Clamp(step_size, FBK_ZERO_F, FBK_ONE_F);
         }
         else
         {
            step_size = FBK_ONE_F;
         }

         /*Check if smoothing or just holding is activated*/
         if (Fbk_Is_False(p_cust_cals->k_bmw_sp25_smooth_camera_signals))
         {
            if (camera_confidence[side] >= min_exist_prob_camera_guardrail)
            {
               /*Take the camera input*/
               step_size = FBK_ONE_F;
            }
            else
            {
               /*Hold the last camera input*/
               step_size = FBK_ZERO_F;
            }
         }

         /*Adjust input values according to the calculated step and size*/
         lateral_position_step = camera_lateral_position[side] - p_guardrail_persistent->guardrail_data[side].camera.lateral_position;
         confidence_step       = camera_confidence[side] - p_guardrail_persistent->guardrail_data[side].camera.confidence;

         /*Do the adjustment of the confidence and the position*/
         p_core_input->guardrail_data[side].camera.lateral_position =
            p_guardrail_persistent->guardrail_data[side].camera.lateral_position + (step_size * lateral_position_step);
         p_core_input->guardrail_data[side].camera.confidence =
            p_guardrail_persistent->guardrail_data[side].camera.confidence + (step_size * confidence_step);

         /*Check if the camera status is valid*/
         p_core_input->guardrail_data[side].camera.status = LCDA_GUARDRAIL_INVALID;
         if (p_core_input->guardrail_data[side].camera.confidence >= p_cals->k_min_exist_prob_camera_guardrail)
         {
            p_core_input->guardrail_data[side].camera.status = LCDA_GUARDRAIL_VALID;
         }

         if (p_guardrail_persistent->bad_guardrail_holding_counter[side] > p_cust_cals->k_bmw_sp25_max_bad_guardrail_holding_counter)
         {
            p_core_input->guardrail_data[side].camera.status     = LCDA_GUARDRAIL_INVALID;
            p_core_input->guardrail_data[side].camera.confidence = p_cals->k_min_exist_prob_camera_guardrail;
         }
      }
   }
   else
   {
      /*Set to default*/
      p_core_input->guardrail_data[FBK_SIDE_LEFT].camera.lateral_position  = FBK_ZERO_F;
      p_core_input->guardrail_data[FBK_SIDE_LEFT].camera.confidence        = FBK_ZERO_F;
      p_core_input->guardrail_data[FBK_SIDE_LEFT].camera.status            = LCDA_GUARDRAIL_INVALID;
      p_core_input->guardrail_data[FBK_SIDE_RIGHT].camera.lateral_position = FBK_ZERO_F;
      p_core_input->guardrail_data[FBK_SIDE_RIGHT].camera.confidence       = FBK_ZERO_F;
      p_core_input->guardrail_data[FBK_SIDE_RIGHT].camera.status           = LCDA_GUARDRAIL_INVALID;
   }

   /*Set persistent guardrail data for next scan index*/
   p_guardrail_persistent->guardrail_data[FBK_SIDE_LEFT].radar.lateral_position =
      p_core_input->guardrail_data[FBK_SIDE_LEFT].radar.lateral_position;
   p_guardrail_persistent->guardrail_data[FBK_SIDE_RIGHT].radar.lateral_position =
      p_core_input->guardrail_data[FBK_SIDE_RIGHT].radar.lateral_position;

   p_guardrail_persistent->guardrail_data[FBK_SIDE_LEFT].camera.lateral_position =
      p_core_input->guardrail_data[FBK_SIDE_LEFT].camera.lateral_position;
   p_guardrail_persistent->guardrail_data[FBK_SIDE_LEFT].camera.confidence =
      p_core_input->guardrail_data[FBK_SIDE_LEFT].camera.confidence;
   p_guardrail_persistent->guardrail_data[FBK_SIDE_LEFT].camera.status = p_core_input->guardrail_data[FBK_SIDE_LEFT].camera.status;
   p_guardrail_persistent->guardrail_data[FBK_SIDE_RIGHT].camera.lateral_position =
      p_core_input->guardrail_data[FBK_SIDE_RIGHT].camera.lateral_position;
   p_guardrail_persistent->guardrail_data[FBK_SIDE_RIGHT].camera.confidence =
      p_core_input->guardrail_data[FBK_SIDE_RIGHT].camera.confidence;
   p_guardrail_persistent->guardrail_data[FBK_SIDE_RIGHT].camera.status = p_core_input->guardrail_data[FBK_SIDE_RIGHT].camera.status;
}


static void Lcda_Set_Hmi_Warn_Settings(Lcda_Core_Input_T *p_core_input,
                                       const Lcda_Input_T *p_lcda_input,
                                       const Lcda_Core_Calibration_T *p_cals)
{
   Lcda_Bmw_Hmi_Warntrigger_T hmi_warn_setting;

   assert(NULL != p_core_input);
   assert(NULL != p_lcda_input);
   assert(NULL != p_cals);

   /* Set hmi warntrigger */
   if (Fbk_Is_True(p_cals->k_lcda_use_default_warntrigger_hmi)
       && ((uint8_t) LCDA_BMW_WARNTRIGGER_VERY_EARLY != p_lcda_input->lcda_warntrigger_hmi))
   {
      /* coverity[misra_c_2012_rule_10_5_violation][Intentional cast from unsigned integer to matching enum type]  */
      hmi_warn_setting = (Lcda_Bmw_Hmi_Warntrigger_T) p_cals->k_lcda_default_warntrigger_hmi;
   }
   else
   {
      /* coverity[misra_c_2012_rule_10_5_violation][Intentional cast from unsigned integer to matching enum type]  */
      hmi_warn_setting = (Lcda_Bmw_Hmi_Warntrigger_T) p_lcda_input->lcda_warntrigger_hmi;
   }

   /* Set warn settings to the core */
   switch (hmi_warn_setting)
   {
      case LCDA_BMW_WARNTRIGGER_LATE:
         p_core_input->warn_settings.bsw_len_factor    = p_cals->k_bsw_warntrigger_late;
         p_core_input->warn_settings.cvw_ttc_threshold = p_cals->k_cvw_ttc + p_cals->k_cvw_warntrigger_late;
         p_core_input->warn_settings.slc_ttc_thres_lon = p_cals->k_slc_critical_lon_ttc + p_cals->k_slc_warntrigger_TTC_lon_late;
         p_core_input->warn_settings.slc_ttc_thres_lat = p_cals->k_slc_critical_lat_ttc + p_cals->k_slc_warntrigger_TTC_lat_late;
         p_core_input->warn_settings.f_use_cvw_lane_change_intention_zone = FBK_FALSE;
         break;
      case LCDA_BMW_WARNTRIGGER_EARLY:
         p_core_input->warn_settings.bsw_len_factor    = p_cals->k_bsw_warntrigger_early;
         p_core_input->warn_settings.cvw_ttc_threshold = p_cals->k_cvw_ttc + p_cals->k_cvw_warntrigger_early;
         p_core_input->warn_settings.slc_ttc_thres_lon = p_cals->k_slc_critical_lon_ttc + p_cals->k_slc_warntrigger_TTC_lon_early;
         p_core_input->warn_settings.slc_ttc_thres_lat = p_cals->k_slc_critical_lat_ttc + p_cals->k_slc_warntrigger_TTC_lat_early;
         p_core_input->warn_settings.f_use_cvw_lane_change_intention_zone = FBK_FALSE;
         break;
      case LCDA_BMW_WARNTRIGGER_VERY_EARLY:
         /* Only used to activate SWA - use normal warn trigger mode*/
         p_core_input->warn_settings.bsw_len_factor                       = LCDA_BMW_DEFAULT_BSW_ADJUSTMENT_FACTOR;
         p_core_input->warn_settings.cvw_ttc_threshold                    = p_cals->k_cvw_ttc;
         p_core_input->warn_settings.slc_ttc_thres_lon                    = p_cals->k_slc_critical_lon_ttc;
         p_core_input->warn_settings.slc_ttc_thres_lat                    = p_cals->k_slc_critical_lat_ttc;
         p_core_input->warn_settings.f_use_cvw_lane_change_intention_zone = FBK_TRUE;
         break;
      case LCDA_BMW_WARNTRIGGER_NORMAL:
      default:
         /* Warntrigger mode normal*/
         p_core_input->warn_settings.bsw_len_factor                       = LCDA_BMW_DEFAULT_BSW_ADJUSTMENT_FACTOR;
         p_core_input->warn_settings.cvw_ttc_threshold                    = p_cals->k_cvw_ttc;
         p_core_input->warn_settings.slc_ttc_thres_lon                    = p_cals->k_slc_critical_lon_ttc;
         p_core_input->warn_settings.slc_ttc_thres_lat                    = p_cals->k_slc_critical_lat_ttc;
         p_core_input->warn_settings.f_use_cvw_lane_change_intention_zone = FBK_FALSE;
         break;
   }
   p_core_input->warn_settings.cvw_rel_vel_range = Create_Float_Range(p_cals->k_cvw_min_object_curvi_relative_speed[FBK_ZERO_UINT],
                                                                      p_cals->k_cvw_max_object_curvi_relative_speed);
}

static void Lcda_Camera_Lane_Plausibilisation(const Lcda_Input_T *p_lcda_input /**< Lcda feature input */,
                                              const Lcda_Customer_Calibration_T *p_cals /**< Lcda calibrations */)
{
   boolean_T f_left_lane_invalid  = (boolean_T) (p_lcda_input->camera_data->lane_existance_probability_first_left
                                                < p_cals->k_bmw_sp25_camera_lane_plausibilisation_exist_prob_min);
   boolean_T f_right_lane_invalid = (boolean_T) (p_lcda_input->camera_data->lane_existance_probability_first_right
                                                 < p_cals->k_bmw_sp25_camera_lane_plausibilisation_exist_prob_min);

   /* Check left lane */
   if (Fbk_Is_False(f_left_lane_invalid))
   {
      /* Increase counter value */
      Sat_Inc_Uint8(&Camera_Lane_Plausibilisation_Counter[FBK_SIDE_LEFT]);

      /* Limit counter value */
      Camera_Lane_Plausibilisation_Counter[FBK_SIDE_LEFT] =
         Fbk_Min(Camera_Lane_Plausibilisation_Counter[FBK_SIDE_LEFT], p_cals->k_bmw_sp25_camera_lane_plausibilisation_counter_max);
   }
   else
   {
      /* Decrease counter value */
      Sat_Dec_Uint8(&Camera_Lane_Plausibilisation_Counter[FBK_SIDE_LEFT]);
   }

   /* Check right lane */
   if (Fbk_Is_False(f_right_lane_invalid))
   {
      /* Increase counter value */
      Sat_Inc_Uint8(&Camera_Lane_Plausibilisation_Counter[FBK_SIDE_RIGHT]);

      /* Limit counter value */
      Camera_Lane_Plausibilisation_Counter[FBK_SIDE_RIGHT] =
         Fbk_Min(Camera_Lane_Plausibilisation_Counter[FBK_SIDE_RIGHT], p_cals->k_bmw_sp25_camera_lane_plausibilisation_counter_max);
   }
   else
   {
      /* Decrease counter value */
      Sat_Dec_Uint8(&Camera_Lane_Plausibilisation_Counter[FBK_SIDE_RIGHT]);
   }
}

static void Lcda_Set_Lane_Change_Counter(const Lcda_Core_Input_T *p_core_input,
                                         const Lcda_Core_Calibration_T *p_cals,
                                         const Lcda_Customer_Calibration_T *p_custom_cals,
                                         const float32_T lane_dist_first,
                                         const float32_T lane_width_raw,
                                         const uint8_t side,
                                         const uint8_t opposite_side,
                                         const boolean_T f_host_drives_over_laneline,
                                         const boolean_T f_lane_plausible)
{
   if (Fbk_Is_True(f_host_drives_over_laneline) && Fbk_Is_True(f_lane_plausible)
       && (lane_width_raw >= p_cals->k_lcda_min_lane_width) && Fbk_Is_False(p_core_input->f_lane_change[opposite_side])
       && ((Fbk_Abs_F(lane_dist_first) <= p_custom_cals->k_bmw_sp25_lane_change_dist_to_laneline_max)
           || Fbk_Is_True(p_core_input->f_lane_change[side])))
   {
      /* Increase counter value */
      Sat_Inc_Uint8(&Lane_Change_Counter[side]);

      /* Limit counter value */
      Lane_Change_Counter[side] = Fbk_Min(Lane_Change_Counter[side], p_custom_cals->k_bmw_sp25_lane_change_counter_max);
   }
   else
   {
      /* Decrease counter value */
      Sat_Dec_Uint8(&Lane_Change_Counter[side]);
   }
}

static void Lcda_Lane_Change_Detection(Lcda_Core_Input_T *p_core_input,
                                       const Fbk_Vehicle_Data_T *p_vehicle_data,
                                       const Lcda_Input_T *p_lcda_input,
                                       const Lcda_Core_Calibration_T *p_cals,
                                       const Lcda_Customer_Calibration_T *p_custom_cals)
{
   float32_T lane_width_raw =
      p_lcda_input->camera_data->lane_distance_first_left - p_lcda_input->camera_data->lane_distance_first_right;
   float32_T host_width_half = Fbk_Half(p_vehicle_data->host_width);
   boolean_T f_host_drives_over_laneline =
      (boolean_T) ((Fbk_Abs_F(p_lcda_input->camera_data->lane_distance_first_left) <= host_width_half)
                   || (Fbk_Abs_F(p_lcda_input->camera_data->lane_distance_first_right) <= host_width_half));
   uint8_t side;
   uint8_t opposite_side;
   boolean_T f_lane_plausible;
   float32_T lane_dist_first;

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      /* Determine which side is the other side of the vehicle */
      if (side == FBK_SIDE_LEFT)
      {
         opposite_side   = FBK_SIDE_RIGHT;
         lane_dist_first = p_lcda_input->camera_data->lane_distance_first_left;
      }
      else
      {
         opposite_side   = FBK_SIDE_LEFT;
         lane_dist_first = p_lcda_input->camera_data->lane_distance_first_right;
      }

      /* Only perform lane change detection for previously plausible camera lanes */
      f_lane_plausible = (boolean_T) ((Camera_Lane_Plausibilisation_Counter[side] > FBK_ZERO_UINT)
                                      || (Fbk_Is_True(p_core_input->f_lane_change[side])));

      /* Lane change detection */
      Lcda_Set_Lane_Change_Counter(p_core_input, p_cals, p_custom_cals, lane_dist_first, lane_width_raw, side, opposite_side,
                                   f_host_drives_over_laneline, f_lane_plausible);

      /* Set lane change flag based on counter value */
      if ((Lane_Change_Counter[side] >= p_custom_cals->k_bmw_sp25_lane_change_counter_min)
          && Fbk_Is_False(p_core_input->f_lane_change[side]))
      {
         p_core_input->f_lane_change[side] = FBK_TRUE;
         Lane_Change_Counter[side]         = p_custom_cals->k_bmw_sp25_lane_change_counter_max;
      }
      if (FBK_ZERO_UINT == Lane_Change_Counter[side])
      {
         p_core_input->f_lane_change[side] = FBK_FALSE;
      }
   }

   /* Reset lane change flags if functionality is disabled or driving condition doesn't match.
    * The reset is performend in the end to keep updating the counter values to use as a debug signal. */
   if (Fbk_Is_False(p_custom_cals->k_bmw_sp25_f_enable_lane_change_detection)
       || (p_vehicle_data->host_speed < p_custom_cals->k_bmw_sp25_lane_change_detection_host_speed_min))
   {
      p_core_input->f_lane_change[FBK_SIDE_LEFT]  = FBK_FALSE;
      p_core_input->f_lane_change[FBK_SIDE_RIGHT] = FBK_FALSE;
   }
}
