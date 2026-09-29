/**
 * @file lcda_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the RNA_SWEET400 post run logic for LCDA.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "lcda_post_run.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_object_validation.h"
#include "fbk_vehicle_data_t.h"
#include "lcda_core_calibration_t.h"
#include "lcda_core_input_t.h"
#include "lcda_core_output_t.h"
#include "lcda_output_t.h"
#include "lcda_rna_sweet400_debug_interface.h"
#include "lcda_rna_sweet400_debug_writer.h"
#include "lcda_types.h"
#include "ml_angle.h"
#include "ml_angle_t.h"
#include "ml_math.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_angle.h"
#include "ml_vector_2d_t.h"
#include "pa_const_macros.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include <assert.h>
#include <string.h>


/*===========================================================================*\
* Local constants defines
\*===========================================================================*/

/* LKA signals max values */

#define LCDA_LKA_MAX_TTC (7.75f)
#define LCDA_LKA_MAX_CURVI_POS_LONG (81.9f)
#define LCDA_LKA_MAX_CURVI_POS_LAT (81.9f)
#define LCDA_LKA_MAX_CURVI_VEL_LONG (36.2f)
#define LCDA_LKA_MAX_CURVI_VEL_LAT (25.5f)

/* LKA signals min values */

#define LCDA_LKA_MIN_TTC (0.0f)
#define LCDA_LKA_MIN_CURVI_POS_LONG (-81.9f)
#define LCDA_LKA_MIN_CURVI_POS_LAT (-81.9f)
#define LCDA_LKA_MIN_CURVI_VEL_LONG (-14.7f)
#define LCDA_LKA_MIN_CURVI_VEL_LAT (-25.4f)

/* LKA signals unavailable values */

#define LCDA_LKA_NA_TTC (7.875f)
#define LCDA_LKA_NA_CURVI_POS_LONG (81.92f)
#define LCDA_LKA_NA_CURVI_POS_LAT (81.92f)
#define LCDA_LKA_NA_CURVI_VEL_LONG (36.4f)
#define LCDA_LKA_NA_CURVI_VEL_LAT (25.7f)
#define LCDA_LKA_NA_OBJECT_CLASS RENAULT_LSS_OBJECT_CLASS_UNKNOWN
#define LCDA_LKA_NA_ALERT_CONDITION RENAULT_LSS_ALERT_CONDITION_OFF
#define LCDA_LKA_NA_MOTION_CLASS RENAULT_LSS_MOTION_CLASS_UNKNOWN
#define LCDA_LKA_NA_CHANGE_STATUS RENAULT_LSS_CHANGE_STATUS_NO_CHANGE;
#define LCDA_LKA_NA_OBJECT_ID (7)

#define LCDA_RNA_THRESHOLD_VEL_FOR_ONCOMING_OBJECTS (0.0f)
#define LCDA_RNA_NUMBER_OF_USED_IDS (5u)
#define LCDA_LKA_ZONE_HYS (1.0f)

/*===========================================================================*\
* Local types definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_2_4_violation]  */
typedef struct LKA_Zone_Tag
{
   float32_T lat_start;
   float32_T lat_end;
   float32_T lon_start;
   float32_T lon_end;
} LKA_Zone_T;

/*===========================================================================*\
* Static variables
\*===========================================================================*/

static LKA_Zone_T Lka_Zone;
static LKA_Zone_T Lka_Zone_Hys;
/* coverity[misra_c_2012_rule_8_9_violation][Defined static variable in dedicated section for overview of memory consumption] */
static boolean_T F_Was_Obj_In_Zone[FBK_NUMBER_OF_SIDES][PA_OBJ_NUMBER_OF_OBJECTS];
/* coverity[misra_c_2012_rule_8_9_violation][Defined static variable in dedicated section for overview of memory consumption] */
static uint8_t Lastly_Freed_Ids[FBK_NUMBER_OF_SIDES];
/* coverity[misra_c_2012_rule_8_9_violation][Defined static variable in dedicated section for overview of memory consumption] */
static boolean_T F_Ids_In_Use[FBK_NUMBER_OF_SIDES][LCDA_RNA_NUMBER_OF_USED_IDS];
/* coverity[misra_c_2012_rule_8_9_violation][Defined static variable in dedicated section for overview of memory consumption] */
static LKA_Object_T Obj_From_Prev_Iteration[FBK_NUMBER_OF_SIDES][LCDA_RNA_NUMBER_OF_OBJECTS];

/*===========================================================================*\
* Local functions prototypes
\*===========================================================================*/

static boolean_T Lcda_Check_If_Obj_In_Zone_Lka(const Pa_Data_T *p_pa_data, const uint8_t tracker_obj_index, const uint8_t side);

static RENAULT_LSS_OBJECT_CLASS_T Lcda_Get_Object_Class_Lka(Pa_Obj_Class_T object_class);

static RENAULT_LSS_CHANGE_STATUS_T Lcda_Get_Change_Status_Lka(Pa_Obj_Status_T track_status);

static RENAULT_LSS_MOTION_CLASS_T Lcda_Get_Motion_Class_Lka(Pa_Obj_Status_T track_status);

static RENAULT_LSS_ALERT_CONDITION_T
Lcda_Get_Alert_Condition_Lka(const Lcda_Output_T *p_lcda_output, float32_T curvi_long_vel_rel, uint8_t tracker_id, uint8_t side);

static uint8_t Lcda_Update_Ids_In_Use_Lka(LKA_Object_T objects_from_prev_iteration[FBK_NUMBER_OF_SIDES][LCDA_RNA_NUMBER_OF_OBJECTS],
                                          const LKA_Object_T p_lka_object[LCDA_RNA_NUMBER_OF_OBJECTS],
                                          boolean_T ids_in_use[FBK_NUMBER_OF_SIDES][LCDA_RNA_NUMBER_OF_USED_IDS],
                                          uint8_t side);

static uint8_t Lcda_Get_Obj_Id_Lka(LKA_Object_T objects_from_prev_iteration[FBK_NUMBER_OF_SIDES][LCDA_RNA_NUMBER_OF_OBJECTS],
                                   boolean_T ids_in_use[FBK_NUMBER_OF_SIDES][LCDA_RNA_NUMBER_OF_USED_IDS],
                                   uint8_t tracker_id,
                                   uint8_t side,
                                   uint8_t lastly_freed_id);

static void Lcda_Get_Relevant_Objects_Lka(const Pa_Data_T *p_pa_data,
                                          uint8_t rel_object_indexes[FBK_NUMBER_OF_SIDES][PA_OBJ_NUMBER_OF_OBJECTS],
                                          uint8_t num_of_relevant_objects[FBK_NUMBER_OF_SIDES]);

static float32_T Lcda_Calculate_Ttc_Lka(const Pa_Data_T *p_pa_data,
                                        const Lcda_Core_Calibration_T *p_cals,
                                        uint8_t temp_obj_index,
                                        Vector_2d_T target_ref);

static void Lcda_Fill_Output_List_Lka(uint8_t side,
                                      Lcda_Output_T *p_lcda_output,
                                      const Pa_Data_T *p_pa_data,
                                      const Lcda_Core_Calibration_T *p_cals,
                                      LKA_Object_T objects_from_prev_iteration[FBK_NUMBER_OF_SIDES][LCDA_RNA_NUMBER_OF_OBJECTS],
                                      uint8_t rel_object_indexes[FBK_NUMBER_OF_SIDES][PA_OBJ_NUMBER_OF_OBJECTS],
                                      const uint8_t num_of_relevant_objects[FBK_NUMBER_OF_SIDES]);

static void Lcda_Customer_Target_Selection(const Pa_Data_T *p_pa_data,
                                           Lcda_Output_T *p_lcda_output,
                                           const Lcda_Core_Calibration_T *p_cals);

static void Lcda_Initialize_Zones_LKA(const Pa_Data_T *p_pa_data, const Lcda_Core_Calibration_T *p_cals);

static void Lcda_Copy_Core_To_Cust_Output(const Lcda_Core_Output_T *p_core_output, Lcda_Output_T *p_lcda_output);

static void Lcda_Saturate_Signals_Lka(LKA_Object_T *p_lka_object, uint8_t index);

static float32_T Lcda_Max_Min(float32_T min_arg_1, float32_T min_arg_2, float32_T max_arg_2);

static float32_T Lcda_Transform_Coord_Lka_X(const float32_T x_coord, const Pa_Data_T *p_pa_data);

static float32_T Lcda_Transform_Coord_Lka_Y(const float32_T y_coord);

static void Lcda_Transform_Customer_Output_Coords_Rna(LKA_Object_T *p_lka_object, const uint8_t index, const Pa_Data_T *p_pa_data);

static Vector_2d_T Lcda_Calculate_Target_Reference_Point(const uint8_t index, const Pa_Data_T *p_pa_data, const uint8_t side);

static void Lcda_Reset_Output(Lcda_Output_T *p_lcda_output);

/*===========================================================================*\
* Global functions definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
void Lcda_Init_Output(Lcda_Output_T *p_lcda_output)
{
   /* coverity[misra_c_2012_rule_17_7_violation][Intentionally ignored return value of memset function since it is not required.] */
   memset(p_lcda_output, 0, sizeof(Lcda_Output_T));
}

void Lcda_Post_Run_Init(void)
{
}

/**
*  This function provides a handle to call the customer specific
* post run functionality.

* \param[in]      const Lcda_Core_Calibration_T *p_cals
* \param[out]      const Lcda_Input_T *p_lcda_input
*
* \return         none
*
* \remark
* \ServID         xx
* \Reentrancy     non-reentrant
* \Synchronism    synchronous
* \Precondition   none
* \Caveats        none
* \Requirements
* \reqtrace{SDD-C2SWA-LCDAPostRunProcessing}
*/

/* clang-format off */
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_lcda_instance" points to a non-constant type.] */
void Lcda_Post_Run(Lcda_Instance_T *p_lcda_instance, const Lcda_Input_T *p_lcda_input, Lcda_Output_T *p_lcda_output, const Fbk_Output_T *p_fbk_output)
/* clang-format on */
{
   const Lcda_Core_Input_T *p_lcda_core_input;
   const Lcda_Core_Output_T *p_lcda_core_output;
   const Lcda_Core_Calibration_T *p_cals;

   assert(NULL != p_lcda_instance);
   assert(NULL != p_lcda_input);
   assert(NULL != p_lcda_output);
   assert(NULL != p_fbk_output);

   p_lcda_core_input  = &p_lcda_instance->core_input;
   p_lcda_core_output = &p_lcda_instance->core_output;
   p_cals             = &p_lcda_instance->calibration;

   /*Reset Lcda output*/
   Lcda_Reset_Output(p_lcda_output);

   /* Copy the core output to the customer output structure here */
   Lcda_Copy_Core_To_Cust_Output(p_lcda_core_output, p_lcda_output);

   /* LKA operation */
   Lcda_Customer_Target_Selection(p_lcda_core_input->p_pa_data, p_lcda_output, p_cals);

   /* Write bin file output */
   Binary_Pass_Lcda_Debug_Rna_Sweet400_zones(Lka_Zone.lat_start, Lka_Zone.lat_end, Lka_Zone.lon_start, Lka_Zone.lon_end,
                                             Lka_Zone_Hys.lat_start, Lka_Zone_Hys.lat_end, Lka_Zone_Hys.lon_start,
                                             Lka_Zone_Hys.lon_end);
   Binary_Lcda_Rna_Sweet400_Fill_Debug_Data(p_lcda_input, p_lcda_output);

   Binary_Lcda_Rna_Sweet400_Write_Bin_File();
}

/*===========================================================================*\
* Local functions definitions
\*===========================================================================*/

static void Lcda_Reset_Output(Lcda_Output_T *p_lcda_output)
{
   uint8_t idx;

   p_lcda_output->core_output.f_lcda_enabled  = FBK_ZERO_UINT;
   p_lcda_output->core_output.f_bsw_enabled   = FBK_ZERO_UINT;
   p_lcda_output->core_output.f_cvw_enabled   = FBK_ZERO_UINT;
   p_lcda_output->core_output.bsw_alert_left  = FBK_ZERO_UINT;
   p_lcda_output->core_output.bsw_alert_right = FBK_ZERO_UINT;
   p_lcda_output->core_output.cvw_alert_left  = FBK_ZERO_UINT;
   p_lcda_output->core_output.cvw_alert_right = FBK_ZERO_UINT;
   p_lcda_output->core_output.bsw_id_left     = PA_INVALID_OBJ_ID;
   p_lcda_output->core_output.bsw_id_right    = PA_INVALID_OBJ_ID;

   p_lcda_output->core_output.cvw_id_left   = PA_INVALID_OBJ_ID;
   p_lcda_output->core_output.cvw_id_right  = PA_INVALID_OBJ_ID;
   p_lcda_output->core_output.cvw_ttc_left  = LCDA_CVW_DEFAULT_NO_ALERT_TTC;
   p_lcda_output->core_output.cvw_ttc_right = LCDA_CVW_DEFAULT_NO_ALERT_TTC;

   p_lcda_output->core_output.LCDA_errors.f_LCDA_input_is_NULL                   = FBK_ZERO_UINT;
   p_lcda_output->core_output.LCDA_errors.f_LCDA_output_is_NULL                  = FBK_ZERO_UINT;
   p_lcda_output->core_output.LCDA_errors.f_LCDA_cals_is_NULL                    = FBK_ZERO_UINT;
   p_lcda_output->core_output.LCDA_errors.f_LCDA_input_vehicle_data_is_NULL      = FBK_ZERO_UINT;
   p_lcda_output->core_output.LCDA_errors.f_LCDA_input_tracker_output_is_NULL    = FBK_ZERO_UINT;
   p_lcda_output->core_output.LCDA_errors.f_createCVWZone_cvw_zone_is_NULL       = FBK_ZERO_UINT;
   p_lcda_output->core_output.LCDA_errors.f_createCVWZone_cvw_zone_hys_is_NULL   = FBK_ZERO_UINT;
   p_lcda_output->core_output.LCDA_errors.f_createCVWZone_LCDA_input_is_NULL     = FBK_ZERO_UINT;
   p_lcda_output->core_output.LCDA_errors.f_createCVWZone_cals_is_NULL           = FBK_ZERO_UINT;
   p_lcda_output->core_output.LCDA_errors.f_createCVWZone_tracker_output_is_NULL = FBK_ZERO_UINT;
   p_lcda_output->core_output.LCDA_errors.f_createCVWZone_vehicle_data_is_NULL   = FBK_ZERO_UINT;
   p_lcda_output->core_output.LCDA_errors.f_createBSWZone_bsw_zone_is_NULL       = FBK_ZERO_UINT;
   p_lcda_output->core_output.LCDA_errors.f_createBSWZone_bsw_zone_hys_is_NULL   = FBK_ZERO_UINT;
   p_lcda_output->core_output.LCDA_errors.f_createBSWZone_LCDA_input_is_NULL     = FBK_ZERO_UINT;
   p_lcda_output->core_output.LCDA_errors.f_createBSWZone_cals_is_NULL           = FBK_ZERO_UINT;
   p_lcda_output->core_output.LCDA_errors.f_createBSWZone_tracker_output_is_NULL = FBK_ZERO_UINT;
   p_lcda_output->core_output.LCDA_errors.f_createBSWZone_vehicle_data_is_NULL   = FBK_ZERO_UINT;
   p_lcda_output->core_output.LCDA_errors.f_CheckBSW_LCDA_input_is_NULL          = FBK_ZERO_UINT;
   p_lcda_output->core_output.LCDA_errors.f_CheckBSW_vehicle_data_is_NULL        = FBK_ZERO_UINT;
   p_lcda_output->core_output.LCDA_errors.f_CheckBSW_tracker_output_is_NULL      = FBK_ZERO_UINT;
   p_lcda_output->core_output.LCDA_errors.f_CheckBSW_cals_is_NULL                = FBK_ZERO_UINT;
   p_lcda_output->core_output.LCDA_errors.f_CheckBSW_candidateInfo_is_NULL       = FBK_ZERO_UINT;
   p_lcda_output->core_output.LCDA_errors.f_CheckCVW_LCDA_input_is_NULL          = FBK_ZERO_UINT;
   p_lcda_output->core_output.LCDA_errors.f_CheckCVW_vehicle_data_is_NULL        = FBK_ZERO_UINT;
   p_lcda_output->core_output.LCDA_errors.f_CheckCVW_tracker_output_is_NULL      = FBK_ZERO_UINT;
   p_lcda_output->core_output.LCDA_errors.f_CheckCVW_cals_is_NULL                = FBK_ZERO_UINT;
   p_lcda_output->core_output.LCDA_errors.f_CheckCVW_candidateInfo_is_NULL       = FBK_ZERO_UINT;
   p_lcda_output->core_output.LCDA_errors.unused                                 = FBK_ZERO_UINT;

   /* Unavailable values */
   for (idx = FBK_ZERO_UINT; idx < LCDA_RNA_NUMBER_OF_OBJECTS; idx++)
   {
      p_lcda_output->customer_output.LKA_Object_Left[idx].lka_curvi_pos_long   = LCDA_LKA_NA_CURVI_POS_LONG;
      p_lcda_output->customer_output.LKA_Object_Left[idx].lka_curvi_pos_lat    = LCDA_LKA_NA_CURVI_POS_LAT;
      p_lcda_output->customer_output.LKA_Object_Left[idx].lka_curvi_vel_long   = LCDA_LKA_NA_CURVI_VEL_LONG;
      p_lcda_output->customer_output.LKA_Object_Left[idx].lka_curvi_vel_lat    = LCDA_LKA_NA_CURVI_VEL_LAT;
      p_lcda_output->customer_output.LKA_Object_Left[idx].lka_ttc              = LCDA_LKA_NA_TTC;
      p_lcda_output->customer_output.LKA_Object_Left[idx].lka_object_class     = LCDA_LKA_NA_OBJECT_CLASS;
      p_lcda_output->customer_output.LKA_Object_Left[idx].lka_alert_condition  = LCDA_LKA_NA_ALERT_CONDITION;
      p_lcda_output->customer_output.LKA_Object_Left[idx].lka_motion_class     = LCDA_LKA_NA_MOTION_CLASS;
      p_lcda_output->customer_output.LKA_Object_Left[idx].lka_change_status    = LCDA_LKA_NA_CHANGE_STATUS;
      p_lcda_output->customer_output.LKA_Object_Left[idx].lka_obj_id           = LCDA_LKA_NA_OBJECT_ID;
      p_lcda_output->customer_output.LKA_Object_Left[idx].lka_vcs_pos_long     = FBK_ZERO_F;
      p_lcda_output->customer_output.LKA_Object_Left[idx].lka_vcs_pos_lat      = FBK_ZERO_F;
      p_lcda_output->customer_output.LKA_Object_Left[idx].lka_vcs_vel_long     = FBK_ZERO_F;
      p_lcda_output->customer_output.LKA_Object_Left[idx].lka_vcs_vel_lat      = FBK_ZERO_F;
      p_lcda_output->customer_output.LKA_Object_Left[idx].lka_tracker_id       = FBK_ZERO_UINT;
      p_lcda_output->customer_output.LKA_Object_Right[idx].lka_curvi_pos_long  = LCDA_LKA_NA_CURVI_POS_LONG;
      p_lcda_output->customer_output.LKA_Object_Right[idx].lka_curvi_pos_lat   = LCDA_LKA_NA_CURVI_POS_LAT;
      p_lcda_output->customer_output.LKA_Object_Right[idx].lka_curvi_vel_long  = LCDA_LKA_NA_CURVI_VEL_LONG;
      p_lcda_output->customer_output.LKA_Object_Right[idx].lka_curvi_vel_lat   = LCDA_LKA_NA_CURVI_VEL_LAT;
      p_lcda_output->customer_output.LKA_Object_Right[idx].lka_ttc             = LCDA_LKA_NA_TTC;
      p_lcda_output->customer_output.LKA_Object_Right[idx].lka_object_class    = LCDA_LKA_NA_OBJECT_CLASS;
      p_lcda_output->customer_output.LKA_Object_Right[idx].lka_alert_condition = LCDA_LKA_NA_ALERT_CONDITION;
      p_lcda_output->customer_output.LKA_Object_Right[idx].lka_motion_class    = LCDA_LKA_NA_MOTION_CLASS;
      p_lcda_output->customer_output.LKA_Object_Right[idx].lka_change_status   = LCDA_LKA_NA_CHANGE_STATUS;
      p_lcda_output->customer_output.LKA_Object_Right[idx].lka_obj_id          = LCDA_LKA_NA_OBJECT_ID;
      p_lcda_output->customer_output.LKA_Object_Right[idx].lka_vcs_pos_long    = FBK_ZERO_F;
      p_lcda_output->customer_output.LKA_Object_Right[idx].lka_vcs_pos_lat     = FBK_ZERO_F;
      p_lcda_output->customer_output.LKA_Object_Right[idx].lka_vcs_vel_long    = FBK_ZERO_F;
      p_lcda_output->customer_output.LKA_Object_Right[idx].lka_vcs_vel_lat     = FBK_ZERO_F;
      p_lcda_output->customer_output.LKA_Object_Right[idx].lka_tracker_id      = FBK_ZERO_UINT;
   }
}

static void Lcda_Copy_Core_To_Cust_Output(const Lcda_Core_Output_T *p_core_output, Lcda_Output_T *p_lcda_output)
{
   if (p_core_output->lcda_status == LCDA_STATUS_ACTIVE)
   {
      p_lcda_output->core_output.f_lcda_enabled = FBK_ONE_UINT;
   }

   if (p_core_output->bsw_core_output.f_bsw_is_enabled)
   {
      p_lcda_output->core_output.f_bsw_enabled = FBK_ONE_UINT;
   }

   if (p_core_output->cvw_core_output.f_cvw_is_enabled)
   {
      p_lcda_output->core_output.f_cvw_enabled = FBK_ONE_UINT;
   }

   if (p_core_output->bsw_core_output.bsw_alert[FBK_SIDE_LEFT] != LCDA_ALERT_STATE_NONE)
   {
      p_lcda_output->core_output.bsw_alert_left = FBK_ONE_UINT;
   }

   if (p_core_output->bsw_core_output.bsw_alert[FBK_SIDE_RIGHT] != LCDA_ALERT_STATE_NONE)
   {
      p_lcda_output->core_output.bsw_alert_right = FBK_ONE_UINT;
   }

   if (p_core_output->cvw_core_output.cvw_alert[FBK_SIDE_LEFT] != LCDA_ALERT_STATE_NONE)
   {
      p_lcda_output->core_output.cvw_alert_left = FBK_ONE_UINT;
   }

   if (p_core_output->cvw_core_output.cvw_alert[FBK_SIDE_RIGHT] != LCDA_ALERT_STATE_NONE)
   {
      p_lcda_output->core_output.cvw_alert_right = FBK_ONE_UINT;
   }

   p_lcda_output->core_output.bsw_id_left  = p_core_output->bsw_core_output.bsw_id[FBK_SIDE_LEFT];
   p_lcda_output->core_output.bsw_id_right = p_core_output->bsw_core_output.bsw_id[FBK_SIDE_RIGHT];

   p_lcda_output->core_output.cvw_id_left   = p_core_output->cvw_core_output.cvw_id[FBK_SIDE_LEFT];
   p_lcda_output->core_output.cvw_id_right  = p_core_output->cvw_core_output.cvw_id[FBK_SIDE_RIGHT];
   p_lcda_output->core_output.cvw_ttc_left  = p_core_output->cvw_core_output.cvw_ttc[FBK_SIDE_LEFT];
   p_lcda_output->core_output.cvw_ttc_right = p_core_output->cvw_core_output.cvw_ttc[FBK_SIDE_RIGHT];
}

static boolean_T Lcda_Check_If_Obj_In_Zone_Lka(const Pa_Data_T *p_pa_data, const uint8_t tracker_obj_index, const uint8_t side)
{
   const LKA_Zone_T *zone;
   Vector_2d_T object_pos;
   float32_T side_modificator;
   const Fbk_Object_Data_T *p_object_data = &p_pa_data->object_data[tracker_obj_index];

   if (FBK_SIDE_LEFT == side)
   {
      side_modificator = -1.0f;
   }
   else
   {
      side_modificator = 1.0f;
   }

   if (F_Was_Obj_In_Zone[side][tracker_obj_index])
   {
      zone = &Lka_Zone_Hys; /* Use hys zone */
   }
   else
   {
      zone = &Lka_Zone; /* Use basic zone */
   }

   object_pos.x = p_object_data->curvi_pos.x;
   object_pos.y = p_object_data->curvi_pos.y;

   /* TODO: Replace this zone checking method with Is_Point_In_Convex_Polygon_Ray_Casting_Method() */
   if (((side_modificator * object_pos.y) > zone->lat_start)
       && ((side_modificator * object_pos.y) < (zone->lat_end + (p_object_data->width / 2.0f)))
       && (object_pos.x < (zone->lon_start + (p_object_data->length / 2.0f))) && (object_pos.x > zone->lon_end)) /* track inside
                                                                                                                    the LKA zone */
   {
      F_Was_Obj_In_Zone[side][tracker_obj_index] = FBK_TRUE;
   }
   else
   {
      F_Was_Obj_In_Zone[side][tracker_obj_index] = FBK_FALSE;
   }
   return F_Was_Obj_In_Zone[side][tracker_obj_index];
}

static void Lcda_Get_Relevant_Objects_Lka(const Pa_Data_T *p_pa_data,
                                          uint8_t rel_object_indexes[FBK_NUMBER_OF_SIDES][PA_OBJ_NUMBER_OF_OBJECTS],
                                          uint8_t num_of_relevant_objects[FBK_NUMBER_OF_SIDES])
{
   uint8_t i, j, side, temp_obj_index;

   num_of_relevant_objects[FBK_SIDE_LEFT]  = FBK_ZERO_UINT;
   num_of_relevant_objects[FBK_SIDE_RIGHT] = FBK_ZERO_UINT;

   for (i = FBK_ZERO_UINT; i < (uint8_t) PA_OBJ_NUMBER_OF_OBJECTS; i++)
   {
      const Fbk_Object_Data_T *p_object = &p_pa_data->object_data[i];
      side                              = Fbk_Get_Obj_Side(p_object->curvi_pos.y);

      if ((PA_OBJ_STATUS_INVALID != p_object->status)
          && (p_object->curvi_vel.x > LCDA_RNA_THRESHOLD_VEL_FOR_ONCOMING_OBJECTS)) /* object is valid and not
                                                                                                         oncoming */
      {
         if (Lcda_Check_If_Obj_In_Zone_Lka(p_pa_data, i, side)) /* track inside the LKA zone */
         {
            rel_object_indexes[side][num_of_relevant_objects[side]] = i;
            num_of_relevant_objects[side]++;
         }
      }
   }

   /* Sort the list (bubble sorting): */
   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      for (i = FBK_ZERO_UINT; (int16_t) i < ((int16_t) num_of_relevant_objects[side] - 1); i++)
      {
         for (j = (uint8_t) (i + FBK_ONE_UINT); j < num_of_relevant_objects[side]; j++)
         {
            if (p_pa_data->object_data[rel_object_indexes[side][i]].curvi_pos.x
                < p_pa_data->object_data[rel_object_indexes[side][j]].curvi_pos.x)
            {
               /*swap*/
               temp_obj_index              = rel_object_indexes[side][j];
               rel_object_indexes[side][j] = rel_object_indexes[side][i];
               rel_object_indexes[side][i] = temp_obj_index;
            }
         }
      }
   }
}

static RENAULT_LSS_ALERT_CONDITION_T
Lcda_Get_Alert_Condition_Lka(const Lcda_Output_T *p_lcda_output, float32_T curvi_long_vel_rel, uint8_t tracker_id, uint8_t side)
{
   RENAULT_LSS_ALERT_CONDITION_T alert_condition = RENAULT_LSS_ALERT_CONDITION_OFF;

   if (((side == FBK_SIDE_LEFT)
        && ((p_lcda_output->core_output.cvw_id_left == tracker_id) || (p_lcda_output->core_output.bsw_id_left == tracker_id)))
       || ((side == FBK_SIDE_RIGHT)
           && ((p_lcda_output->core_output.cvw_id_right == tracker_id) || (p_lcda_output->core_output.bsw_id_right == tracker_id))))
   {
      if (curvi_long_vel_rel > FBK_ZERO_F) /* Simple implementation, not sure if it is in line with requirements */
      {
         alert_condition = RENAULT_LSS_ALERT_CONDITION_TOS;
      }
      else
      {
         alert_condition = RENAULT_LSS_ALERT_CONDITION_SOT;
      }
   }

   return alert_condition;
}

static RENAULT_LSS_OBJECT_CLASS_T Lcda_Get_Object_Class_Lka(Pa_Obj_Class_T object_class)
{
   RENAULT_LSS_OBJECT_CLASS_T rna_object_class;
   switch (object_class)
   {
      case PA_OBJ_CLASS_UNKNOWN:
      {
         rna_object_class = RENAULT_LSS_OBJECT_CLASS_UNKNOWN;
         break;
      }
      case PA_OBJ_CLASS_CAR:
      {
         rna_object_class = RENAULT_LSS_OBJECT_CLASS_CAR;
         break;
      }
      case PA_OBJ_CLASS_TRUCK:
      {
         rna_object_class = RENAULT_LSS_OBJECT_CLASS_TRUCK;
         break;
      }
      case PA_OBJ_CLASS_2WHEEL:
      {
         rna_object_class = RENAULT_LSS_OBJECT_CLASS_MOTORCYCLE;
         break;
      }
      case PA_OBJ_CLASS_PEDESTRIAN:
      {
         rna_object_class = RENAULT_LSS_OBJECT_CLASS_PEDESTRIAN;
         break;
      }
      default:
      {
         rna_object_class = RENAULT_LSS_OBJECT_CLASS_OTHER;
         break;
      }
   }
   return rna_object_class;
}


static RENAULT_LSS_CHANGE_STATUS_T Lcda_Get_Change_Status_Lka(Pa_Obj_Status_T track_status)
{
   RENAULT_LSS_CHANGE_STATUS_T rna_change_status;
   if (PA_OBJ_STATUS_NEW == track_status)
   {
      rna_change_status = RENAULT_LSS_CHANGE_STATUS_CHANGE;
   }
   else
   {
      rna_change_status = RENAULT_LSS_CHANGE_STATUS_NO_CHANGE;
   }

   return rna_change_status;
}


static RENAULT_LSS_MOTION_CLASS_T Lcda_Get_Motion_Class_Lka(Pa_Obj_Status_T track_status)
{
   RENAULT_LSS_MOTION_CLASS_T rna_motion_class;
   if (PA_OBJ_STATUS_COASTED == track_status)
   {
      rna_motion_class = RENAULT_LSS_MOTION_CLASS_UNKNOWN;
   }
   else
   {
      rna_motion_class = RENAULT_LSS_MOTION_CLASS_MOVING_OBJECT;
   }
   return rna_motion_class;
}


static float32_T Lcda_Calculate_Ttc_Lka(const Pa_Data_T *p_pa_data,
                                        const Lcda_Core_Calibration_T *p_cals,
                                        uint8_t temp_obj_index,
                                        Vector_2d_T target_ref)
{
   const Fbk_Object_Data_T *p_tmp_object = &p_pa_data->object_data[temp_obj_index];
   float32_T ttc;
   if (p_tmp_object->curvi_vel_rel.x > p_cals->k_lka_min_rel_vel_for_ttc)
   {
      ttc = (-(target_ref.x - p_pa_data->vehicle_data.rear_axle_position) + p_cals->k_cvw_ttc_long_calculation_offset)
            / p_tmp_object->curvi_vel_rel.x;

      if (ttc < FBK_ZERO_F)
      {
         ttc = LCDA_LKA_MIN_TTC;
      }
   }
   else
   {
      ttc = LCDA_LKA_MAX_TTC;
   }
   return ttc;
}

static uint8_t Lcda_Update_Ids_In_Use_Lka(LKA_Object_T objects_from_prev_iteration[FBK_NUMBER_OF_SIDES][LCDA_RNA_NUMBER_OF_OBJECTS],
                                          const LKA_Object_T p_lka_object[LCDA_RNA_NUMBER_OF_OBJECTS],
                                          boolean_T ids_in_use[FBK_NUMBER_OF_SIDES][LCDA_RNA_NUMBER_OF_USED_IDS],
                                          uint8_t side)
{
   uint8_t k;
   uint8_t lastly_freed_id = FBK_ZERO_UINT;

   /* Update ids in use */
   for (k = FBK_ZERO_UINT; k < LCDA_RNA_NUMBER_OF_USED_IDS; k++)
   {
      if (ids_in_use[side][k]) /* if id = k + 1 is in use */
      {
         /* find object that had id = k + 1 */
         uint8_t j;

         boolean_T still_in_use = FBK_FALSE;
         int8_t index           = -1;
         for (j = FBK_ZERO_UINT; j < LCDA_RNA_NUMBER_OF_OBJECTS; j++)
         {
            if (objects_from_prev_iteration[side][j].lka_obj_id == (k + FBK_ONE_UINT))
            {
               index = (int8_t) j;
               break;
            }
         }

         /* After finding that object, check if it is present in current iteration */
         if (-1 != index)
         {
            for (j = FBK_ZERO_UINT; j < LCDA_RNA_NUMBER_OF_OBJECTS; j++)
            {
               if (objects_from_prev_iteration[side][index].lka_tracker_id == p_lka_object[j].lka_tracker_id)
               {
                  still_in_use = FBK_TRUE;
                  break;
               }
            }
         }
         else
         {
            assert(FBK_FALSE && "this should never be reached. Added 'if' block to fix MISRA finding. Review code!");
         }

         /* If object with id = k + 1 is not present, free that id */
         if (!still_in_use)
         {
            lastly_freed_id     = (uint8_t) (k + FBK_ONE_UINT);
            ids_in_use[side][k] = FBK_FALSE;
         }
      }
   }


   return lastly_freed_id;
}

static uint8_t Lcda_Get_Obj_Id_Lka(LKA_Object_T objects_from_prev_iteration[FBK_NUMBER_OF_SIDES][LCDA_RNA_NUMBER_OF_OBJECTS],
                                   boolean_T ids_in_use[FBK_NUMBER_OF_SIDES][LCDA_RNA_NUMBER_OF_USED_IDS],
                                   uint8_t tracker_id,
                                   uint8_t side,
                                   uint8_t lastly_freed_id)
{
   uint8_t lka_obj_id;

   if (FBK_ZERO_UINT == tracker_id)
   {
      lka_obj_id = FBK_ZERO_UINT;
   }
   else
   {
      uint8_t obj_id = FBK_ZERO_UINT;
      uint8_t j;

      /* Try to find object in table from previous iteration */
      for (j = FBK_ZERO_UINT; j < LCDA_RNA_NUMBER_OF_OBJECTS; j++)
      {
         if (tracker_id == objects_from_prev_iteration[side][j].lka_tracker_id)
         {
            obj_id = objects_from_prev_iteration[side][j].lka_obj_id;
         }
      }

      if (obj_id != FBK_ZERO_UINT)
      {
         /* If found, prescribe its id to the new struct */
         lka_obj_id = obj_id;
      }
      else
      {
         /* If not found, fill obj_id with first that is currently free */
         for (j = FBK_ZERO_UINT; j < LCDA_RNA_NUMBER_OF_USED_IDS; j++)
         {
            if (!ids_in_use[side][j] && ((j + FBK_ONE_UINT) != lastly_freed_id))
            {
               obj_id              = (uint8_t) (j + FBK_ONE_UINT);
               ids_in_use[side][j] = FBK_TRUE;
               break;
            }
         }
         lka_obj_id = obj_id;
      }
   }

   return lka_obj_id;
}

static void Lcda_Fill_Output_List_Lka(uint8_t side,
                                      Lcda_Output_T *p_lcda_output,
                                      const Pa_Data_T *p_pa_data,
                                      const Lcda_Core_Calibration_T *p_cals,
                                      LKA_Object_T objects_from_prev_iteration[FBK_NUMBER_OF_SIDES][LCDA_RNA_NUMBER_OF_OBJECTS],
                                      uint8_t rel_object_indexes[FBK_NUMBER_OF_SIDES][PA_OBJ_NUMBER_OF_OBJECTS],
                                      const uint8_t num_of_relevant_objects[FBK_NUMBER_OF_SIDES])
{
   uint8_t i, temp_obj_index, freed_id;
   LKA_Object_T *p_lka_object;
   Vector_2d_T target_ref;

   if (FBK_SIDE_RIGHT == side)
   {
      p_lka_object = &p_lcda_output->customer_output.LKA_Object_Right[FBK_ZERO_UINT];
   }
   else
   {
      p_lka_object = &p_lcda_output->customer_output.LKA_Object_Left[FBK_ZERO_UINT];
   }

   /* fill output list */
   for (i = 0; i < LCDA_RNA_NUMBER_OF_OBJECTS; i++)
   {
      /* Make sure that there is still a relevant object to be filled */
      if (i < num_of_relevant_objects[side])
      {
         const Fbk_Object_Data_T *p_object;
         temp_obj_index = rel_object_indexes[side][i];
         p_object       = &p_pa_data->object_data[temp_obj_index];
         /* Data not published on CAN */
         p_lka_object[i].lka_vcs_pos_long = p_object->vcs_pos.x;
         p_lka_object[i].lka_vcs_pos_lat  = p_object->vcs_pos.y;
         p_lka_object[i].lka_vcs_vel_long = p_object->vcs_vel.x;
         p_lka_object[i].lka_vcs_vel_lat  = p_object->vcs_vel.y;
         p_lka_object[i].lka_tracker_id   = p_object->id;
         /* Data published on CAN */
         target_ref = Lcda_Calculate_Target_Reference_Point(temp_obj_index, p_pa_data, side);
         /* lka_curvi_long/lat is position of a reference position of an object */
         p_lka_object[i].lka_curvi_pos_long = target_ref.x;
         p_lka_object[i].lka_curvi_pos_lat  = target_ref.y;

         /* lka_curvi_vel_long/lat are relative velocities of a center point of an object*/
         p_lka_object[i].lka_curvi_vel_long = p_object->curvi_vel_rel.x;
         p_lka_object[i].lka_curvi_vel_lat  = p_object->curvi_vel_rel.y;

         /* Map object classes from tracker to RNA_SWEET400 object classes*/
         p_lka_object[i].lka_object_class = Lcda_Get_Object_Class_Lka(p_object->obj_class);

         /* Determine motion class, TODO: moving to stopped object*/
         p_lka_object[i].lka_motion_class = Lcda_Get_Motion_Class_Lka(p_object->status);

         /* Determine change status*/
         p_lka_object[i].lka_change_status = Lcda_Get_Change_Status_Lka(p_object->status);

         /* Calculate ttc*/
         p_lka_object[i].lka_ttc = Lcda_Calculate_Ttc_Lka(p_pa_data, p_cals, temp_obj_index, target_ref);

         /* Determine alert condition*/
         p_lka_object[i].lka_alert_condition =
            Lcda_Get_Alert_Condition_Lka(p_lcda_output, p_object->curvi_vel_rel.x, p_object->id, side);

         /* Tranform coordinates for RNA_SWEET400*/
         Lcda_Transform_Customer_Output_Coords_Rna(p_lka_object, i, p_pa_data);

         /* Saturate signals to match CAN msgs definitions */
         Lcda_Saturate_Signals_Lka(p_lka_object, i);
      }
      else
      {
         break;
      }
   }

   freed_id = Lcda_Update_Ids_In_Use_Lka(objects_from_prev_iteration, p_lka_object, F_Ids_In_Use, side);

   if (Fbk_Is_True(freed_id))
   {
      Lastly_Freed_Ids[side] = freed_id;
   }

   /* fill obj_id in separate loop, because it uses data filled in previous loop*/
   for (i = FBK_ZERO_UINT; i < LCDA_RNA_NUMBER_OF_OBJECTS; i++)
   {
      if (i < num_of_relevant_objects[side])
      {
         /* Determine obj id*/
         p_lka_object[i].lka_obj_id = Lcda_Get_Obj_Id_Lka(objects_from_prev_iteration, F_Ids_In_Use,
                                                          p_lka_object[i].lka_tracker_id, side, Lastly_Freed_Ids[side]);
      }
   }
}

static void Lcda_Customer_Target_Selection(const Pa_Data_T *p_pa_data, Lcda_Output_T *p_lcda_output, const Lcda_Core_Calibration_T *p_cals)
{
   uint8_t i, side;
   uint8_t rel_objects_indexes[FBK_NUMBER_OF_SIDES][PA_OBJ_NUMBER_OF_OBJECTS] = {{0}};
   uint8_t num_of_relevant_objects[FBK_NUMBER_OF_SIDES]                       = {0};

   static boolean_T f_lka_zones_initialized = FBK_FALSE;

   if (!f_lka_zones_initialized)
   {
      Lcda_Initialize_Zones_LKA(p_pa_data, p_cals);
      f_lka_zones_initialized = FBK_TRUE;
   }

   Lcda_Get_Relevant_Objects_Lka(p_pa_data, rel_objects_indexes, num_of_relevant_objects);


   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      Lcda_Fill_Output_List_Lka(side, p_lcda_output, p_pa_data, p_cals, Obj_From_Prev_Iteration, rel_objects_indexes,
                                num_of_relevant_objects);
   }


   for (i = FBK_ZERO_UINT; i < LCDA_RNA_NUMBER_OF_OBJECTS; i++)
   {
      Obj_From_Prev_Iteration[FBK_SIDE_LEFT][i]  = p_lcda_output->customer_output.LKA_Object_Left[i];
      Obj_From_Prev_Iteration[FBK_SIDE_RIGHT][i] = p_lcda_output->customer_output.LKA_Object_Right[i];
   }
}

static void Lcda_Initialize_Zones_LKA(const Pa_Data_T *p_pa_data, const Lcda_Core_Calibration_T *p_cals)
{
   Lka_Zone.lat_start = (0.5f * p_pa_data->vehicle_data.host_width) + p_cals->k_bsw_y0;
   Lka_Zone.lat_end   = Lka_Zone.lat_start + p_cals->k_lka_ov_zone_width;
   Lka_Zone.lon_start = p_cals->k_bsw_x0 - p_pa_data->vehicle_data.host_length;
   Lka_Zone.lon_end   = Lka_Zone.lon_start + p_cals->k_cvw_x_length0 + p_cals->k_cvw_x_length1;

   Lka_Zone_Hys.lat_start = Lka_Zone.lat_start - p_cals->k_bsw_y0_hys;
   Lka_Zone_Hys.lat_end   = Lka_Zone.lat_end + p_cals->k_bsw_y0_hys;
   Lka_Zone_Hys.lon_start = Lka_Zone.lon_start + LCDA_LKA_ZONE_HYS; /* TODO: HARDCODED - Add calibration parameter if necessary */
   Lka_Zone_Hys.lon_end   = Lka_Zone.lon_end - LCDA_LKA_ZONE_HYS;   /* TODO: HARDCODED - Add calibration parameter if necessary */
}

static float32_T Lcda_Max_Min(float32_T min_arg_1, float32_T min_arg_2, float32_T max_arg_2)
{
   return Max(Min(min_arg_1, min_arg_2), max_arg_2);
}

static void Lcda_Saturate_Signals_Lka(LKA_Object_T *p_lka_object, uint8_t index)
{
   p_lka_object[index].lka_curvi_pos_long =
      Lcda_Max_Min(p_lka_object[index].lka_curvi_pos_long, LCDA_LKA_MAX_CURVI_POS_LONG, LCDA_LKA_MIN_CURVI_POS_LONG);
   p_lka_object[index].lka_curvi_pos_lat =
      Lcda_Max_Min(p_lka_object[index].lka_curvi_pos_lat, LCDA_LKA_MAX_CURVI_POS_LAT, LCDA_LKA_MIN_CURVI_POS_LAT);
   p_lka_object[index].lka_curvi_vel_long =
      Lcda_Max_Min(p_lka_object[index].lka_curvi_vel_long, LCDA_LKA_MAX_CURVI_VEL_LONG, LCDA_LKA_MIN_CURVI_VEL_LONG);
   p_lka_object[index].lka_curvi_vel_lat =
      Lcda_Max_Min(p_lka_object[index].lka_curvi_vel_lat, LCDA_LKA_MAX_CURVI_VEL_LAT, LCDA_LKA_MIN_CURVI_VEL_LAT);
   p_lka_object[index].lka_ttc = Lcda_Max_Min(p_lka_object[index].lka_ttc, LCDA_LKA_MAX_TTC, LCDA_LKA_MIN_TTC);
}

static float32_T Lcda_Transform_Coord_Lka_X(const float32_T x_coord, const Pa_Data_T *p_pa_data)
{
   return x_coord - p_pa_data->vehicle_data.rear_axle_position;
}

static float32_T Lcda_Transform_Coord_Lka_Y(const float32_T y_coord)
{
   return -y_coord;
}

static void Lcda_Transform_Customer_Output_Coords_Rna(LKA_Object_T *p_lka_object, const uint8_t index, const Pa_Data_T *p_pa_data)
{
   p_lka_object[index].lka_curvi_pos_lat  = Lcda_Transform_Coord_Lka_Y(p_lka_object[index].lka_curvi_pos_lat);
   p_lka_object[index].lka_curvi_pos_long = Lcda_Transform_Coord_Lka_X(p_lka_object[index].lka_curvi_pos_long, p_pa_data);
   p_lka_object[index].lka_curvi_vel_lat  = Lcda_Transform_Coord_Lka_Y(p_lka_object[index].lka_curvi_vel_lat);
}

static Vector_2d_T Lcda_Calculate_Target_Reference_Point(const uint8_t index, const Pa_Data_T *p_pa_data, const uint8_t side)
{
   float32_T half_length;
   float32_T half_width;
   Vector_2d_T target_object_f;
   Vector_2d_T target_object_c;
   Vector_2d_T target_object_r;
   Vector_2d_T target_ref_f;
   Vector_2d_T target_ref_c;
   Vector_2d_T target_ref_r;
   Vector_2d_T target_ref;
   Angle_T heading;
   Vector_2d_T curvi_position;

   const Fbk_Object_Data_T *p_object_data;
   const Fbk_Vehicle_Data_T *p_vehicle_data;


   assert(p_pa_data != NULL);

   p_object_data  = &p_pa_data->object_data[index];
   p_vehicle_data = &p_pa_data->vehicle_data;

   curvi_position.x = p_object_data->curvi_pos.x;
   curvi_position.y = p_object_data->curvi_pos.y;
   heading          = Create_Angle(p_object_data->curvi_heading);
   half_length      = p_object_data->length * 0.5f;
   half_width       = p_object_data->width * 0.5f;

   /*Calculate relative positions of 3 critical points of the target*/
   if (side == FBK_SIDE_LEFT)
   {
      target_object_f = Create_2d_Vector_Coordinates(half_length, half_width);
      target_object_c = Create_2d_Vector_Coordinates(FBK_ZERO_F, half_width); /*If the target is at rear axle position it should
                                                                        have position = 0 */
      target_object_r = Create_2d_Vector_Coordinates(-half_length, half_width);
   }
   else
   {
      target_object_f = Create_2d_Vector_Coordinates(half_length, -half_width);
      target_object_c = Create_2d_Vector_Coordinates(FBK_ZERO_F, -half_width); /*If the target is at rear axle position it should
                                                                         have position = 0 */
      target_object_r = Create_2d_Vector_Coordinates(-half_length, -half_width);
   }

   /*Application of 2D rotation matrix */
   target_ref_f = Vector_2d_Alg_Rotate(&(heading), &(target_object_f));
   target_ref_c = Vector_2d_Alg_Rotate(&(heading), &(target_object_c));
   target_ref_r = Vector_2d_Alg_Rotate(&(heading), &(target_object_r));

   /*Shifting rotated corner points depending on vehicle position*/
   target_ref_f = Vector_2d_Alg_Add(&(curvi_position), &(target_ref_f));
   target_ref_c = Vector_2d_Alg_Add(&(curvi_position), &(target_ref_c));
   target_ref_r = Vector_2d_Alg_Add(&(curvi_position), &(target_ref_r));

   /*Choose relevant critical point relative to rear axle*/
   if (target_ref_r.x > p_vehicle_data->rear_axle_position)
   {
      target_ref = target_ref_r;
   }
   else if (target_ref_f.x < p_vehicle_data->rear_axle_position)
   {
      target_ref = target_ref_f;
   }
   else
   {
      target_ref.y = target_ref_c.y;
      target_ref.x = p_vehicle_data->rear_axle_position;
   }

   return target_ref;
}
