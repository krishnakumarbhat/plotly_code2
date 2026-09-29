/**
 * @file lcda_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the Honda_SRR6 post run logic for LCDA.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "lcda_post_run.h"
#include "fbk_field_of_interest.h"
#include "fbk_field_of_interest_factory.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_object_validation.h"
#include "fbk_vehicle_data_t.h"
#include "lcda_core_calibration_t.h"
#include "lcda_core_input_t.h"
#include "lcda_core_output_t.h"
#include "lcda_customer_calibration_t.h"
#include "lcda_honda_instance.h"
#include "ml_math.h"
#include "ml_trigonometry.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include <assert.h>

#ifdef BINARY_DEBUG
#include "lcda_debug_writer.h"
#endif /* BINARY_DEBUG */

/*===========================================================================*\
* Local Defines
\*===========================================================================*/

#define LCDA_LKA_NA_TTC (7.875f)
#define LCDA_LKA_NA_CURVI_POS_LONG (81.92f)
#define LCDA_LKA_NA_CURVI_POS_LAT (81.92f)
#define LCDA_LKA_NA_CURVI_VEL_LONG (36.4f)
#define LCDA_LKA_NA_CURVI_VEL_LAT (25.7f)
#define LCDA_LKA_NA_OBJECT_CLASS HONDA_OBJECT_CLASS_UNKNOWN
#define LCDA_LKA_NA_ALERT_CONDITION HONDA_ALERT_CONDITION_OFF
#define LCDA_LKA_NA_MOTION_CLASS HONDA_MOTION_CLASS_UNKNOWN
#define LCDA_LKA_NA_CHANGE_STATUS HONDA_CHANGE_STATUS_NO_CHANGE
#define LCDA_LKA_NA_OBJECT_ID (7)

/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/

#ifdef BINARY_DEBUG
static void Write_Lcda_Output(const Lcda_Output_T *p_lcda_output, const Lcda_Core_Calibration_T *p_cals);
#endif /* BINARY_DEBUG */

/**
 * @brief Map object class from GDSR to Honda object class.
 *
 * @return Honda object class
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-6941}
 * @verification{Check mapping of object classes.}
 */
static HONDA_OBJECT_CLASS_T Lcda_Lka_Get_Object_Class(Pa_Obj_Class_T object_class);

/**
 * @brief Map track status from GDSR to Honda change status.
 *
 * @return Honda change status
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-6940}
 * @verification{Check mapping of track status to change status.}
 */
static HONDA_CHANGE_STATUS_T Lcda_Lka_Get_Change_Status(Pa_Obj_Status_T track_status);

/**
 * @brief Map track status from GDSR to Honda motion class.
 *
 * @return Honda motion class
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-6939}
 * @verification{Check mapping of track status to motion class.}
 */
static HONDA_MOTION_CLASS_T Lcda_Lka_Get_Motion_Class(Pa_Obj_Status_T track_status);

/**
 * @brief Compute Honda alert condition.
 *
 * @return Honda alert condition
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-6938}
 * @verification{Check alert computation.}
 */
static HONDA_ALERT_CONDITION_T Lcda_Lka_Get_Alert_Condition(float32_T curvi_long_vel_rel);

/**
 * @brief Initialize Lka objects in LCDA output.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-6937}
 * @verification{Check LKA object initialization.}
 */
static void Lcda_Lka_Initialize_Objects(Lcda_Output_T *p_lcda_output);

/**
 * @brief Check if object is in beeper zone.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-80970}
 * @verification{}
 */
static void Lcda_Process_Beeper_Level_3_Alert(Lcda_Output_T *p_output,
                                              const Pa_Data_T *p_pa_data,
                                              const Lcda_Core_Calibration_T *p_cals,
                                              const Lcda_Customer_Calibration_T *p_customer_cals,
                                              const Lcda_Core_Output_T *p_core_output,
                                              const boolean_T f_trailer_present,
                                              const boolean_T f_beeper_zone_active);


/**
 * @brief Set parameters for beeper logic based on host speed.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 * @verification{}
 */
static void Lcda_Process_Host_Speed_For_Beeper_Logic(const float32_T host_speed,
                                                     const uint8_t side,
                                                     boolean_T *p_f_narrow_beeper_active_speed,
                                                     boolean_T *p_f_beeper_active_speed,
                                                     const Lcda_Core_Calibration_T *p_cals,
                                                     const Lcda_Customer_Calibration_T *p_customer_cals);

/**
 * @brief Check if object overlaps narrow beeper zone.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 * @verification{}
 */
static boolean_T Lcda_Is_Obj_In_Narrow_Beeper_Zone(uint8_t side,
                                                   const Lcda_Output_T *p_output,
                                                   const Pa_Data_T *p_pa_data,
                                                   const Lcda_Core_Calibration_T *p_cals,
                                                   const Lcda_Customer_Calibration_T *p_customer_cals,
                                                   const Lcda_Core_Output_T *p_core_output);

/**
 * @brief Check if alert should be held for alert status level 3.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 * @verification{}
 */
static void Lcda_Hold_Alert_Obj_Out_Of_Fov(const uint8_t side,
                                           Lcda_Output_T *p_output,
                                           const Pa_Data_T *p_pa_data,
                                           const Lcda_Customer_Calibration_T *p_customer_cals);

/**
 * @brief Resets the LCDA Honda SRR6 specific output.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-6993}
 * @verification{Create test to check is customer outputs are reset to default value}
 */
static void Lcda_Reset_Output(Lcda_Output_T *p_lcda_output);

/**
 * @brief Set Honda alert level based on BSW and CVW alert states
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 * @verification{}
 */
static void Lcda_Set_Honda_Alert_State(Lcda_Output_T *p_lcda_output,
                                       const Lcda_Input_T *p_lcda_input,
                                       const Pa_Data_T *p_pa_data,
                                       const Lcda_Core_Calibration_T *p_cals,
                                       const Lcda_Customer_Calibration_T *p_customer_cals,
                                       const Lcda_Core_Output_T *p_lcda_core_output);


/**
 * @brief Calculate the time, for which the alert should be holded on after the object leaves the radar FOV.
 *
 * @return none
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-6999}
 * @verification{}
 */
static boolean_T Lcda_Should_Alert_Level_Two_Be_Turned_On(const Pa_Data_T *p_pa_data,
                                                          const Lcda_Customer_Calibration_T *p_cals,
                                                          uint8_t cvw_index,
                                                          uint8_t bsw_index);
/**
 * @brief Calculate estimated time and position of the zone exit point. If it is in zone range, time and object is saved as
 * predicted exit time
 *
 * @return exit time and object index (through pointers)
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-6999}
 * @verification{}
 */

static void Lcda_Set_Hold_Alert_Time_And_Index(const Pa_Data_T *p_pa_data,
                                               const Lcda_Core_Calibration_T *p_cals,
                                               const Fbk_Vehicle_Data_T *p_vehicle_data,
                                               const uint8_t object_index,
                                               Lcda_Output_T *p_lcda_output,
                                               const HONDA_SIDE_T honda_side,
                                               const Lcda_Core_Output_T *p_lcda_core_output);

/**
 * @brief Check if there is need for holding of the alert. It happens when ego slows
 * down while alert is ON . If so, function keeps predicted exit times as hold times
 *
 * @return
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-7000}
 * @verification{}
 */
static void Lcda_Hold_Honda_Alert_Before_Reset_Cvw(Lcda_Output_T *p_lcda_output,
                                                   const Lcda_Core_Output_T *p_lcda_core_output,
                                                   const Fbk_Vehicle_Data_T vehicle_data,
                                                   const Lcda_Core_Calibration_T *p_cals,
                                                   const Lcda_Customer_Calibration_T *p_customer_cals);

/**
 * @brief Check if there is need for holding of the alert. It happens when the object moves outside radar range or ego slows
 * down while alert is ON. If so, function keeps predicted exit times as hold times
 *
 * @return
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-7001}
 * @verification{}
 */
static void Lcda_Hold_Honda_Alert_Before_Reset_Bsw(Lcda_Output_T *p_lcda_output,
                                                   const Lcda_Core_Output_T *p_lcda_core_output,
                                                   const Fbk_Vehicle_Data_T vehicle_data,
                                                   const Lcda_Core_Calibration_T *p_cals,
                                                   const Lcda_Customer_Calibration_T *p_customer_cals,
                                                   const Pa_Data_T *p_pa_data);
/**
 * @brief Function to detect if the alert is turned off in next cycle and alert should be held
 *
 * @return True if alert has dropped down
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 * @verification{}
 */
static boolean_T Lcda_Has_Alert_Drop_Down(const Lcda_Output_T *p_lcda_output,
                                          const Lcda_Core_Output_T *p_lcda_core_output,
                                          HONDA_SIDE_T side);

/**
 * @brief Check if the core alert is ON. For CVW, object speed should be above the threshold.
 *
 * @return
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-7003}
 * @verification{}
 */
static boolean_T Lcda_Is_Cvw_Core_Alert_On(const Lcda_Core_Output_T *p_core_output, uint8_t side);


/**
 * @brief Function to fill properties of the given LKA object basing on the core and tracker output
 *
 * @return none
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-92289}
 * @verification{}
 */
static void Lcda_Set_Lka_Object(LKA_Object_T *p_lka_object,
                                uint8_t obj_tracker_index,
                                const Fbk_Vehicle_Data_T *p_vehicle_data,
                                const Lcda_Core_Output_T *p_core_output,
                                const Pa_Data_T *p_pa_data,
                                HONDA_SIDE_T side);


/**
 * @brief Function setting Honda Alert Level Two for the slidethrough scenarios
 *
 * @return none
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-92290}
 * @verification{}
 */
static void Lcda_Process_Slide_Through_Level_2_Alert(Lcda_Output_T *p_lcda_output,
                                                     const Lcda_Input_T *p_lcda_input,
                                                     const Pa_Data_T *p_pa_data,
                                                     const Lcda_Customer_Calibration_T *p_customer_cals,
                                                     const Lcda_Core_Output_T *p_core_output);
/**
 * @brief Function to set lcda status in the post run output using core data
 *
 * @return LCDA function status (0-1)
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-92291}
 * @verification{}
 */
static uint8_t Lcda_Get_Feauture_Status(const Lcda_Output_T *p_lcda_output, const Lcda_Core_Output_T *p_core_output);

/**
 * @brief Function which returns basic information for about obect ids from the core output. Required for Honda alert two
 * calculations
 *
 * @return Enum value for same or different objects
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 * @verification{}
 */
static HONDA_LVL_TWO_OBJECTS_INFO
Lcda_Get_Object_Info(const Lcda_Core_Output_T *p_core_output, uint8_t bsw_index, uint8_t cvw_index, uint8_t side);

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
void Lcda_Init_Output(Lcda_Output_T *p_lcda_output)
{
   uint8_t index;
   assert(NULL != p_lcda_output);

   /* Initialize informations for hold mode and speed EMA filter*/
   for (index = FBK_ZERO_UINT; index < LCDA_HONDA_NUMBER_OF_OBJECTS; index++)
   {
      p_lcda_output->hold_time[index]           = -LCDA_LKA_NA_TTC;
      p_lcda_output->hold_obj_index[index]      = PA_INVALID_OBJ_INDEX;
      p_lcda_output->predicted_exit_time[index] = -LCDA_LKA_NA_TTC;
   }

   /* Reset Lcda Output */
   Lcda_Reset_Output(p_lcda_output);
}

void Lcda_Post_Run_Init(void)
{
   Lcda_Get_Lcda_Honda_Instance()->bsw_alert_left                                  = FBK_ZERO_UINT;
   Lcda_Get_Lcda_Honda_Instance()->bsw_alert_right                                 = FBK_ZERO_UINT;
   Lcda_Get_Lcda_Honda_Instance()->f_narrow_beeper_prev_cycle[FBK_SIDE_LEFT]       = FBK_FALSE;
   Lcda_Get_Lcda_Honda_Instance()->f_narrow_beeper_prev_cycle[FBK_SIDE_RIGHT]      = FBK_FALSE;
   Lcda_Get_Lcda_Honda_Instance()->f_beeper_prev_cycle[FBK_SIDE_LEFT]              = FBK_FALSE;
   Lcda_Get_Lcda_Honda_Instance()->f_beeper_prev_cycle[FBK_SIDE_RIGHT]             = FBK_FALSE;
   Lcda_Get_Lcda_Honda_Instance()->f_lcda_level_3_alert_prev_cycle[FBK_SIDE_LEFT]  = FBK_FALSE;
   Lcda_Get_Lcda_Honda_Instance()->f_lcda_level_3_alert_prev_cycle[FBK_SIDE_RIGHT] = FBK_FALSE;
}

/* clang-format off */
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_lcda_instance" points to a non-constant type.] */
void Lcda_Post_Run(Lcda_Instance_T *p_lcda_instance, const Lcda_Input_T *p_lcda_input, Lcda_Output_T *p_lcda_output, const Fbk_Output_T *p_fbk_output)
/* clang-format on */
{
   const Lcda_Core_Calibration_T *p_cals;
   const Lcda_Customer_Calibration_T *p_customer_cals;
   const Lcda_Core_Output_T *p_lcda_core_output;
   const Lcda_Core_Input_T *p_lcda_core_input;
   const Pa_Data_T *p_pa_data;
   const Fbk_Vehicle_Data_T *p_vehicle_data;
   uint8_t obj_idx;
   /* Objects from each side are supposed to be in that side's sensor's coordinate system,
   this variable will subtract the needed lateral offset */

   LCDA_CUST_SPEC_OUTPUT_T *p_customer_output = &p_lcda_output->customer_output;

   p_lcda_output->bsw_alert_left  = Lcda_Get_Lcda_Honda_Instance()->bsw_alert_left;
   p_lcda_output->bsw_alert_right = Lcda_Get_Lcda_Honda_Instance()->bsw_alert_right;
   p_lcda_output->customer_output = Lcda_Get_Lcda_Honda_Instance()->customer_output;
   for (obj_idx = 0; obj_idx < LCDA_HONDA_NUMBER_OF_OBJECTS; obj_idx++)
   {
      p_lcda_output->hold_obj_index[obj_idx]   = Lcda_Get_Lcda_Honda_Instance()->hold_obj_index[obj_idx];
      p_lcda_output->hold_alert_level[obj_idx] = Lcda_Get_Lcda_Honda_Instance()->hold_alert_level[obj_idx];
      p_lcda_output->hold_object[obj_idx]      = Lcda_Get_Lcda_Honda_Instance()->hold_object[obj_idx];
      p_lcda_output->hold_time[obj_idx]        = Lcda_Get_Lcda_Honda_Instance()->hold_time[obj_idx];
   }

   /* Check if all input pointers are valid */
   assert(NULL != p_lcda_instance);
   assert(NULL != p_lcda_input);
   assert(NULL != p_lcda_output);
   assert(NULL != p_fbk_output);

   p_cals             = &p_lcda_instance->calibration;
   p_customer_cals    = &p_lcda_instance->customer_calibration;
   p_lcda_core_output = &p_lcda_instance->core_output;
   p_lcda_core_input  = &p_lcda_instance->core_input;
   p_pa_data          = p_lcda_core_input->p_pa_data;
   p_vehicle_data     = &p_pa_data->vehicle_data;

   /* Before reset, check if honda alert should be held and set hold time and objects*/
   Lcda_Hold_Honda_Alert_Before_Reset_Bsw(p_lcda_output, p_lcda_core_output, *p_vehicle_data, p_cals, p_customer_cals, p_pa_data);
   Lcda_Hold_Honda_Alert_Before_Reset_Cvw(p_lcda_output, p_lcda_core_output, *p_vehicle_data, p_cals, p_customer_cals);

   /*Reset Lcda output*/
   Lcda_Reset_Output(p_lcda_output);

   /* Check enable flags */
   p_lcda_output->f_bsw_enabled = Fbk_Convert_Bool_To_Uint(p_lcda_core_output->bsw_core_output.f_bsw_is_enabled);
   p_lcda_output->f_cvw_enabled = Fbk_Convert_Bool_To_Uint(p_lcda_core_output->cvw_core_output.f_cvw_is_enabled);

   /* Reduce holding time by frame interval*/
   p_lcda_output->hold_time[HONDA_SIDE_BSW_LEFT] -= p_pa_data->time_diff_to_last_cycle;
   /*check the hold time and set the lka object  to the object, which had turned on hold mode  */
   /* coverity[misra_c_2012_rule_15_7_violation][No non-empty terminating "else" statement.] */
   if (FBK_ZERO_F < p_lcda_output->hold_time[HONDA_SIDE_BSW_LEFT])
   {
      p_customer_output->LKA_Object_Left[FBK_ZERO_UINT] = p_lcda_output->hold_object[HONDA_SIDE_BSW_LEFT];
      p_lcda_output->bsw_alert_left                     = p_lcda_output->hold_alert_level[HONDA_SIDE_BSW_LEFT];
      p_lcda_output->bsw_id_left                        = p_customer_output->LKA_Object_Left[FBK_ZERO_UINT].lka_obj_id;
   }
   /* Map BSW core outputs to Honda outputs */
   else if (LCDA_ALERT_STATE_NONE != p_lcda_core_output->bsw_core_output.bsw_alert[FBK_SIDE_LEFT])
   {
      uint8_t bsw_index = p_lcda_core_output->bsw_core_output.bsw_index[FBK_SIDE_LEFT];
      Lcda_Set_Lka_Object(&(p_customer_output->LKA_Object_Left[FBK_ZERO_UINT]), bsw_index, p_vehicle_data, p_lcda_core_output,
                          p_pa_data, HONDA_SIDE_BSW_LEFT);
      p_lcda_output->bsw_alert_left = (uint8_t) p_lcda_core_output->bsw_core_output.bsw_alert[FBK_SIDE_LEFT];
      p_lcda_output->bsw_id_left    = p_pa_data->object_data[bsw_index].id;

      /* Calculate the time to cross the front bumper*/
      Lcda_Set_Hold_Alert_Time_And_Index(p_pa_data, p_cals, p_vehicle_data, bsw_index, p_lcda_output, HONDA_SIDE_BSW_LEFT,
                                         &p_lcda_instance->core_output);
   }

   /* Reduce holding time by frame interval*/
   p_lcda_output->hold_time[HONDA_SIDE_BSW_RIGHT] -= p_pa_data->time_diff_to_last_cycle;
   /*check the hold time and set the lka object  to the object, which had turned on hold mode  */
   /* coverity[misra_c_2012_rule_15_7_violation][No non-empty terminating "else" statement.] */
   if (FBK_ZERO_F < p_lcda_output->hold_time[HONDA_SIDE_BSW_RIGHT])
   {
      p_customer_output->LKA_Object_Right[FBK_ZERO_UINT] = p_lcda_output->hold_object[HONDA_SIDE_BSW_RIGHT];
      p_lcda_output->bsw_alert_right                     = p_lcda_output->hold_alert_level[HONDA_SIDE_BSW_RIGHT];
      p_lcda_output->bsw_id_right                        = p_customer_output->LKA_Object_Right[FBK_ZERO_UINT].lka_obj_id;
   }
   else if (LCDA_ALERT_STATE_NONE != p_lcda_core_output->bsw_core_output.bsw_alert[FBK_SIDE_RIGHT])
   {
      uint8_t bsw_index = p_lcda_core_output->bsw_core_output.bsw_index[FBK_SIDE_RIGHT];
      Lcda_Set_Lka_Object(&(p_customer_output->LKA_Object_Right[FBK_ZERO_UINT]), bsw_index, p_vehicle_data, p_lcda_core_output,
                          p_pa_data, HONDA_SIDE_BSW_RIGHT);
      p_lcda_output->bsw_alert_right = (uint8_t) p_lcda_core_output->bsw_core_output.bsw_alert[FBK_SIDE_RIGHT];
      p_lcda_output->bsw_id_right    = p_pa_data->object_data[bsw_index].id;

      /* Calculate the time to cross the front bumper*/
      Lcda_Set_Hold_Alert_Time_And_Index(p_pa_data, p_cals, p_vehicle_data, bsw_index, p_lcda_output, HONDA_SIDE_BSW_RIGHT,
                                         &p_lcda_instance->core_output);
   }

   /* Reduce holding time by frame interval*/
   p_lcda_output->hold_time[HONDA_SIDE_CVW_LEFT] -= p_pa_data->time_diff_to_last_cycle;
   /* coverity[misra_c_2012_rule_15_7_violation][No non-empty terminating "else" statement.] */
   if (FBK_ZERO_F < p_lcda_output->hold_time[HONDA_SIDE_CVW_LEFT])
   {
      p_customer_output->LKA_Object_Left[FBK_ONE_UINT] = p_lcda_output->hold_object[HONDA_SIDE_CVW_LEFT];
      p_lcda_output->cvw_alert_left                    = p_lcda_output->hold_alert_level[HONDA_SIDE_CVW_LEFT];
      p_lcda_output->cvw_id_left                       = p_customer_output->LKA_Object_Left[FBK_ONE_UINT].lka_obj_id;
   }
   /* Map CVW core outputs to Honda outputs */
   else if (Lcda_Is_Cvw_Core_Alert_On(p_lcda_core_output, FBK_SIDE_LEFT))
   {
      uint8_t cvw_index = p_lcda_core_output->cvw_core_output.cvw_index[FBK_SIDE_LEFT];
      Lcda_Set_Lka_Object(&(p_customer_output->LKA_Object_Left[FBK_ONE_UINT]), cvw_index, p_vehicle_data, p_lcda_core_output,
                          p_pa_data, HONDA_SIDE_CVW_LEFT);
      p_lcda_output->cvw_alert_left = (uint8_t) p_lcda_core_output->cvw_core_output.cvw_alert[FBK_SIDE_LEFT];
      p_lcda_output->cvw_id_left    = p_lcda_core_output->cvw_core_output.cvw_id[FBK_SIDE_LEFT];
      p_lcda_output->cvw_ttc_left   = p_lcda_core_output->cvw_core_output.cvw_ttc[FBK_SIDE_LEFT];

      /* Calculate the time to cross the front bumper*/
      Lcda_Set_Hold_Alert_Time_And_Index(p_pa_data, p_cals, p_vehicle_data, cvw_index, p_lcda_output, HONDA_SIDE_CVW_LEFT,
                                         &p_lcda_instance->core_output);
   }

   /* Reduce holding time by frame interval*/
   p_lcda_output->hold_time[HONDA_SIDE_CVW_RIGHT] -= p_pa_data->time_diff_to_last_cycle;
   /* coverity[misra_c_2012_rule_15_7_violation][No non-empty terminating "else" statement.] */
   if (FBK_ZERO_F < p_lcda_output->hold_time[HONDA_SIDE_CVW_RIGHT])
   {
      p_customer_output->LKA_Object_Right[FBK_ONE_UINT] = p_lcda_output->hold_object[HONDA_SIDE_CVW_RIGHT];
      p_lcda_output->cvw_alert_right                    = p_lcda_output->hold_alert_level[HONDA_SIDE_CVW_RIGHT];
      p_lcda_output->cvw_id_right                       = p_customer_output->LKA_Object_Right[FBK_ONE_UINT].lka_obj_id;
   }
   /* Map CVW core outputs to Honda outputs */
   else if (Lcda_Is_Cvw_Core_Alert_On(p_lcda_core_output, FBK_SIDE_RIGHT))
   {
      uint8_t cvw_index = p_lcda_core_output->cvw_core_output.cvw_index[FBK_SIDE_RIGHT];
      Lcda_Set_Lka_Object(&(p_customer_output->LKA_Object_Right[FBK_ONE_UINT]), cvw_index, p_vehicle_data, p_lcda_core_output,
                          p_pa_data, HONDA_SIDE_CVW_RIGHT);

      p_lcda_output->cvw_alert_right = (uint8_t) p_lcda_core_output->cvw_core_output.cvw_alert[FBK_SIDE_RIGHT];
      p_lcda_output->cvw_id_right    = p_lcda_core_output->cvw_core_output.cvw_id[FBK_SIDE_RIGHT];
      p_lcda_output->cvw_ttc_right   = p_lcda_core_output->cvw_core_output.cvw_ttc[FBK_SIDE_RIGHT];

      /* Calculate the time to cross the front bumper*/
      Lcda_Set_Hold_Alert_Time_And_Index(p_pa_data, p_cals, p_vehicle_data, cvw_index, p_lcda_output, HONDA_SIDE_CVW_RIGHT,
                                         &p_lcda_instance->core_output);
   }


   /* Set honda alert state based on BSW and CVW alerts. */
   Lcda_Set_Honda_Alert_State(p_lcda_output, p_lcda_input, p_pa_data, p_cals, p_customer_cals, p_lcda_core_output);


   /* Set enable status*/
   p_lcda_output->f_lcda_enabled = Lcda_Get_Feauture_Status(p_lcda_output, p_lcda_core_output);

   Lcda_Get_Lcda_Honda_Instance()->bsw_alert_left  = p_lcda_output->bsw_alert_left;
   Lcda_Get_Lcda_Honda_Instance()->bsw_alert_right = p_lcda_output->bsw_alert_right;
   Lcda_Get_Lcda_Honda_Instance()->customer_output = p_lcda_output->customer_output;
   for (obj_idx = 0; obj_idx < LCDA_HONDA_NUMBER_OF_OBJECTS; obj_idx++)
   {
      Lcda_Get_Lcda_Honda_Instance()->hold_obj_index[obj_idx]   = p_lcda_output->hold_obj_index[obj_idx];
      Lcda_Get_Lcda_Honda_Instance()->hold_alert_level[obj_idx] = p_lcda_output->hold_alert_level[obj_idx];
      Lcda_Get_Lcda_Honda_Instance()->hold_object[obj_idx]      = p_lcda_output->hold_object[obj_idx];
      Lcda_Get_Lcda_Honda_Instance()->hold_time[obj_idx]        = p_lcda_output->hold_time[obj_idx];
   }

   /* Write bin file output */
#ifdef BINARY_DEBUG
   Write_Lcda_Output(p_lcda_output, p_cals);
#endif
}


/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/


/* coverity[misra_c_2012_rule_15_7_violation][No non-empty terminating "else" statement.] */
static void Lcda_Hold_Honda_Alert_Before_Reset_Bsw(Lcda_Output_T *p_lcda_output,
                                                   const Lcda_Core_Output_T *p_lcda_core_output,
                                                   const Fbk_Vehicle_Data_T vehicle_data,
                                                   const Lcda_Core_Calibration_T *p_cals,
                                                   const Lcda_Customer_Calibration_T *p_customer_cals,
                                                   const Pa_Data_T *p_pa_data)
{
   boolean_T f_slow_down;
   boolean_T f_lost_in_fov;
   boolean_T f_alert_drop;

   /* proceed and set hold time and object only if:
    * current hold time is negative
    * last alert (before reset) is ON
    * new alert (from core) is OFF
    * Object is invalid or vehicle has slowed down
    *
    * Stop alert holding if ego stops completely(speed is "as low as possible") DDG - 3917 */

   if (vehicle_data.host_speed > p_customer_cals->k_honda_ego_speed_stop_holding)
   {
      f_slow_down = (boolean_T) ((p_customer_cals->k_honda_srr6_enable_alert_hold_due_slow_down)
                                 && (vehicle_data.host_speed
                                     < (p_cals->k_lcda_host_activation_speed_min - p_cals->k_lcda_host_activation_speed_min_hys)));

      /* BSW, left side*/
      f_alert_drop  = Lcda_Has_Alert_Drop_Down(p_lcda_output, p_lcda_core_output, HONDA_SIDE_BSW_LEFT);
      f_lost_in_fov = (boolean_T) ((p_customer_cals->k_honda_srr6_enable_alert_hold_due_out_of_fov)
                                   && (PA_OBJ_STATUS_INVALID
                                       == p_pa_data->object_data[p_lcda_output->hold_obj_index[HONDA_SIDE_BSW_LEFT]].status));


      if (f_alert_drop && (f_slow_down || f_lost_in_fov))
      {
         /* set hold time as estimated predicted exit time and use saved object as new LKA object*/
         p_lcda_output->hold_object[HONDA_SIDE_BSW_LEFT]      = p_lcda_output->customer_output.LKA_Object_Left[FBK_ZERO_UINT];
         p_lcda_output->hold_alert_level[HONDA_SIDE_BSW_LEFT] = p_lcda_output->bsw_alert_left;
         /* if the alert is held due to object leaving radar FOV, limit the holding time */
         p_lcda_output->hold_time[HONDA_SIDE_BSW_LEFT] = f_lost_in_fov
                                                            ? Fbk_Min(p_lcda_output->predicted_exit_time[HONDA_SIDE_BSW_LEFT],
                                                                      p_customer_cals->k_honda_max_hold_time_after_out_of_fov)
                                                            : p_lcda_output->predicted_exit_time[HONDA_SIDE_BSW_LEFT];
      }

      /* BSW, right side*/
      f_alert_drop  = Lcda_Has_Alert_Drop_Down(p_lcda_output, p_lcda_core_output, HONDA_SIDE_BSW_RIGHT);
      f_lost_in_fov = (boolean_T) ((p_customer_cals->k_honda_srr6_enable_alert_hold_due_out_of_fov)
                                   && (PA_OBJ_STATUS_INVALID
                                       == p_pa_data->object_data[p_lcda_output->hold_obj_index[HONDA_SIDE_BSW_RIGHT]].status));

      if (f_alert_drop && (f_slow_down || f_lost_in_fov))
      {
         /* set hold time as estimated predicted exit time and use saved object as new LKA object*/
         p_lcda_output->hold_object[HONDA_SIDE_BSW_RIGHT]      = p_lcda_output->customer_output.LKA_Object_Right[FBK_ZERO_UINT];
         p_lcda_output->hold_alert_level[HONDA_SIDE_BSW_RIGHT] = p_lcda_output->bsw_alert_right;
         /* if the alert is held due to object leaving radar FOV, limit the holding time */
         p_lcda_output->hold_time[HONDA_SIDE_BSW_RIGHT] = f_lost_in_fov
                                                             ? Fbk_Min(p_lcda_output->predicted_exit_time[HONDA_SIDE_BSW_RIGHT],
                                                                       p_customer_cals->k_honda_max_hold_time_after_out_of_fov)
                                                             : p_lcda_output->predicted_exit_time[HONDA_SIDE_BSW_RIGHT];
      }
   }
   else
   {
      p_lcda_output->hold_time[HONDA_SIDE_BSW_RIGHT] = -LCDA_LKA_NA_TTC;
      p_lcda_output->hold_time[HONDA_SIDE_BSW_LEFT]  = -LCDA_LKA_NA_TTC;
   }
}

static boolean_T Lcda_Has_Alert_Drop_Down(const Lcda_Output_T *p_lcda_output,
                                          const Lcda_Core_Output_T *p_lcda_core_output,
                                          HONDA_SIDE_T side)
{
   Lcda_Alert_State_T core_alert;
   uint8_t client_alert;

   switch (side)
   {
      case HONDA_SIDE_BSW_LEFT:
         core_alert   = p_lcda_core_output->bsw_core_output.bsw_alert[FBK_SIDE_LEFT];
         client_alert = p_lcda_output->bsw_alert_left;
         break;
      case HONDA_SIDE_BSW_RIGHT:
         core_alert   = p_lcda_core_output->bsw_core_output.bsw_alert[FBK_SIDE_RIGHT];
         client_alert = p_lcda_output->bsw_alert_right;
         break;
      case HONDA_SIDE_CVW_LEFT:
         core_alert   = p_lcda_core_output->cvw_core_output.cvw_alert[FBK_SIDE_LEFT];
         client_alert = p_lcda_output->cvw_alert_left;
         break;
      case HONDA_SIDE_CVW_RIGHT:
         core_alert   = p_lcda_core_output->cvw_core_output.cvw_alert[FBK_SIDE_RIGHT];
         client_alert = p_lcda_output->cvw_alert_right;
         break;
      default:
         core_alert   = LCDA_ALERT_STATE_NONE;
         client_alert = FBK_ZERO_UINT;
         break;
   }

   return (boolean_T) ((client_alert != FBK_ZERO_UINT) && (core_alert == LCDA_ALERT_STATE_NONE)
                       && (p_lcda_output->hold_time[side] < FBK_ZERO_F));
}


static void Lcda_Hold_Honda_Alert_Before_Reset_Cvw(Lcda_Output_T *p_lcda_output,
                                                   const Lcda_Core_Output_T *p_lcda_core_output,
                                                   const Fbk_Vehicle_Data_T vehicle_data,
                                                   const Lcda_Core_Calibration_T *p_cals,
                                                   const Lcda_Customer_Calibration_T *p_customer_cals)
{
   boolean_T f_slow_down = (boolean_T) (vehicle_data.host_speed
                                        < (p_cals->k_lcda_host_activation_speed_min - p_cals->k_lcda_host_activation_speed_min_hys));

   /* proceed and set hold time and object only if:
    * current hold time is negative
    * last alert (before reset) is ON
    * new alert (from core) is OFF
    * vehicle has slowed down
    */

   if ((p_customer_cals->k_honda_srr6_enable_alert_hold_due_slow_down)
       && (vehicle_data.host_speed > p_customer_cals->k_honda_ego_speed_stop_holding))
   {
      /* CVW, left side*/

      if (Lcda_Has_Alert_Drop_Down(p_lcda_output, p_lcda_core_output, HONDA_SIDE_CVW_LEFT) && f_slow_down)
      {
         /* set hold time as estimated predicted exit time and use saved object as new LKA object*/
         p_lcda_output->hold_object[HONDA_SIDE_CVW_LEFT]      = p_lcda_output->customer_output.LKA_Object_Left[FBK_ONE_UINT];
         p_lcda_output->hold_alert_level[HONDA_SIDE_CVW_LEFT] = p_lcda_output->cvw_alert_left;
         p_lcda_output->hold_time[HONDA_SIDE_CVW_LEFT]        = p_lcda_output->predicted_exit_time[HONDA_SIDE_CVW_LEFT];
      }

      /* CVW, right side*/
      if (Lcda_Has_Alert_Drop_Down(p_lcda_output, p_lcda_core_output, HONDA_SIDE_CVW_RIGHT) && f_slow_down)
      {
         /* set hold time as estimated predicted exit time and use saved object as new LKA object*/
         p_lcda_output->hold_object[HONDA_SIDE_CVW_RIGHT]      = p_lcda_output->customer_output.LKA_Object_Right[FBK_ONE_UINT];
         p_lcda_output->hold_alert_level[HONDA_SIDE_CVW_RIGHT] = p_lcda_output->cvw_alert_right;
         p_lcda_output->hold_time[HONDA_SIDE_CVW_RIGHT]        = p_lcda_output->predicted_exit_time[HONDA_SIDE_CVW_RIGHT];
      }
   }
   else
   {
      p_lcda_output->hold_time[HONDA_SIDE_CVW_RIGHT] = -LCDA_LKA_NA_TTC;
      p_lcda_output->hold_time[HONDA_SIDE_CVW_LEFT]  = -LCDA_LKA_NA_TTC;
   }
}

static void Lcda_Set_Hold_Alert_Time_And_Index(const Pa_Data_T *p_pa_data,
                                               const Lcda_Core_Calibration_T *p_cals,
                                               const Fbk_Vehicle_Data_T *p_vehicle_data,
                                               const uint8_t object_index,
                                               Lcda_Output_T *p_lcda_output,
                                               const HONDA_SIDE_T honda_side,
                                               const Lcda_Core_Output_T *p_lcda_core_output)
{
   /* Set the bsw or cvw zone lateral coordinates */
   float32_T cross_point_y_min;
   float32_T cross_point_y_max;
   float32_T cross_point_x;
   if ((HONDA_SIDE_CVW_LEFT == honda_side) || (HONDA_SIDE_CVW_RIGHT == honda_side))
   {
      cross_point_y_min = p_cals->k_cvw_y0 + Fbk_Half(p_vehicle_data->host_width);
      cross_point_y_max = cross_point_y_min + p_cals->k_cvw_y_width0;
      cross_point_x     = p_cals->k_cvw_x0 - p_vehicle_data->host_length;
   }
   else
   {
      cross_point_y_min = p_cals->k_bsw_y0 + Fbk_Half(p_vehicle_data->host_width);
      cross_point_y_max = cross_point_y_min + p_cals->k_bsw_y_width;
      cross_point_x     = p_lcda_core_output->bsw_core_output.bsw_zone[honda_side].points[0].x;
   }
   /* Calculate the predicted exit point from the zone*/
   if (Fbk_Abs_F(p_pa_data->object_data[object_index].curvi_vel_rel.x) > EPSILON)
   {
      float32_T time_cross =
         (cross_point_x - p_pa_data->object_data[object_index].curvi_pos.x) / p_pa_data->object_data[object_index].curvi_vel_rel.x;
      float32_T cross_point_y =
         p_pa_data->object_data[object_index].curvi_pos.y + (p_pa_data->object_data[object_index].curvi_vel_rel.y * time_cross);
      /* if cross point is in the zone keep time and index info */
      if ((Fbk_Abs_F(cross_point_y) > cross_point_y_min) && (Fbk_Abs_F(cross_point_y) < cross_point_y_max))
      {
         p_lcda_output->predicted_exit_time[honda_side] = time_cross;
         p_lcda_output->hold_obj_index[honda_side]      = object_index;
      }
   }
}

static void Lcda_Reset_Output(Lcda_Output_T *p_lcda_output)
{
   uint8_t index;

   /*Reset flags*/
   p_lcda_output->f_lcda_enabled = FBK_ZERO_UINT;
   p_lcda_output->f_bsw_enabled  = FBK_ZERO_UINT;
   p_lcda_output->f_cvw_enabled  = FBK_ZERO_UINT;

   /*Alert level*/
   p_lcda_output->bsw_alert_left  = FBK_ZERO_UINT;
   p_lcda_output->bsw_alert_right = FBK_ZERO_UINT;
   p_lcda_output->cvw_alert_left  = FBK_ZERO_UINT;
   p_lcda_output->cvw_alert_right = FBK_ZERO_UINT;

   /*Additional properties*/
   p_lcda_output->cvw_id_left   = PA_INVALID_OBJ_ID;
   p_lcda_output->cvw_id_right  = PA_INVALID_OBJ_ID;
   p_lcda_output->bsw_id_left   = PA_INVALID_OBJ_ID;
   p_lcda_output->bsw_id_right  = PA_INVALID_OBJ_ID;
   p_lcda_output->cvw_ttc_left  = LCDA_LKA_NA_TTC;
   p_lcda_output->cvw_ttc_right = LCDA_LKA_NA_TTC;

   /*Reset object related attributes*/
   Lcda_Lka_Initialize_Objects(p_lcda_output);

   /* Prevent from negative out of range in hold times*/
   for (index = FBK_ZERO_UINT; index < LCDA_HONDA_NUMBER_OF_OBJECTS; index++)
   {
      p_lcda_output->hold_time[index] = Fbk_Max(p_lcda_output->hold_time[index], -LCDA_LKA_NA_TTC);
   }

   Lcda_Get_Lcda_Honda_Instance()->bsw_alert_left  = FBK_ZERO_UINT;
   Lcda_Get_Lcda_Honda_Instance()->bsw_alert_right = FBK_ZERO_UINT;
}

static HONDA_OBJECT_CLASS_T Lcda_Lka_Get_Object_Class(Pa_Obj_Class_T object_class)
{
   HONDA_OBJECT_CLASS_T rna_object_class;
   switch (object_class)
   {
      case PA_OBJ_CLASS_UNKNOWN:
      {
         rna_object_class = HONDA_OBJECT_CLASS_UNKNOWN;
         break;
      }
      case PA_OBJ_CLASS_CAR:
      {
         rna_object_class = HONDA_OBJECT_CLASS_CAR;
         break;
      }
      case PA_OBJ_CLASS_TRUCK:
      {
         rna_object_class = HONDA_OBJECT_CLASS_TRUCK;
         break;
      }
      case PA_OBJ_CLASS_2WHEEL:
      {
         rna_object_class = HONDA_OBJECT_CLASS_MOTORCYCLE;
         break;
      }
      case PA_OBJ_CLASS_PEDESTRIAN:
      {
         rna_object_class = HONDA_OBJECT_CLASS_PEDESTRIAN;
         break;
      }
      default:
      {
         rna_object_class = HONDA_OBJECT_CLASS_OTHER;
         break;
      }
   }
   return rna_object_class;
}

static HONDA_CHANGE_STATUS_T Lcda_Lka_Get_Change_Status(Pa_Obj_Status_T track_status)
{
   HONDA_CHANGE_STATUS_T rna_change_status;
   if (PA_OBJ_STATUS_NEW == track_status)
   {
      rna_change_status = HONDA_CHANGE_STATUS_CHANGE;
   }
   else
   {
      rna_change_status = HONDA_CHANGE_STATUS_NO_CHANGE;
   }

   return rna_change_status;
}

static HONDA_MOTION_CLASS_T Lcda_Lka_Get_Motion_Class(Pa_Obj_Status_T track_status)
{
   HONDA_MOTION_CLASS_T rna_motion_class;
   if (PA_OBJ_STATUS_COASTED == track_status)
   {
      rna_motion_class = HONDA_MOTION_CLASS_UNKNOWN;
   }
   else
   {
      rna_motion_class = HONDA_MOTION_CLASS_MOVING_OBJECT;
   }
   return rna_motion_class;
}

static HONDA_ALERT_CONDITION_T Lcda_Lka_Get_Alert_Condition(float32_T curvi_long_vel_rel)
{
   HONDA_ALERT_CONDITION_T alert_condition;
   if (curvi_long_vel_rel > FBK_ZERO_F) /* Simple implementation, not sure if it is in line with requirements */
   {
      alert_condition = HONDA_ALERT_CONDITION_TOS; /* target over subject */
   }
   else
   {
      alert_condition = HONDA_ALERT_CONDITION_SOT; /* subject over target */
   }

   return alert_condition;
}


static void Lcda_Lka_Initialize_Objects(Lcda_Output_T *p_lcda_output)
{
   uint8_t index;

   /* Asserts */
   assert(NULL != p_lcda_output);

   for (index = FBK_ZERO_UINT; index < LCDA_HONDA_NUMBER_OF_OBJECTS; index++)
   {
      p_lcda_output->customer_output.LKA_Object_Left[index].lka_alert_condition = LCDA_LKA_NA_ALERT_CONDITION;
      p_lcda_output->customer_output.LKA_Object_Left[index].lka_change_status   = LCDA_LKA_NA_CHANGE_STATUS;
      p_lcda_output->customer_output.LKA_Object_Left[index].lka_curvi_pos_lat   = LCDA_LKA_NA_CURVI_POS_LAT;
      p_lcda_output->customer_output.LKA_Object_Left[index].lka_curvi_pos_long  = LCDA_LKA_NA_CURVI_POS_LONG;
      p_lcda_output->customer_output.LKA_Object_Left[index].lka_curvi_vel_lat   = LCDA_LKA_NA_CURVI_VEL_LAT;
      p_lcda_output->customer_output.LKA_Object_Left[index].lka_curvi_vel_long  = LCDA_LKA_NA_CURVI_VEL_LONG;
      p_lcda_output->customer_output.LKA_Object_Left[index].lka_motion_class    = LCDA_LKA_NA_MOTION_CLASS;
      p_lcda_output->customer_output.LKA_Object_Left[index].lka_object_class    = LCDA_LKA_NA_OBJECT_CLASS;
      p_lcda_output->customer_output.LKA_Object_Left[index].lka_obj_id          = LCDA_LKA_NA_OBJECT_ID;
      p_lcda_output->customer_output.LKA_Object_Left[index].lka_ttc             = LCDA_LKA_NA_TTC;

      p_lcda_output->customer_output.LKA_Object_Right[index].lka_alert_condition = LCDA_LKA_NA_ALERT_CONDITION;
      p_lcda_output->customer_output.LKA_Object_Right[index].lka_change_status   = LCDA_LKA_NA_CHANGE_STATUS;
      p_lcda_output->customer_output.LKA_Object_Right[index].lka_curvi_pos_lat   = LCDA_LKA_NA_CURVI_POS_LAT;
      p_lcda_output->customer_output.LKA_Object_Right[index].lka_curvi_pos_long  = LCDA_LKA_NA_CURVI_POS_LONG;
      p_lcda_output->customer_output.LKA_Object_Right[index].lka_curvi_vel_lat   = LCDA_LKA_NA_CURVI_VEL_LAT;
      p_lcda_output->customer_output.LKA_Object_Right[index].lka_curvi_vel_long  = LCDA_LKA_NA_CURVI_VEL_LONG;
      p_lcda_output->customer_output.LKA_Object_Right[index].lka_motion_class    = LCDA_LKA_NA_MOTION_CLASS;
      p_lcda_output->customer_output.LKA_Object_Right[index].lka_object_class    = LCDA_LKA_NA_OBJECT_CLASS;
      p_lcda_output->customer_output.LKA_Object_Right[index].lka_obj_id          = LCDA_LKA_NA_OBJECT_ID;
      p_lcda_output->customer_output.LKA_Object_Right[index].lka_ttc             = LCDA_LKA_NA_TTC;
   }
}

static uint8_t Lcda_Get_Feauture_Status(const Lcda_Output_T *p_lcda_output, const Lcda_Core_Output_T *p_core_output)
{
   uint8_t lcda_status = FBK_ZERO_UINT;
   /* If alerst is held due to slow down, keep lcda_enable flag on, as requested by client */
   if ((LCDA_STATUS_ACTIVE == p_core_output->lcda_status)
       || ((p_core_output->lcda_status == LCDA_STATUS_DEACTIVATED_LOW_EGO_SPEED)
           && ((p_lcda_output->honda_alert_state[FBK_SIDE_LEFT] != HONDA_ALERT_STATE_OFF)
               || (p_lcda_output->honda_alert_state[FBK_SIDE_RIGHT] != HONDA_ALERT_STATE_OFF))))
   {
      lcda_status = FBK_ONE_UINT;
   }
   return lcda_status;
}

static void Lcda_Set_Honda_Alert_State(Lcda_Output_T *p_lcda_output,
                                       const Lcda_Input_T *p_lcda_input,
                                       const Pa_Data_T *p_pa_data,
                                       const Lcda_Core_Calibration_T *p_cals,
                                       const Lcda_Customer_Calibration_T *p_customer_cals,
                                       const Lcda_Core_Output_T *p_lcda_core_output)
{
   /* Asserts */
   assert(NULL != p_lcda_output);
   assert(NULL != p_lcda_core_output);

   /* Left side */
   if (((uint8_t) LCDA_ALERT_STATE_LEVEL_2 == p_lcda_output->bsw_alert_left)
       || ((uint8_t) LCDA_ALERT_STATE_LEVEL_2 == p_lcda_output->cvw_alert_left))
   {
      p_lcda_output->honda_alert_state[FBK_SIDE_LEFT] = HONDA_ALERT_STATE_LEVEL4;
   }
   else if (((uint8_t) LCDA_ALERT_STATE_LEVEL_1 == p_lcda_output->bsw_alert_left)
            || ((uint8_t) LCDA_ALERT_STATE_LEVEL_1 == p_lcda_output->cvw_alert_left))
   {
      p_lcda_output->honda_alert_state[FBK_SIDE_LEFT] = HONDA_ALERT_STATE_LEVEL1;
   }
   else
   {
      p_lcda_output->honda_alert_state[FBK_SIDE_LEFT] = HONDA_ALERT_STATE_OFF;
   }

   /* Right side */
   if (((uint8_t) LCDA_ALERT_STATE_LEVEL_2 == p_lcda_output->bsw_alert_right)
       || ((uint8_t) LCDA_ALERT_STATE_LEVEL_2 == p_lcda_output->cvw_alert_right))
   {
      p_lcda_output->honda_alert_state[FBK_SIDE_RIGHT] = HONDA_ALERT_STATE_LEVEL4;
   }
   else if (((uint8_t) LCDA_ALERT_STATE_LEVEL_1 == p_lcda_output->bsw_alert_right)
            || ((uint8_t) LCDA_ALERT_STATE_LEVEL_1 == p_lcda_output->cvw_alert_right))
   {
      p_lcda_output->honda_alert_state[FBK_SIDE_RIGHT] = HONDA_ALERT_STATE_LEVEL1;
   }
   else
   {
      p_lcda_output->honda_alert_state[FBK_SIDE_RIGHT] = HONDA_ALERT_STATE_OFF;
   }

   Lcda_Process_Slide_Through_Level_2_Alert(p_lcda_output, p_lcda_input, p_pa_data, p_customer_cals, p_lcda_core_output);

   /* If there is cvw or bsw alert and indicator active on same side verify if beeper honda alert 3 level should be set*/
   if (((uint8_t) LCDA_ALERT_STATE_LEVEL_2 == p_lcda_output->bsw_alert_left)
       || ((uint8_t) LCDA_ALERT_STATE_LEVEL_2 == p_lcda_output->bsw_alert_right)
       || ((uint8_t) LCDA_ALERT_STATE_LEVEL_2 == p_lcda_output->cvw_alert_left)
       || ((uint8_t) LCDA_ALERT_STATE_LEVEL_2 == p_lcda_output->cvw_alert_right))
   {
      Lcda_Process_Beeper_Level_3_Alert(p_lcda_output, p_pa_data, p_cals, p_customer_cals, p_lcda_core_output,
                                        p_lcda_input->f_trailer_present, p_lcda_input->f_beeper_zone);
   }
}

static HONDA_LVL_TWO_OBJECTS_INFO
Lcda_Get_Object_Info(const Lcda_Core_Output_T *p_core_output, uint8_t bsw_index, uint8_t cvw_index, uint8_t side)
{
   HONDA_LVL_TWO_OBJECTS_INFO type;
   /* If object are same and both are in still the bsw zone, set 1*/
   if ((p_core_output->bsw_core_output.bsw_id[side] == p_core_output->cvw_core_output.cvw_id[side])
       && p_core_output->bsw_core_output.f_obj_in_bsw_zone[bsw_index] && p_core_output->bsw_core_output.f_obj_in_bsw_zone[cvw_index])
   {
      type = HONDA_LVL_TWO_SAME_OBJECTS;
   }
   /* If cvw and bsw objects are different and both trigger alert level 1, return 2*/
   else if ((LCDA_ALERT_STATE_LEVEL_1 == p_core_output->bsw_core_output.bsw_alert[side])
            && (LCDA_ALERT_STATE_LEVEL_1 == p_core_output->cvw_core_output.cvw_alert[side])
            && (p_core_output->bsw_core_output.bsw_id[side] != p_core_output->cvw_core_output.cvw_id[side]))
   {
      type = HONDA_LVL_TWO_DIFF_OBJECTS;
   }
   else
   {
      /* return 0*/
      type = HONDA_LVL_TWO_UNKNOWN;
   }
   return type;
}

static void Lcda_Process_Slide_Through_Level_2_Alert(Lcda_Output_T *p_lcda_output,
                                                     const Lcda_Input_T *p_lcda_input,
                                                     const Pa_Data_T *p_pa_data,
                                                     const Lcda_Customer_Calibration_T *p_customer_cals,
                                                     const Lcda_Core_Output_T *p_core_output)
{
   uint8_t bsw_index;
   uint8_t cvw_index;
   uint8_t side;
   boolean_T current_lvl_2_jugde;
   boolean_T f_low_alert_time;
   const LKA_Object_T *lka_bsw;
   const LKA_Object_T *lka_cvw;
   static LKA_Object_T lcda_last_level_2_crit_lka_obj[2][2];
   static boolean_T lcda_last_level_2_alert_was_on[2];
   static float32_T lcda_level_2_alert_duration[2];
   static boolean_T lcda_last_level_2_judge[2];


   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      /* Check if slide through is enabled */
      if (Fbk_Is_True(p_lcda_input->f_slide_through))
      {
         /* At first check if previous alert was on and consider the holding */
         if (lcda_last_level_2_alert_was_on[side])
         {
            /* check current alert duration*/
            f_low_alert_time = (boolean_T) (lcda_level_2_alert_duration[side] < p_customer_cals->k_honda_alert_level_two_holding_time);

            /* take the lka objects from persistent data*/
            lka_bsw = &(lcda_last_level_2_crit_lka_obj[side][FBK_ZERO_UINT]);
            lka_cvw = &(lcda_last_level_2_crit_lka_obj[side][FBK_ONE_UINT]);

            /* Hold alert if its duration is too low */
            if (f_low_alert_time)
            {
               p_lcda_output->honda_alert_state[side] = HONDA_ALERT_STATE_LEVEL2;
               lcda_level_2_alert_duration[side] += p_pa_data->time_diff_to_last_cycle;
               lcda_last_level_2_alert_was_on[side] = FBK_TRUE;
               lcda_last_level_2_judge[side]        = FBK_TRUE;
               /* set output objects using saved persistent data*/
               if (FBK_SIDE_LEFT == side)
               {
                  p_lcda_output->customer_output.LKA_Object_Left[FBK_ZERO_UINT] = (*lka_bsw);
                  p_lcda_output->customer_output.LKA_Object_Left[FBK_ONE_UINT]  = (*lka_cvw);
                  p_lcda_output->bsw_id_left                                    = lka_bsw->lka_obj_id;
                  p_lcda_output->cvw_id_left                                    = lka_cvw->lka_obj_id;
               }
               else
               {
                  p_lcda_output->customer_output.LKA_Object_Right[FBK_ZERO_UINT] = (*lka_bsw);
                  p_lcda_output->customer_output.LKA_Object_Right[FBK_ONE_UINT]  = (*lka_cvw);
                  p_lcda_output->bsw_id_right                                    = lka_bsw->lka_obj_id;
                  p_lcda_output->cvw_id_right                                    = lka_cvw->lka_obj_id;
               }
            }
         }
         /* If no holding is required, process standard conditions for LVL 2 alert: both core aleret should be level 1 and cvw
          * object should be different than bsw*/
         else if (HONDA_LVL_TWO_DIFF_OBJECTS
                  == Lcda_Get_Object_Info(p_core_output, p_core_output->bsw_core_output.bsw_index[side],
                                          p_core_output->cvw_core_output.cvw_index[side], side))
         {
            /* Take the indexes and lka objects */
            bsw_index = p_core_output->bsw_core_output.bsw_index[side];
            cvw_index = p_core_output->cvw_core_output.cvw_index[side];
            lka_bsw   = (FBK_SIDE_LEFT == side) ? (&(p_lcda_output->customer_output.LKA_Object_Left[FBK_ZERO_UINT]))
                                                : (&(p_lcda_output->customer_output.LKA_Object_Right[FBK_ZERO_UINT]));
            lka_cvw   = (FBK_SIDE_LEFT == side) ? (&(p_lcda_output->customer_output.LKA_Object_Left[FBK_ONE_UINT]))
                                                : (&(p_lcda_output->customer_output.LKA_Object_Right[FBK_ONE_UINT]));

            /* Check the judgement for position and speed criteria. */
            current_lvl_2_jugde = Lcda_Should_Alert_Level_Two_Be_Turned_On(p_pa_data, p_customer_cals, cvw_index, bsw_index);

            /* If judgement signal is rising edge, set the alerts and static(quasi - persistent)
                           data */
            if (current_lvl_2_jugde && Fbk_Is_False(lcda_last_level_2_judge[side]))
            {
               p_lcda_output->honda_alert_state[side] = HONDA_ALERT_STATE_LEVEL2;
               lcda_level_2_alert_duration[side] += p_pa_data->time_diff_to_last_cycle;
               lcda_last_level_2_alert_was_on[side]                = FBK_TRUE;
               lcda_last_level_2_crit_lka_obj[side][FBK_ZERO_UINT] = *lka_bsw;
               lcda_last_level_2_crit_lka_obj[side][FBK_ONE_UINT]  = *lka_cvw;
            }
            lcda_last_level_2_judge[side] = current_lvl_2_jugde;
         }
         else
         {
            /* If neither holding or different bsw/cvw objects present, set judge flag to false*/
            lcda_last_level_2_judge[side] = FBK_FALSE;
         }
      }
      else
      {
         lcda_last_level_2_judge[side] = FBK_FALSE;
      }

      /* Set persistent variable indicating alert level*/
      if (HONDA_ALERT_STATE_LEVEL2 != p_lcda_output->honda_alert_state[side])
      {
         lcda_level_2_alert_duration[side]    = FBK_ZERO_F;
         lcda_last_level_2_alert_was_on[side] = FBK_FALSE;
      }
   }
}

static boolean_T Lcda_Should_Alert_Level_Two_Be_Turned_On(const Pa_Data_T *p_pa_data,
                                                          const Lcda_Customer_Calibration_T *p_cals,
                                                          uint8_t cvw_index,
                                                          uint8_t bsw_index)
{
   boolean_T f_is_cvw_object_faster_than_bsw_object;
   boolean_T f_is_cvw_object_in_slide_through_zone;
   boolean_T f_is_cvw_object_front_behind_ego_front;
   boolean_T f_has_cvw_object_overtaken_cvw_object;
   float32_T cvw_hwidth, bsw_hwidth, cvw_hlength, bsw_hlength, bsw_abs_heading;

   const Fbk_Object_Data_T *p_cvw_object = &p_pa_data->object_data[cvw_index];
   const Fbk_Object_Data_T *p_bsw_object = &p_pa_data->object_data[bsw_index];
   /* Get helper variables*/
   cvw_hwidth      = Fbk_Half(p_cvw_object->width);
   bsw_hwidth      = Fbk_Half(p_bsw_object->width);
   cvw_hlength     = Fbk_Half(p_cvw_object->length);
   bsw_hlength     = Fbk_Half(p_bsw_object->length);
   bsw_abs_heading = Fbk_Abs_F(p_bsw_object->curvi_heading);

   /* Check if cvw object front bumper do not cross the ego front bumper. Assuming ego front bumper is at x=0.0*/
   f_is_cvw_object_front_behind_ego_front = (boolean_T) (FBK_ZERO_F > (p_cvw_object->curvi_pos.x + cvw_hlength));
   /* Check if the cvw objects has overtaken bsw object */
   f_has_cvw_object_overtaken_cvw_object =
      (boolean_T) (p_cvw_object->curvi_pos.x > (p_bsw_object->curvi_pos.x + cvw_hlength + bsw_hlength));

   /* Check if CVW object has higher relative velocity over BSW and larger that vel_min (applies when cvw is behind bsw) */
   f_is_cvw_object_faster_than_bsw_object =
      (boolean_T) (p_cvw_object->curvi_vel_rel.x
                   > (p_bsw_object->curvi_vel_rel.x + p_cals->k_honda_min_relative_speed_for_alert_level_two));

   /* Check if CVW object is fully inside the slide through zone - optional setting. */
   if (Fbk_Is_True(p_cals->k_honda_is_slide_through_zone_considered_for_cvw_alert_level_two))
   {
      float32_T inner_zone, outer_zone, inner_obj, outer_obj;
      /* Calculate inner(ego) and outer lateral edges of the slidethrough zone. Take into account the heading of bsw object and
       * margins */
      inner_zone = Fbk_Half(p_pa_data->vehicle_data.host_width) + p_cals->k_honda_ego_lat_overlap_slide_through_zone;
      outer_zone = Fbk_Abs_F(p_bsw_object->curvi_pos.y) - (bsw_hlength * Fast_Sin(bsw_abs_heading))
                   - (bsw_hwidth * Fast_Cos(bsw_abs_heading)) - p_cals->k_honda_object_lat_overlap_slide_through_zone;
      /* Calculate edges of the cvw obj zone.*/
      inner_obj = Fbk_Abs_F(p_cvw_object->curvi_pos.y) - cvw_hwidth;
      outer_obj = inner_obj + (2.0f * cvw_hwidth);

      f_is_cvw_object_in_slide_through_zone = (boolean_T) ((inner_obj > inner_zone) && (outer_obj < outer_zone));
   }
   else
   {
      /* If disabled, check if the BSW object lat *distance is greater than CVW object */
      f_is_cvw_object_in_slide_through_zone =
         (boolean_T) (Fbk_Abs_F(p_bsw_object->curvi_pos.y)
                      > (Fbk_Abs_F(p_cvw_object->curvi_pos.y)
                         + Fbk_Half(p_pa_data->object_data[bsw_index].width + p_pa_data->object_data[cvw_index].width)));
   }

   return (boolean_T) (f_is_cvw_object_front_behind_ego_front
                       && (f_is_cvw_object_faster_than_bsw_object || f_has_cvw_object_overtaken_cvw_object)
                       && f_is_cvw_object_in_slide_through_zone);
}


static void Lcda_Process_Beeper_Level_3_Alert(Lcda_Output_T *p_output,
                                              const Pa_Data_T *p_pa_data,
                                              const Lcda_Core_Calibration_T *p_cals,
                                              const Lcda_Customer_Calibration_T *p_customer_cals,
                                              const Lcda_Core_Output_T *p_core_output,
                                              const boolean_T f_trailer_present,
                                              const boolean_T f_beeper_zone_active)
{
   uint8_t side;
   boolean_T f_beeper_zone_overlap;
   Honda_Alert_State_T updated_status_side;

   boolean_T f_narrow_buzzer_active_speed = FBK_FALSE;
   boolean_T f_buzzer_active_speed        = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_output);
   assert(NULL != p_pa_data);
   assert(NULL != p_cals);
   assert(NULL != p_core_output);


   /* For each side check if most critical object fulfills alert level 3 criteria, if so set honda alert state to level 3 */
   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      updated_status_side = p_output->honda_alert_state[side];
      Lcda_Process_Host_Speed_For_Beeper_Logic(p_pa_data->vehicle_data.host_speed, side, &f_narrow_buzzer_active_speed,
                                               &f_buzzer_active_speed, p_cals, p_customer_cals);

      if (Fbk_Is_True(f_buzzer_active_speed) && (p_output->honda_alert_state[side] > HONDA_ALERT_STATE_LEVEL1))
      {


         updated_status_side                                                   = HONDA_ALERT_STATE_LEVEL3;
         Lcda_Get_Lcda_Honda_Instance()->f_lcda_level_3_alert_prev_cycle[side] = FBK_TRUE;

         /* The only case when alert level 3 should not be set (narrow beeper zone with trailer attached) */
         if (Fbk_Is_True(f_beeper_zone_active) && Fbk_Is_True(f_narrow_buzzer_active_speed) && Fbk_Is_True(f_trailer_present))
         {
            updated_status_side                                                   = p_output->honda_alert_state[side];
            Lcda_Get_Lcda_Honda_Instance()->f_lcda_level_3_alert_prev_cycle[side] = FBK_FALSE;
         }

         /* Case when alert level 3 should be set only when object overlaps narrow beeper zone */
         if (Fbk_Is_True(f_beeper_zone_active) && Fbk_Is_True(f_narrow_buzzer_active_speed) && Fbk_Is_False(f_trailer_present))
         {
            /* Until there is no overlap, keep alert state from lcda output */
            updated_status_side                                                   = p_output->honda_alert_state[side];
            Lcda_Get_Lcda_Honda_Instance()->f_lcda_level_3_alert_prev_cycle[side] = FBK_FALSE;

            f_beeper_zone_overlap =
               Lcda_Is_Obj_In_Narrow_Beeper_Zone(side, p_output, p_pa_data, p_cals, p_customer_cals, p_core_output);

            if (Fbk_Is_True(f_beeper_zone_overlap))
            {
               updated_status_side                                                   = HONDA_ALERT_STATE_LEVEL3;
               Lcda_Get_Lcda_Honda_Instance()->f_lcda_level_3_alert_prev_cycle[side] = FBK_TRUE;
            }
         }
      }

      p_output->honda_alert_state[side] = updated_status_side;

      /* Holding alert when object out of FOV */
      Lcda_Hold_Alert_Obj_Out_Of_Fov(side, p_output, p_pa_data, p_customer_cals);
   }
}


static boolean_T Lcda_Is_Cvw_Core_Alert_On(const Lcda_Core_Output_T *p_core_output, uint8_t side)
{
   boolean_T f_is_core_on;
   Lcda_Alert_State_T core_alert_state;
   uint8_t obj_idx;

   obj_idx          = p_core_output->cvw_core_output.cvw_index[side];
   core_alert_state = p_core_output->cvw_core_output.cvw_alert[side];
   f_is_core_on     = (boolean_T) ((LCDA_ALERT_STATE_NONE != core_alert_state) && (PA_INVALID_OBJ_INDEX != obj_idx));

   return (boolean_T) f_is_core_on;
}


static void Lcda_Set_Lka_Object(LKA_Object_T *p_lka_object,
                                uint8_t obj_tracker_index,
                                const Fbk_Vehicle_Data_T *p_vehicle_data,
                                const Lcda_Core_Output_T *p_core_output,
                                const Pa_Data_T *p_pa_data,
                                HONDA_SIDE_T side)
{
   float32_T lateral_offset;
   float32_T long_offset;
   float32_T lat_vel_sign;
   float32_T long_vel_sign;

   p_lka_object->lka_motion_class    = Lcda_Lka_Get_Motion_Class(p_pa_data->object_data[obj_tracker_index].status);
   p_lka_object->lka_object_class    = Lcda_Lka_Get_Object_Class(p_pa_data->object_data[obj_tracker_index].obj_class);
   p_lka_object->lka_alert_condition = Lcda_Lka_Get_Alert_Condition(p_pa_data->object_data[obj_tracker_index].curvi_vel_rel.x);
   p_lka_object->lka_change_status   = Lcda_Lka_Get_Change_Status(p_pa_data->object_data[obj_tracker_index].status);

   switch (side)
   {
      case HONDA_SIDE_BSW_LEFT:
         lateral_offset           = -Fbk_Half(p_vehicle_data->host_width);
         long_offset              = p_vehicle_data->host_length;
         lat_vel_sign             = -FBK_ONE_F;
         long_vel_sign            = FBK_ONE_F;
         p_lka_object->lka_ttc    = LCDA_LKA_NA_TTC;
         p_lka_object->lka_obj_id = p_pa_data->object_data[obj_tracker_index].id;
         break;
      case HONDA_SIDE_BSW_RIGHT:
         lateral_offset           = Fbk_Half(p_vehicle_data->host_width);
         long_offset              = p_vehicle_data->host_length;
         lat_vel_sign             = FBK_ONE_F;
         long_vel_sign            = -FBK_ONE_F;
         p_lka_object->lka_ttc    = LCDA_LKA_NA_TTC;
         p_lka_object->lka_obj_id = p_pa_data->object_data[obj_tracker_index].id;
         break;
      case HONDA_SIDE_CVW_LEFT:
         lateral_offset           = -Fbk_Half(p_vehicle_data->host_width);
         long_offset              = -p_vehicle_data->host_length;
         lat_vel_sign             = FBK_ONE_F;
         long_vel_sign            = -FBK_ONE_F;
         p_lka_object->lka_ttc    = p_core_output->cvw_core_output.cvw_ttc[FBK_SIDE_LEFT];
         p_lka_object->lka_obj_id = p_pa_data->object_data[obj_tracker_index].id;
         break;
      case HONDA_SIDE_CVW_RIGHT:
         lateral_offset           = Fbk_Half(p_vehicle_data->host_width);
         long_offset              = -p_vehicle_data->host_length;
         lat_vel_sign             = -FBK_ONE_F;
         long_vel_sign            = FBK_ONE_F;
         p_lka_object->lka_ttc    = p_core_output->cvw_core_output.cvw_ttc[FBK_SIDE_RIGHT];
         p_lka_object->lka_obj_id = p_pa_data->object_data[obj_tracker_index].id;
         break;
      default:
         long_offset              = FBK_ZERO_F;
         lateral_offset           = FBK_ZERO_F;
         lat_vel_sign             = FBK_ZERO_F;
         long_vel_sign            = FBK_ZERO_F;
         p_lka_object->lka_obj_id = LCDA_LKA_NA_OBJECT_ID;
         p_lka_object->lka_ttc    = LCDA_LKA_NA_TTC;
         break;
   }

   p_lka_object->lka_curvi_pos_lat  = -(p_pa_data->object_data[obj_tracker_index].curvi_pos.y - lateral_offset);
   p_lka_object->lka_curvi_pos_long = p_pa_data->object_data[obj_tracker_index].curvi_pos.x + long_offset;
   p_lka_object->lka_curvi_vel_lat  = lat_vel_sign * p_pa_data->object_data[obj_tracker_index].curvi_vel.y;
   p_lka_object->lka_curvi_vel_long = long_vel_sign * p_pa_data->object_data[obj_tracker_index].curvi_vel.x;
}

static void Lcda_Process_Host_Speed_For_Beeper_Logic(const float32_T host_speed,
                                                     const uint8_t side,
                                                     boolean_T *p_f_narrow_beeper_active_speed,
                                                     boolean_T *p_f_beeper_active_speed,
                                                     const Lcda_Core_Calibration_T *p_cals,
                                                     const Lcda_Customer_Calibration_T *p_customer_cals)
{
   float32_T beeper_active_speed_h, beeper_active_speed_l;

   /* Check if all input pointers are valid */
   assert(NULL != p_f_narrow_beeper_active_speed);
   assert(NULL != p_f_beeper_active_speed);

   beeper_active_speed_h = p_cals->k_lcda_host_activation_speed_min;
   beeper_active_speed_l = p_cals->k_lcda_host_activation_speed_min - p_cals->k_lcda_host_activation_speed_min_hys;

   /* Set narrow beeper activation flags */
   if (host_speed > p_customer_cals->k_lcda_honda_narrow_beeper_max_speed_h)
   {
      *p_f_narrow_beeper_active_speed = FBK_FALSE;
   }
   else if (host_speed < p_customer_cals->k_lcda_honda_narrow_beeper_max_speed_l)
   {
      *p_f_narrow_beeper_active_speed = FBK_TRUE;
   }
   else if ((host_speed >= p_customer_cals->k_lcda_honda_narrow_beeper_max_speed_l)
            && (host_speed <= p_customer_cals->k_lcda_honda_narrow_beeper_max_speed_h)
            && Fbk_Is_True(Lcda_Get_Lcda_Honda_Instance()->f_narrow_beeper_prev_cycle[side]))
   {
      *p_f_narrow_beeper_active_speed = FBK_TRUE;
   }
   else
   {
      *p_f_narrow_beeper_active_speed = FBK_FALSE;
   }

   /* Set beeper activation flags */
   if (host_speed > beeper_active_speed_h)
   {
      *p_f_beeper_active_speed = FBK_TRUE;
   }
   else if (host_speed < beeper_active_speed_l)
   {
      *p_f_beeper_active_speed = FBK_FALSE;
   }
   else if ((host_speed >= beeper_active_speed_l) && (host_speed <= beeper_active_speed_h)
            && Fbk_Is_True(Lcda_Get_Lcda_Honda_Instance()->f_beeper_prev_cycle[side]))
   {
      *p_f_beeper_active_speed = FBK_TRUE;
   }
   else
   {
      *p_f_beeper_active_speed = FBK_FALSE;
   }

   /* Assign recent parameters to persistent data for another cycle*/
   Lcda_Get_Lcda_Honda_Instance()->f_narrow_beeper_prev_cycle[side] = *p_f_narrow_beeper_active_speed;
   Lcda_Get_Lcda_Honda_Instance()->f_beeper_prev_cycle[side]        = *p_f_beeper_active_speed;
}


static boolean_T Lcda_Is_Obj_In_Narrow_Beeper_Zone(uint8_t side,
                                                   const Lcda_Output_T *p_output,
                                                   const Pa_Data_T *p_pa_data,
                                                   const Lcda_Core_Calibration_T *p_cals,
                                                   const Lcda_Customer_Calibration_T *p_customer_cals,
                                                   const Lcda_Core_Output_T *p_core_output)
{
   float32_T side_sign;
   float32_T host_length;
   float32_T half_host_width;
   uint8_t obj_index;
   Fbk_Field_Of_Interest_T beeper_zone, obj_foi;
   float32_T zone_width, zone_length, hyst_long, hyst_lat;
   Vector_2d_T zone_start, obj_vcs_pos;
   boolean_T f_zone_overlap;

   /* Set zone dimensions */
   zone_length      = p_customer_cals->k_honda_beeper_zone_length;
   zone_width       = p_customer_cals->k_honda_beeper_zone_width;
   hyst_long        = p_customer_cals->k_honda_beeper_zone_long_hys;
   hyst_lat         = p_customer_cals->k_honda_beeper_zone_lat_hys;
   beeper_zone.size = FBK_FOI_SIZE_TETRAGON;
   /* Define basic point of the zone */
   host_length     = p_pa_data->vehicle_data.host_length;
   half_host_width = Fbk_Half((float32_T) p_pa_data->vehicle_data.host_width);
   zone_start.x    = p_cals->k_bsw_x0 - host_length;
   zone_start.y    = half_host_width;

   side_sign = Fbk_Convert_Obj_Side_To_Sign(side);

   if (HONDA_ALERT_STATE_LEVEL3 == p_output->honda_alert_state[side])
   {
      beeper_zone.points[0].x = zone_start.x;
      beeper_zone.points[1].x = zone_start.x;
      beeper_zone.points[2].x = zone_start.x - zone_length - hyst_long;
      beeper_zone.points[3].x = zone_start.x - zone_length - hyst_long;

      beeper_zone.points[0].y = side_sign * zone_start.y;
      beeper_zone.points[1].y = side_sign * (zone_start.y + zone_width + hyst_lat);
      beeper_zone.points[2].y = side_sign * (zone_start.y + zone_width + hyst_lat);
      beeper_zone.points[3].y = side_sign * zone_start.y;
   }
   else
   {
      beeper_zone.points[0].x = zone_start.x;
      beeper_zone.points[1].x = zone_start.x;
      beeper_zone.points[2].x = zone_start.x - zone_length;
      beeper_zone.points[3].x = zone_start.x - zone_length;

      beeper_zone.points[0].y = side_sign * zone_start.y;
      beeper_zone.points[1].y = side_sign * (zone_start.y + zone_width);
      beeper_zone.points[2].y = side_sign * (zone_start.y + zone_width);
      beeper_zone.points[3].y = side_sign * zone_start.y;
   }

   /* Create object FOI */
   obj_index = p_core_output->bsw_core_output.bsw_index[side];
   if (PA_INVALID_OBJ_INDEX != obj_index)
   {

      obj_vcs_pos.x = p_pa_data->object_data[obj_index].vcs_pos.x;
      obj_vcs_pos.y = p_pa_data->object_data[obj_index].vcs_pos.y;
      Fbk_Create_Field_Of_Interest_From_Object_Data(&obj_foi, obj_vcs_pos, p_pa_data->object_data[obj_index].length,
                                                    p_pa_data->object_data[obj_index].width,
                                                    p_pa_data->object_data[obj_index].vcs_heading);
      /* Check if object is in beeper zone */
      f_zone_overlap = Fbk_Are_Fields_Of_Interest_Overlapping(&beeper_zone, &obj_foi);
   }
   else
   {
      f_zone_overlap = FBK_FALSE;
   }

   return f_zone_overlap;
}

static void Lcda_Hold_Alert_Obj_Out_Of_Fov(const uint8_t side,
                                           Lcda_Output_T *p_output,
                                           const Pa_Data_T *p_pa_data,
                                           const Lcda_Customer_Calibration_T *p_customer_cals)
{
   boolean_T f_lost_in_fov;
   boolean_T f_alert_dropped;

   f_lost_in_fov = (boolean_T) ((p_customer_cals->k_honda_srr6_enable_alert_hold_due_out_of_fov)
                                && (p_output->hold_obj_index[side] != (uint32_t) PA_INVALID_OBJ_INDEX)
                                && (PA_OBJ_STATUS_INVALID == p_pa_data->object_data[p_output->hold_obj_index[side]].status));

   f_alert_dropped = (boolean_T) (Lcda_Get_Lcda_Honda_Instance()->f_lcda_level_3_alert_prev_cycle[side]
                                  && (p_output->honda_alert_state[side] != HONDA_ALERT_STATE_LEVEL3));

   if (Fbk_Is_True(f_alert_dropped) && Fbk_Is_True(f_lost_in_fov))
   {
      Lcda_Get_Lcda_Honda_Instance()->f_lcda_level_3_alert_prev_cycle[side] = FBK_FALSE;
      if (p_output->honda_alert_state[side] == HONDA_ALERT_STATE_LEVEL4)
      {
         p_output->honda_alert_state[side]                                     = HONDA_ALERT_STATE_LEVEL3;
         Lcda_Get_Lcda_Honda_Instance()->f_lcda_level_3_alert_prev_cycle[side] = FBK_TRUE;
      }
   }
}

#ifdef BINARY_DEBUG
static void Write_Lcda_Output(const Lcda_Output_T *p_lcda_output, const Lcda_Core_Calibration_T *p_cals)
{
   uint8_t k;

   LCDA_STORE_VAL_MGR_WPR("honda_srr6_f_lcda_enabled", p_lcda_output->f_lcda_enabled);
   LCDA_STORE_VAL_MGR_WPR("honda_srr6_f_bsw_enabled", p_lcda_output->f_bsw_enabled);
   LCDA_STORE_VAL_MGR_WPR("honda_srr6_f_cvw_enabled", p_lcda_output->f_cvw_enabled);

   LCDA_STORE_VAL_MGR_WPR("honda_srr6_bsw_alert_left", p_lcda_output->bsw_alert_left);
   LCDA_STORE_VAL_MGR_WPR("honda_srr6_bsw_id_left", p_lcda_output->bsw_id_left);
   LCDA_STORE_VAL_MGR_WPR("honda_srr6_bsw_alert_right", p_lcda_output->bsw_alert_right);
   LCDA_STORE_VAL_MGR_WPR("honda_srr6_bsw_id_right", p_lcda_output->bsw_id_right);

   LCDA_STORE_VAL_MGR_WPR("honda_srr6_cvw_alert_left", p_lcda_output->cvw_alert_left);
   LCDA_STORE_VAL_MGR_WPR("honda_srr6_cvw_id_left", p_lcda_output->cvw_id_left);
   LCDA_STORE_VAL_MGR_WPR("honda_srr6_cvw_ttc_left", p_lcda_output->cvw_ttc_left);
   LCDA_STORE_VAL_MGR_WPR("honda_srr6_cvw_alert_right", p_lcda_output->cvw_alert_right);
   LCDA_STORE_VAL_MGR_WPR("honda_srr6_cvw_id_right", p_lcda_output->cvw_id_right);
   LCDA_STORE_VAL_MGR_WPR("honda_srr6_cvw_ttc_right", p_lcda_output->cvw_ttc_right);

   for (k = FBK_ZERO_UINT; k < FBK_NUMBER_OF_SIDES; k++)
   {
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("honda_srr6_alert_state", p_lcda_output->honda_alert_state[k], k);
   }

   for (k = FBK_ZERO_UINT; k < LCDA_HONDA_NUMBER_OF_OBJECTS; k++)
   {
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("honda_srr6_lka_alert_condition_left",
                                    p_lcda_output->customer_output.LKA_Object_Left[k].lka_alert_condition, k);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("honda_srr6_lka_change_status_left",
                                    p_lcda_output->customer_output.LKA_Object_Left[k].lka_change_status, k);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("honda_srr6_lka_curvi_pos_lat_left",
                                    p_lcda_output->customer_output.LKA_Object_Left[k].lka_curvi_pos_lat, k);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("honda_srr6_lka_curvi_pos_long_left",
                                    p_lcda_output->customer_output.LKA_Object_Left[k].lka_curvi_pos_long, k);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("honda_srr6_lka_curvi_vel_lat_left",
                                    p_lcda_output->customer_output.LKA_Object_Left[k].lka_curvi_vel_lat, k);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("honda_srr6_lka_curvi_vel_long_left",
                                    p_lcda_output->customer_output.LKA_Object_Left[k].lka_curvi_vel_long, k);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("honda_srr6_lka_motion_class_left",
                                    p_lcda_output->customer_output.LKA_Object_Left[k].lka_motion_class, k);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("honda_srr6_lka_object_class_left",
                                    p_lcda_output->customer_output.LKA_Object_Left[k].lka_object_class, k);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("honda_srr6_lka_obj_id_left", p_lcda_output->customer_output.LKA_Object_Left[k].lka_obj_id, k);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("honda_srr6_lka_ttc_left", p_lcda_output->customer_output.LKA_Object_Left[k].lka_ttc, k);

      LCDA_STORE_ARRAY_ELEM_MGR_WPR("honda_srr6_lka_alert_condition_right",
                                    p_lcda_output->customer_output.LKA_Object_Right[k].lka_alert_condition, k);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("honda_srr6_lka_change_status_right",
                                    p_lcda_output->customer_output.LKA_Object_Right[k].lka_change_status, k);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("honda_srr6_lka_curvi_pos_lat_right",
                                    p_lcda_output->customer_output.LKA_Object_Right[k].lka_curvi_pos_lat, k);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("honda_srr6_lka_curvi_pos_long_right",
                                    p_lcda_output->customer_output.LKA_Object_Right[k].lka_curvi_pos_long, k);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("honda_srr6_lka_curvi_vel_lat_right",
                                    p_lcda_output->customer_output.LKA_Object_Right[k].lka_curvi_vel_lat, k);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("honda_srr6_lka_curvi_vel_long_right",
                                    p_lcda_output->customer_output.LKA_Object_Right[k].lka_curvi_vel_long, k);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("honda_srr6_lka_motion_class_right",
                                    p_lcda_output->customer_output.LKA_Object_Right[k].lka_motion_class, k);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("honda_srr6_lka_object_class_right",
                                    p_lcda_output->customer_output.LKA_Object_Right[k].lka_object_class, k);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("honda_srr6_lka_obj_id_right", p_lcda_output->customer_output.LKA_Object_Right[k].lka_obj_id, k);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("honda_srr6_lka_ttc_right", p_lcda_output->customer_output.LKA_Object_Right[k].lka_ttc, k);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("honda_srr6_hold_time", p_lcda_output->hold_time[k], k);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("honda_srr6_predicted_exit_time", p_lcda_output->predicted_exit_time[k], k);
   }
}
#endif
