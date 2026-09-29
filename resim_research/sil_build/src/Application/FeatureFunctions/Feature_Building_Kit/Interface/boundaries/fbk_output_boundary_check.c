/**
 * @file fbk_output_boundary_check.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains functions that check whether fbk outputs are in given boundaries.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_output_boundary_check.h"
#include "fbk_index_lookup.h"
#include "fbk_macros.h"
#include "pa_const_macros.h"
#include <assert.h>

/*===========================================================================*\
* Local function prototypes
\*===========================================================================*/

/**
 * @brief Checks whether output of ageing module is in boundaries
 *
 * @return True when outputs are within their boundaries
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-218677}
 * @verification{Create a superordinate test to check whether all outputs are in given boundaries}
 **/
static boolean_T Fbk_Is_Object_Ageing_Output_In_Boundaries(const Fbk_Age_Ctr_T *p_age_properties);

/*===========================================================================*\
* Global function definition
\*===========================================================================*/

boolean_T Fbk_Are_Outputs_In_Boundary(const Fbk_Index_Id_Lookup_Table_T *p_index_id_lookup_table, const Fbk_Age_Ctr_T *p_age_properties)
{
   boolean_T f_fbk_outputs_in_boundaries = (boolean_T) (Fbk_Is_True(Fbk_Is_Id_Index_Lut_In_Boundaries(p_index_id_lookup_table))
                                                        && Fbk_Is_Object_Ageing_Output_In_Boundaries(p_age_properties));

   return f_fbk_outputs_in_boundaries;
}

/*===========================================================================*\
* Local function definitions
\*===========================================================================*/

static boolean_T Fbk_Is_Object_Ageing_Output_In_Boundaries(const Fbk_Age_Ctr_T *p_age_properties)
{
   uint8_t index;
   boolean_T f_ageing_infos_in_boundaries = FBK_TRUE;

   assert(p_age_properties != NULL);

   for (index = FBK_ZERO_UINT; index < PA_OBJ_NUMBER_OF_OBJECTS; index++)
   {
      f_ageing_infos_in_boundaries =
         (boolean_T) (Fbk_Is_True(f_ageing_infos_in_boundaries) && (p_age_properties->stage_age[index] >= FBK_ONE_UINT));
   }
   return f_ageing_infos_in_boundaries;
}
