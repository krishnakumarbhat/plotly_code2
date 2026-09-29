#ifndef FBK_INDEX_LOOKUP_H
#define FBK_INDEX_LOOKUP_H

/**
 * @file fbk_index_lookup.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function prototypes that handle index lookup for given tracker object id values.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_macros.h"
#include "pa_const_macros.h"
#include "pa_data.h"
#include "pa_reuse.h"

/*===========================================================================*\
* Global Function Prototypes
\*===========================================================================*/
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

   typedef struct
   {
      uint8_t lookup_table[PA_OBJ_NUMBER_OF_OBJECTS + FBK_ONE_UINT];
   } Fbk_Index_Id_Lookup_Table_T;

   /**
    * @brief Returns the object index for a given object id.
    *
    * @return object index
    *
    * @SRS{SF-295}
    * @SAE{}
    * @SDD{SF-4153}
    * @verification{}
    */
   uint8_t Fbk_Get_Object_Index_From_Id(const Fbk_Index_Id_Lookup_Table_T *p_fbk_index_id_lookup_table,
                                        const uint8_t object_id /**< object identification number */);

   /**
    * @brief Checks whether output of id index lookuptable is in boundaries
    *
    * @return True when outputs are within their boundaries
    *
    * @SRS{}
    * @SAE{}
    * @SDD{SF-4157}
    * @verification{}
    **/
   boolean_T Fbk_Is_Id_Index_Lut_In_Boundaries(const Fbk_Index_Id_Lookup_Table_T *p_fbk_index_id_lookup_table);

   /**
    * @brief Updates the object index and id pairs once every cycle.
    *
    * @return void
    *
    * @SRS{SF-295}
    * @SAE{}
    * @SDD{SF-4154}
    * @verification{}
    */
   void Fbk_Update_Index_Id_Lookup_Table(Fbk_Index_Id_Lookup_Table_T *p_fbk_index_id_lookup_table,
                                         const Pa_Data_T *p_pa_data /**< PA context */);


   /**
    * @brief Resets the lookup table to default values.
    *
    * @return void
    *
    * @SRS{SF-295}
    * @SAE{}
    * @SDD{SF-4155}
    * @verification{}
    */
   void Fbk_Reset_Index_Id_Lookup_Table(Fbk_Index_Id_Lookup_Table_T *p_fbk_index_id_lookup_table);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FBK_INDEX_LOOKUP_H */
