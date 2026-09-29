/**
 * @file esa.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Implementation of ESA main module
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */


/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "esa.h"
#include "esa_create_zone.h"
#include "esa_debug_interface.h"
#include "esa_types.h"
#include "fbk_field_of_interest.h"
#include "fbk_field_of_interest_factory.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_object_validation.h"
#include "fbk_vehicle_data_t.h"
#include "ml_math.h"
#include "ml_saturated_math.h"
#include "ml_vector_2d_t.h"
#include "pa_const_macros.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include <assert.h>

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
 * @SRD{CSCSA-68633}
 * @SAD{CSCSA-83853}
 * @SDD{CSCSA-65976}
 * @verification{Create a tests to verify that output data is return correctly from function output}
 */
static void Esa_Process(Esa_Core_Output_T *p_esa_core_output /**< ESA core output */,
                        Esa_Persistent_T *p_esa_persistent /**< ESA persistent */,
                        const Esa_Core_Input_T *p_esa_core_input /**< ESA core input */,
                        const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                        const Esa_Core_Calibration_T *p_esa_calibration /**< ESA calibrations */);

/**
 * @brief Checks whether basic object properties are fulfilled so that the object can be used for further analysis.
 *
 * @return True when object is a valid ESA candidate.
 *
 * @SRD{CSCSA-69926,CSCSA-69922,CSCSA-69920}
 * @SAD{CSCSA-83853}
 * @SDD{CSCSA-65978}
 * @verification{Create a test with an object which has valid tracker output attributes, so that true is returned.}
 */
static boolean_T Esa_Is_Valid_Object(const Fbk_Object_Data_T *p_object /**< FBK object */,
                                     const Esa_Core_Calibration_T *p_esa_calibration /**< ESA calibrations */);


/**
 * @brief Resets Esa core output signals to its default.
 *
 * @return void
 *
 * @SRD{}
 * @SAD{}
 * @SDD{CSCSA-65972}
 * @verification{Check whether esa core output are reset to their default values.}
 */
static void Esa_Reset_Core_Output(Esa_Core_Output_T *p_esa_core_output /**< Esa core output */);


/**
 * @brief Resets Esa core input signals to its default.
 *
 * @return void
 *
 * @SRD{CSCSA-68587}
 * @SAD{CSCSA-83853}
 * @SDD{CSCSA-65974}
 * @verification{Check whether esa core input are reset to their default values.}
 */
static void Esa_Init_Core_Input(Esa_Core_Input_T *p_esa_core_input /**< Esa core input */);


/**
 * @brief Updates persistent esa modes with the following info from the ego vehicle state:
 *		  1. Checks if the host is above activation speed (applies hysteresis).
 *        2. Processes and stores the turn signal info.
 *        3. Checks if the feature needs to be disabled due to low curve radius (applies hysteresis).
 *
 * @return void
 *
 * @SRD{CSCSA-68625,CSCSA-68626,CSCSA-122858}
 * @SAD{CSCSA-83853}
 * @SDD{CSCSA-65988}
 * @verification{Create a test where it is checked whether all persistent modes of esa are filled accordingly.}
 */
static void Esa_Update_Vehicle_States(const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                      const Esa_Core_Calibration_T *p_esa_calibration /**< Esa calibrations */,
                                      Esa_Persistent_T *p_esa_persistent /**< Esa persistent data */);


/**
 * @brief Checks whether host is above the activation speed threshold.
 * It computes the host activation speed threshold by adding the speed hysteresis
 * value if the host was previously above the activation speed threshhold.
 *
 * @return True when the host is withing the activation speed threshold
 *
 * @SRD{CSCSA-72562}
 * @SAD{CSCSA-83853}
 * @SDD{CSCSA-65989}
 * @verification{Create tests where in one of them the hysteresis is applied and in the other not. Check that the the host is
 * inside those ranges.}
 */
static boolean_T Esa_Is_Host_Speed_In_Activation_Range(const Esa_Core_Calibration_T *p_esa_calibration /**< Esa calibrations */,
                                                       const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                                       const Esa_Persistent_T *p_esa_persistent /**< Esa persistent data */);
/**
 * @brief Returns the current ESA Enabling State based on the core input and
 * calibration parameter.
 *
 * @return Esa_Core_Status_T
 *
 * @SRD{CSCSA-68629,CSCSA-72560,CSCSA-72561}
 * @SAD{CSCSA-83853}
 * @SDD{CSCSA-65990}
 * @verification{Create tests where ESA is either enabled by calibration or by its input.}
 */
static Esa_Core_Status_T Esa_Get_Enable_Status(const Esa_Core_Calibration_T *p_esa_calibration /**< Esa calibrations */,
                                               const Esa_Core_Input_T *p_esa_core_input /**< Esa core input */);

/**
 * @brief Returns the current ESA Activation State based on info in the persistent esa structure
 *		  It is assumed that ESA is already enabled.
 *
 * @return Esa_Core_Status_T
 *
 * @SRD{CSCSA-68625,CSCSA-68626}
 * @SAD{CSCSA-83853}
 * @SDD{CSCSA-65991}
 * @verification{Create a test where Esa is not disabled due to curve radius or activation speed. Only then the activated state is
 * expected.}
 */
static Esa_Core_Status_T Esa_Get_Activation_Status(const Esa_Core_Calibration_T *p_esa_calibration /**< Esa calibrations */,
                                                   const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                                   const Esa_Persistent_T *p_esa_persistent /**< Esa persistent data */);

/**
 * @brief Returns the side of the Host where the Object is placed, based on its lateral position, VCS or curvilinear.
 *
 * @return uint8_t
 *
 * @SRD{}
 * @SAD{}
 * @SDD{CSCSA-87222}
 * @verification{Create a test to check whether the side is determined properly.}
 */
static uint8_t Esa_Get_Object_Side(const Esa_Object_T *p_esa_object, const Coordinate_System_T coordinate_system);


/**
 * @brief Checks if ESA should be disabled due to low curve radius.
 *
 * @return True when curve radius is less than the specified threshold
 *
 * @SRD{CSCSA-68626}
 * @SAD{CSCSA-83853}
 * @SDD{CSCSA-65992}
 * @verification{Create a test to check whether Esa gets disabled when low curve radius is available.}
 */
static boolean_T Esa_Is_Disabled_By_Small_Curve_Radius(const Esa_Core_Calibration_T *p_esa_calibration /**< Esa calibrations */,
                                                       const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                                       const Esa_Persistent_T *p_esa_persistent /**< Esa persistent data */);

/**
 * @brief Resets Esa core output on the given side.
 *
 * @return void
 *
 * @SRD{CSCSA-68594,CSCSA-68628}
 * @SAD{CSCSA-83853}
 * @SDD{CSCSA-65983}
 * @verification{Check whether Esa core output on the given side is reset correctly.}
 */
static void Esa_Clear_Core_Output_One_Side(Esa_Core_Output_T *p_esa_core_output /**< Esa core output */,
                                           const uint8_t side /**< side index */);


/**
 * @brief Initializes Esa object data.
 *
 * @return void
 *
 * @SRD{CSCSA-122858}
 * @SAD{CSCSA-83853}
 * @SDD{CSCSA-65984}
 * @verification{Check whether Esa object data is initialized correctly.}
 */
static void Esa_Init_Object_Data(Esa_Object_T *p_esa_object /**< Esa object data */,
                                 const Fbk_Object_Data_T *p_tracker_object /**< Fbk tracker object */);

/**
 * @brief Maps Esa core output data to the persistent Esa data structure.
 *
 * @return void
 *
 * @SRD{CSCSA-68628,CSCSA-69922}
 * @SAD{CSCSA-83853}
 * @SDD{CSCSA-65985}
 * @verification{Check whether persistent Esa data is updated correctly.}
 */
static void Esa_Fill_Side_Persistents(Esa_Persistent_T *p_esa_persistent /**< Esa persistent data */,
                                      const Esa_Core_Output_T *p_esa_core_output /**< Esa core output */);

/**
 * @brief Sets most critical Esa object based on objects deceleration to reach the hosts speed.
 *
 * @return void
 *
 * @SRD{CSCSA-69929,CSCSA-69927,CSCSA-69925,CSCSA-69917}
 * @SAD{CSCSA-83853}
 * @SDD{CSCSA-65986}
 * @verification{Check whether most critical esa objects data is updated correctly.}
 */
static void Esa_Set_Most_Critical_Object(Esa_Core_Output_T *p_esa_core_output /**< Esa core output */,
                                         const Esa_Object_T *p_esa_object /**< Esa object data */,
                                         const Esa_Core_Calibration_T *p_esa_calibration /**< Esa calibrations */);

/**
 * @brief Checks whether object was most critical object for Esa in the last cycle.
 *
 * @return True when object caused an Esa warning in the last cycle.
 *
 * @SRD{CSCSA-69925,CSCSA-72566}
 * @SAD{CSCSA-83853}
 * @SDD{CSCSA-65987}
 * @verification{Create a test with an object which has caused an Esa warning in the last cycle.}
 */
static boolean_T Esa_Was_Most_Critical_Obj_Last_Cycle(const Esa_Object_T *p_esa_object /**< Esa object data */,
                                                      const Esa_Persistent_T *p_esa_persistent /**< Esa persistent data*/);

/**
 * @brief Checks whether Ttc is below a internal determined threshold. This threshold might have an hysteresis applied,
 *        when the object has been Esa critical before.
 *
 * @return True when Ttc is below a given threshold
 *
 * @SRD{CSCSA-69918}
 * @SAD{CSCSA-83853}
 * @SDD{CSCSA-65973}
 * @verification{Create tests where a ttc hysteresis is applied and and one case where it is not. When the given objects ttc is
 * below the threshold, true is expected.}
 */
static boolean_T Esa_Is_Ttc_Below_Threshold(const Esa_Object_T *p_esa_object /**< Esa object data */,
                                            const Esa_Core_Calibration_T *p_esa_calibration /**< Esa calibrations */,
                                            const Esa_Persistent_T *p_esa_persistent /**< Esa persistent data */);

/**
 * @brief Check if the passed objects deceleration value is above a value that is deemed safe via cal setting.
 *        To avoid warn level toggeling, a hysteresis value is used if the object was critical last cycle.
 *
 * @return True when objects deceleration to reach host speed is below a given threshold
 *
 * @SRD{}
 * @SAD{}
 * @SDD{CSCSA-65975}
 * @verification{Create tests where a deceleration hysteresis is applied and and one case where it is not. When the given objects
 * deceleration to reach host speed is below the threshold, true is expected.}
 */
static boolean_T Esa_Is_Deceleration_Critical(const Esa_Object_T *p_esa_object /**< Esa object data */,
                                              const Esa_Core_Calibration_T *p_esa_calibration /**< Esa calibrations */,
                                              const Esa_Persistent_T *p_esa_persistent /**< Esa persistent data */);

/**
 * @brief Checks whether object is relevant for Esa.
 *
 * @return True when object is relevant for Esa
 *
 * @SRD{}
 * @SAD{}
 * @SDD{CSCSA-65977}
 * @verification{Create a object in a test with a status of mature or coasted but with a curvi heading and a longitudinal curvi
 * velocity less than the used threshold. Only then true is expected.}
 */
static boolean_T Esa_Is_Object_Relevant(const Fbk_Object_Data_T *p_tracker_object /**< Fbk tracker data */,
                                        const Esa_Core_Calibration_T *p_esa_calibration /**< Esa calibrations */);

/**
 * @brief Makes use of the FBK functionality to create a field of interest for a given tracker object.
 * Can be used for VCS and curvi object properties.
 *
 * @return Object field of interest for given object and coordinate system.
 *
 * @SRS{CSCSA-69920}
 * @SAE{CSCSA-83853}
 * @SDD{CSCSA-65979}
 * @verification{Create a tests to check is foi is set up correctly}
 */
static void Esa_Create_Object_Field_Of_Interest(Fbk_Field_Of_Interest_T *p_object_foi,     /**< Object field of interest */
                                                const Fbk_Object_Data_T *p_tracker_object, /**< Fbk tracker object */
                                                const Coordinate_System_T coordinate_system /**< Coordinate system to use */);


/**
 * @brief Check is object area is overapping with warning zone
 *
 * @return True when object is in zone
 *
 * @SRD{CSCSA-69920}
 * @SAD{CSCSA-83853}
 * @SDD{CSCSA-65981}
 * @verification{Create a test to check is overlapping of the zone and target is being detected correctly}
 */
static boolean_T Esa_Is_Object_In_Zone(const Fbk_Object_Data_T *p_tracker_object, /**< Fbk tracker object */
                                       const Fbk_Field_Of_Interest_T *p_zone,     /** Field of intrest esa zone*/
                                       const Coordinate_System_T coordinate_system);


/**
 * @brief Increment vale of mature counter for object inside zone
 *
 * @return void
 *
 * @SRD{CSCSA-68631}
 * @SAD{CSCSA-83853}
 * @SDD{CSCSA-65982}
 * @verification{Create a test to check is increment is correctly added for mature object in the zone}
 */
static void Esa_Increment_Mature_Count_In_Zone(uint8_t *p_mature_counter, /** Value of mature counter */
                                               const Pa_Obj_Status_T status /** Tracker object status */);


/**
 * @brief Process ESA object based on data recived from tracker
 *
 * @return void
 *
 * @SRD{CSCSA-72566}
 * @SAD{}
 * @SDD{CSCSA-65968}
 * @verification{Check is passed esa object is correctly processed by function}
 */
static void Esa_Process_Object(Esa_Core_Output_T *p_esa_core_output /**< ESA core output */,
                               Esa_Persistent_T *p_esa_persistent /**< ESA persistent */,
                               const Fbk_Object_Data_T *p_tracker_object /**< FBK tracker object */,
                               const Esa_Core_Input_T *p_esa_core_input /**< ESA core input */,
                               const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                               const Esa_Core_Calibration_T *p_esa_calibration /**< ESA calibrations */);


/**
 * @brief Processes the Esa output with application of the Esa alert holding logic.
 *
 * @return void
 *
 * @SRD{CSCSA-68594}
 * @SAD{CSCSA-83854,CSCSA-83856}
 * @SDD{CSCSA-65969}
 * @verification{Check that the holding counter is not increased if an active alert is present on the corresponding side.}
 */
static void Esa_Process_Output(Esa_Core_Output_T *p_esa_core_output /**< Esa core output*/,
                               Esa_Persistent_T *p_esa_persistent /**< Esa persistent data*/,
                               const Esa_Core_Calibration_T *p_esa_calibration /**< Esa calibrations*/);


/**
 * @brief Calculates Time To Collision
 *
 * @return void
 *
 * @SRD{}
 * @SAD{}
 * @SDD{CSCSA-65970}
 * @verification{Check that the TTC is correctly calculated for passed tracker object}
 */
static float32_T Esa_Get_Longitudinal_TTC(const Fbk_Object_Data_T *p_tracker_object /**< Fbk tracker object */,
                                          const float32_T ego_length /**Length of ego vehicle */);


/**
 * @brief Calculates TTP for tracker object
 *
 * @return void
 *
 * @SRD{CSCSA-68594,CSCSA-69934}
 * @SAD{CSCSA-83853}
 * @SDD{CSCSA-65971}
 * @verification{Check that the Time To Pass is correctly calculated for tracker object}
 */
static float32_T Esa_Get_TTP(const Fbk_Object_Data_T *p_tracker_object /**< Fbk tracker object */);

/*===========================================================================*\
* Global Functions Definition
\*===========================================================================*/

void Esa_Reset(Esa_Core_Input_T *p_esa_core_input, Esa_Core_Output_T *p_esa_core_output, Esa_Persistent_T *p_esa_persistent)
{
   /* Asserts */
   assert(NULL != p_esa_core_input);
   assert(NULL != p_esa_core_output);
   assert(NULL != p_esa_persistent);

   Esa_Init_Core_Input(p_esa_core_input);

   Esa_Reset_Persistent_Data(p_esa_persistent);

   Esa_Reset_Core_Output(p_esa_core_output);
}


void Esa_Core_Run(Esa_Core_Output_T *p_esa_core_output,
                  Esa_Core_Input_T *p_esa_core_input,
                  Esa_Persistent_T *p_esa_persistent,
                  const Esa_Core_Calibration_T *p_esa_calibration)
{
   Esa_Core_Status_T esa_core_status;
   const Fbk_Vehicle_Data_T *p_vehicle_data;

   /* Asserts */
   assert(NULL != p_esa_core_input);
   assert(NULL != p_esa_core_output);
   assert(NULL != p_esa_persistent);
   assert(NULL != p_esa_calibration);

   /* Reset debug data */
   Binary_Esa_Debug_Reset_Data();

   p_vehicle_data  = &(p_esa_core_input->p_pa_data->vehicle_data);
   esa_core_status = Esa_Get_Enable_Status(p_esa_calibration, p_esa_core_input);

   if (ESA_CORE_STATUS_ENABLED == esa_core_status)
   {
      /* Process Vehicle States*/
      Esa_Update_Vehicle_States(p_vehicle_data, p_esa_calibration, p_esa_persistent);

      /* Update the output variable with the activation status */
      esa_core_status = Esa_Get_Activation_Status(p_esa_calibration, p_vehicle_data, p_esa_persistent);

      /* Check if ESA is active and reset otherwise */
      if (ESA_CORE_STATUS_ACTIVE == esa_core_status)
      {
         esa_core_status = ESA_CORE_STATUS_ENABLED;
      }
      else
      {
         Esa_Reset_Persistent_Data(p_esa_persistent);
         Esa_Reset_Core_Output(p_esa_core_output);
      }

      Esa_Process(p_esa_core_output, p_esa_persistent, p_esa_core_input, p_vehicle_data, p_esa_calibration);
   }
   else
   {
      /* ESA is disabled so reinitialize all outputs and don't do anything else */
      Esa_Reset(p_esa_core_input, p_esa_core_output, p_esa_persistent);
   }

   p_esa_core_output->esa_core_status = esa_core_status;

   /* Pass general data to debug data */
   Binary_Esa_Debug_Pass_General_Data(p_esa_core_input, p_esa_core_output, p_esa_persistent, p_esa_calibration);
}


/* coverity[misra_c_2012_rule_8_7_violation][This function is needed by the ESA debug writer] */
void Esa_Reset_Persistent_Data(Esa_Persistent_T *p_esa_persistent)
{
   uint8_t iobj;
   uint8_t side;

   /* Assert */
   assert(NULL != p_esa_persistent);

   /* Reset mature counts */
   for (iobj = FBK_ZERO_UINT; iobj < PA_OBJ_NUMBER_OF_OBJECTS; iobj++)
   {
      p_esa_persistent->mature_count_in_esa_zone[iobj] = FBK_ZERO_UINT;
   }

   /* Clear the output from the previous cycle for both sides and the holding counter */
   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      p_esa_persistent->prev_esa_alert_obj_index[side] = PA_INVALID_OBJ_INDEX;
      p_esa_persistent->prev_esa_alert_obj_id[side]    = PA_INVALID_OBJ_ID;
      p_esa_persistent->esa_hold_counter[side]         = FBK_ZERO_UINT;
   }

   p_esa_persistent->f_host_speed_in_activation_range = FBK_FALSE;
   p_esa_persistent->f_esa_disabled_low_curve_radius  = FBK_FALSE;
}


/*===========================================================================*\
* Local Functions Definitions
\*===========================================================================*/

static uint8_t Esa_Get_Object_Side(const Esa_Object_T *p_esa_object, const Coordinate_System_T coordinate_system)
{
   uint8_t side;
   float32_T y_pos;

   assert(NULL != p_esa_object);

   switch (coordinate_system)
   {
      case ESA_USE_VCS:
         /* Use VCS lateral position */
         y_pos = p_esa_object->p_tracker_data->vcs_pos.y;
         break;

      case ESA_USE_CURVI:
      default:
         /* Use curvi lateral position */
         y_pos = p_esa_object->p_tracker_data->curvi_pos.y;
         break;
   }

   side = Fbk_Get_Obj_Side(y_pos);

   return side;
}


static void Esa_Process(Esa_Core_Output_T *p_esa_core_output,
                        Esa_Persistent_T *p_esa_persistent,
                        const Esa_Core_Input_T *p_esa_core_input,
                        const Fbk_Vehicle_Data_T *p_vehicle_data,
                        const Esa_Core_Calibration_T *p_esa_calibration)
{
   uint8_t idx;
   uint8_t i_side;
   const Fbk_Object_Data_T *p_tracker_object;

   /* Asserts */
   assert(NULL != p_esa_core_output);
   assert(NULL != p_esa_persistent);
   assert(NULL != p_esa_core_input);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_esa_calibration);

   if (Fbk_Is_True(ESA_CORE_STATUS_ENABLED == p_esa_core_output->esa_core_status))
   {
      Binary_Esa_Debug_Pass_Default_Zone(p_esa_core_input, p_esa_calibration);

      /* Initialize data of the most critical object for this cycle */
      for (i_side = FBK_ZERO_UINT; i_side < FBK_NUMBER_OF_SIDES; i_side++)
      {
         Esa_Clear_Core_Output_One_Side(p_esa_core_output, i_side);
      }

      for (idx = FBK_ZERO_UINT; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
      {
         p_tracker_object = &(p_esa_core_input->p_pa_data->object_data[idx]);
         if (Esa_Is_Valid_Object(p_tracker_object, p_esa_calibration))
         {
            Esa_Process_Object(p_esa_core_output, p_esa_persistent, p_tracker_object, p_esa_core_input, p_vehicle_data,
                               p_esa_calibration);
         }
      }

      Esa_Process_Output(p_esa_core_output, p_esa_persistent, p_esa_calibration);

      /* Fill side persistent data */
      Esa_Fill_Side_Persistents(p_esa_persistent, p_esa_core_output);

      Binary_Esa_Debug_Pass_Persistent_Data(p_esa_persistent);
   }
}

static boolean_T Esa_Is_Valid_Object(const Fbk_Object_Data_T *p_object, const Esa_Core_Calibration_T *p_esa_calibration)
{
   boolean_T f_obj_is_valid = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_object);
   assert(NULL != p_esa_calibration);

   if ((PA_OBJ_STATUS_INVALID != p_object->status) && (p_object->age >= p_esa_calibration->k_esa_min_track_age)
       && (Fbk_Abs_F(p_object->vcs_pos.x) <= p_esa_calibration->k_esa_max_range))
   {
      f_obj_is_valid = FBK_TRUE;
   }

   return f_obj_is_valid;
}

static void Esa_Init_Core_Input(Esa_Core_Input_T *p_esa_core_input)
{
   /* Asserts */
   assert(NULL != p_esa_core_input);

   p_esa_core_input->lane_width         = FBK_ZERO_F;
   p_esa_core_input->lane_center_offset = FBK_ZERO_F;

   p_esa_core_input->f_esa_enabled = FBK_FALSE;

   /* Trailer information */
   p_esa_core_input->trailer.f_trailer_present = FBK_FALSE;
   p_esa_core_input->trailer.length            = FBK_ZERO_F;
   p_esa_core_input->trailer.width             = FBK_ZERO_F;
   p_esa_core_input->trailer.angle             = FBK_ZERO_F;
}

static boolean_T Esa_Is_Host_Speed_In_Activation_Range(const Esa_Core_Calibration_T *p_esa_calibration,
                                                       const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                       const Esa_Persistent_T *p_esa_persistent)
{
   float32_T min_host_activation_speed;
   float32_T max_host_activation_speed;
   boolean_T f_host_speed_in_activation_range = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_esa_calibration);
   assert(NULL != p_esa_persistent);

   /* If host speed was previously in activation range, then apply hysteresis */
   if (Fbk_Is_True(p_esa_persistent->f_host_speed_in_activation_range))
   {
      min_host_activation_speed =
         p_esa_calibration->k_esa_host_activation_speed_min - p_esa_calibration->k_esa_host_activation_speed_min_hys;
      max_host_activation_speed =
         p_esa_calibration->k_esa_host_activation_speed_max + p_esa_calibration->k_esa_host_activation_speed_max_hys;
   }
   else
   {
      min_host_activation_speed = p_esa_calibration->k_esa_host_activation_speed_min;
      max_host_activation_speed = p_esa_calibration->k_esa_host_activation_speed_max;
   }

   if ((p_vehicle_data->host_speed >= min_host_activation_speed) && (p_vehicle_data->host_speed <= max_host_activation_speed))
   {
      f_host_speed_in_activation_range = FBK_TRUE;
   }

   return f_host_speed_in_activation_range;
}


static void Esa_Update_Vehicle_States(const Fbk_Vehicle_Data_T *p_vehicle_data,
                                      const Esa_Core_Calibration_T *p_esa_calibration,
                                      Esa_Persistent_T *p_esa_persistent)
{
   /* Asserts */
   assert(NULL != p_esa_calibration);
   assert(NULL != p_esa_persistent);

   /* Process the host activation speed */
   p_esa_persistent->f_host_speed_in_activation_range =
      Esa_Is_Host_Speed_In_Activation_Range(p_esa_calibration, p_vehicle_data, p_esa_persistent);

   /* Process the curve radius */
   p_esa_persistent->f_esa_disabled_low_curve_radius =
      Esa_Is_Disabled_By_Small_Curve_Radius(p_esa_calibration, p_vehicle_data, p_esa_persistent);
}


static boolean_T Esa_Is_Disabled_By_Small_Curve_Radius(const Esa_Core_Calibration_T *p_esa_calibration,
                                                       const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                       const Esa_Persistent_T *p_esa_persistent)
{
   boolean_T f_esa_disabled;
   float32_T min_curve_radius = p_esa_calibration->k_esa_min_curve_radius;

   /* Asserts */
   assert(NULL != p_esa_calibration);
   assert(NULL != p_esa_persistent);

   /* If host was previously below the minimum curve radius, then apply hysteresis now */
   if (Fbk_Is_True(p_esa_persistent->f_esa_disabled_low_curve_radius))
   {
      min_curve_radius += p_esa_calibration->k_esa_min_curve_radius_hys;
   }

   /* Check if ESA should be disabled because of small curve radius */
   if (Fbk_Is_True(p_esa_calibration->k_esa_f_allow_min_curve_radius))
   {
      if (Fbk_Abs_F(p_vehicle_data->curvature) > THRESHOLD_IS_ZERO)
      {
         if (Fbk_Abs_F(1.0f / p_vehicle_data->curvature) < min_curve_radius)
         {
            f_esa_disabled = FBK_TRUE;
         }
         else
         {
            f_esa_disabled = FBK_FALSE;
         }
      }
      else
      {
         f_esa_disabled = FBK_FALSE;
      }
   }
   else
   {
      f_esa_disabled = FBK_FALSE;
   }

   return f_esa_disabled;
}


static Esa_Core_Status_T Esa_Get_Enable_Status(const Esa_Core_Calibration_T *p_esa_calibration, const Esa_Core_Input_T *p_esa_core_input)
{
   Esa_Core_Status_T esa_core_status;

   /* Asserts */
   assert(NULL != p_esa_calibration);
   assert(NULL != p_esa_core_input);


   /* If the use cal is enabled, then the calibration determines the enable state */
   if (Fbk_Is_True(p_esa_calibration->k_esa_f_enable_via_cal))
   {
      if (Fbk_Is_True(p_esa_calibration->k_esa_f_enable))
      {
         esa_core_status = ESA_CORE_STATUS_ENABLED;
      }
      else
      {
         esa_core_status = ESA_CORE_STATUS_DISABLED_BY_CAL;
      }
   }
   else
   {
      /* Use the enable flag from the core input if the use cal is false */
      if (p_esa_core_input->f_esa_enabled)
      {
         esa_core_status = ESA_CORE_STATUS_ENABLED;
      }
      else
      {
         esa_core_status = ESA_CORE_STATUS_DISABLED_BY_INPUT;
      }
   }

   return esa_core_status;
}


static Esa_Core_Status_T Esa_Get_Activation_Status(const Esa_Core_Calibration_T *p_esa_calibration,
                                                   const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                   const Esa_Persistent_T *p_esa_persistent)
{
   Esa_Core_Status_T esa_active_status;

   /* Assert */
   assert(NULL != p_esa_persistent);

   /* Check if ESA needs to be deactivated */
   if (Fbk_Is_False(p_esa_persistent->f_host_speed_in_activation_range))
   {
      /* Since host speed range check was already done beforehand, we only need to find out if host speed was too low or too high.
       * Therefore check thresholds without hysteresis. */
      if (p_vehicle_data->host_speed < p_esa_calibration->k_esa_host_activation_speed_min)
      {
         esa_active_status = ESA_CORE_STATUS_DEACTIVATED_LOW_EGO_SPEED;
      }
      else if (p_vehicle_data->host_speed > p_esa_calibration->k_esa_host_activation_speed_max)
      {
         esa_active_status = ESA_CORE_STATUS_DEACTIVATED_HIGH_EGO_SPEED;
      }
      else
      {
         /* This should not be possible and would indicate an error in the host speed range check. */
         esa_active_status = ESA_CORE_STATUS_DEACTIVATED_INTERNAL_ERROR;
      }
   }
   else if (Fbk_Is_True(p_esa_persistent->f_esa_disabled_low_curve_radius))
   {
      esa_active_status = ESA_CORE_STATUS_DEACTIVATED_LOW_CURVE_RADIUS;
   }
   else
   {
      esa_active_status = ESA_CORE_STATUS_ACTIVE;
   }

   return esa_active_status;
}


static void Esa_Reset_Core_Output(Esa_Core_Output_T *p_esa_core_output)
{
   uint8_t side;

   /* Assert */
   assert(NULL != p_esa_core_output);

   p_esa_core_output->esa_core_status = ESA_CORE_STATUS_ENABLED;

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      Esa_Clear_Core_Output_One_Side(p_esa_core_output, side);
   }
}


static void Esa_Process_Object(Esa_Core_Output_T *p_esa_core_output,
                               Esa_Persistent_T *p_esa_persistent,
                               const Fbk_Object_Data_T *p_tracker_object,
                               const Esa_Core_Input_T *p_esa_core_input,
                               const Fbk_Vehicle_Data_T *p_vehicle_data,
                               const Esa_Core_Calibration_T *p_esa_calibration)
{
   /* Asserts */
   assert(NULL != p_esa_core_output);
   assert(NULL != p_esa_persistent);
   assert(NULL != p_tracker_object);
   assert(NULL != p_esa_core_input);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_esa_calibration);

   if (Fbk_Is_True(Esa_Is_Object_Relevant(p_tracker_object, p_esa_calibration)))
   {
      Esa_Object_T curr_obj;

      /* Initialize ESA object information */
      Esa_Init_Object_Data(&curr_obj, p_tracker_object);

      /* Determine ego side of current object */
      curr_obj.ego_side = Esa_Get_Object_Side(&curr_obj, ESA_USE_CURVI);

      Esa_Create_Zone(&curr_obj, p_esa_persistent->mature_count_in_esa_zone[curr_obj.p_tracker_data->id - FBK_ONE_UINT],
                      p_esa_core_input, p_esa_calibration);

      curr_obj.f_obj_in_zone = Esa_Is_Object_In_Zone(curr_obj.p_tracker_data, &curr_obj.zone, ESA_USE_CURVI);

      if (Fbk_Is_True(curr_obj.f_obj_in_zone))
      {
         /* Increment the counter for object in the ESA zone */
         Esa_Increment_Mature_Count_In_Zone(&(p_esa_persistent->mature_count_in_esa_zone[curr_obj.p_tracker_data->id - FBK_ONE_UINT]),
                                            curr_obj.p_tracker_data->status);

         /* Get longitudinal TTC */
         curr_obj.long_ttc = Esa_Get_Longitudinal_TTC(curr_obj.p_tracker_data, p_vehicle_data->host_length);

         /* Get TTP */
         curr_obj.ttp = Esa_Get_TTP(curr_obj.p_tracker_data);

         if ((curr_obj.long_ttc > THRESHOLD_IS_ZERO) && (curr_obj.long_ttc < ESA_DEFAULT_LARGE_TTC))
         {
            /* Calculate the deceleration required to reach the host vehicle speed */
            curr_obj.obj_decel_to_reach_host_speed = (curr_obj.p_tracker_data->curvi_vel_rel.x / curr_obj.long_ttc);
         }

         curr_obj.obj_long_dist =
            Fbk_Max(FBK_ZERO_F, -p_vehicle_data->host_length
                                   - (curr_obj.p_tracker_data->curvi_pos.x + (0.5f * curr_obj.p_tracker_data->length)));

         /* Check if the longitudinal TTC is below the given threshold value */
         curr_obj.f_obj_ttc_below_threshold = Esa_Is_Ttc_Below_Threshold(&curr_obj, p_esa_calibration, p_esa_persistent);

         /* Check if the currently required deceleration is above the given threshold value */
         curr_obj.f_obj_decel_above_threshold = Esa_Is_Deceleration_Critical(&curr_obj, p_esa_calibration, p_esa_persistent);

         /* Check if the obj passes criteria to issue an alert */
         if (p_esa_persistent->mature_count_in_esa_zone[curr_obj.p_tracker_data->id - FBK_ONE_UINT]
             >= p_esa_calibration->k_esa_min_mature_cycles)
         {
            /* Check if current object is the most critical object for its side */
            Esa_Set_Most_Critical_Object(p_esa_core_output, &curr_obj, p_esa_calibration);
         }
      }
      else
      {
         /* Object is not in zone so clear the mature in zone count */
         p_esa_persistent->mature_count_in_esa_zone[curr_obj.p_tracker_data->id - FBK_ONE_UINT] = FBK_ZERO_UINT;
      }

      /* Pass object attributes to debug structure */
      Binary_Esa_Debug_Pass_Object_Attributes(&curr_obj);
   }
   else
   {
      /* Object is not valid so clear the count in zone */
      p_esa_persistent->mature_count_in_esa_zone[p_tracker_object->id - FBK_ONE_UINT] = FBK_ZERO_UINT;
   }
}


static void Esa_Init_Object_Data(Esa_Object_T *p_esa_object, const Fbk_Object_Data_T *p_tracker_object)
{
   /* Asserts */
   assert(NULL != p_esa_object);
   assert(NULL != p_tracker_object);

   p_esa_object->long_ttc                      = ESA_DEFAULT_LARGE_TTC;
   p_esa_object->ttp                           = FBK_ZERO_F;
   p_esa_object->obj_decel_to_reach_host_speed = FBK_ZERO_F;
   p_esa_object->obj_long_dist                 = -ESA_DEFAULT_OBJ_DIST;
   p_esa_object->ego_side                      = FBK_SIDE_UNDEFINED;
   p_esa_object->f_obj_in_zone                 = FBK_FALSE;
   p_esa_object->f_obj_ttc_below_threshold     = FBK_FALSE;
   p_esa_object->f_obj_decel_above_threshold   = FBK_FALSE;
   p_esa_object->p_tracker_data                = p_tracker_object;

   Fbk_Reset_Field_Of_Interest(&p_esa_object->zone);
}


static void Esa_Clear_Core_Output_One_Side(Esa_Core_Output_T *p_esa_core_output, const uint8_t side)
{
   /* Assert */
   assert(NULL != p_esa_core_output);

   p_esa_core_output->esa_alert[side]                     = FBK_FALSE;
   p_esa_core_output->esa_index[side]                     = PA_INVALID_OBJ_INDEX;
   p_esa_core_output->esa_id[side]                        = PA_INVALID_OBJ_ID;
   p_esa_core_output->esa_ttc[side]                       = ESA_DEFAULT_LARGE_TTC;
   p_esa_core_output->esa_ttp[side]                       = FBK_ZERO_F;
   p_esa_core_output->esa_decel_to_reach_host_speed[side] = FBK_ZERO_F;
   p_esa_core_output->esa_long_distance[side]             = -ESA_DEFAULT_OBJ_DIST;
}


static void Esa_Fill_Side_Persistents(Esa_Persistent_T *p_esa_persistent, const Esa_Core_Output_T *p_esa_core_output)
{
   uint8_t side;

   /* Asserts */
   assert(NULL != p_esa_persistent);
   assert(NULL != p_esa_core_output);

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      /* Save the ESA outputs for the next run */
      p_esa_persistent->prev_esa_alert_obj_index[side] = p_esa_core_output->esa_index[side];
      p_esa_persistent->prev_esa_alert_obj_id[side]    = p_esa_core_output->esa_id[side];
   }
}


static void Esa_Set_Most_Critical_Object(Esa_Core_Output_T *p_esa_core_output,
                                         const Esa_Object_T *p_esa_object,
                                         const Esa_Core_Calibration_T *p_esa_calibration)
{
   uint8_t side              = p_esa_object->ego_side;
   boolean_T object_relevant = FBK_TRUE; /* TRUE by default because of existence in ESA zone, can be reset by other conditions */

   /* Asserts */
   assert(NULL != p_esa_core_output);
   assert(NULL != p_esa_object);

   if (Fbk_Is_True(p_esa_calibration->k_esa_f_allow_obj_critical_ttc_and_deceleration)
       && (Fbk_Is_False(p_esa_object->f_obj_ttc_below_threshold) || Fbk_Is_False(p_esa_object->f_obj_decel_above_threshold)))
   {
      object_relevant = FBK_FALSE;
   }

   /* The objects deceleration to reach the hosts speed is checked to determine the criticality compared to other ESA alert
    * candidates. */
   if (Fbk_Is_True(object_relevant)
       && ((Fbk_Is_True(p_esa_calibration->k_esa_f_allow_obj_selection_ttc)
            && (p_esa_object->long_ttc < p_esa_core_output->esa_ttc[side]))
           || (Fbk_Is_True(p_esa_calibration->k_esa_f_allow_obj_selection_deceleration)
               && (p_esa_object->obj_decel_to_reach_host_speed > p_esa_core_output->esa_decel_to_reach_host_speed[side]))
           || (Fbk_Is_True(p_esa_calibration->k_esa_f_allow_obj_selection_long_distance)
               && (p_esa_object->obj_long_dist < p_esa_core_output->esa_long_distance[side]))))
   {
      p_esa_core_output->esa_alert[side]                     = FBK_TRUE;
      p_esa_core_output->esa_index[side]                     = p_esa_object->p_tracker_data->index;
      p_esa_core_output->esa_id[side]                        = p_esa_object->p_tracker_data->id;
      p_esa_core_output->esa_ttc[side]                       = p_esa_object->long_ttc;
      p_esa_core_output->esa_ttp[side]                       = p_esa_object->ttp;
      p_esa_core_output->esa_decel_to_reach_host_speed[side] = p_esa_object->obj_decel_to_reach_host_speed;
      p_esa_core_output->esa_long_distance[side]             = p_esa_object->obj_long_dist;
   }
}


static boolean_T Esa_Was_Most_Critical_Obj_Last_Cycle(const Esa_Object_T *p_esa_object, const Esa_Persistent_T *p_esa_persistent)
{
   uint8_t side;
   boolean_T result = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_esa_object);
   assert(NULL != p_esa_persistent);

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      if (p_esa_object->p_tracker_data->id == p_esa_persistent->prev_esa_alert_obj_id[side])
      {
         result = FBK_TRUE;
      }
   }

   return result;
}


static boolean_T Esa_Is_Ttc_Below_Threshold(const Esa_Object_T *p_esa_object,
                                            const Esa_Core_Calibration_T *p_esa_calibration,
                                            const Esa_Persistent_T *p_esa_persistent)
{
   boolean_T f_obj_ttc_below_threshold;
   float32_T long_ttc_threshold;

   /* Asserts */
   assert(NULL != p_esa_object);
   assert(NULL != p_esa_calibration);
   assert(NULL != p_esa_persistent);

   if (Esa_Was_Most_Critical_Obj_Last_Cycle(p_esa_object, p_esa_persistent))
   {
      long_ttc_threshold = p_esa_calibration->k_esa_critical_longitudinal_ttc_hys;
   }
   else
   {
      long_ttc_threshold = p_esa_calibration->k_esa_critical_longitudinal_ttc;
   }

   f_obj_ttc_below_threshold = (boolean_T) (Fbk_Is_True(p_esa_calibration->k_esa_f_allow_obj_critical_ttc_and_deceleration)
                                            && (p_esa_object->long_ttc > FBK_ZERO_F)
                                            && (p_esa_object->long_ttc <= long_ttc_threshold));

   return f_obj_ttc_below_threshold;
}


static boolean_T Esa_Is_Deceleration_Critical(const Esa_Object_T *p_esa_object,
                                              const Esa_Core_Calibration_T *p_esa_calibration,
                                              const Esa_Persistent_T *p_esa_persistent)
{
   boolean_T f_obj_decel_above_threshold;
   float32_T threshold_decel;

   /* Asserts */
   assert(NULL != p_esa_object);
   assert(NULL != p_esa_calibration);
   assert(NULL != p_esa_persistent);

   if (Esa_Was_Most_Critical_Obj_Last_Cycle(p_esa_object, p_esa_persistent))
   {
      threshold_decel = p_esa_calibration->k_esa_obj_safe_deceleration_threshold_hys;
   }
   else
   {
      threshold_decel = p_esa_calibration->k_esa_obj_safe_deceleration_threshold;
   }

   f_obj_decel_above_threshold = (boolean_T) (Fbk_Is_True(p_esa_calibration->k_esa_f_allow_obj_critical_ttc_and_deceleration)
                                              && (p_esa_object->obj_decel_to_reach_host_speed > FBK_ZERO_F)
                                              && (p_esa_object->obj_decel_to_reach_host_speed >= threshold_decel));

   return f_obj_decel_above_threshold;
}


static boolean_T Esa_Is_Object_Relevant(const Fbk_Object_Data_T *p_tracker_object, const Esa_Core_Calibration_T *p_esa_calibration)
{
   boolean_T f_is_relevant_obj = FBK_FALSE;
   /* Asserts */
   assert(NULL != p_tracker_object);
   assert(NULL != p_esa_calibration);

   if (((PA_OBJ_STATUS_MATURE == p_tracker_object->status) || (PA_OBJ_STATUS_COASTED == p_tracker_object->status))
       && (Fbk_Abs_F(p_tracker_object->curvi_heading) <= p_esa_calibration->k_esa_max_curvi_heading_abs)
       && (Fbk_Abs_F(p_tracker_object->curvi_vel.x) >= p_esa_calibration->k_esa_min_obj_curvi_long_vel_abs)
       && (p_tracker_object->existence_probability >= p_esa_calibration->k_esa_min_exist_prob))
   {
      f_is_relevant_obj = FBK_TRUE;
   }

   return f_is_relevant_obj;
}


static void Esa_Create_Object_Field_Of_Interest(Fbk_Field_Of_Interest_T *p_object_foi,
                                                const Fbk_Object_Data_T *p_tracker_object,
                                                const Coordinate_System_T coordinate_system)
{
   float32_T heading;

   /* Create FoI based on given coordinate system */
   switch (coordinate_system)
   {
      case ESA_USE_VCS:
         /* Create FoI for given object in VCS */
         heading = p_tracker_object->vcs_heading;
         break;

      case ESA_USE_CURVI:
      default:
         /* Create FoI for given object in curvi coordinates */
         heading = p_tracker_object->curvi_heading;
         break;
   }

   Fbk_Create_Field_Of_Interest_From_Object_Data(p_object_foi, p_tracker_object->curvi_pos, p_tracker_object->length,
                                                 p_tracker_object->width, heading);
}


static boolean_T Esa_Is_Object_In_Zone(const Fbk_Object_Data_T *p_tracker_object,
                                       const Fbk_Field_Of_Interest_T *p_zone,
                                       const Coordinate_System_T coordinate_system)
{
   boolean_T f_object_in_zone;
   Fbk_Field_Of_Interest_T object_foi;

   /* Asserts */
   assert(NULL != p_tracker_object);
   assert(NULL != p_zone);

   /* Create a FoI for the given object */
   Esa_Create_Object_Field_Of_Interest(&object_foi, p_tracker_object, coordinate_system);

   /* Check FoI overlap */
   f_object_in_zone = Fbk_Are_Fields_Of_Interest_Overlapping(&object_foi, p_zone);

   return f_object_in_zone;
}


static void Esa_Increment_Mature_Count_In_Zone(uint8_t *p_mature_counter, const Pa_Obj_Status_T status)
{
   /* Asserts */
   assert(NULL != p_mature_counter);

   /* Start incrementing the count in zone only when the track status is mature */
   if (PA_OBJ_STATUS_MATURE == status)
   {
      Sat_Inc_Uint8(p_mature_counter);
   }
}


static float32_T Esa_Get_Longitudinal_TTC(const Fbk_Object_Data_T *p_tracker_object, const float32_T ego_length)
{
   float32_T long_ttc = ESA_DEFAULT_LARGE_TTC;

   /* Only calculate long TTC for objects moving towards the ego vehicle */
   if (p_tracker_object->curvi_vel_rel.x > FBK_ZERO_F)
   {
      /* Calculate the object distance of the front bumper of object to the rear bumper of the ego vehicle */
      float32_T obj_distance = -p_tracker_object->curvi_pos.x - (0.5f * p_tracker_object->length) - ego_length;

      /* Inverted condition FBK_ZERO_F <= obj_distance because of tricky checking "EQ 0.0f" */
      if (Fbk_Is_False(FBK_ZERO_F > obj_distance))
      {
         /* Calculate the ttc */
         long_ttc = obj_distance / p_tracker_object->curvi_vel_rel.x;
      }
   }

   return long_ttc;
}


static float32_T Esa_Get_TTP(const Fbk_Object_Data_T *p_tracker_object)
{
   float32_T ttp = ESA_DEFAULT_LARGE_TTC;

   /* Only calculate TTP for objects moving towards the ego vehicle */
   if (p_tracker_object->curvi_vel_rel.x > FBK_ZERO_F)
   {
      /* Calculate the distance of the rear bumper of the object to the front bumper of the ego vehicle */
      float32_T distance_to_pass = Fbk_Max(FBK_ZERO_F, -(p_tracker_object->curvi_pos.x - (0.5f * p_tracker_object->length)));

      /* Calculate the TTP */
      ttp = distance_to_pass / p_tracker_object->curvi_vel_rel.x;
   }

   return ttp;
}


static void Esa_Process_Output(Esa_Core_Output_T *p_esa_core_output,
                               Esa_Persistent_T *p_esa_persistent,
                               const Esa_Core_Calibration_T *p_esa_calibration)
{
   uint8_t side_index;

   /* Asserts */
   assert(NULL != p_esa_calibration);
   assert(NULL != p_esa_core_output);
   assert(NULL != p_esa_persistent);

   for (side_index = FBK_ZERO_UINT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {
      if (Fbk_Is_True(p_esa_core_output->esa_alert[side_index]))
      {
         /* Reset ESA Alert holding counter */
         p_esa_persistent->esa_hold_counter[side_index] = FBK_ZERO_UINT;
      }
      /* If there is no alert for this cycle, then check if the alert from the previous cycle needs to be held */
      else if ((PA_INVALID_OBJ_ID != p_esa_persistent->prev_esa_alert_obj_id[side_index])
               && (p_esa_persistent->esa_hold_counter[side_index] < p_esa_calibration->k_esa_alert_holding_cycles))
      {
         Sat_Inc_Uint8(&(p_esa_persistent->esa_hold_counter[side_index]));

         Esa_Clear_Core_Output_One_Side(p_esa_core_output, side_index);
         p_esa_core_output->esa_alert[side_index] = FBK_TRUE;
         p_esa_core_output->esa_index[side_index] = p_esa_persistent->prev_esa_alert_obj_index[side_index];
         p_esa_core_output->esa_id[side_index]    = p_esa_persistent->prev_esa_alert_obj_id[side_index];
      }
      else
      {
         /* Reset ESA Alert holding counter */
         p_esa_persistent->esa_hold_counter[side_index] = FBK_ZERO_UINT;
      }
   }
}
