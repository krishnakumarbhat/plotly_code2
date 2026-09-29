/**
 * @file ced_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the BMW_SP25 post run logic for CED.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ced_post_run.h"
#include "ced_bmw_sp25_types.h"
#include "ced_core_input_t.h"
#include "ced_core_output_t.h"
#include "ced_input_t.h"
#include "ced_post_run_boardnet.h"
#include "ced_post_run_boardnet_interior_light.h"
#include "ced_post_run_boardnet_mirror_led.h"
#include "ced_post_run_boardnet_optical_element.h"
#include "ced_state_machine.h"
#include "ced_types.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "ml_trigonometry.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "pt_output_t.h"

#include <assert.h>

#ifdef BINARY_DEBUG
#include "ced_debug_writer.h"
#endif /* BINARY_DEBUG */

/*===========================================================================*\
* Defines
\*===========================================================================*/

#define CED_KPH2MPS (0.277778f)
#define CED_K_OBJ_CLASS_MAP_VEL_MAX_THRES (30.0f)

/*===========================================================================*\
* Local Data Prototypes
\*===========================================================================*/

typedef enum
{
   SFE_OBJECT_CLASS_UNKNOWN    = (0), /**< 0*/
   SFE_OBJECT_CLASS_PEDESTRIAN = (1), /**< 1*/
   SFE_OBJECT_CLASS_MOTORCYCLE = (2), /**< 2*/
   SFE_OBJECT_CLASS_CAR        = (3), /**< 3*/
   SFE_OBJECT_CLASS_TRUCK      = (4), /**< 4*/
   SFE_OBJECT_CLASS_BICYCLE    = (5)  /**< 5*/
} SFE_object_class_T;

/*===========================================================================*\
* Local Function Prototypes
\*===========================================================================*/


/**
 * @brief Sets Bus Outputs To Default Values
 *
 * @return void
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
static void Ced_Reset_Bus_Signals_Output(Bmw_Ced_Output_Bus_Signals_T *p_ced_output_bus_signals);

/**
 * @brief Sets Bus Outputs while in Not Active States
 *
 * @return void
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
static void Ced_Set_Outputs_If_Not_Active_State(Bmw_Ced_Output_Bus_Signals_T *p_ced_output_bus_signals);


/**
 * @brief Sets Bus Outputs while in Active States
 *
 * @return void
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
static void Ced_Set_Outputs_In_Active_State(Ced_Output_T *p_ced_output);

/**
 * @brief Mapping of Enum Values of Optical Warning as per warning received
 *
 * @return void
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
static CED_OUTPUT_WARNING_OPTICAL_T Ced_Map_Optical_Warning_As_Per_Target_Travel_Direction(const Ced_Output_T *p_ced_output,
                                                                                           const Ced_Door_Position_T ced_door_position);
/**
 * @brief Mapping of Enum Values of Ambient Light Warning as per warning received
 *
 * @return void
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
static CED_OUTPUT_WARNING_AMBIENT_LIGHTS_T Ced_Map_Ambient_Lights_Warning_As_Per_Door_Warning_Levels(
   const Ced_Output_T *p_ced_output, const Ced_Door_Position_T ced_door_position);

#ifdef BINARY_DEBUG
static void Write_Ced_Output(const Ced_Output_T *p_ced_output);
#endif /* BINARY_DEBUG */

/**
 * @brief Update output from core output.
 *
 * @return void
 *
 * @SRS{SF-55}
 * @SAE{SF-2404}
 * @SDD{SF-3410}
 * @verification{}
 */
static void Ced_Update_Output(Ced_Output_T *p_ced_output /**< CED output data */,
                              const Ced_Input_T *p_ced_input /**< CED input data */,
                              const Ced_Core_Input_T *p_ced_core_input /**< CED core input data */,
                              const Ced_Core_Output_T *p_ced_core_output /**< CED core output data */
);

/**
 * @brief Reset SFE output.
 *
 * @return void
 *
 * @SRS{SF-55}
 * @SAE{SF-2404}
 * @SDD{SF-3411}
 * @verification{Create unit test which will confirm that reset routine maps output correctly to default state}
 */
static void Ced_Reset_Output(Ced_Output_T *p_ced_output /**< CED output data */);

/**
 * @brief Update PCAN signals.
 *
 * @return void
 *
 * @SRS{SF-55}
 * @SAE{SF-2404}
 * @SDD{SF-3412}
 * @verification{}
 */
static void Ced_Update_Pcan_Signals(Ced_Output_T *p_ced_output /**< CED output data */,
                                    const Ced_Core_Output_T *p_ced_core_output /**< CED core output data */,
                                    const Ced_Input_T *p_ced_input /**< CED input data */);

/**
 * @brief Map object class to SFE specific object classes.
 *
 * @return SFE specific object class
 *
 * @SRS{SF-55}
 * @SAE{SF-2404}
 * @SDD{SF-3413}
 * @verification{}
 */
static SFE_object_class_T Ced_Map_Object_Class_To_Sfe(Pa_Obj_Class_T tracker_obj_class /**< PA object class */,
                                                      float32_T obj_speed /**< object speed */);

/**
 * @brief Map travel direction to BMW specific enum.
 *
 * @return SFE specific travel direction
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-3453}
 * @verification{}
 */
static Ced_Target_Travel_Direction_T Ced_Map_Travel_Direction_To_Sfe(uint8_t ced_travel_direction /**< CED travel direction */);

/**
 * @brief Maps the warning signals to p_ced_output.
 *
 * @return fills p_ced_output
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
static void Ced_Map_Warning_Signals(Ced_Output_T *p_ced_output,
                                    const Ced_Core_Input_T *p_ced_core_input,
                                    const Ced_Core_Output_T *p_ced_core_output);

/**
 * @brief Writes the alert state from Core alert to ced output alert.
 *
 * @return fills p_ced_output
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
static void Ced_Set_Alert_State(Ced_Output_T *p_ced_output, const Ced_Core_Output_T *p_ced_core_output);


/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/


static void Ced_Reset_Bus_Signals_Output(Bmw_Ced_Output_Bus_Signals_T *p_ced_output_bus_signals)
{
   /* Filling/Initialising Bmw_Ced_Output_Bus_Signals_T required for State-Machine */

   p_ced_output_bus_signals->ced_output_warning_optical_front_right = NOT_ACTIVE;
   p_ced_output_bus_signals->ced_output_warning_optical_front_left  = NOT_ACTIVE;
   p_ced_output_bus_signals->ced_output_warning_optical_rear_right  = NOT_ACTIVE;
   p_ced_output_bus_signals->ced_output_warning_optical_rear_left   = NOT_ACTIVE;

   p_ced_output_bus_signals->ced_output_warning_mirror_led_right = CED_MIRROR_LIGHT_WARNING_OFF;
   p_ced_output_bus_signals->ced_output_warning_mirror_led_left  = CED_MIRROR_LIGHT_WARNING_OFF;

   p_ced_output_bus_signals->ced_output_warning_ambient_lights_front_right = AMBIENT_LIGHTS_NO_WARNING;
   p_ced_output_bus_signals->ced_output_warning_ambient_lights_front_left  = AMBIENT_LIGHTS_NO_WARNING;
   p_ced_output_bus_signals->ced_output_warning_ambient_lights_rear_right  = AMBIENT_LIGHTS_NO_WARNING;
   p_ced_output_bus_signals->ced_output_warning_ambient_lights_rear_left   = AMBIENT_LIGHTS_NO_WARNING;

   p_ced_output_bus_signals->ced_output_warning_acoustic_front_right = ACOUSTIC_NO_WARNING;
   p_ced_output_bus_signals->ced_output_warning_acoustic_front_left  = ACOUSTIC_NO_WARNING;
   p_ced_output_bus_signals->ced_output_warning_acoustic_rear_right  = ACOUSTIC_NO_WARNING;
   p_ced_output_bus_signals->ced_output_warning_acoustic_rear_left   = ACOUSTIC_NO_WARNING;

   p_ced_output_bus_signals->ced_output_door_stop_automatic_opening_left  = FBK_FALSE;
   p_ced_output_bus_signals->ced_output_door_stop_automatic_opening_right = FBK_FALSE;

   p_ced_output_bus_signals->ced_output_door_lock_electronic_front_right = FBK_FALSE;
   p_ced_output_bus_signals->ced_output_door_lock_electronic_front_left  = FBK_FALSE;
   p_ced_output_bus_signals->ced_output_door_lock_electronic_rear_right  = FBK_FALSE;
   p_ced_output_bus_signals->ced_output_door_lock_electronic_rear_left   = FBK_FALSE;

   p_ced_output_bus_signals->ced_output_display_door = DISPLAY_DOOR_NO_WARNING;
}

static void Ced_Set_Outputs_In_Active_State(Ced_Output_T *p_ced_output)
{
   /*Setting Optical Warning For Respective Door Position*/
   p_ced_output->ced_output_bus_signals.ced_output_warning_optical_front_left =
      Ced_Map_Optical_Warning_As_Per_Target_Travel_Direction(p_ced_output, CED_DOOR_POSITION_FRONT_LEFT);
   p_ced_output->ced_output_bus_signals.ced_output_warning_optical_front_right =
      Ced_Map_Optical_Warning_As_Per_Target_Travel_Direction(p_ced_output, CED_DOOR_POSITION_FRONT_RIGHT);
   p_ced_output->ced_output_bus_signals.ced_output_warning_optical_rear_left =
      Ced_Map_Optical_Warning_As_Per_Target_Travel_Direction(p_ced_output, CED_DOOR_POSITION_REAR_LEFT);
   p_ced_output->ced_output_bus_signals.ced_output_warning_optical_rear_right =
      Ced_Map_Optical_Warning_As_Per_Target_Travel_Direction(p_ced_output, CED_DOOR_POSITION_REAR_RIGHT);

   /*Setting Ambient Lights Warning For Respective Door Position*/
   p_ced_output->ced_output_bus_signals.ced_output_warning_ambient_lights_front_left =
      Ced_Map_Ambient_Lights_Warning_As_Per_Door_Warning_Levels(p_ced_output, CED_DOOR_POSITION_FRONT_LEFT);
   p_ced_output->ced_output_bus_signals.ced_output_warning_ambient_lights_front_right =
      Ced_Map_Ambient_Lights_Warning_As_Per_Door_Warning_Levels(p_ced_output, CED_DOOR_POSITION_FRONT_RIGHT);
   p_ced_output->ced_output_bus_signals.ced_output_warning_ambient_lights_rear_left =
      Ced_Map_Ambient_Lights_Warning_As_Per_Door_Warning_Levels(p_ced_output, CED_DOOR_POSITION_REAR_LEFT);
   p_ced_output->ced_output_bus_signals.ced_output_warning_ambient_lights_rear_right =
      Ced_Map_Ambient_Lights_Warning_As_Per_Door_Warning_Levels(p_ced_output, CED_DOOR_POSITION_REAR_RIGHT);

   /*Setting Mirror Led Warning For Respective Door Side*/
   p_ced_output->ced_output_bus_signals.ced_output_warning_mirror_led_right =
      p_ced_output->ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_RIGHT];
   p_ced_output->ced_output_bus_signals.ced_output_warning_mirror_led_left =
      p_ced_output->ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_LEFT];

   /*Calling the functions to set remaining signals whose data is not set yet as per boardnet mapping.
    * These signals are currently hard-coded with default values.
    */
   p_ced_output->ced_output_bus_signals.ced_output_warning_acoustic_front_right      = ACOUSTIC_NO_WARNING;
   p_ced_output->ced_output_bus_signals.ced_output_door_stop_automatic_opening_right = FBK_FALSE;
   p_ced_output->ced_output_bus_signals.ced_output_door_lock_electronic_front_right  = FBK_FALSE;

   p_ced_output->ced_output_bus_signals.ced_output_warning_acoustic_front_left      = ACOUSTIC_NO_WARNING;
   p_ced_output->ced_output_bus_signals.ced_output_door_stop_automatic_opening_left = FBK_FALSE;
   p_ced_output->ced_output_bus_signals.ced_output_door_lock_electronic_front_left  = FBK_FALSE;

   p_ced_output->ced_output_bus_signals.ced_output_warning_acoustic_rear_right       = ACOUSTIC_NO_WARNING;
   p_ced_output->ced_output_bus_signals.ced_output_door_stop_automatic_opening_right = FBK_FALSE;
   p_ced_output->ced_output_bus_signals.ced_output_door_lock_electronic_rear_right   = FBK_FALSE;

   p_ced_output->ced_output_bus_signals.ced_output_warning_acoustic_rear_left       = ACOUSTIC_NO_WARNING;
   p_ced_output->ced_output_bus_signals.ced_output_door_stop_automatic_opening_left = FBK_FALSE;
   p_ced_output->ced_output_bus_signals.ced_output_door_lock_electronic_rear_left   = FBK_FALSE;

   p_ced_output->ced_output_bus_signals.ced_output_display_door = DISPLAY_DOOR_NO_WARNING;
}

static void Ced_Set_Outputs_If_Not_Active_State(Bmw_Ced_Output_Bus_Signals_T *p_ced_output_bus_signals)
{
   Ced_Reset_Bus_Signals_Output(p_ced_output_bus_signals);
}

/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_ced_instance" points to a non-constant type.] */
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
void Ced_Post_Run_Init(Ced_Instance_T *p_ced_instance)
{
   assert(NULL != p_ced_instance);
}


static CED_OUTPUT_WARNING_OPTICAL_T Ced_Map_Optical_Warning_As_Per_Target_Travel_Direction(const Ced_Output_T *p_ced_output,
                                                                                           const Ced_Door_Position_T ced_door_position)
{
   Ced_Target_Travel_Direction_T travel_direction =
      p_ced_output->ced_output_warning_indicators.ced_output_warning_optical[ced_door_position];
   CED_OUTPUT_WARNING_OPTICAL_T warning_result;
   switch (travel_direction)
   {
      case REAR_DIRECTION:
         warning_result = ACTIVE_APPROACH_REAR;
         break;
      case FRONT_DIRECTION:
         warning_result = ACTIVE_APPROACH_FRONT;
         break;
      case UNDEF_DIRECTION:
      default:
         warning_result = NOT_ACTIVE;
         break;
   }
   return warning_result;
}

static CED_OUTPUT_WARNING_AMBIENT_LIGHTS_T Ced_Map_Ambient_Lights_Warning_As_Per_Door_Warning_Levels(
   const Ced_Output_T *p_ced_output, const Ced_Door_Position_T ced_door_position)
{
   Ced_Door_Warning_Levels_T door_warning_level =
      p_ced_output->ced_output_warning_indicators.ced_output_warning_ambient[ced_door_position];
   CED_OUTPUT_WARNING_AMBIENT_LIGHTS_T warning_result;

   switch (door_warning_level)
   {
      case CED_DOOR_WARNING_LEVEL_1:
         warning_result = AMBIENT_LIGHTS_WARNING_LEVEL_1;
         break;
      case CED_DOOR_WARNING_LEVEL_2:
         warning_result = AMBIENT_LIGHTS_WARNING_LEVEL_2;
         break;
      case CED_DOOR_WARNING_LEVEL_NO_WARNING:
      default:
         warning_result = AMBIENT_LIGHTS_NO_WARNING;
         break;
   }
   return warning_result;
}

// clang-format off
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_ced_instance" points to a non-constant type] */
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
void Ced_Post_Run(Ced_Instance_T *p_ced_instance, const Ced_Input_T *p_ced_input,  Ced_Output_T *p_ced_output)
// clang-format on
{
   assert(NULL != p_ced_instance);
   assert(NULL != p_ced_input);
   assert(NULL != p_ced_output);

   /* Set SFE ouput */
   Ced_Reset_Output(p_ced_output);
   Ced_Update_Output(p_ced_output, p_ced_input, &p_ced_instance->core_input, &p_ced_instance->core_output);

   /* Set the Main Ced Qualifier (This normally should be set by SM-Core-Output) */
   p_ced_output->ced_output_bus_signals.qualifier_ced_function_state = *(Ced_Get_State_Output_Ptr());

   /*Setting the outputs when state is not ACTIVE State*/
   if (CED_STATE_ACTIVE != p_ced_output->ced_output_bus_signals.qualifier_ced_function_state)
   {
      Ced_Set_Outputs_If_Not_Active_State(&(p_ced_output->ced_output_bus_signals));
   }
   else /*Setting the outputs when state in ACTIVE State*/
   {
      Ced_Set_Outputs_In_Active_State(p_ced_output);
   }

   /* Update PCAN output signals*/
   Ced_Update_Pcan_Signals(p_ced_output, &p_ced_instance->core_output, p_ced_input); // obsolete no pcan existing anymore

   /* Write bin file output */
#ifdef BINARY_DEBUG
   Write_Ced_Output(p_ced_output);
#endif
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static void Ced_Update_Output(Ced_Output_T *p_ced_output,
                              const Ced_Input_T *p_ced_input,
                              const Ced_Core_Input_T *p_ced_core_input,
                              const Ced_Core_Output_T *p_ced_core_output)
{
   /* BMW SRR5 Safe Exit Outputs */
   p_ced_output->SFE_CED_Status = Fbk_Convert_Bool_To_Uint(p_ced_input->f_ced_enable);
   /* Maps the input vairables to output variables which can be read in easily */
   Ced_Map_Warning_Signals(p_ced_output, p_ced_core_input, p_ced_core_output);

   Ced_Check_And_Set_Occupant_Detection(&p_ced_output->ced_output_occupant_detection, &p_ced_input->bmw_boardnet_signals);

   Ced_Control_Mirror_Led(p_ced_output, p_ced_core_output, p_ced_input);

   Ced_Control_Optical_Elements(p_ced_output, p_ced_core_output, p_ced_input);

   Ced_Control_Interior_Lights(p_ced_output, p_ced_core_output, p_ced_input);
}

static void Ced_Set_Alert_State(Ced_Output_T *p_ced_output, const Ced_Core_Output_T *p_ced_core_output)
{
   /* Only set alert level right for a TTC above the minimum threshold */
   if (p_ced_output->SFE_CED_ttc_right >= FBK_ZERO_F)
   {
      p_ced_output->SFE_CED_alert_right = (uint8_t) p_ced_core_output->ced_alert[FBK_SIDE_RIGHT];
   }


   /* Only set alert level left for a TTC above the minimum threshold */
   if (p_ced_output->SFE_CED_ttc_left >= FBK_ZERO_F)
   {
      p_ced_output->SFE_CED_alert_left = (uint8_t) p_ced_core_output->ced_alert[FBK_SIDE_LEFT];
   }
}

static void Ced_Map_Warning_Signals(Ced_Output_T *p_ced_output,
                                    const Ced_Core_Input_T *p_ced_core_input,
                                    const Ced_Core_Output_T *p_ced_core_output)
{


   /* index of alert object for left and right side */
   const Fbk_Object_Data_T *p_object_data;

   uint8_t obj_index_right = p_ced_core_output->ced_index[FBK_SIDE_RIGHT];
   uint8_t obj_index_left  = p_ced_core_output->ced_index[FBK_SIDE_LEFT];


   Ced_Set_Alert_State(p_ced_output, p_ced_core_output);

   /* Warning for right side */
   p_ced_output->SFE_CED_dir_right =
      (uint8_t) Ced_Map_Travel_Direction_To_Sfe(p_ced_core_output->ced_object_direction[FBK_SIDE_RIGHT]);
   p_ced_output->SFE_CED_ttc_right       = Fbk_Max(FBK_ZERO_F, p_ced_core_output->ced_ttc[FBK_SIDE_RIGHT]);
   p_ced_output->SFE_CED_id_right        = p_ced_core_output->ced_id[FBK_SIDE_RIGHT];
   p_ced_output->SFE_CED_unique_id_right = p_ced_core_output->ced_unique_id[FBK_SIDE_RIGHT];

   /* Fill tracker dependent outputs only for valid alert index */
   if (((uint8_t) CED_NO_ALERT) != p_ced_output->SFE_CED_alert_right)
   {
      p_object_data                         = &(p_ced_core_input->p_pa_data->object_data[obj_index_right]);
      p_ced_output->SFE_CED_obj_speed_right = p_object_data->speed;
      p_ced_output->SFE_CED_obj_type_right = (uint8_t) Ced_Map_Object_Class_To_Sfe(p_object_data->obj_class, p_object_data->speed);
      p_ced_output->SFE_CED_obj_heading_right = p_object_data->vcs_heading;


      p_ced_output->SFE_CED_obj_lateral_pos_right =
         ((float32_T) 0.5f
          * ((2.0f * p_object_data->vcs_pos.y) + (Fast_Sin(p_object_data->vcs_heading) * (p_object_data->length))
             + (Fast_Cos(p_object_data->vcs_heading) * (p_object_data->width))));


      p_ced_output->SFE_CED_obj_long_pos_right =
         ((float32_T) 0.5f
          * ((2.0f * p_object_data->vcs_pos.x) + (Fast_Cos(p_object_data->vcs_heading) * (p_object_data->length))
             - (Fast_Sin(p_object_data->vcs_heading) * (p_object_data->width))));
   }

   /* Warning for left side */
   p_ced_output->SFE_CED_dir_left = (uint8_t) Ced_Map_Travel_Direction_To_Sfe(p_ced_core_output->ced_object_direction[FBK_SIDE_LEFT]);
   p_ced_output->SFE_CED_ttc_left       = Fbk_Max(FBK_ZERO_F, p_ced_core_output->ced_ttc[FBK_SIDE_LEFT]);
   p_ced_output->SFE_CED_id_left        = p_ced_core_output->ced_id[FBK_SIDE_LEFT];
   p_ced_output->SFE_CED_unique_id_left = p_ced_core_output->ced_unique_id[FBK_SIDE_LEFT];

   /* Fill tracker dependent outputs only for valid alert index */
   if (((uint8_t) CED_NO_ALERT) != p_ced_output->SFE_CED_alert_left)
   {
      p_object_data                        = &(p_ced_core_input->p_pa_data->object_data[obj_index_left]);
      p_ced_output->SFE_CED_obj_speed_left = p_object_data->speed;
      p_ced_output->SFE_CED_obj_type_left  = (uint8_t) Ced_Map_Object_Class_To_Sfe(p_object_data->obj_class, p_object_data->speed);
      p_ced_output->SFE_CED_obj_heading_left = p_object_data->vcs_heading;

      p_ced_output->SFE_CED_obj_lateral_pos_left =
         ((float32_T) 0.5f
          * ((2.0f * p_object_data->vcs_pos.y) + (Fast_Sin(p_object_data->vcs_heading) * (p_object_data->length))
             + (Fast_Cos(p_object_data->vcs_heading) * (p_object_data->width))));

      p_ced_output->SFE_CED_obj_long_pos_left =
         ((float32_T) 0.5f
          * ((2.0f * p_object_data->vcs_pos.x) + (Fast_Cos(p_object_data->vcs_heading) * (p_object_data->length))
             - (Fast_Sin(p_object_data->vcs_heading) * (p_object_data->width))));
   }
}

static void Ced_Reset_Output(Ced_Output_T *p_ced_output)
{
   /* SFE/CED Output for State Machine and Alert Mechanism, use these signals for UDP Logging */
   p_ced_output->SFE_CED_Status = FBK_ZERO_UINT;

   p_ced_output->SFE_CED_alert_right           = (uint8_t) CED_NO_ALERT;
   p_ced_output->SFE_CED_dir_right             = (uint8_t) UNDEF_DIRECTION;
   p_ced_output->SFE_CED_ttc_right             = CED_INVALID_TIME;
   p_ced_output->SFE_CED_ttp_right             = CED_INVALID_TIME;
   p_ced_output->SFE_CED_id_right              = FBK_ZERO_UINT;
   p_ced_output->SFE_CED_unique_id_right       = FBK_ZERO_UINT;
   p_ced_output->SFE_CED_obj_speed_right       = FBK_ZERO_F;
   p_ced_output->SFE_CED_obj_type_right        = (uint8_t) SFE_OBJECT_CLASS_UNKNOWN;
   p_ced_output->SFE_CED_obj_heading_right     = FBK_ZERO_F;
   p_ced_output->SFE_CED_obj_lateral_pos_right = CED_INVALID_DISTANCE;
   p_ced_output->SFE_CED_obj_long_pos_right    = CED_INVALID_DISTANCE;

   p_ced_output->SFE_CED_alert_left           = (uint8_t) CED_NO_ALERT;
   p_ced_output->SFE_CED_dir_left             = (uint8_t) UNDEF_DIRECTION;
   p_ced_output->SFE_CED_ttc_left             = CED_INVALID_TIME;
   p_ced_output->SFE_CED_ttp_left             = CED_INVALID_TIME;
   p_ced_output->SFE_CED_id_left              = FBK_ZERO_UINT;
   p_ced_output->SFE_CED_unique_id_left       = FBK_ZERO_UINT;
   p_ced_output->SFE_CED_obj_speed_left       = FBK_ZERO_F;
   p_ced_output->SFE_CED_obj_type_left        = (uint8_t) SFE_OBJECT_CLASS_UNKNOWN;
   p_ced_output->SFE_CED_obj_heading_left     = FBK_ZERO_F;
   p_ced_output->SFE_CED_obj_lateral_pos_left = CED_INVALID_DISTANCE;
   p_ced_output->SFE_CED_obj_long_pos_left    = CED_INVALID_DISTANCE;

   /* pCAN Signals used for Debugging and ORCAS */
   p_ced_output->SFE_CED_rear_status           = FBK_ZERO_UINT;
   p_ced_output->SFE_CED_rear_alert_right      = (uint8_t) CED_NO_ALERT;
   p_ced_output->SFE_CED_rear_alert_left       = (uint8_t) CED_NO_ALERT;
   p_ced_output->SFE_CED_rear_id_right         = FBK_ZERO_UINT;
   p_ced_output->SFE_CED_rear_id_left          = FBK_ZERO_UINT;
   p_ced_output->SFE_CED_rear_path_match_right = FBK_ZERO_UINT;
   p_ced_output->SFE_CED_rear_path_match_left  = FBK_ZERO_UINT;
   p_ced_output->SFE_CED_rear_ttc_right        = CED_INVALID_TIME;
   p_ced_output->SFE_CED_rear_ttc_left         = CED_INVALID_TIME;
   p_ced_output->SFE_CED_rear_ttp_right        = CED_INVALID_TIME;
   p_ced_output->SFE_CED_rear_ttp_left         = CED_INVALID_TIME;
   p_ced_output->SFE_CED_rear_lat_right        = CED_INVALID_DISTANCE;
   p_ced_output->SFE_CED_rear_lat_left         = CED_INVALID_DISTANCE;

   p_ced_output->SFE_CED_front_status           = FBK_ZERO_UINT;
   p_ced_output->SFE_CED_front_alert_right      = (uint8_t) CED_NO_ALERT;
   p_ced_output->SFE_CED_front_alert_left       = (uint8_t) CED_NO_ALERT;
   p_ced_output->SFE_CED_front_id_right         = FBK_ZERO_UINT;
   p_ced_output->SFE_CED_front_id_left          = FBK_ZERO_UINT;
   p_ced_output->SFE_CED_front_path_match_right = FBK_ZERO_UINT;
   p_ced_output->SFE_CED_front_path_match_left  = FBK_ZERO_UINT;
   p_ced_output->SFE_CED_front_ttc_right        = CED_INVALID_TIME;
   p_ced_output->SFE_CED_front_ttc_left         = CED_INVALID_TIME;
   p_ced_output->SFE_CED_front_ttp_right        = CED_INVALID_TIME;
   p_ced_output->SFE_CED_front_ttp_left         = CED_INVALID_TIME;
   p_ced_output->SFE_CED_front_lat_right        = CED_INVALID_DISTANCE;
   p_ced_output->SFE_CED_front_lat_left         = CED_INVALID_DISTANCE;
}

static void Ced_Update_Pcan_Signals(Ced_Output_T *p_ced_output, const Ced_Core_Output_T *p_ced_core_output, const Ced_Input_T *p_ced_input)
{
   /* Update of status flag */
   p_ced_output->SFE_CED_rear_status  = Fbk_Convert_Bool_To_Uint(p_ced_input->f_ced_rear_mode);
   p_ced_output->SFE_CED_front_status = Fbk_Convert_Bool_To_Uint(p_ced_input->f_ced_front_mode);

   /* Alert in rear right sector */
   if ((((uint8_t) REAR_DIRECTION) == p_ced_output->SFE_CED_dir_right)
       && (((uint8_t) CED_NO_ALERT) != p_ced_output->SFE_CED_alert_right))
   {
      p_ced_output->SFE_CED_rear_alert_right = p_ced_output->SFE_CED_alert_right;
      p_ced_output->SFE_CED_rear_id_right    = p_ced_output->SFE_CED_id_right;
      p_ced_output->SFE_CED_rear_ttc_right   = p_ced_output->SFE_CED_ttc_right;
      p_ced_output->SFE_CED_rear_ttp_right   = p_ced_output->SFE_CED_ttp_right;
      p_ced_output->SFE_CED_rear_lat_right   = p_ced_core_output->ced_object_predicted_lat_pos[FBK_SIDE_RIGHT];
      p_ced_output->SFE_CED_rear_path_match_right =
         Fbk_Convert_Bool_To_Uint(PT_DEFAULT_MATCH_INDEX != p_ced_core_output->ced_object_path_match_index[FBK_SIDE_RIGHT]);
   }

   /* Alert in rear left sector */
   if ((((uint8_t) REAR_DIRECTION) == p_ced_output->SFE_CED_dir_left) && (((uint8_t) CED_NO_ALERT) != p_ced_output->SFE_CED_alert_left))
   {
      p_ced_output->SFE_CED_rear_alert_left = p_ced_output->SFE_CED_alert_left;
      p_ced_output->SFE_CED_rear_id_left    = p_ced_output->SFE_CED_id_left;
      p_ced_output->SFE_CED_rear_ttc_left   = p_ced_output->SFE_CED_ttc_left;
      p_ced_output->SFE_CED_rear_ttp_left   = p_ced_output->SFE_CED_ttp_left;
      p_ced_output->SFE_CED_rear_lat_left   = p_ced_core_output->ced_object_predicted_lat_pos[FBK_SIDE_LEFT];
      p_ced_output->SFE_CED_rear_path_match_left =
         Fbk_Convert_Bool_To_Uint(PT_DEFAULT_MATCH_INDEX != p_ced_core_output->ced_object_path_match_index[FBK_SIDE_LEFT]);
   }

   /* Alert in front right sector */
   if ((((uint8_t) FRONT_DIRECTION) == p_ced_output->SFE_CED_dir_right)
       && (((uint8_t) CED_NO_ALERT) != p_ced_output->SFE_CED_alert_right))
   {
      p_ced_output->SFE_CED_front_alert_right = p_ced_output->SFE_CED_alert_right;
      p_ced_output->SFE_CED_front_id_right    = p_ced_output->SFE_CED_id_right;
      p_ced_output->SFE_CED_front_ttc_right   = p_ced_output->SFE_CED_ttc_right;
      p_ced_output->SFE_CED_front_ttp_right   = p_ced_output->SFE_CED_ttp_right;
      p_ced_output->SFE_CED_front_lat_right   = p_ced_core_output->ced_object_predicted_lat_pos[FBK_SIDE_RIGHT];
      p_ced_output->SFE_CED_front_path_match_right =
         Fbk_Convert_Bool_To_Uint(PT_DEFAULT_MATCH_INDEX != p_ced_core_output->ced_object_path_match_index[FBK_SIDE_RIGHT]);
   }

   /* Alert in front left sector */
   if ((((uint8_t) FRONT_DIRECTION) == p_ced_output->SFE_CED_dir_left)
       && (((uint8_t) CED_NO_ALERT) != p_ced_output->SFE_CED_alert_left))
   {
      p_ced_output->SFE_CED_front_alert_left = p_ced_output->SFE_CED_alert_left;
      p_ced_output->SFE_CED_front_id_left    = p_ced_output->SFE_CED_id_left;
      p_ced_output->SFE_CED_front_ttc_left   = p_ced_output->SFE_CED_ttc_left;
      p_ced_output->SFE_CED_front_ttp_left   = p_ced_output->SFE_CED_ttp_left;
      p_ced_output->SFE_CED_front_lat_left   = p_ced_core_output->ced_object_predicted_lat_pos[FBK_SIDE_LEFT];
      p_ced_output->SFE_CED_front_path_match_left =
         Fbk_Convert_Bool_To_Uint(PT_DEFAULT_MATCH_INDEX != p_ced_core_output->ced_object_path_match_index[FBK_SIDE_LEFT]);
   }
}

static SFE_object_class_T Ced_Map_Object_Class_To_Sfe(Pa_Obj_Class_T tracker_obj_class, float32_T obj_speed)
{
   SFE_object_class_T sfe_obj_class;
   float32_T velocity_threshold = CED_K_OBJ_CLASS_MAP_VEL_MAX_THRES * CED_KPH2MPS;

   switch (tracker_obj_class)
   {
      case PA_OBJ_CLASS_TRUCK:
         sfe_obj_class = SFE_OBJECT_CLASS_TRUCK;
         break;
      case PA_OBJ_CLASS_CAR:
         sfe_obj_class = SFE_OBJECT_CLASS_CAR;
         break;
      case PA_OBJ_CLASS_2WHEEL:
         if (obj_speed < velocity_threshold)
         {
            sfe_obj_class = SFE_OBJECT_CLASS_BICYCLE;
         }
         else
         {
            sfe_obj_class = SFE_OBJECT_CLASS_MOTORCYCLE;
         }
         break;
      case PA_OBJ_CLASS_PEDESTRIAN:
         sfe_obj_class = SFE_OBJECT_CLASS_PEDESTRIAN;
         break;
      case PA_OBJ_CLASS_UNKNOWN:
      default:
         sfe_obj_class = SFE_OBJECT_CLASS_UNKNOWN;
         break;
   }

   return sfe_obj_class;
}

static Ced_Target_Travel_Direction_T Ced_Map_Travel_Direction_To_Sfe(uint8_t ced_travel_direction)
{
   Ced_Target_Travel_Direction_T sfe_travel_direction;

   switch (ced_travel_direction)
   {
      case FBK_SIDE_FRONT:
         sfe_travel_direction = FRONT_DIRECTION;
         break;
      case FBK_SIDE_REAR:
         sfe_travel_direction = REAR_DIRECTION;
         break;
      case FBK_SIDE_UNDEFINED:
      default:
         sfe_travel_direction = UNDEF_DIRECTION;
         break;
   }

   return sfe_travel_direction;
}


#ifdef BINARY_DEBUG
static void Write_Ced_Output(const Ced_Output_T *p_ced_output)
{
   /* check input parameters */
   assert(NULL != p_ced_output);

   /* Log Safe Exit specific data*/
   CED_STORE_VAL_MGR_WPR("SFE_CED_alert_left", p_ced_output->SFE_CED_alert_left);
   CED_STORE_VAL_MGR_WPR("SFE_CED_alert_right", p_ced_output->SFE_CED_alert_right);
   CED_STORE_VAL_MGR_WPR("SFE_CED_dir_left", p_ced_output->SFE_CED_dir_left);
   CED_STORE_VAL_MGR_WPR("SFE_CED_dir_right", p_ced_output->SFE_CED_dir_right);
   CED_STORE_VAL_MGR_WPR("SFE_CED_id_left", p_ced_output->SFE_CED_id_left);
   CED_STORE_VAL_MGR_WPR("SFE_CED_id_right", p_ced_output->SFE_CED_id_right);
   CED_STORE_VAL_MGR_WPR("SFE_CED_ttc_left", p_ced_output->SFE_CED_ttc_left);
   CED_STORE_VAL_MGR_WPR("SFE_CED_ttc_right", p_ced_output->SFE_CED_ttc_right);
   CED_STORE_VAL_MGR_WPR("SFE_CED_obj_speed_left", p_ced_output->SFE_CED_obj_speed_left);
   CED_STORE_VAL_MGR_WPR("SFE_CED_obj_speed_right", p_ced_output->SFE_CED_obj_speed_right);
   CED_STORE_VAL_MGR_WPR("SFE_CED_obj_type_left", p_ced_output->SFE_CED_obj_type_left);
   CED_STORE_VAL_MGR_WPR("SFE_CED_obj_type_right", p_ced_output->SFE_CED_obj_type_right);
   CED_STORE_VAL_MGR_WPR("SFE_CED_obj_heading_right", p_ced_output->SFE_CED_obj_heading_right);
   CED_STORE_VAL_MGR_WPR("SFE_CED_obj_heading_left", p_ced_output->SFE_CED_obj_heading_left);
   CED_STORE_VAL_MGR_WPR("SFE_CED_obj_lateral_pos_right", p_ced_output->SFE_CED_obj_lateral_pos_right);
   CED_STORE_VAL_MGR_WPR("SFE_CED_obj_lateral_pos_left", p_ced_output->SFE_CED_obj_lateral_pos_left);
   CED_STORE_VAL_MGR_WPR("SFE_CED_obj_long_pos_right", p_ced_output->SFE_CED_obj_long_pos_right);
   CED_STORE_VAL_MGR_WPR("SFE_CED_obj_long_pos_left", p_ced_output->SFE_CED_obj_long_pos_left);
   CED_STORE_VAL_MGR_WPR("SFE_CED_State_Machine_Output", p_ced_output->ced_output_bus_signals.qualifier_ced_function_state);
}
#endif /* BINARY_DEBUG */
