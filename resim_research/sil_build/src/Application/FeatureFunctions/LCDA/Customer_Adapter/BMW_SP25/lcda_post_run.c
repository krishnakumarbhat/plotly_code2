/**
 * @file lcda_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the BMW SRR5 post run logic for LCDA.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "lcda_post_run.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "lane_model.h"
#include "lcda_bmw_sp25_debug_interface.h"
#include "lcda_bmw_sp25_debug_writer.h"
#include "lcda_bmw_sp25_types.h"
#include "lcda_core_input_t.h"
#include "lcda_core_output_t.h"
#include "lcda_customer_calibration_t.h"
#include "lcda_state_machine.h"
#include "lcda_types.h"
#include "ml_checked_rounding.h"
#include "ml_math.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include <assert.h>
#include <string.h>

/*===========================================================================*\
* File Scope typedefs
\*===========================================================================*/

/* Defines the LCDA object type for the BMW SRR5 output. */
typedef enum
{
   LCDA_OBJ_TYPE_NONE = (0),
   LCDA_OBJ_TYPE_BSW  = (1),
   LCDA_OBJ_TYPE_CVW  = (2),
   LCDA_OBJ_TYPE_SLC  = (3),
   LCDA_OBJ_TYPE_ELC  = (4)
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Lcda_Obj_Type_T;

/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/

/**
 * @brief Sets BSW related alerts.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2781}
 * @SDD{SF-6957}
 */
static void Lcda_Set_Bsw_Alert(const Lcda_Input_T *p_lcda_input,
                               const Pa_Data_T *p_pa_data,
                               const Lcda_Core_Output_T *p_lcda_core_output,
                               const Lcda_Customer_Calibration_T *p_cals,
                               const uint8_t side,
                               Lcda_Output_T *p_lcda_output,
                               uint8_t bsw_index[FBK_NUMBER_OF_SIDES]);

/**
 * @brief Sets CVW related alerts.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2781}
 * @SDD{SF-6958}
 */
static void Lcda_Set_Cvw_Alert(const Lcda_Core_Output_T *p_lcda_core_output,
                               const uint8_t side,
                               Lcda_Output_T *p_lcda_output,
                               uint8_t cvw_index[FBK_NUMBER_OF_SIDES]);

/**
 * @brief Sets SLC related alerts.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2781}
 * @SDD{SF-6959}
 */
static void Lcda_Set_Slc_Alert(const Lcda_Core_Output_T *p_lcda_core_output, const uint8_t side, Lcda_Output_T *p_lcda_output);

/**
 * @brief Sets AWA related alerts.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2781}
 * @SDD{SF-6960}
 */
static void Lcda_Set_Awa_Alert(const Lcda_Core_Output_T *p_lcda_core_output, const uint8_t side, Lcda_Output_T *p_lcda_output);

/**
 * @brief Fill object information for the left side.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2781}
 * @SDD{SF-6961}
 */
static void Lcda_Fill_Object_Left(const Pa_Data_T *p_pa_data,
                                  const Lcda_Core_Input_T *p_lcda_core_input,
                                  const Lcda_Core_Output_T *p_lcda_core_output,
                                  const Lcda_Customer_Calibration_T *p_cals,
                                  const uint8_t bsw_index[FBK_NUMBER_OF_SIDES],
                                  Lcda_Output_T *p_lcda_output);

/**
 * @brief Fill object information for the right side.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2781}
 * @SDD{SF-6962}
 */
static void Lcda_Fill_Object_Right(const Lcda_Core_Input_T *p_lcda_core_input,
                                   const Fbk_Vehicle_Data_T *p_vehicle_data,
                                   const Lcda_Core_Output_T *p_lcda_core_output,
                                   const Lcda_Customer_Calibration_T *p_cals,
                                   const uint8_t bsw_index[FBK_NUMBER_OF_SIDES],
                                   Lcda_Output_T *p_lcda_output);

/**
 * @brief Fill critical object information on Output Bus Signal for the Right side.
 *
 * @return void
 *
 * @SRD{}
 * @SAD{}
 * @SDD{}
 */
static void Lcda_Fill_Critical_Object_Right_Data_on_Output_Bus_Signal(const Pa_Data_T *p_pa_data,
                                                                      Lcda_Output_T *p_lcda_output,
                                                                      const Lcda_Core_Output_T *p_lcda_core_output);


/**
 * @brief Fill critical object information on Output Bus Signal for the Left side.
 *
 * @return void
 *
 * @SRD{}
 * @SAD{}
 * @SDD{}
 */
static void Lcda_Fill_Critical_Object_Left_Data_on_Output_Bus_Signal(const Pa_Data_T *p_pa_data,
                                                                     Lcda_Output_T *p_lcda_output,
                                                                     const Lcda_Core_Output_T *p_lcda_core_output);

/**
 * @brief Transforms BMW SRR5 object output for left side to the BMW coordinate system.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2781}
 * @SDD{SF-6855}
 * @verification{}
 */
static void Lcda_Transformation_To_Bmw_Coord_System_Left(const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                                         Lcda_Output_T *p_lcda_output /**< Lcda output*/);

/**
 * @brief Transforms BMW SRR5 object output for right side to the BMW coordinate system.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2781}
 * @SDD{SF-6858}
 * @verification{}
 */
static void Lcda_Transformation_To_Bmw_Coord_System_Right(const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                                          Lcda_Output_T *p_lcda_output /**< Lcda output*/);


/**
 * @brief Transforms BMW SRR5 object output bus signals of BSW for left side to the BMW coordinate system.
 *
 * @return void
 *
 * @SRD{}
 * @SAD{}
 * @SDD{}
 * @verification{}
 */
static void Lcda_Transform_Output_Bus_Signals_of_BSW_To_Bmw_Coord_System_Left(
   const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */, Lcda_Output_T *p_lcda_output /**< Lcda output*/);

/**
 * @brief Transforms BMW SRR5 object output bus signals of BSW for right side to the BMW coordinate system.
 *
 * @return void
 *
 * @SRD{}
 * @SAD{}
 * @SDD{}
 * @verification{}
 */
static void Lcda_Transform_Output_Bus_Signals_of_BSW_To_Bmw_Coord_System_Right(
   const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */, Lcda_Output_T *p_lcda_output /**< Lcda output*/);

/**
 * @brief Transforms BMW SRR5 object output bus signals of CVW for left side to the BMW coordinate system.
 *
 * @return void
 *
 * @SRD{}
 * @SAD{}
 * @SDD{}
 * @verification{}
 */
static void Lcda_Transform_Output_Bus_Signals_of_CVW_To_Bmw_Coord_System_Left(
   const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */, Lcda_Output_T *p_lcda_output /**< Lcda output*/);

/**
 * @brief Transforms BMW SRR5 object output bus signals of CVW for right side to the BMW coordinate system.
 *
 * @return void
 *
 * @SRD{}
 * @SAD{}
 * @SDD{}
 * @verification{}
 */
static void Lcda_Transform_Output_Bus_Signals_of_CVW_To_Bmw_Coord_System_Right(
   const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */, Lcda_Output_T *p_lcda_output /**< Lcda output*/);

/**
 * @brief Transforms BMW SRR5 object output bus signals of SLC for left side to the BMW coordinate system.
 *
 * @return void
 *
 * @SRD{}
 * @SAD{}
 * @SDD{}
 * @verification{}
 */
static void Lcda_Transform_Output_Bus_Signals_of_SLC_To_Bmw_Coord_System_Left(
   const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */, Lcda_Output_T *p_lcda_output /**< Lcda output*/);

/**
 * @brief Transforms BMW SRR5 object output bus signals of SLC for right side to the BMW coordinate system.
 *
 * @return void
 *
 * @SRD{}
 * @SAD{}
 * @SDD{}
 * @verification{}
 */
static void Lcda_Transform_Output_Bus_Signals_of_SLC_To_Bmw_Coord_System_Right(
   const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */, Lcda_Output_T *p_lcda_output /**< Lcda output*/);


/**
 * @brief  Wrapper for the setting of existence probability.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2781}
 * @SDD{SF-6857}
 * @verification{Check whether the existence probability is set correctly depending whether lane change intention is given.}
 */
static void Lcda_Set_Existence_Probability(uint16_t *p_exist_prob_to_set /**< property of lcda output to modify*/,
                                           const Lcda_Core_Input_T *p_lcda_core_input /**< Lcda core input*/,
                                           const Lcda_Customer_Calibration_T *p_cals /**< Lcda calibrations*/,
                                           const uint8_t index /**<index to access data on*/);

/**
 * @brief Converts boolean_T flags to uint8_t without MISRA violations
 *
 * @return flag converted as uint8_t
 *
 * @SRS{}
 * @SAE{SF-2781}
 * @SDD{SF-6918}
 * @verification{}
 */
static uint8_t Lcda_Get_Uint8_Flag_From_Boolean(const boolean_T flag /* Boolean flag to convert */);

/**
 * @brief Checks if given object index is most likely a trailer behind the host vehicle.
 *
 * @return true if given object is likely a trailer
 *
 * @SRS{}
 * @SAE{SF-2781}
 * @SDD{SF-6925}
 * @verification{}
 */
static boolean_T Lcda_Is_Object_Trailer(const Lcda_Input_T *p_lcda_input /**< Lcda input */,
                                        const Pa_Data_T *p_pa_data /**< FBK vehicle data */,
                                        const uint8_t object_index /**< object index */,
                                        const Lcda_Customer_Calibration_T *p_cals /**< Lcda calibrations*/);

/**
 * @brief Restricts the CVW alert for objects further away than the specified zone length limit for China.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2781}
 * @SDD{SF-6926}
 * @verification{}
 */
static void
Lcda_Limit_Cvw_Alert_By_Zone_Length(Lcda_Output_T *p_lcda_output /**< Lcda output */,
                                    const Lcda_Input_T *p_lcda_input /**< Lcda input */,
                                    const Pa_Data_T *p_pa_data /**< FBK vehicle data */,
                                    const uint8_t cvw_index[FBK_NUMBER_OF_SIDES] /**< CVW index array */,
                                    const boolean_T f_previous_cvw_alert[FBK_NUMBER_OF_SIDES] /**< Previous CVW alert flag array */,
                                    const Lcda_Customer_Calibration_T *p_cals /**< Lcda calibration */);

/**
 * @brief Resets the BMW_SP25 specific LCDA output.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2781}
 * @SDD{SF-6986}
 * @verification{}
 */
static void Lcda_Reset_Output(Lcda_Output_T *p_lcda_output);

/**
 * @brief Resets the BMW_SP25 specific LCDA Output Bus Signals.
 *
 * @return void
 *
 * @SRD{}
 * @SAD{WI-17531}
 * @SDD{WI-27970}
 * @verification{}
 */
static void Lcda_Reset_Bmw_LCDA_Output_Bus_Signals(Lcda_Output_T *p_lcda_output);

/**
 * @brief Resets the BMW_SP25 specific LCDA Output Bus Signals Event Data-Qualifier , Extended-Data-Qualifier and FF subfunctions
 * Status.
 *
 * @return void
 *
 * @SRD{}
 * @SAD{WI-17531}
 * @SDD{WI-27970}
 * @verification{}
 */
static void Lcda_Reset_Bmw_LCDA_Output_Bus_Signals_Qualifiers_And_FF_Status(Lcda_Output_T *p_lcda_output);


/**
 * @brief Set the BMW_SP25 specific LCDA Output Bus Signals When Not in Active State.
 *
 * @return void
 *
 * @SRD{}
 * @SAD{WI-17531}
 * @SDD{WI-27970}
 * @verification{}
 */
static void Lcda_Set_Bmw_Output_Bus_Signals_When_Not_In_Active_State(Lcda_Output_T *p_lcda_output);


/**
 * @brief Set the BMW_SP25 specific LCDA Output Bus Signals When in Active State.
 *
 * @return void
 *
 * @SRD{}
 * @SAD{WI-17531}
 * @SDD{WI-27970}
 * @verification{}
 */
static void Lcda_Set_Bmw_Output_Bus_Signals_When_In_Active_State(const Pa_Data_T *p_pa_data,
                                                                 Lcda_Output_T *p_lcda_output,
                                                                 const Lcda_Core_Output_T *p_lcda_core_output);

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_5_violation][Symbol "Lcda_Get_Lane_Model_Output" is declared more than once.] */
extern const Lane_Model_Output_T *Lcda_Get_Lane_Model_Output(void); // from lcda_pre_run.c

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Lcda_Init_Output(Lcda_Output_T *p_lcda_output)
{
   /* coverity[misra_c_2012_rule_17_7_violation][Intentionally ignored return value of memset function since it is not required.] */
   memset(p_lcda_output, 0, sizeof(Lcda_Output_T));
}

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_lcda_instance" points to a non-constant type.] */
void Lcda_Post_Run_Init(void)
{
}

/* clang-format off */
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_lcda_instance" points to a non-constant type.] */
void Lcda_Post_Run(Lcda_Instance_T *p_lcda_instance, const Lcda_Input_T *p_lcda_input, Lcda_Output_T *p_lcda_output, const Fbk_Output_T *p_fbk_output)
/* clang-format on */
{
   /*Get the Pre Run State Machine Outputs*/
   const LCDA_FF_State_T *p_lcda_states;
   const Pa_Data_T *p_pa_data;
   const Fbk_Vehicle_Data_T *p_vehicle_data;
   uint8_t side;
   const Lane_Model_Output_T *p_lane_model;
   const Lcda_Core_Input_T *p_lcda_core_input;
   const Lcda_Core_Output_T *p_lcda_core_output;
   const Lcda_Customer_Calibration_T *p_custom_cals;

   boolean_T f_previous_cvw_alert[FBK_NUMBER_OF_SIDES] = {FBK_FALSE, FBK_FALSE};
   uint8_t bsw_index[FBK_NUMBER_OF_SIDES]              = {PA_INVALID_OBJ_INDEX, PA_INVALID_OBJ_INDEX};
   uint8_t cvw_index[FBK_NUMBER_OF_SIDES]              = {PA_INVALID_OBJ_INDEX, PA_INVALID_OBJ_INDEX};

   /* Asserts */
   assert(NULL != p_lcda_instance);
   assert(NULL != p_lcda_input);
   assert(NULL != p_lcda_output);
   assert(NULL != p_fbk_output);

   Lcda_Reset_Output(p_lcda_output);
   Lcda_Reset_Bmw_LCDA_Output_Bus_Signals_Qualifiers_And_FF_Status(p_lcda_output);
   Lcda_Reset_Bmw_LCDA_Output_Bus_Signals(p_lcda_output);

   p_lcda_states      = Lcda_Get_State_Output_Ptr();
   p_lane_model       = Lcda_Get_Lane_Model_Output();
   p_lcda_core_input  = &p_lcda_instance->core_input;
   p_lcda_core_output = &p_lcda_instance->core_output;
   p_custom_cals      = &p_lcda_instance->customer_calibration;

   p_lcda_output->lane_width         = p_lane_model->lane_width;
   p_lcda_output->lane_center_offset = p_lane_model->lane_center_offset;

   /* Set the Main Ced Qualifier (This normally should be set by SM-Core-Output) */
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_qualifier_lcda_function_state = *p_lcda_states;

   /* Fill vehicle data */
   p_pa_data      = p_lcda_core_input->p_pa_data;
   p_vehicle_data = &p_pa_data->vehicle_data;

   /*Reset Lcda output to default*/
   Lcda_Reset_Output(p_lcda_output);

   /*Reset Lcda output Bus Signals to default*/
   Lcda_Reset_Bmw_LCDA_Output_Bus_Signals(p_lcda_output);

   /* Check general LCDA state */
   if (LCDA_STATUS_ACTIVE == p_lcda_core_output->lcda_status)
   {
      p_lcda_output->f_lcda_enabled = FBK_ONE_UINT;
   }

   /* Check enable flags */
   p_lcda_output->f_bsw_enabled = Lcda_Get_Uint8_Flag_From_Boolean(p_lcda_core_output->bsw_core_output.f_bsw_is_enabled);
   p_lcda_output->f_cvw_enabled = Lcda_Get_Uint8_Flag_From_Boolean(p_lcda_core_output->cvw_core_output.f_cvw_is_enabled);
   p_lcda_output->f_slc_enabled = Lcda_Get_Uint8_Flag_From_Boolean(p_lcda_core_output->slc_core_output.f_slc_is_enabled);
   p_lcda_output->f_awa_enabled = Lcda_Get_Uint8_Flag_From_Boolean(p_lcda_core_output->elc_core_output.f_elc_is_enabled);

   /* Set outputs for each side */
   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      /* bsw alert */
      Lcda_Set_Bsw_Alert(p_lcda_input, p_pa_data, p_lcda_core_output, p_custom_cals, side, p_lcda_output, bsw_index);

      /* Set flag if CVW alert was active previously */
      f_previous_cvw_alert[side] = (boolean_T) (Fbk_Is_True(FBK_ZERO_UINT != p_lcda_output->cvw_alert[side]));

      /* cvw alert */
      Lcda_Set_Cvw_Alert(p_lcda_core_output, side, p_lcda_output, cvw_index);

      /* slc alert */
      Lcda_Set_Slc_Alert(p_lcda_core_output, side, p_lcda_output);

      /* awa alert */
      Lcda_Set_Awa_Alert(p_lcda_core_output, side, p_lcda_output);
   }

   if (Fbk_Is_True(p_lcda_input->f_lcda_enable_cvw_limit_zone))
   {
      /* Check for CVW zone limit */
      Lcda_Limit_Cvw_Alert_By_Zone_Length(p_lcda_output, p_lcda_input, p_pa_data, cvw_index, f_previous_cvw_alert, p_custom_cals);
   }

   /* fill lcda object interface for left side */
   Lcda_Fill_Object_Left(p_pa_data, p_lcda_core_input, p_lcda_core_output, p_custom_cals, bsw_index, p_lcda_output);

   /* fill lcda object interface for right side */
   Lcda_Fill_Object_Right(p_lcda_core_input, p_vehicle_data, p_lcda_core_output, p_custom_cals, bsw_index, p_lcda_output);


   if (LCDA_STATE_ACTIVE != p_lcda_output->bmw_lcda_output_bus_signals.bmw_qualifier_lcda_function_state)
   {
      Lcda_Set_Bmw_Output_Bus_Signals_When_Not_In_Active_State(p_lcda_output);
   }
   else
   {
      Lcda_Set_Bmw_Output_Bus_Signals_When_In_Active_State(p_pa_data, p_lcda_output, p_lcda_core_output);
   }

   /* Write bin file output */
   Binary_Lcda_Bmw_Sp25_Fill_Debug_Data(p_lcda_input, p_lcda_output);
   Binary_Lcda_Bmw_Sp25_Write_Bin_File();
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static void Lcda_Reset_Output(Lcda_Output_T *p_lcda_output)
{
   uint8_t idx;

   /*Reset flags*/
   p_lcda_output->f_lcda_enabled = FBK_ZERO_UINT;
   p_lcda_output->f_bsw_enabled  = FBK_ZERO_UINT;
   p_lcda_output->f_cvw_enabled  = FBK_ZERO_UINT;
   p_lcda_output->f_slc_enabled  = FBK_ZERO_UINT;
   p_lcda_output->f_awa_enabled  = FBK_ZERO_UINT;

   /* Reset output for orcas */
   for (idx = FBK_ZERO_UINT; idx < FBK_NUMBER_OF_SIDES; idx++)
   {
      p_lcda_output->bsw_alert[idx]                   = FBK_ZERO_UINT;
      p_lcda_output->bsw_id[idx]                      = PA_INVALID_OBJ_ID;
      p_lcda_output->bsw_unique_id[idx]               = PA_INVALID_OBJ_ID;
      p_lcda_output->bsw_ttp[idx]                     = LCDA_DEFAULT_LARGE_TTP;
      p_lcda_output->bsw_ttle[idx]                    = LCDA_DEFAULT_LARGE_TTLE;
      p_lcda_output->cvw_alert[idx]                   = FBK_ZERO_UINT;
      p_lcda_output->cvw_id[idx]                      = PA_INVALID_OBJ_ID;
      p_lcda_output->cvw_unique_id[idx]               = PA_INVALID_OBJ_ID;
      p_lcda_output->cvw_ttc[idx]                     = LCDA_CVW_DEFAULT_NO_ALERT_TTC;
      p_lcda_output->cvw_ttp[idx]                     = LCDA_DEFAULT_LARGE_TTP;
      p_lcda_output->cvw_ttle[idx]                    = LCDA_DEFAULT_LARGE_TTLE;
      p_lcda_output->slc_alert[idx]                   = FBK_ZERO_UINT;
      p_lcda_output->slc_id[idx]                      = PA_INVALID_OBJ_ID;
      p_lcda_output->slc_unique_id[idx]               = PA_INVALID_OBJ_ID;
      p_lcda_output->slc_ttc[idx]                     = LCDA_DEFAULT_LARGE_TTC;
      p_lcda_output->slc_ttp[idx]                     = LCDA_DEFAULT_LARGE_TTP;
      p_lcda_output->slc_lane_change_probability[idx] = LCDA_SLC_PROBABILITY_NONE;
      p_lcda_output->awa_alert[idx]                   = FBK_ZERO_UINT;
      p_lcda_output->awa_id[idx]                      = PA_INVALID_OBJ_ID;
      p_lcda_output->awa_ttc[idx]                     = LCDA_DEFAULT_LARGE_TTC;
      p_lcda_output->awa_dec[idx]                     = FBK_ZERO_F;
   }

   /* Reset Lane model output*/
   p_lcda_output->lane_width                         = FBK_ZERO_F;
   p_lcda_output->lane_center_offset                 = FBK_ZERO_F;
   p_lcda_output->lane_lateral_speed[FBK_SIDE_LEFT]  = FBK_ZERO_F;
   p_lcda_output->lane_lateral_speed[FBK_SIDE_RIGHT] = FBK_ZERO_F;

   /* Reset object related attributes*/
   p_lcda_output->lcda_object_type_left                    = (uint8_t) LCDA_OBJ_TYPE_NONE;
   p_lcda_output->lcda_object_id_left                      = FBK_ZERO_UINT;
   p_lcda_output->lcda_object_unique_id_left               = FBK_ZERO_UINT;
   p_lcda_output->lcda_object_px_left                      = FBK_ZERO_F;
   p_lcda_output->lcda_object_py_left                      = FBK_ZERO_F;
   p_lcda_output->lcda_object_ttc_left                     = LCDA_DEFAULT_LARGE_TTC;
   p_lcda_output->lcda_object_vx_left                      = FBK_ZERO_F;
   p_lcda_output->lcda_object_vy_left                      = FBK_ZERO_F;
   p_lcda_output->lcda_object_existance_probability_left   = FBK_ZERO_UINT;
   p_lcda_output->lcda_object_lane_change_probability_left = FBK_ZERO_UINT;

   p_lcda_output->lcda_object_type_right                    = (uint8_t) LCDA_OBJ_TYPE_NONE;
   p_lcda_output->lcda_object_id_right                      = FBK_ZERO_UINT;
   p_lcda_output->lcda_object_unique_id_right               = FBK_ZERO_UINT;
   p_lcda_output->lcda_object_px_right                      = FBK_ZERO_F;
   p_lcda_output->lcda_object_py_right                      = FBK_ZERO_F;
   p_lcda_output->lcda_object_ttc_right                     = LCDA_DEFAULT_LARGE_TTC;
   p_lcda_output->lcda_object_vx_right                      = FBK_ZERO_F;
   p_lcda_output->lcda_object_vy_right                      = FBK_ZERO_F;
   p_lcda_output->lcda_object_existance_probability_right   = FBK_ZERO_UINT;
   p_lcda_output->lcda_object_lane_change_probability_right = FBK_ZERO_UINT;
}


static void Lcda_Reset_Bmw_LCDA_Output_Bus_Signals_Qualifiers_And_FF_Status(Lcda_Output_T *p_lcda_output)
{
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_event_data_qualifier = EVENT_DATA_NOT_AVAILABLE;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_extended_qualifier   = EXTENDED_QUALIFIER_EVENT_DATA_INVALID_OR_TIMEOUT;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_status_bsw    = BSW_FUNCTION_INTERFACE_IS_NOT_AVAILABLE;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_status_cvw    = CVW_FUNCTION_INTERFACE_IS_NOT_AVAILABLE;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_status_slc    = SLC_FUNCTION_INTERFACE_IS_NOT_AVAILABLE;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_heartbeat            = FBK_ZERO_UINT;
}

static void Lcda_Reset_Bmw_LCDA_Output_Bus_Signals(Lcda_Output_T *p_lcda_output)
{
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.hour          = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.minute        = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.second        = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_id_left                      = FBK_ZERO_UINT;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_left              = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_left              = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_width_left                   = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_length_left                  = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_left                     = LCDA_DEFAULT_LARGE_TTC;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_left                     = LCDA_DEFAULT_LARGE_TTP;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_left                    = LCDA_DEFAULT_LARGE_TTLE;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_left              = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_left              = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_left   = FBK_ZERO_UINT;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_left = FBK_ZERO_UINT;

   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.hour          = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.minute        = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.second        = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_id_right                      = FBK_ZERO_UINT;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_right              = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_right              = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_width_right                   = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_length_right                  = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_right                     = LCDA_DEFAULT_LARGE_TTC;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_right                     = LCDA_DEFAULT_LARGE_TTP;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_right                    = LCDA_DEFAULT_LARGE_TTLE;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_right              = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_right              = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_right   = FBK_ZERO_UINT;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_right = FBK_ZERO_UINT;

   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left_cvw.hour          = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left_cvw.minute        = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left_cvw.second        = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_id_left_cvw                      = FBK_ZERO_UINT;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_left_cvw              = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_left_cvw              = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_width_left_cvw                   = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_length_left_cvw                  = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_left_cvw                     = LCDA_DEFAULT_LARGE_TTC;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_left_cvw                     = LCDA_DEFAULT_LARGE_TTP;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_left_cvw                    = LCDA_DEFAULT_LARGE_TTLE;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_left_cvw              = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_left_cvw              = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_left_cvw   = FBK_ZERO_UINT;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_left_cvw = FBK_ZERO_UINT;

   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right_cvw.hour          = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right_cvw.minute        = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right_cvw.second        = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_id_right_cvw                      = FBK_ZERO_UINT;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_right_cvw              = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_right_cvw              = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_width_right_cvw                   = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_length_right_cvw                  = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_right_cvw                     = LCDA_DEFAULT_LARGE_TTC;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_right_cvw                     = LCDA_DEFAULT_LARGE_TTP;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_right_cvw                    = LCDA_DEFAULT_LARGE_TTLE;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_right_cvw              = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_right_cvw              = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_right_cvw   = FBK_ZERO_UINT;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_right_cvw = FBK_ZERO_UINT;

   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left_slc.hour          = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left_slc.minute        = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left_slc.second        = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_id_left_slc                      = FBK_ZERO_UINT;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_left_slc              = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_left_slc              = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_width_left_slc                   = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_length_left_slc                  = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_left_slc                     = LCDA_DEFAULT_LARGE_TTC;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_left_slc                     = LCDA_DEFAULT_LARGE_TTP;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_left_slc              = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_left_slc              = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_left_slc   = FBK_ZERO_UINT;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_left_slc = FBK_ZERO_UINT;

   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right_slc.hour          = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right_slc.minute        = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right_slc.second        = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_id_right_slc                      = FBK_ZERO_UINT;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_right_slc              = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_right_slc              = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_width_right_slc                   = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_length_right_slc                  = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_right_slc                     = LCDA_DEFAULT_LARGE_TTC;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_right_slc                     = LCDA_DEFAULT_LARGE_TTP;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_right_slc              = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_right_slc              = FBK_ZERO_F;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_right_slc   = FBK_ZERO_UINT;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_right_slc = FBK_ZERO_UINT;
}

static void Lcda_Set_Bmw_Output_Bus_Signals_When_Not_In_Active_State(Lcda_Output_T *p_lcda_output)
{
   if (LCDA_STATE_NOT_AVAILABLE == p_lcda_output->bmw_lcda_output_bus_signals.bmw_qualifier_lcda_function_state)
   {
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_event_data_qualifier = EVENT_DATA_NOT_AVAILABLE;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_extended_qualifier   = EXTENDED_QUALIFIER_EVENT_DATA_INVALID_OR_TIMEOUT;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_status_bsw    = BSW_FUNCTION_INTERFACE_IS_NOT_AVAILABLE;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_status_cvw    = CVW_FUNCTION_INTERFACE_IS_NOT_AVAILABLE;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_status_slc    = SLC_FUNCTION_INTERFACE_IS_NOT_AVAILABLE;
   }
   else if (LCDA_STATE_INACTIVE == p_lcda_output->bmw_lcda_output_bus_signals.bmw_qualifier_lcda_function_state)
   {
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_event_data_qualifier = EVENT_DATA_NOT_AVAILABLE;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_extended_qualifier   = EXTENDED_QUALIFIER_EVENT_DATA_INVALID_OR_TIMEOUT;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_status_bsw    = BSW_ZONE_OUTSIDE_SYSTEM_BOUNDARIES;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_status_cvw    = CVW_ZONE_OUTSIDE_SYSTEM_BOUNDARIES;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_status_slc    = SLC_ZONE_OUTSIDE_SYSTEM_BOUNDARIES;

      Lcda_Reset_Bmw_LCDA_Output_Bus_Signals(p_lcda_output);
   }
   else if (LCDA_STATE_ERROR == p_lcda_output->bmw_lcda_output_bus_signals.bmw_qualifier_lcda_function_state)
   {
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_event_data_qualifier = EVENT_DATA_NOT_AVAILABLE;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_extended_qualifier   = EXTENDED_QUALIFIER_EVENT_DATA_INVALID_OR_TIMEOUT;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_status_bsw    = BSW_FUNCTION_REPORTS_ERROR;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_status_cvw    = CVW_FUNCTION_REPORTS_ERROR;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_status_slc    = SLC_FUNCTION_REPORTS_ERROR;
   }
   else
   {
      /* do nothing */
   }
}

static void Lcda_Set_Bmw_Output_Bus_Signals_When_In_Active_State(const Pa_Data_T *p_pa_data,
                                                                 Lcda_Output_T *p_lcda_output,
                                                                 const Lcda_Core_Output_T *p_lcda_core_output)
{
   /*By default we consider/initialise sub-function (BSW,CVW,SLC) status qualifiers with NO_VEHICLE_IN_THE_RIGHT_AND_LEFT_BSW_ZONE
    *If we get alert and a critical object get detected then these qualifier values would be overwriten according
    *in subsequent function calls
    */
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_status_bsw = NO_VEHICLE_IN_THE_RIGHT_AND_LEFT_BSW_ZONE;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_status_cvw = NO_VEHICLE_IN_THE_RIGHT_AND_LEFT_CVW_ZONE;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_status_slc = NO_VEHICLE_IN_THE_RIGHT_AND_LEFT_SLC_ZONE;
   Lcda_Fill_Critical_Object_Right_Data_on_Output_Bus_Signal(p_pa_data, p_lcda_output, p_lcda_core_output);
   Lcda_Fill_Critical_Object_Left_Data_on_Output_Bus_Signal(p_pa_data, p_lcda_output, p_lcda_core_output);
}

static void Lcda_Set_Bsw_Alert(const Lcda_Input_T *p_lcda_input,
                               const Pa_Data_T *p_pa_data,
                               const Lcda_Core_Output_T *p_lcda_core_output,
                               const Lcda_Customer_Calibration_T *p_cals,
                               const uint8_t side,
                               Lcda_Output_T *p_lcda_output,
                               uint8_t bsw_index[FBK_NUMBER_OF_SIDES])
{
   if (LCDA_ALERT_STATE_NONE != p_lcda_core_output->bsw_core_output.bsw_alert[side])
   {
      boolean_T f_is_object_trailer =
         Lcda_Is_Object_Trailer(p_lcda_input, p_pa_data, p_lcda_core_output->bsw_core_output.bsw_index[side], p_cals);

      if (Fbk_Is_False(f_is_object_trailer))
      {
         p_lcda_output->bsw_alert[side]     = (uint8_t) p_lcda_core_output->bsw_core_output.bsw_alert[side];
         p_lcda_output->bsw_id[side]        = p_lcda_core_output->bsw_core_output.bsw_id[side];
         p_lcda_output->bsw_unique_id[side] = p_lcda_core_output->bsw_core_output.bsw_unique_id[side];
         p_lcda_output->bsw_ttp[side]       = p_lcda_core_output->bsw_core_output.bsw_ttp[side];
         p_lcda_output->bsw_ttle[side]      = p_lcda_core_output->bsw_core_output.bsw_ttle[side];
         bsw_index[side]                    = p_lcda_core_output->bsw_core_output.bsw_index[side];
      }
   }
}

static void Lcda_Set_Cvw_Alert(const Lcda_Core_Output_T *p_lcda_core_output,
                               const uint8_t side,
                               Lcda_Output_T *p_lcda_output,
                               uint8_t cvw_index[FBK_NUMBER_OF_SIDES])
{
   if (LCDA_ALERT_STATE_NONE != p_lcda_core_output->cvw_core_output.cvw_alert[side])
   {
      p_lcda_output->cvw_alert[side]     = (uint8_t) p_lcda_core_output->cvw_core_output.cvw_alert[side];
      p_lcda_output->cvw_id[side]        = p_lcda_core_output->cvw_core_output.cvw_id[side];
      p_lcda_output->cvw_unique_id[side] = p_lcda_core_output->cvw_core_output.cvw_unique_id[side];
      p_lcda_output->cvw_ttp[side]       = p_lcda_core_output->cvw_core_output.cvw_ttp[side];
      p_lcda_output->cvw_ttle[side]      = p_lcda_core_output->cvw_core_output.cvw_ttle[side];
      cvw_index[side]                    = p_lcda_core_output->cvw_core_output.cvw_index[side];
   }

   p_lcda_output->cvw_ttc[side] = p_lcda_core_output->cvw_core_output.cvw_ttc[side];
}

static void Lcda_Set_Slc_Alert(const Lcda_Core_Output_T *p_lcda_core_output, const uint8_t side, Lcda_Output_T *p_lcda_output)
{
   if (Fbk_Is_True(p_lcda_core_output->slc_core_output.slc_alert[side]))
   {
      p_lcda_output->slc_alert[side]     = FBK_ONE_UINT;
      p_lcda_output->slc_id[side]        = p_lcda_core_output->slc_core_output.slc_id[side];
      p_lcda_output->slc_unique_id[side] = p_lcda_core_output->slc_core_output.slc_unique_id[side];
   }

   p_lcda_output->slc_ttc[side]                     = p_lcda_core_output->slc_core_output.slc_lat_ttc[side];
   p_lcda_output->slc_ttp[side]                     = p_lcda_core_output->slc_core_output.slc_ttp[side];
   p_lcda_output->slc_lane_change_probability[side] = p_lcda_core_output->slc_core_output.slc_lane_change_prob[side];
}

static void Lcda_Set_Awa_Alert(const Lcda_Core_Output_T *p_lcda_core_output, const uint8_t side, Lcda_Output_T *p_lcda_output)
{
   if (Fbk_Is_True(p_lcda_core_output->elc_core_output.elc_alert[side]))
   {
      p_lcda_output->awa_alert[side] = FBK_ONE_UINT;
      p_lcda_output->awa_id[side]    = p_lcda_core_output->elc_core_output.elc_id[side];
   }

   p_lcda_output->awa_ttc[side] = p_lcda_core_output->elc_core_output.elc_ttc[side];
   p_lcda_output->awa_dec[side] = p_lcda_core_output->elc_core_output.elc_decel_to_reach_host_speed[side];
}

static void Lcda_Fill_Object_Left(const Pa_Data_T *p_pa_data,
                                  const Lcda_Core_Input_T *p_lcda_core_input,
                                  const Lcda_Core_Output_T *p_lcda_core_output,
                                  const Lcda_Customer_Calibration_T *p_cals,
                                  const uint8_t bsw_index[FBK_NUMBER_OF_SIDES],
                                  Lcda_Output_T *p_lcda_output)
{
   const float32_T percentage_factor        = 100.0f;
   const Fbk_Vehicle_Data_T *p_vehicle_data = &p_pa_data->vehicle_data;

   if (FBK_ZERO_UINT != p_lcda_output->bsw_alert[FBK_SIDE_LEFT])
   {
      uint8_t index                             = bsw_index[FBK_SIDE_LEFT];
      const Fbk_Object_Data_T *p_object_data    = &p_pa_data->object_data[index];
      p_lcda_output->lcda_object_type_left      = (uint8_t) LCDA_OBJ_TYPE_BSW;
      p_lcda_output->lcda_object_id_left        = p_object_data->id;
      p_lcda_output->lcda_object_unique_id_left = p_lcda_core_output->bsw_core_output.bsw_unique_id[FBK_SIDE_LEFT];
      p_lcda_output->lcda_object_width_left     = p_object_data->width;
      p_lcda_output->lcda_object_length_left    = p_object_data->length;
      p_lcda_output->lcda_object_px_left        = p_object_data->vcs_pos.x;
      p_lcda_output->lcda_object_py_left        = p_object_data->vcs_pos.y;
      p_lcda_output->lcda_object_ttc_left       = FBK_ZERO_F;
      p_lcda_output->lcda_object_vx_left        = p_object_data->vcs_vel.x;
      p_lcda_output->lcda_object_vy_left        = p_object_data->vcs_vel.y;
      Lcda_Set_Existence_Probability(&(p_lcda_output->lcda_object_existance_probability_left), p_lcda_core_input, p_cals, index);
      p_lcda_output->lcda_object_lane_change_probability_left = FBK_ZERO_UINT;
      Lcda_Transformation_To_Bmw_Coord_System_Left(p_vehicle_data, p_lcda_output);
   } /* end if bsw alert left */
   if (FBK_ZERO_UINT != p_lcda_output->cvw_alert[FBK_SIDE_LEFT])
   {
      uint8_t index                             = p_lcda_core_output->cvw_core_output.cvw_index[FBK_SIDE_LEFT];
      const Fbk_Object_Data_T *p_object_data    = &p_pa_data->object_data[index];
      p_lcda_output->lcda_object_type_left      = (uint8_t) LCDA_OBJ_TYPE_CVW;
      p_lcda_output->lcda_object_id_left        = p_object_data->id;
      p_lcda_output->lcda_object_unique_id_left = p_lcda_core_output->cvw_core_output.cvw_unique_id[FBK_SIDE_LEFT];
      p_lcda_output->lcda_object_width_left     = p_object_data->width;
      p_lcda_output->lcda_object_length_left    = p_object_data->length;
      p_lcda_output->lcda_object_px_left        = p_object_data->vcs_pos.x;
      p_lcda_output->lcda_object_py_left        = p_object_data->vcs_pos.y;
      p_lcda_output->lcda_object_ttc_left       = p_lcda_core_output->cvw_core_output.cvw_ttc[FBK_SIDE_LEFT];
      p_lcda_output->lcda_object_vx_left        = p_object_data->vcs_vel.x;
      p_lcda_output->lcda_object_vy_left        = p_object_data->vcs_vel.y;
      Lcda_Set_Existence_Probability(&(p_lcda_output->lcda_object_existance_probability_left), p_lcda_core_input, p_cals, index);
      p_lcda_output->lcda_object_lane_change_probability_left = FBK_ZERO_UINT;
      Lcda_Transformation_To_Bmw_Coord_System_Left(p_vehicle_data, p_lcda_output);
   } /* end if cvw alert */
   if (Fbk_Is_True(p_lcda_output->slc_alert[FBK_SIDE_LEFT]))
   {
      uint8_t index                             = p_lcda_core_output->slc_core_output.slc_index[FBK_SIDE_LEFT];
      const Fbk_Object_Data_T *p_object_data    = &p_pa_data->object_data[index];
      p_lcda_output->lcda_object_type_left      = (uint8_t) LCDA_OBJ_TYPE_SLC;
      p_lcda_output->lcda_object_id_left        = p_object_data->id;
      p_lcda_output->lcda_object_unique_id_left = p_lcda_core_output->slc_core_output.slc_unique_id[FBK_SIDE_LEFT];
      p_lcda_output->lcda_object_width_left     = p_object_data->width;
      p_lcda_output->lcda_object_length_left    = p_object_data->length;
      p_lcda_output->lcda_object_px_left        = p_object_data->vcs_pos.x;
      p_lcda_output->lcda_object_py_left        = p_object_data->vcs_pos.y;
      p_lcda_output->lcda_object_ttc_left       = p_lcda_core_output->slc_core_output.slc_lat_ttc[FBK_SIDE_LEFT];
      p_lcda_output->lcda_object_vx_left        = p_object_data->vcs_vel.x;
      p_lcda_output->lcda_object_vy_left        = p_object_data->vcs_vel.y;
      Lcda_Set_Existence_Probability(&(p_lcda_output->lcda_object_existance_probability_left), p_lcda_core_input, p_cals, index);
      p_lcda_output->lcda_object_lane_change_probability_left =
         (uint8_t) Ml_Roundf((percentage_factor * p_lcda_core_output->slc_core_output.slc_lane_change_prob[FBK_SIDE_LEFT]));
      Lcda_Transformation_To_Bmw_Coord_System_Left(p_vehicle_data, p_lcda_output);
   } /* end if slc alert */
   if (Fbk_Is_True(p_lcda_output->awa_alert[FBK_SIDE_LEFT]))
   {
      uint8_t index                          = p_lcda_core_output->elc_core_output.elc_index[FBK_SIDE_LEFT];
      const Fbk_Object_Data_T *p_object_data = &p_pa_data->object_data[index];
      p_lcda_output->lcda_object_type_left   = (uint8_t) LCDA_OBJ_TYPE_ELC;
      p_lcda_output->lcda_object_id_left     = p_object_data->id;
      p_lcda_output->lcda_object_width_left  = p_object_data->width;
      p_lcda_output->lcda_object_length_left = p_object_data->length;
      p_lcda_output->lcda_object_px_left     = p_object_data->vcs_pos.x;
      p_lcda_output->lcda_object_py_left     = p_object_data->vcs_pos.y;
      p_lcda_output->lcda_object_ttc_left    = FBK_ZERO_F;
      p_lcda_output->lcda_object_vx_left     = p_object_data->vcs_vel.x;
      p_lcda_output->lcda_object_vy_left     = p_object_data->vcs_vel.y;
      Lcda_Set_Existence_Probability(&(p_lcda_output->lcda_object_existance_probability_left), p_lcda_core_input, p_cals, index);

      p_lcda_output->lcda_object_lane_change_probability_left = FBK_ZERO_UINT;
      Lcda_Transformation_To_Bmw_Coord_System_Left(p_vehicle_data, p_lcda_output);
   } /* end if elc alert */
   else
   {
      /* Do nothing */
   } /* end if neither bsw nor cvw nor slc nor elc alert left */
}

static void Lcda_Fill_Object_Right(const Lcda_Core_Input_T *p_lcda_core_input,
                                   const Fbk_Vehicle_Data_T *p_vehicle_data,
                                   const Lcda_Core_Output_T *p_lcda_core_output,
                                   const Lcda_Customer_Calibration_T *p_cals,
                                   const uint8_t bsw_index[FBK_NUMBER_OF_SIDES],
                                   Lcda_Output_T *p_lcda_output)
{
   const float32_T percentage_factor = 100.0f;
   if (FBK_ZERO_UINT != p_lcda_output->bsw_alert[FBK_SIDE_RIGHT])
   {
      uint8_t index                              = bsw_index[FBK_SIDE_RIGHT];
      const Fbk_Object_Data_T *p_object_data     = &p_lcda_core_input->p_pa_data->object_data[index];
      p_lcda_output->lcda_object_type_right      = (uint8_t) LCDA_OBJ_TYPE_BSW;
      p_lcda_output->lcda_object_id_right        = p_lcda_core_output->bsw_core_output.bsw_id[FBK_SIDE_RIGHT];
      p_lcda_output->lcda_object_unique_id_right = p_lcda_core_output->bsw_core_output.bsw_unique_id[FBK_SIDE_RIGHT];
      p_lcda_output->lcda_object_width_right     = p_object_data->width;
      p_lcda_output->lcda_object_length_right    = p_object_data->length;
      p_lcda_output->lcda_object_px_right        = p_object_data->vcs_pos.x;
      p_lcda_output->lcda_object_py_right        = p_object_data->vcs_pos.y;
      p_lcda_output->lcda_object_ttc_right       = FBK_ZERO_F;
      p_lcda_output->lcda_object_vx_right        = p_object_data->vcs_vel.x;
      p_lcda_output->lcda_object_vy_right        = p_object_data->vcs_vel.y;
      Lcda_Set_Existence_Probability(&(p_lcda_output->lcda_object_existance_probability_right), p_lcda_core_input, p_cals, index);
      p_lcda_output->lcda_object_lane_change_probability_right = FBK_ZERO_UINT;
      Lcda_Transformation_To_Bmw_Coord_System_Right(p_vehicle_data, p_lcda_output);
   } /* end if bsw alert right */
   if (FBK_ZERO_UINT != p_lcda_output->cvw_alert[FBK_SIDE_RIGHT])
   {
      uint8_t index                              = p_lcda_core_output->cvw_core_output.cvw_index[FBK_SIDE_RIGHT];
      const Fbk_Object_Data_T *p_object_data     = &p_lcda_core_input->p_pa_data->object_data[index];
      p_lcda_output->lcda_object_type_right      = (uint8_t) LCDA_OBJ_TYPE_CVW;
      p_lcda_output->lcda_object_id_right        = p_lcda_core_output->cvw_core_output.cvw_id[FBK_SIDE_RIGHT];
      p_lcda_output->lcda_object_unique_id_right = p_lcda_core_output->cvw_core_output.cvw_unique_id[FBK_SIDE_RIGHT];
      p_lcda_output->lcda_object_width_right     = p_object_data->width;
      p_lcda_output->lcda_object_length_right    = p_object_data->length;
      p_lcda_output->lcda_object_px_right        = p_object_data->vcs_pos.x;
      p_lcda_output->lcda_object_py_right        = p_object_data->vcs_pos.y;
      p_lcda_output->lcda_object_ttc_right       = p_lcda_core_output->cvw_core_output.cvw_ttc[FBK_SIDE_RIGHT];
      p_lcda_output->lcda_object_vx_right        = p_object_data->vcs_vel.x;
      p_lcda_output->lcda_object_vy_right        = p_object_data->vcs_vel.y;
      Lcda_Set_Existence_Probability(&(p_lcda_output->lcda_object_existance_probability_right), p_lcda_core_input, p_cals, index);
      p_lcda_output->lcda_object_lane_change_probability_right = FBK_ZERO_UINT;
      Lcda_Transformation_To_Bmw_Coord_System_Right(p_vehicle_data, p_lcda_output);
   } /* end if cvw alert */
   if (Fbk_Is_True(p_lcda_output->slc_alert[FBK_SIDE_RIGHT]))
   {
      uint8_t index                              = p_lcda_core_output->slc_core_output.slc_index[FBK_SIDE_RIGHT];
      const Fbk_Object_Data_T *p_object_data     = &p_lcda_core_input->p_pa_data->object_data[index];
      p_lcda_output->lcda_object_type_right      = (uint8_t) LCDA_OBJ_TYPE_SLC;
      p_lcda_output->lcda_object_id_right        = p_lcda_core_output->slc_core_output.slc_id[FBK_SIDE_RIGHT];
      p_lcda_output->lcda_object_unique_id_right = p_lcda_core_output->slc_core_output.slc_unique_id[FBK_SIDE_RIGHT];
      p_lcda_output->lcda_object_width_right     = p_object_data->width;
      p_lcda_output->lcda_object_length_right    = p_object_data->length;
      p_lcda_output->lcda_object_px_right        = p_object_data->vcs_pos.x;
      p_lcda_output->lcda_object_py_right        = p_object_data->vcs_pos.y;
      p_lcda_output->lcda_object_ttc_right       = p_lcda_core_output->slc_core_output.slc_lat_ttc[FBK_SIDE_RIGHT];
      p_lcda_output->lcda_object_vx_right        = p_object_data->vcs_vel.x;
      p_lcda_output->lcda_object_vy_right        = p_object_data->vcs_vel.y;
      Lcda_Set_Existence_Probability(&(p_lcda_output->lcda_object_existance_probability_right), p_lcda_core_input, p_cals, index);
      p_lcda_output->lcda_object_lane_change_probability_right =
         (uint8_t) Ml_Roundf((percentage_factor * p_lcda_core_output->slc_core_output.slc_lane_change_prob[FBK_SIDE_RIGHT]));
      Lcda_Transformation_To_Bmw_Coord_System_Right(p_vehicle_data, p_lcda_output);
   } /* end if slc alert */
   if (Fbk_Is_True(p_lcda_output->awa_alert[FBK_SIDE_RIGHT]))
   {
      uint8_t index                           = p_lcda_core_output->elc_core_output.elc_index[FBK_SIDE_RIGHT];
      const Fbk_Object_Data_T *p_object_data  = &p_lcda_core_input->p_pa_data->object_data[index];
      p_lcda_output->lcda_object_type_right   = (uint8_t) LCDA_OBJ_TYPE_ELC;
      p_lcda_output->lcda_object_id_right     = p_lcda_core_output->elc_core_output.elc_id[FBK_SIDE_RIGHT];
      p_lcda_output->lcda_object_width_right  = p_object_data->width;
      p_lcda_output->lcda_object_length_right = p_object_data->length;
      p_lcda_output->lcda_object_px_right     = p_object_data->vcs_pos.x;
      p_lcda_output->lcda_object_py_right     = p_object_data->vcs_pos.y;
      p_lcda_output->lcda_object_ttc_right    = FBK_ZERO_F;
      p_lcda_output->lcda_object_vx_right     = p_object_data->vcs_vel.x;
      p_lcda_output->lcda_object_vy_right     = p_object_data->vcs_vel.y;
      Lcda_Set_Existence_Probability(&(p_lcda_output->lcda_object_existance_probability_right), p_lcda_core_input, p_cals, index);

      p_lcda_output->lcda_object_lane_change_probability_right = FBK_ZERO_UINT;
      Lcda_Transformation_To_Bmw_Coord_System_Right(p_vehicle_data, p_lcda_output);
   } /* end if elc alert */
   else
   {
      /*Do nothing*/
   } /* end if neither bsw nor cvw nor slc nor elc alert right */
}


static void Lcda_Fill_Critical_Object_Right_Data_on_Output_Bus_Signal(const Pa_Data_T *p_pa_data,
                                                                      Lcda_Output_T *p_lcda_output,
                                                                      const Lcda_Core_Output_T *p_lcda_core_output)
{
   const Fbk_Vehicle_Data_T *p_vehicle_data = &p_pa_data->vehicle_data;
   const float32_T percentage_factor        = 100.0f;
   if (FBK_ZERO_UINT != p_lcda_output->bsw_alert[FBK_SIDE_RIGHT])
   {
      const uint8_t bsw_index                = p_lcda_core_output->bsw_core_output.bsw_index[FBK_SIDE_RIGHT];
      const Fbk_Object_Data_T *p_object_data = &p_pa_data->object_data[bsw_index];
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_event_data_qualifier = EVENT_DATA_AVAILABLE;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_extended_qualifier   = NORMAL_OPERATION_MODE;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_status_bsw    = VEHICLE_IN_THE_RIGHT_BSW_ZONE;

      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.hour   = LCDA_MOCKED_TIMESTAMP_VALUE;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.minute = LCDA_MOCKED_TIMESTAMP_VALUE;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.second = LCDA_MOCKED_TIMESTAMP_VALUE;

      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_id_right = p_lcda_core_output->bsw_core_output.bsw_id[FBK_SIDE_RIGHT];
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_right = p_object_data->vcs_pos.x;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_right = p_object_data->vcs_pos.y;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_right =
         p_lcda_core_output->bsw_core_output.bsw_ttp[FBK_SIDE_RIGHT];
      /* TTC does not exist in core_output_t's bsw_core_output structure currently, mapped to zero currently. */
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_right = FBK_ZERO_F;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_right =
         p_lcda_core_output->bsw_core_output.bsw_ttle[FBK_SIDE_RIGHT];
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_right = p_object_data->vcs_vel.x;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_right = p_object_data->vcs_vel.y;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_width_right      = p_object_data->width;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_length_right     = p_object_data->length;
      /* bmw_lcda_object_existance_probability_right is mapped in function Lcda_Fill_Object_Right().Currently using the same.
       * But ultimately it should be mapped directly here also from core_output
       */
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_right =
         p_lcda_output->lcda_object_existance_probability_right;
      /* bmw_lcda_object_lane_change_probability_right does not exist in core_output_t .
       * Hence mapping it to zero currently
       */
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_right = FBK_ZERO_UINT;

      Lcda_Transform_Output_Bus_Signals_of_BSW_To_Bmw_Coord_System_Right(p_vehicle_data, p_lcda_output);

   } /* end if bsw alert right */
   else
   {
      /*do nothing*/
   }
   if (FBK_ZERO_UINT != p_lcda_output->cvw_alert[FBK_SIDE_RIGHT])
   {
      const uint8_t cvw_index                = p_lcda_core_output->cvw_core_output.cvw_index[FBK_SIDE_RIGHT];
      const Fbk_Object_Data_T *p_object_data = &p_pa_data->object_data[cvw_index];
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_event_data_qualifier = EVENT_DATA_AVAILABLE;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_extended_qualifier   = NORMAL_OPERATION_MODE;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_status_cvw    = VEHICLE_IN_THE_RIGHT_CVW_ZONE;

      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.hour   = LCDA_MOCKED_TIMESTAMP_VALUE;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.minute = LCDA_MOCKED_TIMESTAMP_VALUE;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.second = LCDA_MOCKED_TIMESTAMP_VALUE;

      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_id_right_cvw =
         p_lcda_core_output->cvw_core_output.cvw_id[FBK_SIDE_RIGHT];
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_right_cvw = p_object_data->vcs_pos.x;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_right_cvw = p_object_data->vcs_pos.y;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_right_cvw =
         p_lcda_core_output->cvw_core_output.cvw_ttc[FBK_SIDE_RIGHT];
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_right_cvw =
         p_lcda_core_output->cvw_core_output.cvw_ttp[FBK_SIDE_RIGHT];
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_right_cvw =
         p_lcda_core_output->cvw_core_output.cvw_ttle[FBK_SIDE_RIGHT];
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_right_cvw = p_object_data->vcs_vel.x;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_right_cvw = p_object_data->vcs_vel.y;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_width_right_cvw      = p_object_data->width;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_length_right_cvw     = p_object_data->length;
      /* bmw_lcda_object_existance_probability_right is mapped in function Lcda_Fill_Object_Right().Currently using the same.
       * But ultimately it should be mapped directly here also from core_output
       */
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_right_cvw =
         p_lcda_output->lcda_object_existance_probability_right;
      /* bmw_lcda_object_lane_change_probability_right does not exist in core_output_t .
       * Hence mapping it to zero currently
       */
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_right_cvw = FBK_ZERO_UINT;

      Lcda_Transform_Output_Bus_Signals_of_CVW_To_Bmw_Coord_System_Right(p_vehicle_data, p_lcda_output);

   } /* end if cvw alert */
   else
   {
      /*do nothing*/
   }
   if (Fbk_Is_True(p_lcda_output->slc_alert[FBK_SIDE_RIGHT]))
   {
      const uint8_t slc_index                = p_lcda_core_output->slc_core_output.slc_index[FBK_SIDE_RIGHT];
      const Fbk_Object_Data_T *p_object_data = &p_pa_data->object_data[slc_index];
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_event_data_qualifier = EVENT_DATA_AVAILABLE;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_extended_qualifier   = NORMAL_OPERATION_MODE;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_status_slc    = VEHICLE_IN_THE_RIGHT_SLC_ZONE;

      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.hour   = LCDA_MOCKED_TIMESTAMP_VALUE;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.minute = LCDA_MOCKED_TIMESTAMP_VALUE;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.second = LCDA_MOCKED_TIMESTAMP_VALUE;

      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_id_right_slc =
         p_lcda_core_output->slc_core_output.slc_id[FBK_SIDE_RIGHT];
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_right_slc = p_object_data->vcs_pos.x;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_right_slc = p_object_data->vcs_pos.y;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_right_slc =
         p_lcda_core_output->slc_core_output.slc_lat_ttc[FBK_SIDE_RIGHT];
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_right_slc =
         p_lcda_core_output->slc_core_output.slc_ttp[FBK_SIDE_RIGHT];
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_right_slc = p_object_data->vcs_vel.x;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_right_slc = p_object_data->vcs_vel.y;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_width_right_slc      = p_object_data->width;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_length_right_slc     = p_object_data->length;
      /* bmw_lcda_object_existance_probability_right is mapped in function Lcda_Fill_Object_Right().Currently using the same.
       * But ultimately it should be mapped directly here also from core_output
       */
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_right_slc =
         p_lcda_output->lcda_object_existance_probability_right;

      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_right_slc =
         (uint8_t) Ml_Roundf((percentage_factor * p_lcda_core_output->slc_core_output.slc_lane_change_prob[FBK_SIDE_RIGHT]));

      Lcda_Transform_Output_Bus_Signals_of_SLC_To_Bmw_Coord_System_Right(p_vehicle_data, p_lcda_output);
   } /* end if slc alert */
   else
   {
      /*Do nothing*/
   } /* end if neither bsw nor cvw nor slc nor elc alert right */
}

static void Lcda_Fill_Critical_Object_Left_Data_on_Output_Bus_Signal(const Pa_Data_T *p_pa_data,
                                                                     Lcda_Output_T *p_lcda_output,
                                                                     const Lcda_Core_Output_T *p_lcda_core_output)
{
   const float32_T percentage_factor        = 100.0f;
   const Fbk_Vehicle_Data_T *p_vehicle_data = &p_pa_data->vehicle_data;
   if (FBK_ZERO_UINT != p_lcda_output->bsw_alert[FBK_SIDE_LEFT])
   {
      const uint8_t bsw_index                = p_lcda_core_output->bsw_core_output.bsw_index[FBK_SIDE_LEFT];
      const Fbk_Object_Data_T *p_object_data = &p_pa_data->object_data[bsw_index];
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_event_data_qualifier = EVENT_DATA_AVAILABLE;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_extended_qualifier   = NORMAL_OPERATION_MODE;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_status_bsw    = VEHICLE_IN_THE_LEFT_BSW_ZONE;

      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.hour   = LCDA_MOCKED_TIMESTAMP_VALUE;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.minute = LCDA_MOCKED_TIMESTAMP_VALUE;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.second = LCDA_MOCKED_TIMESTAMP_VALUE;

      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_id_left = p_lcda_core_output->bsw_core_output.bsw_id[FBK_SIDE_LEFT];
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_left = p_object_data->vcs_pos.x;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_left = p_object_data->vcs_pos.y;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_left = p_lcda_core_output->bsw_core_output.bsw_ttp[FBK_SIDE_LEFT];
      /* TTC does not exist in core_output_t's bsw_core_output structure currently, mapped to zero currently. */
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_left = FBK_ZERO_F;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_left =
         p_lcda_core_output->bsw_core_output.bsw_ttle[FBK_SIDE_LEFT];
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_left = p_object_data->vcs_vel.x;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_left = p_object_data->vcs_vel.y;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_width_left      = p_object_data->width;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_length_left     = p_object_data->length;
      /* bmw_lcda_object_existance_probability_left is mapped in function Lcda_Fill_Object_Left().Currently using the same.
       * But ultimately it should be mapped directly here also from core_output
       */
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_left =
         p_lcda_output->lcda_object_existance_probability_left;
      /* bmw_lcda_object_lane_change_probability_left does not exist in core_output_t .
       * Hence mapping it to zero currently
       */
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_left =
         p_lcda_output->lcda_object_lane_change_probability_left;

      Lcda_Transform_Output_Bus_Signals_of_BSW_To_Bmw_Coord_System_Left(p_vehicle_data, p_lcda_output);
   } /* end if bsw alert right */
   else
   {
      /*do nothing*/
   }
   if (FBK_ZERO_UINT != p_lcda_output->cvw_alert[FBK_SIDE_LEFT])
   {
      const uint8_t cvw_index                = p_lcda_core_output->cvw_core_output.cvw_index[FBK_SIDE_LEFT];
      const Fbk_Object_Data_T *p_object_data = &p_pa_data->object_data[cvw_index];
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_event_data_qualifier = EVENT_DATA_AVAILABLE;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_status_cvw    = VEHICLE_IN_THE_LEFT_CVW_ZONE;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_extended_qualifier   = NORMAL_OPERATION_MODE;

      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.hour   = LCDA_MOCKED_TIMESTAMP_VALUE;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.minute = LCDA_MOCKED_TIMESTAMP_VALUE;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.second = LCDA_MOCKED_TIMESTAMP_VALUE;

      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_id_left_cvw =
         p_lcda_core_output->cvw_core_output.cvw_id[FBK_SIDE_LEFT];
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_left_cvw = p_object_data->vcs_pos.x;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_left_cvw = p_object_data->vcs_pos.y;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_left_cvw =
         p_lcda_core_output->cvw_core_output.cvw_ttc[FBK_SIDE_LEFT];
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_left_cvw =
         p_lcda_core_output->cvw_core_output.cvw_ttp[FBK_SIDE_LEFT];
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_left_cvw =
         p_lcda_core_output->cvw_core_output.cvw_ttle[FBK_SIDE_LEFT];
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_left_cvw = p_object_data->vcs_vel.x;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_left_cvw = p_object_data->vcs_vel.y;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_width_left_cvw      = p_object_data->width;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_length_left_cvw     = p_object_data->length;
      /* bmw_lcda_object_existance_probability_left is mapped in function Lcda_Fill_Object_Left().Currently using the same.
       * But ultimately it should be mapped directly here also from core_output
       */
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_left_cvw =
         p_lcda_output->lcda_object_existance_probability_left;
      /* bmw_lcda_object_lane_change_probability_left does not exist in core_output_t .
       * Hence mapping it to zero currently
       */
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_left_cvw =
         p_lcda_output->lcda_object_lane_change_probability_left;

      Lcda_Transform_Output_Bus_Signals_of_CVW_To_Bmw_Coord_System_Left(p_vehicle_data, p_lcda_output);
   } /* end if cvw alert */
   else
   {
      /*do nothing*/
   }
   if (Fbk_Is_True(p_lcda_output->slc_alert[FBK_SIDE_LEFT]))
   {
      const uint8_t slc_index                = p_lcda_core_output->slc_core_output.slc_index[FBK_SIDE_LEFT];
      const Fbk_Object_Data_T *p_object_data = &p_pa_data->object_data[slc_index];
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_event_data_qualifier = EVENT_DATA_AVAILABLE;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_extended_qualifier   = NORMAL_OPERATION_MODE;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_status_slc    = VEHICLE_IN_THE_LEFT_SLC_ZONE;

      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.hour   = LCDA_MOCKED_TIMESTAMP_VALUE;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.minute = LCDA_MOCKED_TIMESTAMP_VALUE;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.second = LCDA_MOCKED_TIMESTAMP_VALUE;

      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_id_left_slc =
         p_lcda_core_output->slc_core_output.slc_id[FBK_SIDE_LEFT];
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_left_slc = p_object_data->vcs_pos.x;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_left_slc = p_object_data->vcs_pos.y;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_left_slc =
         p_lcda_core_output->slc_core_output.slc_lat_ttc[FBK_SIDE_LEFT];
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_left_slc =
         p_lcda_core_output->slc_core_output.slc_ttp[FBK_SIDE_LEFT];
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_left_slc = p_object_data->vcs_vel.x;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_left_slc = p_object_data->vcs_vel.y;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_width_left_slc      = p_object_data->width;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_length_left_slc     = p_object_data->length;
      /* bmw_lcda_object_existance_probability_left is mapped in function Lcda_Fill_Object_Left().Currently using the same.
       * But ultimately it should be mapped directly here also from core_output
       */
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_left_slc =
         p_lcda_output->lcda_object_existance_probability_left;
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_left_slc =
         (uint8_t) Ml_Roundf((percentage_factor * p_lcda_core_output->slc_core_output.slc_lane_change_prob[FBK_SIDE_LEFT]));

      Lcda_Transform_Output_Bus_Signals_of_SLC_To_Bmw_Coord_System_Left(p_vehicle_data, p_lcda_output);

   } /* end if slc alert */
   else
   {
      /*do nothing*/
   }
}


static void Lcda_Transformation_To_Bmw_Coord_System_Left(const Fbk_Vehicle_Data_T *p_vehicle_data, Lcda_Output_T *p_lcda_output)
{
   p_lcda_output->lcda_object_px_left = p_lcda_output->lcda_object_px_left - p_vehicle_data->rear_axle_position;
   p_lcda_output->lcda_object_py_left = -FBK_ONE_F * p_lcda_output->lcda_object_py_left;
   p_lcda_output->lcda_object_vy_left = -FBK_ONE_F * p_lcda_output->lcda_object_vy_left;
}

static void Lcda_Transformation_To_Bmw_Coord_System_Right(const Fbk_Vehicle_Data_T *p_vehicle_data, Lcda_Output_T *p_lcda_output)
{
   p_lcda_output->lcda_object_px_right = p_lcda_output->lcda_object_px_right - p_vehicle_data->rear_axle_position;
   p_lcda_output->lcda_object_py_right = -FBK_ONE_F * p_lcda_output->lcda_object_py_right;
   p_lcda_output->lcda_object_vy_right = -FBK_ONE_F * p_lcda_output->lcda_object_vy_right;
}

static void Lcda_Transform_Output_Bus_Signals_of_BSW_To_Bmw_Coord_System_Left(const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                                              Lcda_Output_T *p_lcda_output)
{
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_left =
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_left - p_vehicle_data->rear_axle_position;

   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_left =
      -FBK_ONE_F * p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_left;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_left =
      -FBK_ONE_F * p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_left;
}

static void Lcda_Transform_Output_Bus_Signals_of_BSW_To_Bmw_Coord_System_Right(const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                                               Lcda_Output_T *p_lcda_output)
{
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_right =
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_right - p_vehicle_data->rear_axle_position;

   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_right =
      -FBK_ONE_F * p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_right;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_right =
      -FBK_ONE_F * p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_right;
}

static void Lcda_Transform_Output_Bus_Signals_of_CVW_To_Bmw_Coord_System_Left(const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                                              Lcda_Output_T *p_lcda_output)
{
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_left_cvw =
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_left_cvw - p_vehicle_data->rear_axle_position;

   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_left_cvw =
      -FBK_ONE_F * p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_left_cvw;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_left_cvw =
      -FBK_ONE_F * p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_left_cvw;
}

static void Lcda_Transform_Output_Bus_Signals_of_CVW_To_Bmw_Coord_System_Right(const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                                               Lcda_Output_T *p_lcda_output)
{
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_right_cvw =
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_right_cvw - p_vehicle_data->rear_axle_position;

   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_right_cvw =
      -FBK_ONE_F * p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_right_cvw;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_right_cvw =
      -FBK_ONE_F * p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_right_cvw;
}

static void Lcda_Transform_Output_Bus_Signals_of_SLC_To_Bmw_Coord_System_Left(const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                                              Lcda_Output_T *p_lcda_output)
{
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_left_slc =
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_left_slc - p_vehicle_data->rear_axle_position;

   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_left_slc =
      -FBK_ONE_F * p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_left_slc;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_left_slc =
      -FBK_ONE_F * p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_left_slc;
}

static void Lcda_Transform_Output_Bus_Signals_of_SLC_To_Bmw_Coord_System_Right(const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                                               Lcda_Output_T *p_lcda_output)
{
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_right_slc =
      p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_right_slc - p_vehicle_data->rear_axle_position;

   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_right_slc =
      -FBK_ONE_F * p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_right_slc;
   p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_right_slc =
      -FBK_ONE_F * p_lcda_output->bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_right_slc;
}


static void Lcda_Set_Existence_Probability(uint16_t *p_exist_prob_to_set,
                                           const Lcda_Core_Input_T *p_lcda_core_input,
                                           const Lcda_Customer_Calibration_T *p_cals,
                                           const uint8_t index)
{
   const Fbk_Object_Data_T *p_object_data;
   assert(NULL != p_exist_prob_to_set);
   assert(NULL != p_lcda_core_input);
   assert(NULL != p_cals);

   p_object_data = &p_lcda_core_input->p_pa_data->object_data[index];

   if (Fbk_Is_False(p_lcda_core_input->warn_settings.f_use_cvw_lane_change_intention_zone))
   {
      *p_exist_prob_to_set = (uint16_t) Ml_Roundf((100.0f * p_object_data->existence_probability));
   }
   else
   {
      *p_exist_prob_to_set =
         (uint16_t) Ml_Roundf((100.0f * Max(p_cals->k_bmw_sp25_exist_prob_lc_intention, p_object_data->existence_probability)));
   }
}

static uint8_t Lcda_Get_Uint8_Flag_From_Boolean(const boolean_T flag)
{
   uint8_t uint_flag = FBK_ZERO_UINT;

   if (Fbk_Is_True(flag))
   {
      uint_flag = FBK_ONE_UINT;
   }

   return uint_flag;
}

static boolean_T Lcda_Is_Object_Trailer(const Lcda_Input_T *p_lcda_input,
                                        const Pa_Data_T *p_pa_data,
                                        const uint8_t object_index,
                                        const Lcda_Customer_Calibration_T *p_cals)
{
   boolean_T f_object_is_trailer = FBK_FALSE;
   const Fbk_Vehicle_Data_T *p_vehicle_data;

   assert(NULL != p_lcda_input);

   p_vehicle_data = &p_pa_data->vehicle_data;

   if (Fbk_Is_True(p_lcda_input->f_lcda_trailer_mode) && Fbk_Is_True(p_lcda_input->f_lcda_trailer_connected)
       && (PA_INVALID_OBJ_INDEX != object_index))
   {
      /* A trailer is connected. */
      const Fbk_Object_Data_T *p_object_data = &p_pa_data->object_data[object_index];
      float32_T obj_long_pos                 = p_object_data->vcs_pos.x;
      float32_T obj_lat_pos                  = p_object_data->vcs_pos.y;
      float32_T obj_width                    = p_object_data->width;
      float32_T obj_length                   = p_object_data->length;

      float32_T host_vehicle_length = p_vehicle_data->host_length;
      float32_T host_vehicle_width  = p_vehicle_data->host_width;

      /* The lateral bike carrier buffer should be dependent on the object width and limited by a maximum buffer width. This
       * ensures that at least any part of the object is actually located behind the ego vehicle. */
      float32_T lateral_bike_carrier_buffer = Min(Fbk_Half(obj_width), p_cals->k_bmw_sp25_trailer_mode_max_bike_carrier_buffer);


      if ((obj_long_pos <= -host_vehicle_length)
          && (obj_long_pos >= -(host_vehicle_length + p_cals->k_bmw_sp25_trailer_mode_max_trailer_length))
          && (Fbk_Abs_F(obj_lat_pos) <= Fbk_Half(host_vehicle_width)))
      {
         /* Based on the object position it is likely the connected trailer behind the host vehicle. */
         f_object_is_trailer = FBK_TRUE;
      }
      else if ((obj_long_pos <= -host_vehicle_length)
               && (obj_long_pos
                   >= -(host_vehicle_length + Fbk_Half(obj_length) + p_cals->k_bmw_sp25_trailer_mode_max_bike_carrier_distance))
               && (Fbk_Abs_F(obj_lat_pos) <= (Fbk_Half(host_vehicle_width) + lateral_bike_carrier_buffer)))
      {
         /* Based on the object properties it is likely the connected bike carrier behind the host vehicle. */
         f_object_is_trailer = FBK_TRUE;
      }
      else
      {
         /* Object is most likely neither a connected trailer nor a connected bike carrier */
         f_object_is_trailer = FBK_FALSE;
      }
   }

   return f_object_is_trailer;
}


static void Lcda_Limit_Cvw_Alert_By_Zone_Length(Lcda_Output_T *p_lcda_output,
                                                const Lcda_Input_T *p_lcda_input,
                                                const Pa_Data_T *p_pa_data,
                                                const uint8_t cvw_index[FBK_NUMBER_OF_SIDES],
                                                const boolean_T f_previous_cvw_alert[FBK_NUMBER_OF_SIDES],
                                                const Lcda_Customer_Calibration_T *p_cals)
{
   uint8_t side;
   const Fbk_Vehicle_Data_T *p_vehicle_data;

   /* Asserts */
   assert(NULL != p_lcda_output);
   assert(NULL != p_lcda_input);
   assert(NULL != p_cals);

   p_vehicle_data = &p_pa_data->vehicle_data;

   /* Iterate over both host vehicle sides. */
   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      /* Check for active CVW alert on this side */
      if (FBK_ZERO_UINT != p_lcda_output->cvw_alert[side])
      {
         uint8_t obj_index                      = cvw_index[side];
         const Fbk_Object_Data_T *p_object_data = &p_pa_data->object_data[obj_index];
         float32_T obj_dist_to_host =
            Fbk_Abs_F(p_object_data->curvi_pos.x) - Fbk_Half(p_object_data->length) - p_vehicle_data->host_length;
         float32_T cvw_range_limit = (float32_T) p_lcda_input->lcda_cvw_limit_zone_range;

         if (Fbk_Is_False(f_previous_cvw_alert[side]))
         {
            /* Decrease range limit if there was no previously active cvw alert */
            cvw_range_limit -= p_cals->k_bmw_sp25_cvw_limit_zone_range_hys;
         }

         if ((PA_OBJ_STATUS_INVALID != p_object_data->status) && (obj_dist_to_host > cvw_range_limit))
         {
            /* Object is further away than the zone length limit. Suppress CVW alert. */
            p_lcda_output->cvw_alert[side] = FBK_ZERO_UINT;
            p_lcda_output->cvw_id[side]    = PA_INVALID_OBJ_ID;
         }
      }
   }
}
