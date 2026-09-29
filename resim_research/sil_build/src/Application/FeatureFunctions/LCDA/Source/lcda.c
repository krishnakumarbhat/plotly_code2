/**
 * @file lcda.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Implementation of LCDA main module which calls submodules BSW, CVW, SLC and ELC
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */


/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "lcda.h"
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_output.h"
#include "fbk_vehicle_data_t.h"
#include "lcda_core_calibration_t.h"
#include "lcda_core_input_t.h"
#include "lcda_core_output_t.h"
#include "lcda_debug_interface.h"
#include "lcda_instance.h"
#include "lcda_persistent_t.h"
#include "lcda_process_bsw.h"
#include "lcda_process_cvw.h"
#include "lcda_process_elc.h"
#include "lcda_process_slc.h"
#include "lcda_types.h"
#include "ml_math.h"
#include "ml_saturated_math.h"
#include "ml_vector_2d_t.h"
#include "pa_const_macros.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include <assert.h>

/*===========================================================================*\
* Local defines
\*===========================================================================*/

#define LCDA_DEFAULT_FACTOR (1.0f)

/*===========================================================================*\
* File Scope variables
\*===========================================================================*/

/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/


/**
 * @brief Calls the pre and post processing as well as the core subroutine for each submodule of LCDA.
 *        The called submodules depend on whether they are activated.
 *
 *
 * @return void
 *
 * @SRS{SF-1041}
 * @SAE{SF-2779}
 * @SDD{SF-6545}
 * @verification{Create tests where slc, elc, cvw and bsw are activated respectively and where a warning is triggered by each of
 * the modules}
 */
static void Lcda_Process_All_Active_Submodules(Lcda_Instance_T *p_lcda_instance, const Fbk_Output_T *p_fbk_output);

/**
 * @brief Checks whether basic object properties are fulfilled so that the object can be used for further analysis.
 *
 * @return True when object is a valid LCDA candidate.
 *
 * @SRS{SF-1015,SF-1062,SF-1076,CSCSA-121708,CSCSA-164366,CSCSA-121709}
 * @SAE{SF-2779}
 * @SDD{SF-6544}
 * @verification{Create a test with an object which has valid tracker output attributes, so that true is returned.}
 */
static boolean_T Lcda_Is_Valid_Object(const Fbk_Object_Data_T *p_object_data,
                                      const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */);

/**
 * @brief Checks whether VRU object properties shall be valid for algorithm, so that the object can be used for further analysis.
 *
 * @return True when object is a valid LCDA candidate.
 *
 * @SRS{SF-1015,SF-1062,SF-1076,CSCSA-151000,CSCSA-151001}
 * @SAE{SF-2779}
 * @SDD{CSCSA-141096}
 * @verification{Create a test with an VRU object which shall be valid for algorithm.}
 */
static boolean_T Lcda_Is_VRU_Object_Valid(const Fbk_Object_Data_T *p_object_data,
                                          const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */);

/**
 * @brief Resets Lcda core input signals to its default.
 *
 * @return void
 *
 * @SRS{SF-1049}
 * @SAE{SF-2779}
 * @SDD{SF-6536}
 * @verification{Check whether lcda core input are reset to their default values.}
 */
static void Lcda_Init_Core_Input(Lcda_Core_Input_T *p_core_input /**< Lcda core input */,
                                 const Pa_Data_T *p_pa_data /**< context data */);

/**
 * @brief Updates persistent lcda modes with the following info from the ego vehicle state:
 *		  1. Checks if the host is above activation speed (applies hysteresis).
 *        2. Processes and stores the turn signal info.
 *        3. Checks if the feature needs to be disabled due to low curve radius (applies hysteresis).
 *
 * @return void
 *
 * @SRS{CSCSA-68353,SF-985,SF-988}
 * @SAE{SF-2779}
 * @SDD{SF-6547}
 * @verification{Create a test where it is checked whether all persistent modes of lcda are filled accordingly.}
 */
static void Lcda_Update_Vehicle_States(const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                       const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
                                       Lcda_Persistent_T *p_lcda_persistent /**< Lcda persistent data */);


/**
 * @brief Checks whether host is above the activation speed threshold.
 * It computes the host activation speed threshold by adding the speed hysteresis
 * value if the host was previously above the activation speed threshhold.
 *
 * @return True when the host is withing the activation speed threshold
 *
 * @SRS{SF-985,SF-986,CSCSA-120760,CSCSA-122235}
 * @SAE{SF-2779}
 * @SDD{SF-6541}
 * @verification{Create tests where in one of them the hysteresis is applied and in the other not. Check that the the host is
 * inside those ranges.}
 */
static boolean_T Lcda_Is_Host_Speed_In_Activation_Range(const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
                                                        const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                                        const Lcda_Persistent_T *p_lcda_persistent /**< Lcda persistent data */);
/**
 * @brief Returns the current LCDA Enabling State based on the core input and
 * calibration parameter.
 *
 * @return Lcda_Status_T
 *
 * @SRS{SF-981,CSCSA-120760}
 * @SAE{SF-2779}
 * @SDD{SF-6535}
 * @verification{Create tests where LCDA is either enabled by calibration or by its input.}
 */
static Lcda_Status_T Lcda_Get_Enable_Status(const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
                                            const Lcda_Core_Input_T *p_core_input /**< Lcda core input */);

/**
 * @brief  Returns the current LCDA Activation State based on info in the persistent lcda structure
 *		   It is assumed that LCDA is already enabled.
 *
 * @return Lcda_Status_T
 *
 * @SRS{SF-991,CSCSA-120760}
 * @SAE{SF-2779}
 * @SDD{SF-6534}
 * @verification{Create a test where lcda is not disabled due to curve radius or activation speed. Only then the activated state is
 *expected.}
 */
static Lcda_Status_T Lcda_Get_Activation_Status(const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
                                                const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                                const Lcda_Persistent_T *p_lcda_persistent /**< Lcda persistent data */);

/**
 * @brief  returns the Bsw enable state based on the calibration
 *         if k_bsw_enable_via_cal is set. Otherwise the enable state from the core input is used.
 *
 * @return True when Bsw is enabled
 *
 * @SRS{SF-1035,CSCSA-120764,CSCSA-122252}
 * @SAE{SF-2779}
 * @SDD{SF-6538}
 * @verification{Create tests where Bsw is enabled either by calibration or by input.}
 */
static boolean_T Lcda_Is_Bsw_Activated(const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
                                       const Lcda_Core_Input_T *p_core_input /**< Lcda core input */);

/**
 * @brief  returns the Cvw enable state based on the calibration
 *         if k_cvw_enable_via_cal is set. Otherwise the enable state from the core input is used.
 *
 * @return True when Cvw is enabled
 *
 * @SRS{SF-1035,CSCSA-122253,CSCSA-120763}
 * @SAE{SF-2779}
 * @SDD{SF-6539}
 * @verification{Create tests where Cvw is enabled either by calibration or by input.}
 */
static boolean_T Lcda_Is_Cvw_Activated(const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
                                       const Lcda_Core_Input_T *p_core_input /**< Lcda core input */);

/**
 * @brief  returns the Slc enable state based on the calibration
 *         if k_slc_enable_via_cal is set. Otherwise the enable state from the core input is used.
 *
 * @return True when Slc is enabled
 *
 * @SRS{SF-1035}
 * @SAE{SF-2779}
 * @SDD{SF-6543}
 * @verification{Create tests where Slc is enabled either by calibration or by input.}
 */
static boolean_T Lcda_Is_Slc_Activated(const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
                                       const Lcda_Core_Input_T *p_core_input /**< Lcda core input */);

/**
 * @brief  returns the Elc enable state based on the calibration
 *         if k_elc_enable_via_cal is set. Otherwise the enable state from the core input is used.
 *
 * @return True when Elc is enabled
 *
 * @SRS{SF-1035}
 * @SAE{SF-2779}
 * @SDD{SF-6540}
 * @verification{Create tests where Elc is enabled either by calibration or by input.}
 */
static boolean_T Lcda_Is_Elc_Activated(const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
                                       const Lcda_Core_Input_T *p_core_input /**< Lcda core input */);


/**
 * @brief  Updates the LCDA persistent data with the
 *         turn signal from the vehicle. If the current vehicle turn signal value is
 *         TURN_SIGNAL_NONE, then the previous value of the turn signal held in the persistance
 *         is maintained for k_lcda_turn_signal_coast_cycles before it is reset to TURN_SIGNAL_NONE.
 *
 * @return void
 *
 * @SRS{CSCSA-68353}
 * @SAE{SF-2779}
 * @SDD{SF-6546}
 * @verification{Check whether the vehicle turn signal processing logic is applied correctly.}
 */
static void Lcda_Process_Veh_Turn_Signal(const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                         const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
                                         Lcda_Persistent_T *p_lcda_persistent /**< Lcda persistent data */);


/**
 * @brief  Checks if curve radious is within the specified threshold.
 *
 * @return True when curve radius is more than specified threshold.
 *
 * @SRS{SF-988,SF-989,SF-1040,CSCSA-120760,CSCSA-122234}
 * @SAE{SF-2779}
 * @SDD{CSCSA-46128}
 * @verification{Create tests to check whether curve radious is within specified threshold.}
 */
static boolean_T Lcda_Is_Curve_Radius_Valid(const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
                                            const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                            const Lcda_Persistent_T *p_lcda_persistent /**< Lcda persistent data */);

/*===========================================================================*\
* Global Functions Definition
\*===========================================================================*/

void Lcda_Reset(Lcda_Instance_T *p_lcda_instance, const Pa_Data_T *p_pa_data)
{
   Lcda_Core_Output_T *p_core_output;
   /* Asserts */
   assert(NULL != p_lcda_instance);

   Lcda_Init_Core_Input(&p_lcda_instance->core_input, p_pa_data);

   /* reset core output structure */
   p_core_output              = &p_lcda_instance->core_output;
   p_core_output->lcda_status = LCDA_STATUS_ENABLED;

   /* reset sub features */
   Lcda_Reset_Bsw_Core(&p_core_output->bsw_core_output, &p_lcda_instance->bsw_persistent, &p_lcda_instance->persistent,
                       &p_lcda_instance->core_input);
   Lcda_Reset_Cvw_Core(&p_core_output->cvw_core_output, &p_lcda_instance->persistent, &p_lcda_instance->cvw_persistent);
   Lcda_Reset_Slc_Core(&p_core_output->slc_core_output, &p_lcda_instance->slc_persistent);
   Lcda_Reset_Elc_Core(&p_core_output->elc_core_output, &p_lcda_instance->elc_persistent);

   Lcda_Init_Persistent(&p_lcda_instance->persistent);
}

void Lcda_Core_Run(Lcda_Instance_T *p_lcda_instance, const Fbk_Output_T *p_fbk_output)
{
   Lcda_Status_T lcda_enable_status;
   const Fbk_Vehicle_Data_T *p_vehicle_data;

   /* Asserts */
   assert(NULL != p_lcda_instance);
   assert(NULL != p_fbk_output);

   /* Reset debug data */
   Binary_Lcda_Debug_Reset_Data();


   /* Fill vehicle data */
   p_vehicle_data = &p_lcda_instance->core_input.p_pa_data->vehicle_data;

   lcda_enable_status = Lcda_Get_Enable_Status(&p_lcda_instance->calibration, &p_lcda_instance->core_input);

   if (LCDA_STATUS_ENABLED == lcda_enable_status)
   {
      /* Process Vehicle States*/
      Lcda_Update_Vehicle_States(p_vehicle_data, &p_lcda_instance->calibration, &p_lcda_instance->persistent);

      /* Update the output variable with the activation status */
      p_lcda_instance->core_output.lcda_status =
         Lcda_Get_Activation_Status(&p_lcda_instance->calibration, p_vehicle_data, &p_lcda_instance->persistent);

      /* Check if BSW is active and reset otherwise */
      if ((LCDA_STATUS_ACTIVE == p_lcda_instance->core_output.lcda_status)
          && (Fbk_Is_True(Lcda_Is_Bsw_Activated(&p_lcda_instance->calibration, &p_lcda_instance->core_input))))
      {
         p_lcda_instance->core_output.bsw_core_output.f_bsw_is_enabled = FBK_TRUE;
         p_lcda_instance->persistent.f_bsw_prev_reset                  = FBK_FALSE;
      }
      else
      {
         if (Fbk_Is_False(p_lcda_instance->persistent.f_bsw_prev_reset))
         {
            Lcda_Reset_Bsw_Core(&p_lcda_instance->core_output.bsw_core_output, &p_lcda_instance->bsw_persistent,
                                &p_lcda_instance->persistent, &p_lcda_instance->core_input);
         }
      }

      /* Check if CVW is active and reset otherwise */
      if ((LCDA_STATUS_ACTIVE == p_lcda_instance->core_output.lcda_status)
          && (Fbk_Is_True(Lcda_Is_Cvw_Activated(&p_lcda_instance->calibration, &p_lcda_instance->core_input))))
      {
         p_lcda_instance->core_output.cvw_core_output.f_cvw_is_enabled = FBK_TRUE;
         p_lcda_instance->persistent.f_cvw_prev_reset                  = FBK_FALSE;
      }
      else
      {
         if (Fbk_Is_False(p_lcda_instance->persistent.f_cvw_prev_reset))
         {
            Lcda_Reset_Cvw_Core(&p_lcda_instance->core_output.cvw_core_output, &p_lcda_instance->persistent,
                                &p_lcda_instance->cvw_persistent);
         }
      }

      /* Check if SLC is active and reset otherwise */
      if ((LCDA_STATUS_ACTIVE == p_lcda_instance->core_output.lcda_status)
          && (Fbk_Is_True(Lcda_Is_Slc_Activated(&p_lcda_instance->calibration, &p_lcda_instance->core_input))))
      {
         p_lcda_instance->core_output.slc_core_output.f_slc_is_enabled = FBK_TRUE;
      }
      else
      {
         Lcda_Reset_Slc_Core(&p_lcda_instance->core_output.slc_core_output, &p_lcda_instance->slc_persistent);
      }

      /* Check if ELC is active and reset otherwise */
      if ((LCDA_STATUS_ACTIVE == p_lcda_instance->core_output.lcda_status)
          && (Fbk_Is_True(Lcda_Is_Elc_Activated(&p_lcda_instance->calibration, &p_lcda_instance->core_input))))
      {
         p_lcda_instance->core_output.elc_core_output.f_elc_is_enabled = FBK_TRUE;
      }
      else
      {
         Lcda_Reset_Elc_Core(&p_lcda_instance->core_output.elc_core_output, &p_lcda_instance->elc_persistent);
      }

      /* Process all active submodules */
      Lcda_Process_All_Active_Submodules(p_lcda_instance, p_fbk_output);
   }
   else
   {
      /* LCDA is disabled so reinitialize all outputs and don't do anything else */
      Lcda_Reset(p_lcda_instance, p_fbk_output->p_pa_data);
      p_lcda_instance->core_output.lcda_status = lcda_enable_status;
   }

   /* Pass general data to debug data */
   Binary_Lcda_Debug_Pass_General_Data(&p_lcda_instance->core_input, &p_lcda_instance->core_output, &p_lcda_instance->persistent,
                                       &p_lcda_instance->calibration);
}

/* coverity[misra_c_2012_rule_8_7_violation][This function is needed by the LCDA debug writer] */
void Lcda_Init_Persistent(Lcda_Persistent_T *p_lcda_persistent)
{
   p_lcda_persistent->turn_signal_held                 = TURN_SIGNAL_NONE;
   p_lcda_persistent->turn_signal_held_counter         = FBK_ZERO_UINT;
   p_lcda_persistent->f_host_speed_in_activation_range = FBK_FALSE;
   p_lcda_persistent->f_curve_radius_valid             = FBK_FALSE;
   p_lcda_persistent->f_bsw_prev_reset                 = FBK_FALSE;
   p_lcda_persistent->f_cvw_prev_reset                 = FBK_FALSE;
}

/*===========================================================================*\
* Local Functions Definitions
\*===========================================================================*/

static void Lcda_Process_All_Active_Submodules(Lcda_Instance_T *p_lcda_instance, const Fbk_Output_T *p_fbk_output)
{
   uint8_t idx;
   float32_T cvw_ttc_threshold[FBK_NUMBER_OF_SIDES];
   boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES];
   Lcda_Core_Output_T *p_core_output;
   const Lcda_Core_Input_T *p_core_input;
   const Lcda_Core_Calibration_T *p_cals;
   boolean_T f_bsw_active;
   boolean_T f_cvw_active;
   boolean_T f_slc_active;
   boolean_T f_elc_active;

   /* Asserts */
   assert(NULL != p_lcda_instance);
   assert(NULL != p_fbk_output);

   p_core_output = &p_lcda_instance->core_output;
   p_core_input  = &p_lcda_instance->core_input;
   p_cals        = &p_lcda_instance->calibration;

   f_bsw_active = p_core_output->bsw_core_output.f_bsw_is_enabled;
   f_cvw_active = p_core_output->cvw_core_output.f_cvw_is_enabled;
   f_slc_active = p_core_output->slc_core_output.f_slc_is_enabled;
   f_elc_active = p_core_output->elc_core_output.f_elc_is_enabled;

   if (f_bsw_active)
   {
      Lcda_Preprocess_Bsw(&p_core_output->bsw_core_output, p_core_input, p_cals, p_fbk_output, &p_lcda_instance->bsw_persistent);
   }
   if (f_cvw_active)
   {
      Lcda_Preprocess_Cvw(&p_core_output->cvw_core_output, cvw_ttc_threshold, f_use_small_lc_intention_zone, p_core_input, p_cals,
                          p_fbk_output, &p_lcda_instance->cvw_persistent);
   }
   if (f_slc_active)
   {
      Lcda_Preprocess_Slc(&p_core_output->slc_core_output, p_core_input, p_cals);
   }
   if (f_elc_active)
   {
      Lcda_Preprocess_Elc(&p_core_output->elc_core_output, p_core_input, p_cals);
   }

   for (idx = FBK_ZERO_UINT; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
   {
      if (Lcda_Is_Valid_Object(&p_core_input->p_pa_data->object_data[idx], p_cals))
      {
         const Fbk_Object_Data_T *p_tracker_object = &p_core_input->p_pa_data->object_data[idx];

         if (f_bsw_active)
         {
            Lcda_Process_Bsw_Object(&p_core_output->bsw_core_output, p_tracker_object, p_core_input, p_cals,
                                    &p_lcda_instance->persistent, &p_lcda_instance->bsw_persistent, &p_lcda_instance->cvw_persistent);
         }
         if (f_cvw_active)
         {
            Lcda_Process_Cvw_Object(&p_core_output->cvw_core_output, cvw_ttc_threshold, f_use_small_lc_intention_zone,
                                    p_tracker_object, p_core_input, p_cals, &p_lcda_instance->persistent,
                                    &p_lcda_instance->cvw_persistent);
         }
         if (f_slc_active)
         {
            Lcda_Process_Slc_Object(&p_core_output->slc_core_output, p_tracker_object, p_core_input, p_cals,
                                    &p_lcda_instance->slc_persistent);
         }
         if (f_elc_active)
         {
            Lcda_Process_Elc_Object(&p_core_output->elc_core_output, p_tracker_object, p_core_input, p_cals,
                                    &p_lcda_instance->elc_persistent);
         }
      }
   }

   if (f_bsw_active)
   {
      Lcda_Postprocess_Bsw(&p_lcda_instance->core_output.bsw_core_output, &p_lcda_instance->bsw_persistent,
                           &p_lcda_instance->core_input, p_cals, &p_lcda_instance->persistent, p_fbk_output);
   }
   if (f_cvw_active)
   {
      Lcda_Postprocess_Cvw(&p_core_output->cvw_core_output, f_use_small_lc_intention_zone, p_cals, &p_lcda_instance->persistent,
                           &p_lcda_instance->cvw_persistent);
   }
   if (f_slc_active)
   {
      Lcda_Postprocess_Slc(&p_core_output->slc_core_output, p_cals, &p_lcda_instance->slc_persistent);
   }
   if (f_elc_active)
   {
      Lcda_Postprocess_Elc(&p_core_output->elc_core_output, p_cals, &p_lcda_instance->elc_persistent);
   }
}

static boolean_T Lcda_Is_Valid_Object(const Fbk_Object_Data_T *p_object_data, const Lcda_Core_Calibration_T *p_cals)
{
   boolean_T f_obj_is_valid = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_object_data);
   assert(NULL != p_cals);

   if ((PA_OBJ_STATUS_INVALID != p_object_data->status) && (p_object_data->age >= p_cals->k_lcda_min_track_age)
       && (Fbk_Abs_F(p_object_data->vcs_pos.x) <= p_cals->k_lcda_max_range) && (p_object_data->f_moveable)
       && (Fbk_Is_False(p_cals->k_lcda_f_enable_obj_reflection_flag_check && p_object_data->f_reflection))
       && Lcda_Is_VRU_Object_Valid(p_object_data, p_cals))
   {
      f_obj_is_valid = FBK_TRUE;
   }

   return f_obj_is_valid;
}

static boolean_T Lcda_Is_VRU_Object_Valid(const Fbk_Object_Data_T *p_object_data, const Lcda_Core_Calibration_T *p_cals)
{
   boolean_T f_vru_obj_is_valid = FBK_TRUE;
   float32_T obj_area           = p_object_data->width * p_object_data->length;
   Pa_Obj_Class_T obj_class     = p_object_data->obj_class;

   /* Asserts */
   assert(NULL != p_object_data);
   assert(NULL != p_cals);

   switch (obj_class)
   {
      case PA_OBJ_CLASS_UNKNOWN:
      case PA_OBJ_CLASS_PEDESTRIAN:
         if ((obj_area < p_cals->k_lcda_pedestrian_min_size) && (p_object_data->speed < p_cals->k_lcda_pedestrian_min_speed))
         {
            f_vru_obj_is_valid = FBK_FALSE;
         }

         break;

      case PA_OBJ_CLASS_2WHEEL:
         if ((obj_area < p_cals->k_lcda_2wheel_min_size) && (p_object_data->speed < p_cals->k_lcda_2wheel_min_speed))
         {
            f_vru_obj_is_valid = FBK_FALSE;
         }

         break;
      default:
         f_vru_obj_is_valid = FBK_TRUE;
         break;
   }

   return f_vru_obj_is_valid;
}

static void Lcda_Init_Core_Input(Lcda_Core_Input_T *p_core_input, const Pa_Data_T *p_pa_data)
{
   uint8_t k;

   /* Asserts */
   assert(NULL != p_core_input);

   p_core_input->p_pa_data = p_pa_data; // NULL is accepted value

   p_core_input->lane_width         = FBK_ZERO_F;
   p_core_input->lane_center_offset = FBK_ZERO_F;

   p_core_input->initial_bsw_zone.size     = LCDA_NUMBER_OF_ZONE_POINTS;
   p_core_input->initial_bsw_zone_hys.size = LCDA_NUMBER_OF_ZONE_POINTS;
   p_core_input->initial_cvw_zone.size     = LCDA_NUMBER_OF_ZONE_POINTS;
   p_core_input->initial_cvw_zone_hys.size = LCDA_NUMBER_OF_ZONE_POINTS;

   for (k = FBK_ZERO_UINT; k < LCDA_NUMBER_OF_ZONE_POINTS; k++)
   {
      p_core_input->initial_bsw_zone.points[k].x = FBK_ZERO_F;
      p_core_input->initial_bsw_zone.points[k].y = FBK_ZERO_F;

      p_core_input->initial_bsw_zone_hys.points[k].x = FBK_ZERO_F;
      p_core_input->initial_bsw_zone_hys.points[k].y = FBK_ZERO_F;

      p_core_input->initial_cvw_zone.points[k].x = FBK_ZERO_F;
      p_core_input->initial_cvw_zone.points[k].y = FBK_ZERO_F;

      p_core_input->initial_cvw_zone_hys.points[k].x = FBK_ZERO_F;
      p_core_input->initial_cvw_zone_hys.points[k].y = FBK_ZERO_F;
   }

   p_core_input->warn_settings.bsw_len_factor                       = LCDA_DEFAULT_FACTOR;
   p_core_input->warn_settings.cvw_ttc_threshold                    = FBK_ZERO_F;
   p_core_input->warn_settings.slc_ttc_thres_lon                    = FBK_ZERO_F;
   p_core_input->warn_settings.slc_ttc_thres_lat                    = FBK_ZERO_F;
   p_core_input->warn_settings.f_use_cvw_lane_change_intention_zone = FBK_FALSE;

   p_core_input->guardrail_data[FBK_SIDE_LEFT].radar.lateral_position  = FBK_ZERO_F;
   p_core_input->guardrail_data[FBK_SIDE_LEFT].radar.confidence        = FBK_ZERO_F;
   p_core_input->guardrail_data[FBK_SIDE_LEFT].radar.status            = LCDA_GUARDRAIL_INVALID;
   p_core_input->guardrail_data[FBK_SIDE_RIGHT].radar.lateral_position = FBK_ZERO_F;
   p_core_input->guardrail_data[FBK_SIDE_RIGHT].radar.confidence       = FBK_ZERO_F;
   p_core_input->guardrail_data[FBK_SIDE_RIGHT].radar.status           = LCDA_GUARDRAIL_INVALID;

   p_core_input->guardrail_data[FBK_SIDE_LEFT].camera.lateral_position  = FBK_ZERO_F;
   p_core_input->guardrail_data[FBK_SIDE_LEFT].camera.confidence        = FBK_ZERO_F;
   p_core_input->guardrail_data[FBK_SIDE_LEFT].camera.status            = LCDA_GUARDRAIL_INVALID;
   p_core_input->guardrail_data[FBK_SIDE_RIGHT].camera.lateral_position = FBK_ZERO_F;
   p_core_input->guardrail_data[FBK_SIDE_RIGHT].camera.confidence       = FBK_ZERO_F;
   p_core_input->guardrail_data[FBK_SIDE_RIGHT].camera.status           = LCDA_GUARDRAIL_INVALID;

   p_core_input->f_lane_change[FBK_SIDE_LEFT]  = FBK_FALSE;
   p_core_input->f_lane_change[FBK_SIDE_RIGHT] = FBK_FALSE;

   p_core_input->enabled_flags.f_lcda_enabled     = FBK_FALSE;
   p_core_input->enabled_flags.f_bsw_enabled      = FBK_FALSE;
   p_core_input->enabled_flags.f_cvw_enabled      = FBK_FALSE;
   p_core_input->enabled_flags.f_slc_enabled      = FBK_FALSE;
   p_core_input->enabled_flags.f_elc_enabled      = FBK_FALSE;
   p_core_input->enabled_flags.f_dropback_enabled = FBK_FALSE;
   p_core_input->enabled_flags.f_fallback_enabled = FBK_FALSE;

   p_core_input->bsw_zone_calculation_mode     = BSW_ZONE_CALC_DEFAULT;
   p_core_input->cvw_crit_mode[FBK_SIDE_LEFT]  = CVW_CRIT_LONG_DIST;
   p_core_input->cvw_crit_mode[FBK_SIDE_RIGHT] = CVW_CRIT_LONG_DIST;

   /* Trailer information */
   p_core_input->trailer.f_trailer_present = FBK_FALSE;
   p_core_input->trailer.length            = FBK_ZERO_F;
   p_core_input->trailer.width             = FBK_ZERO_F;
   p_core_input->trailer.angle             = FBK_ZERO_F;
}

static boolean_T Lcda_Is_Host_Speed_In_Activation_Range(const Lcda_Core_Calibration_T *p_cals,
                                                        const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                        const Lcda_Persistent_T *p_lcda_persistent)
{
   float32_T min_host_activation_speed;
   float32_T max_host_activation_speed;
   boolean_T f_host_speed_in_activation_range = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_cals);
   assert(NULL != p_lcda_persistent);

   /* If host speed was previously in activation range, then apply hysteresis */
   if (Fbk_Is_True(p_lcda_persistent->f_host_speed_in_activation_range))
   {
      min_host_activation_speed = p_cals->k_lcda_host_activation_speed_min - p_cals->k_lcda_host_activation_speed_min_hys;
      max_host_activation_speed = p_cals->k_lcda_host_activation_speed_max + p_cals->k_lcda_host_activation_speed_max_hys;
   }
   else
   {
      min_host_activation_speed = p_cals->k_lcda_host_activation_speed_min;
      max_host_activation_speed = p_cals->k_lcda_host_activation_speed_max;
   }

   if ((p_vehicle_data->host_speed >= min_host_activation_speed) && (p_vehicle_data->host_speed <= max_host_activation_speed))
   {
      f_host_speed_in_activation_range = FBK_TRUE;
   }

   return f_host_speed_in_activation_range;
}


static void Lcda_Update_Vehicle_States(const Fbk_Vehicle_Data_T *p_vehicle_data,
                                       const Lcda_Core_Calibration_T *p_cals,
                                       Lcda_Persistent_T *p_lcda_persistent)
{
   /* Asserts */
   assert(NULL != p_cals);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_lcda_persistent);

   /* Process the host activation speed */
   p_lcda_persistent->f_host_speed_in_activation_range =
      Lcda_Is_Host_Speed_In_Activation_Range(p_cals, p_vehicle_data, p_lcda_persistent);

   /* Verify if curve radius is valid */
   p_lcda_persistent->f_curve_radius_valid = Lcda_Is_Curve_Radius_Valid(p_cals, p_vehicle_data, p_lcda_persistent);

   /* hold turn signal */
   Lcda_Process_Veh_Turn_Signal(p_vehicle_data, p_cals, p_lcda_persistent);
}


static void Lcda_Process_Veh_Turn_Signal(const Fbk_Vehicle_Data_T *p_vehicle_data,
                                         const Lcda_Core_Calibration_T *p_cals,
                                         Lcda_Persistent_T *p_lcda_persistent)
{
   Lcda_Turn_Signal_T turn_signal;

   /* Asserts */
   assert(NULL != p_cals);
   assert(NULL != p_lcda_persistent);

   /* coverity[misra_c_2012_rule_10_5_violation][Intentional cast from unsigned integer to matching enum type]  */
   turn_signal = (Lcda_Turn_Signal_T) p_vehicle_data->turn_signal;

   if (turn_signal != TURN_SIGNAL_NONE)
   {
      p_lcda_persistent->turn_signal_held         = turn_signal;
      p_lcda_persistent->turn_signal_held_counter = FBK_ZERO_UINT;
   }
   else if (p_lcda_persistent->turn_signal_held != TURN_SIGNAL_NONE)
   {
      Sat_Inc_Uint8(&(p_lcda_persistent->turn_signal_held_counter));
      if (p_lcda_persistent->turn_signal_held_counter > p_cals->k_lcda_turn_signal_coast_cycles)
      {
         p_lcda_persistent->turn_signal_held = TURN_SIGNAL_NONE;
      }
   }
   else
   {
      /* No change and let the persistence variable for turn signal remain TURN_SIGNAL_NONE */
   }
}

static boolean_T Lcda_Is_Curve_Radius_Valid(const Lcda_Core_Calibration_T *p_cals,
                                            const Fbk_Vehicle_Data_T *p_vehicle_data,
                                            const Lcda_Persistent_T *p_lcda_persistent)
{
   boolean_T f_radius_valid;
   boolean_T f_previous_radius_valid = p_lcda_persistent->f_curve_radius_valid;
   float32_T min_curve_radius        = p_cals->k_lcda_min_curve_radius;
   float32_T curve_radius;

   /* Asserts */
   assert(NULL != p_cals);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_lcda_persistent);


   /* If host was previously below the minimum curve radius, then apply hysteresis now */
   if (Fbk_Is_False(f_previous_radius_valid))
   {
      min_curve_radius += p_cals->k_lcda_min_curve_radius_hys;
   }

   /* Verify current radius  */
   if (Fbk_Abs_F(p_vehicle_data->curvature) > THRESHOLD_IS_ZERO)
   {
      curve_radius = Fbk_Abs_F(1.0f / p_vehicle_data->curvature);
      if (curve_radius >= min_curve_radius)
      {
         f_radius_valid = FBK_TRUE;
      }
      else
      {
         f_radius_valid = FBK_FALSE;
      }
   }
   else
   {
      f_radius_valid = FBK_TRUE;
   }

   return f_radius_valid;
}

static Lcda_Status_T Lcda_Get_Enable_Status(const Lcda_Core_Calibration_T *p_cals, const Lcda_Core_Input_T *p_core_input)
{
   Lcda_Status_T lcda_status;

   /* Asserts */
   assert(NULL != p_cals);
   assert(NULL != p_core_input);

   /* If the use cal is enabled, then the calibration determines the enable state */
   if (Fbk_Is_True(p_cals->k_lcda_enable_via_cal))
   {
      if (Fbk_Is_True(p_cals->k_lcda_enable))
      {
         lcda_status = LCDA_STATUS_ENABLED;
      }
      else
      {
         lcda_status = LCDA_STATUS_DISABLED_BY_CAL;
      }
   }
   else
   {
      /* Use the enable flag from the core input if the use cal is false */
      if (p_core_input->enabled_flags.f_lcda_enabled)
      {
         lcda_status = LCDA_STATUS_ENABLED;
      }
      else
      {
         lcda_status = LCDA_STATUS_DISABLED_BY_INPUT;
      }
   }


   return lcda_status;
}


static Lcda_Status_T Lcda_Get_Activation_Status(const Lcda_Core_Calibration_T *p_cals,
                                                const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                const Lcda_Persistent_T *p_lcda_persistent)
{
   Lcda_Status_T lcda_active_status;

   /* Assert */
   assert(NULL != p_lcda_persistent);

   /* Check if LCDA needs to be deactivated */
   if (Fbk_Is_False(p_lcda_persistent->f_host_speed_in_activation_range))
   {
      /* Since host speed range check was already done beforehand, we only need to find out if host speed was too low or too high.
       * Therefore check thresholds without hysteresis. */
      if (p_vehicle_data->host_speed < p_cals->k_lcda_host_activation_speed_min)
      {
         lcda_active_status = LCDA_STATUS_DEACTIVATED_LOW_EGO_SPEED;
      }
      else if (p_vehicle_data->host_speed > p_cals->k_lcda_host_activation_speed_max)
      {
         lcda_active_status = LCDA_STATUS_DEACTIVATED_HIGH_EGO_SPEED;
      }
      else
      {
         /* This should not be possible and would indicate an error in the host speed range check. */
         lcda_active_status = LCDA_STATUS_DEACTIVATED_INTERNAL_ERROR;
      }
   }
   /* Check curvilinearity */
   else if (Fbk_Is_True(p_cals->k_lcda_f_disable_due_to_small_curve_radius) && Fbk_Is_False(p_lcda_persistent->f_curve_radius_valid))
   {
      lcda_active_status = LCDA_STATUS_DEACTIVATED_LOW_CURVE_RADIUS;
   }
   else
   {
      lcda_active_status = LCDA_STATUS_ACTIVE;
   }

   return lcda_active_status;
}


static boolean_T Lcda_Is_Bsw_Activated(const Lcda_Core_Calibration_T *p_cals, const Lcda_Core_Input_T *p_core_input)
{
   boolean_T f_bsw_enabled;

   /* Asserts */
   assert(NULL != p_cals);
   assert(NULL != p_core_input);

   /* Check BSW enable flag */
   if (Fbk_Is_True(p_cals->k_bsw_enable_via_cal))
   {
      f_bsw_enabled = p_cals->k_bsw_enable;
   }
   else
   {
      f_bsw_enabled = p_core_input->enabled_flags.f_bsw_enabled;
   }

   return f_bsw_enabled;
}


static boolean_T Lcda_Is_Cvw_Activated(const Lcda_Core_Calibration_T *p_cals, const Lcda_Core_Input_T *p_core_input)
{
   boolean_T f_cvw_enabled;

   /* Asserts */
   assert(NULL != p_cals);
   assert(NULL != p_core_input);

   if (Fbk_Is_True(p_cals->k_cvw_enable_via_cal))
   {
      f_cvw_enabled = p_cals->k_cvw_enable;
   }
   else
   {
      f_cvw_enabled = p_core_input->enabled_flags.f_cvw_enabled;
   }

   return f_cvw_enabled;
}


static boolean_T Lcda_Is_Slc_Activated(const Lcda_Core_Calibration_T *p_cals, const Lcda_Core_Input_T *p_core_input)
{
   boolean_T f_slc_enabled;

   /* Asserts */
   assert(NULL != p_cals);
   assert(NULL != p_core_input);

   if (Fbk_Is_True(p_cals->k_slc_enable_via_cal))
   {
      f_slc_enabled = p_cals->k_slc_enable;
   }
   else
   {
      f_slc_enabled = p_core_input->enabled_flags.f_slc_enabled;
   }

   return f_slc_enabled;
}


static boolean_T Lcda_Is_Elc_Activated(const Lcda_Core_Calibration_T *p_cals, const Lcda_Core_Input_T *p_core_input)
{
   boolean_T f_elc_enabled;

   /* Asserts */
   assert(NULL != p_cals);
   assert(NULL != p_core_input);

   if (Fbk_Is_True(p_cals->k_elc_enable_via_cal))
   {
      f_elc_enabled = p_cals->k_elc_enable;
   }
   else
   {
      f_elc_enabled = p_core_input->enabled_flags.f_elc_enabled;
   }

   return f_elc_enabled;
}
