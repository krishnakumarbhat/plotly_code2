/**
 * @file recw_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the BMW SRR5 post run logic for RECW.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */
/*===========================================================================*\
 * Includes
\*===========================================================================*/

#include "recw_post_run.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "recw_bmw_sp25_types.h"
#include "recw_core_input_t.h"
#include "recw_core_output_t.h"
#include "recw_input_t.h"
#include "recw_output_t.h"
#include "recw_state_machine.h"
#include <assert.h>

#ifdef BINARY_DEBUG
#include "fbk_object_data_t.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "recw_core_input_t.h"

#include "recw_debug_writer.h"
#endif

/*===========================================================================*\
* Local Function Prototypes
\*===========================================================================*/

#ifdef BINARY_DEBUG
static void Write_Recw_Output(const Recw_Output_T *p_ced_output);
#endif /* BINARY_DEBUG */

/**
 * @brief Resets RECW output.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2961}
 * @SDD{SF-7999}
 * @verification{}
 */
static void Recw_Reset_Bmw_Sp25_Output(Recw_Output_T *p_recw_output /**< Recw output */);

/**
 * @brief Updates RECW output from RECW core output.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2961}
 * @SDD{SF-8005}
 * @verification{}
 */
static void Recw_Update_Bmw_Sp25_Output(Recw_Output_T *p_recw_output /**< Recw output */,
                                        const Recw_Input_T *p_recw_input /**< Recw input */,
                                        const Recw_Core_Output_T *p_core_output /**< Recw core output */,
                                        const Pa_Data_T *p_pa_data /**< Platform abstraction data */);

/**
 * @brief Maps tracker object class to RECW BMW SRR5 CDC object class.
 *
 * @return RECW BMW SRR5 CDC object class of object
 *
 * @SRS{}
 * @SAE{SF-2961}
 * @SDD{SF-8002}
 * @verification{}
 */
static Recw_Bmw_Sp25_Object_Class_Cdc_T Recw_Map_Object_Class_Cdc(Pa_Obj_Class_T pa_obj_class /**< PA object class */,
                                                                  float32_T obj_speed /**< object speed */);


/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

// clang-format off
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
void Recw_Post_Run(const Recw_Instance_T *p_recw_instance, const Recw_Input_T *p_recw_input, Recw_Output_T *p_recw_output)
// clang-format on
{
   /* check NULL pointers */
   assert(NULL != p_recw_instance);
   assert(NULL != p_recw_input);
   assert(NULL != p_recw_output);

   Recw_Reset_Bmw_Sp25_Output(p_recw_output);
   Recw_Update_Bmw_Sp25_Output(p_recw_output, p_recw_input, &p_recw_instance->core_output, p_recw_instance->core_input.p_pa_data);

   /* Write bin file output */
#ifdef BINARY_DEBUG
   Write_Recw_Output(p_recw_output);
#endif
} /* end of function */

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static void Recw_Reset_Bmw_Sp25_Output(Recw_Output_T *p_recw_output)
{
   p_recw_output->recw_status = (uint8_t) RECW_BMW_SP25_INACTIVE;

   p_recw_output->recw_status_collision_warning = (uint8_t) RECW_BMW_SP25_WARNING_LEVEL_WARNING_INACTIVE;
   p_recw_output->recw_status_precrash          = (uint8_t) RECW_BMW_SP25_WARNING_LEVEL_PRECRASH_INACTIVE;
   p_recw_output->recw_obj_id                   = FBK_ZERO_UINT;
   p_recw_output->recw_obj_unique_id            = FBK_ZERO_UINT;
   p_recw_output->recw_ttc                      = FBK_ZERO_F;
   p_recw_output->recw_obj_distance             = FBK_ZERO_F;
   p_recw_output->recw_obj_approach_speed       = FBK_ZERO_F;
   p_recw_output->recw_obj_lat_pos              = FBK_ZERO_F;
   p_recw_output->recw_obj_long_pos             = FBK_ZERO_F;
   p_recw_output->recw_obj_heading              = FBK_ZERO_F;
   p_recw_output->recw_crash_probability        = FBK_ZERO_F;
   p_recw_output->recw_overlap                  = FBK_ZERO_F;

   p_recw_output->recw_ttc_warning_threshold = FBK_ZERO_F;

   p_recw_output->recw_obj_class     = FBK_ZERO_UINT;
   p_recw_output->recw_obj_class_cdc = (uint8_t) RECW_BMW_SP25_OBJECT_CLASS_CDC_UNKNOWN;
   p_recw_output->recw_sm_state      = RECW_SM_NOT_AVAILABLE;
}

static void Recw_Update_Bmw_Sp25_Output(Recw_Output_T *p_recw_output,
                                        const Recw_Input_T *p_recw_input,
                                        const Recw_Core_Output_T *p_core_output,
                                        const Pa_Data_T *p_pa_data)
{
   Recw_State_Machine_Post_Run(p_recw_input, p_core_output, p_pa_data, p_recw_output);
   if (Fbk_Is_True(p_recw_input->c_recw_enable) && (RECW_BMW_SP25_TYPE_NO_FUNCTION != p_recw_input->recw_type))
   {
      p_recw_output->recw_status = (uint8_t) RECW_BMW_SP25_ACTIVE;

      if ((PA_INVALID_OBJ_ID != p_core_output->recw_id) && (RECW_NO_ALERT != p_core_output->recw_alert_level))
      {
         if ((RECW_ALERT_ACTIVE_LEVEL_1 == p_core_output->recw_alert_level)
             && ((RECW_BMW_SP25_TYPE_WARNING_AND_PRECRASH == p_recw_input->recw_type)
                 || (RECW_BMW_SP25_TYPE_WARNING_ONLY == p_recw_input->recw_type)))
         {
            p_recw_output->recw_ttc_warning_threshold = p_core_output->ttc_threshold_alert_level_1;
            p_recw_output->recw_obj_id                = p_core_output->recw_id;
            p_recw_output->recw_obj_unique_id         = p_core_output->recw_unique_id;
         }
         else if ((RECW_ALERT_ACTIVE_LEVEL_2 == p_core_output->recw_alert_level)
                  && ((RECW_BMW_SP25_TYPE_WARNING_AND_PRECRASH == p_recw_input->recw_type)
                      || (RECW_BMW_SP25_TYPE_PRECRASH_ONLY == p_recw_input->recw_type)))
         {
            p_recw_output->recw_obj_id            = p_core_output->recw_id;
            p_recw_output->recw_obj_unique_id     = p_core_output->recw_unique_id;
            p_recw_output->recw_crash_probability = RECW_BMW_SP25_CONVERT_TO_PERCENTAGE(p_core_output->recw_crash_prob_combined);
            p_recw_output->recw_ttc_warning_threshold = p_core_output->ttc_threshold_alert_level_1;
            p_recw_output->recw_obj_lat_pos           = p_pa_data->object_data[p_core_output->recw_index].vcs_pos.y;
            p_recw_output->recw_obj_long_pos          = p_pa_data->object_data[p_core_output->recw_index].vcs_pos.x;
            p_recw_output->recw_obj_class_cdc         = (uint8_t) Recw_Map_Object_Class_Cdc(
                       p_pa_data->object_data[p_core_output->recw_index].obj_class, p_pa_data->object_data[p_core_output->recw_index].speed);
         }
         else
         {
            /* Do nothing */
         }
      }
      else
      {
         /* Do nothing */
      }
   }
   else
   {
      /* Do nothing */
   }
}

static Recw_Bmw_Sp25_Object_Class_Cdc_T Recw_Map_Object_Class_Cdc(Pa_Obj_Class_T pa_obj_class, float32_T obj_speed)
{
   Recw_Bmw_Sp25_Object_Class_Cdc_T bmw_obj_class_cdc;

   switch (pa_obj_class)
   {
      case PA_OBJ_CLASS_UNKNOWN:
         bmw_obj_class_cdc = RECW_BMW_SP25_OBJECT_CLASS_CDC_UNKNOWN;
         break;
      case PA_OBJ_CLASS_PEDESTRIAN:
         bmw_obj_class_cdc = RECW_BMW_SP25_OBJECT_CLASS_CDC_PEDESTRIAN;
         break;
      case PA_OBJ_CLASS_2WHEEL:
         if (obj_speed < RECW_BMW_SP25_CDC_MAPPING_VELOCITY_THRESHOLD)
         {
            bmw_obj_class_cdc = RECW_BMW_SP25_OBJECT_CLASS_CDC_BICYCLE;
         }
         else
         {
            bmw_obj_class_cdc = RECW_BMW_SP25_OBJECT_CLASS_CDC_MOTORCYCLE;
         }
         break;
      case PA_OBJ_CLASS_CAR:
         bmw_obj_class_cdc = RECW_BMW_SP25_OBJECT_CLASS_CDC_CAR;
         break;
      case PA_OBJ_CLASS_TRUCK:
         bmw_obj_class_cdc = RECW_BMW_SP25_OBJECT_CLASS_CDC_TRUCK;
         break;
      default:
         bmw_obj_class_cdc = RECW_BMW_SP25_OBJECT_CLASS_CDC_UNKNOWN;
         break;
   }

   return bmw_obj_class_cdc;
}

#ifdef BINARY_DEBUG
static void Write_Recw_Output(const Recw_Output_T *p_recw_output)
{
   RECW_STORE_VAL_MGR_WPR("RECW_BMW_SP25_status", p_recw_output->recw_status);
   RECW_STORE_VAL_MGR_WPR("RECW_BMW_SP25_status_collision_warning", p_recw_output->recw_status_collision_warning);
   RECW_STORE_VAL_MGR_WPR("RECW_BMW_SP25_status_precrash", p_recw_output->recw_status_precrash);
   RECW_STORE_VAL_MGR_WPR("RECW_BMW_SP25_edr_drasy_event_ID62_status_collision_warning_side_radar_rear",
                          p_recw_output->edr_drasy_event_ID62_status_collision_warning_side_radar_rear);
   RECW_STORE_VAL_MGR_WPR("RECW_BMW_SP25_edr_drasy_event_ID62_status_pre_crash_side_radar_rear",
                          p_recw_output->edr_drasy_event_ID62_status_pre_crash_side_radar_rear);
   RECW_STORE_VAL_MGR_WPR("RECW_BMW_SP25_obj_id", p_recw_output->recw_obj_id);
   RECW_STORE_VAL_MGR_WPR("RECW_BMW_SP25_ttc", p_recw_output->recw_ttc);
   RECW_STORE_VAL_MGR_WPR("RECW_BMW_SP25_obj_distance", p_recw_output->recw_obj_distance);
   RECW_STORE_VAL_MGR_WPR("RECW_BMW_SP25_obj_approach_speed", p_recw_output->recw_obj_approach_speed);
   RECW_STORE_VAL_MGR_WPR("RECW_BMW_SP25_obj_lat_pos", p_recw_output->recw_obj_lat_pos);
   RECW_STORE_VAL_MGR_WPR("RECW_BMW_SP25_obj_long_pos", p_recw_output->recw_obj_long_pos);
   RECW_STORE_VAL_MGR_WPR("RECW_BMW_SP25_obj_heading", p_recw_output->recw_obj_heading);
   RECW_STORE_VAL_MGR_WPR("RECW_BMW_SP25_crash_probability", p_recw_output->recw_crash_probability);
   RECW_STORE_VAL_MGR_WPR("RECW_BMW_SP25_overlap", p_recw_output->recw_overlap);
   RECW_STORE_VAL_MGR_WPR("RECW_BMW_SP25_ttc_warning_threshold", p_recw_output->recw_ttc_warning_threshold);
   RECW_STORE_VAL_MGR_WPR("RECW_BMW_SP25_obj_class", p_recw_output->recw_obj_class);
   RECW_STORE_VAL_MGR_WPR("RECW_BMW_SP25_obj_class_cdc", p_recw_output->recw_obj_class_cdc);
   RECW_STORE_VAL_MGR_WPR("RECW_BMW_SP25_sm_state", p_recw_output->recw_sm_state);
}
#endif /* BINARY_DEBUG */
