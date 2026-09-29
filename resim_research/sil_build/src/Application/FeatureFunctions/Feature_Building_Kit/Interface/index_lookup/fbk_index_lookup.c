/**
 * @file fbk_index_lookup.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains functions that handle index lookup for given tracker object id values.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_index_lookup.h"
#include "fbk_debug_interface.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "pa_const_macros.h"
#include "pa_shared_types.h"
#include <assert.h>

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/
boolean_T Fbk_Is_Id_Index_Lut_In_Boundaries(const Fbk_Index_Id_Lookup_Table_T *p_fbk_index_id_lookup_table)
{
   boolean_T f_all_indices_in_expected_boundaries = FBK_TRUE;
   uint8_t object_id;

   for (object_id = FBK_ONE_UINT; object_id <= PA_OBJ_NUMBER_OF_OBJECTS; object_id++)
   {
      /* Check that all object indices are either set to invalid or are in range */
      if ((PA_INVALID_OBJ_INDEX != p_fbk_index_id_lookup_table->lookup_table[object_id])
          && (p_fbk_index_id_lookup_table->lookup_table[object_id] >= PA_OBJ_NUMBER_OF_OBJECTS))
      {
         f_all_indices_in_expected_boundaries = FBK_FALSE;
         break;
      }
   }

   return f_all_indices_in_expected_boundaries;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
uint8_t Fbk_Get_Object_Index_From_Id(const Fbk_Index_Id_Lookup_Table_T *p_fbk_index_id_lookup_table, const uint8_t object_id)
{
   uint8_t obj_index = PA_INVALID_OBJ_INDEX;

   /* Check that object ID is in range before accessing look-up table */
   if ((PA_INVALID_OBJ_ID != object_id) && (object_id <= PA_OBJ_NUMBER_OF_OBJECTS))
   {
      obj_index = p_fbk_index_id_lookup_table->lookup_table[object_id];
   }

   /* Return object index for the given object id */
   return obj_index;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
void Fbk_Update_Index_Id_Lookup_Table(Fbk_Index_Id_Lookup_Table_T *p_fbk_index_id_lookup_table, const Pa_Data_T *p_pa_data)
{
   /* Iterator value */
   uint8_t object_index;

   /* Assert */
   assert(NULL != p_pa_data);

   /* Reset all look-up table entries */
   Fbk_Reset_Index_Id_Lookup_Table(p_fbk_index_id_lookup_table);

   /* Update look-up table for valid objects */
   for (object_index = FBK_ZERO_UINT; object_index < PA_OBJ_NUMBER_OF_OBJECTS; object_index++)
   {
      uint8_t object_id = p_pa_data->object_data[object_index].id;
      /* Check that object ID is in range before accessing look-up table */
      if ((PA_INVALID_OBJ_ID != object_id) && (object_id <= PA_OBJ_NUMBER_OF_OBJECTS))
      {
         p_fbk_index_id_lookup_table->lookup_table[object_id] = object_index;
      }
   }

   Binary_Fbk_Debug_Pass_Idx_Lut(p_fbk_index_id_lookup_table->lookup_table);
}

void Fbk_Reset_Index_Id_Lookup_Table(Fbk_Index_Id_Lookup_Table_T *p_fbk_index_id_lookup_table)
{
   uint8_t object_id;

   /* Reset all values in the look-up table */
   for (object_id = FBK_ZERO_UINT; object_id <= PA_OBJ_NUMBER_OF_OBJECTS; object_id++)
   {
      p_fbk_index_id_lookup_table->lookup_table[object_id] = PA_INVALID_OBJ_INDEX;
   }

   Binary_Fbk_Debug_Pass_Idx_Lut(p_fbk_index_id_lookup_table->lookup_table);
}
