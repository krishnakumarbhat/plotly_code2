/**
 * @file lcda_debug_writer.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function declarations for LCDA bin writer functions.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "lcda_debug_interface.h"
#include "fbk_macros.h"
#include "lcda_core_calibration_t.h"
#include "lcda_create_bsw_zone.h"
#include "lcda_create_cvw_zone.h"
#include "lcda_debug_writer.h"
#include "lcda_input_t.h"
#include "lcda_output_t.h"
#include "lcda_persistent_t.h"
#include "lcda_process_elc.h"
#include "lcda_process_slc.h"
#include "lcda_types.h"
#include "pa_obj_in.h"
#include "pa_reuse.h"
#include "pa_vehicle_in.h"
#include <assert.h>
#include <string.h>

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#if defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER)

/*===========================================================================*\
 * File Scope variables
\*===========================================================================*/

static Lcda_Debug_Data_T lcda_debug_data;
static boolean_T lcda_debug_processed_module;

/*===========================================================================*\
* Debug interface
\*===========================================================================*/

Lcda_Debug_Data_T *Lcda_Get_Debug_Data(void)
{
   return (&lcda_debug_data);
}

/*===========================================================================*\
* Helper functions to fill and reset debug data
\*===========================================================================*/

void Lcda_Debug_Reset_Data(void)
{
   uint8_t i;

   memset(&lcda_debug_data, 0, sizeof(Lcda_Debug_Data_T));

   for (i = FBK_ZERO_UINT; i < PA_OBJ_NUMBER_OF_OBJECTS; i++)
   {
      lcda_debug_data.lcda_debug_output.cvw_object_data[i].ttc = LCDA_CVW_DEFAULT_NO_ALERT_TTC;
   }

   for (i = FBK_ZERO_UINT; i < PA_OBJ_NUMBER_OF_OBJECTS; i++)
   {
      lcda_debug_data.lcda_debug_output.slc_object_data[i].lat_ttc = LCDA_DEFAULT_LARGE_TTC;
      lcda_debug_data.lcda_debug_output.slc_object_data[i].lon_ttc = LCDA_DEFAULT_LARGE_TTC;
   }
}

void Lcda_Debug_Pass_General_Data(const Lcda_Core_Input_T *p_lcda_core_input,
                                  const Lcda_Core_Output_T *p_lcda_core_output,
                                  const Lcda_Persistent_T *p_lcda_persistent,
                                  const Lcda_Core_Calibration_T *p_lcda_cal)
{
   /* Assert that all passed pointers are valid. */
   assert(NULL != p_lcda_core_input);
   assert(NULL != p_lcda_core_output);
   assert(NULL != p_lcda_persistent);
   assert(NULL != p_lcda_cal);

   /* Copy data from internal interfaces to debug output interface. */
   lcda_debug_data.lcda_core_input  = *p_lcda_core_input;
   lcda_debug_data.lcda_core_output = *p_lcda_core_output;
   lcda_debug_data.lcda_persistent  = *p_lcda_persistent;
   memcpy(&lcda_debug_data.lcda_calibration, p_lcda_cal, sizeof(Lcda_Core_Calibration_T));
}

void Lcda_Debug_Pass_Sw_Version(const uint16_t lcda_sw_major_version, const uint16_t lcda_sw_minor_version)
{
   /* Store version from FF iface. */
   lcda_debug_data.lcda_version.lcda_sw_major_version = lcda_sw_major_version;
   lcda_debug_data.lcda_version.lcda_sw_minor_version = lcda_sw_minor_version;
}

void Lcda_Debug_Pass_Bsw_Persistent_Data(Lcda_Bsw_Persistent_T *p_Bsw_Persistent_data)
{
   lcda_debug_data.lcda_debug_output.bsw_persistent = *p_Bsw_Persistent_data;
}

void Lcda_Debug_Pass_Cvw_Persistent_Data(Lcda_Cvw_Persistent_T *p_cvw_persistent_data)
{
   lcda_debug_data.lcda_debug_output.cvw_persistent = *p_cvw_persistent_data;
}

void Lcda_Debug_Pass_Elc_Persistent_Data(Lcda_Elc_Persistent_T *p_elc_persistent_data)
{
   lcda_debug_data.lcda_debug_output.elc_persistent = *p_elc_persistent_data;
}

void Lcda_Debug_Pass_Slc_Persistent_Data(Lcda_Slc_Persistent_T *p_slc_persistent_data)
{
   lcda_debug_data.lcda_debug_output.slc_persistent = *p_slc_persistent_data;
}

void Lcda_Debug_Pass_Bsw_Object_Attributes(Bsw_Object_T *p_bsw_object)
{
   uint8_t index = p_bsw_object->p_tracker_data->index;

   /* Assert */
   assert(NULL != p_bsw_object);

   /* Store object attributes in debug structure */
   lcda_debug_data.lcda_debug_output.bsw_object_data[index] = *p_bsw_object;
}

void Lcda_Debug_Pass_Cvw_Object_Attributes(Cvw_Object_T *p_cvw_object)
{
   uint8_t index = p_cvw_object->p_tracker_data->index;

   /* Assert */
   assert(NULL != p_cvw_object);

   /* Store object attributes in debug structure */
   lcda_debug_data.lcda_debug_output.cvw_object_data[index] = *p_cvw_object;
}

void Lcda_Debug_Pass_Elc_Object_Attributes(Elc_Object_T *p_elc_object)
{
   uint8_t index = p_elc_object->p_tracker_data->index;

   /* Assert */
   assert(NULL != p_elc_object);

   /* Store object attributes in debug structure */
   lcda_debug_data.lcda_debug_output.elc_object_data[index] = *p_elc_object;
}

void Lcda_Debug_Pass_Slc_Object_Attributes(Slc_Object_T *p_slc_object)
{
   uint8_t index = p_slc_object->p_tracker_data->index;

   /* Assert */
   assert(NULL != p_slc_object);

   /* Store object attributes in debug structure */
   lcda_debug_data.lcda_debug_output.slc_object_data[index] = *p_slc_object;
}

void Lcda_Debug_Pass_Bsw_Default_Zone(const Lcda_Core_Input_T *p_core_input,
                                      const Fbk_Vehicle_Data_T *p_vehicle_data,
                                      const Lcda_Core_Calibration_T *p_cals)
{
   Fbk_Field_Of_Interest_T default_bsw_zone;
   Fbk_Field_Of_Interest_T default_bsw_zone_hys;
   Bsw_Object_T bsw_object          = {0};
   Fbk_Object_Data_T tracker_object = {0};

   bsw_object.ego_side       = FBK_SIDE_RIGHT;
   bsw_object.p_tracker_data = &tracker_object;
   tracker_object.index      = FBK_ZERO_UINT;

   /* Asserts */
   assert(NULL != p_core_input);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_cals);

   /* Create a default zone and set it for the first object */
   Lcda_Create_Bsw_Zone(&default_bsw_zone, &default_bsw_zone_hys, &bsw_object, p_core_input, p_cals);

   /* Store default zone in debug structure */
   lcda_debug_data.lcda_debug_output.bsw_default_zone = default_bsw_zone;
}

void Lcda_Debug_Pass_Cvw_Default_Zone(const Lcda_Core_Input_T *p_core_input,
                                      const Fbk_Vehicle_Data_T *p_vehicle_data,
                                      const Lcda_Core_Calibration_T *p_cals,
                                      Lcda_Cvw_Persistent_T *p_cvw_persistent)
{
   Fbk_Field_Of_Interest_T default_cvw_zone;
   Fbk_Field_Of_Interest_T default_cvw_zone_hys;
   Cvw_Object_T cvw_object                                      = {0};
   Fbk_Object_Data_T tracker_object                             = {0};
   boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES] = {FBK_FALSE, FBK_FALSE};

   cvw_object.ego_side       = FBK_SIDE_RIGHT;
   cvw_object.p_tracker_data = &tracker_object;
   tracker_object.index      = FBK_ZERO_UINT;

   /* Asserts */
   assert(NULL != p_core_input);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_cals);

   /* Create a default zone and set it for the first object */
   Lcda_Create_Cvw_Zone(&default_cvw_zone, &default_cvw_zone_hys, f_use_small_lc_intention_zone, &cvw_object, p_core_input,
                        p_vehicle_data, p_cals, p_cvw_persistent);

   /* Store default zone in debug structure */
   lcda_debug_data.lcda_debug_output.cvw_default_zone = default_cvw_zone;
}

void Lcda_Debug_Pass_Elc_Default_Zone(const Lcda_Core_Input_T *p_core_input, const Lcda_Core_Calibration_T *p_cals)
{
   Elc_Object_T elc_object          = {0};
   Fbk_Object_Data_T tracker_object = {0};

   elc_object.ego_side       = FBK_SIDE_RIGHT;
   elc_object.p_tracker_data = &tracker_object;
   tracker_object.width      = FBK_ZERO_UINT;

   /* Asserts */
   assert(NULL != p_core_input);
   assert(NULL != p_cals);

   /* Create a default zone and set it for the first object */
   Lcda_Create_Elc_Object_Zone(&elc_object, FBK_ZERO_UINT, p_core_input, p_cals);

   /* Store default zone in debug structure */
   lcda_debug_data.lcda_debug_output.elc_default_zone = elc_object.zone;
}

void Lcda_Debug_Pass_Slc_Default_Zone(const Lcda_Core_Input_T *p_core_input, const Lcda_Core_Calibration_T *p_cals)
{
   Slc_Object_T slc_object          = {0};
   Fbk_Object_Data_T tracker_object = {0};

   slc_object.ego_side       = FBK_SIDE_RIGHT;
   slc_object.p_tracker_data = &tracker_object;
   tracker_object.width      = FBK_ZERO_UINT;

   /* Asserts */
   assert(NULL != p_core_input);
   assert(NULL != p_cals);

   /* Create a default zone and set it for the first object */
   Lcda_Create_Slc_Object_Zone(&slc_object, FBK_ZERO_UINT, p_core_input, p_cals);

   /* Store default zone in debug structure */
   lcda_debug_data.lcda_debug_output.slc_default_zone = slc_object.zone;
}

void Lcda_Debug_Pass_Object_Ref_Point(const Vector_2d_T *p_ref_point, const uint8_t object_id)
{
   /* Asserts */
   assert(NULL != p_ref_point);

   if (LCDA_BSW == lcda_debug_processed_module)
   {
      lcda_debug_data.lcda_debug_output.obj_ref_point_bsw[object_id] = *p_ref_point;
   }
   else if (LCDA_CVW == lcda_debug_processed_module)
   {
      lcda_debug_data.lcda_debug_output.obj_ref_point_cvw[object_id] = *p_ref_point;
   }
   else
   {
      /* TBD: support for other LCDA submodules */
   }
}

void Lcda_Debug_Pass_Processed_Submodule(const Lcda_Processed_Module lcda_processed_module)
{
   /* Asserts */
   assert(LCDA_UNDEF != lcda_processed_module);

   lcda_debug_processed_module = lcda_processed_module;
}
#endif /* defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER) */
