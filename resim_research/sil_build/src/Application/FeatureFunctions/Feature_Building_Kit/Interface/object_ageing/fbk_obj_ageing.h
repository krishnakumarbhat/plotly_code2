#ifndef FBK_OBJ_AGING_H
#define FBK_OBJ_AGING_H

/**
 * @file fbk_obj_ageing.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function prototypes that handle aging of object.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/
/*fbk includes*/
#include "fbk_iface_types.h"

/*pa includes*/
#include "pa_data.h"

/*===========================================================================*\
* Global Function Prototypes
\*===========================================================================*/
#ifdef __cplusplus
extern "C"
{
#endif

   /**
    * @brief Updates the aging of each object.
    *
    * @return void
    *
    * @SRS{SF-296}
    * @SAE{}
    * @SDD{SF-4152}
    * @verification{}
    */
   void Fbk_Update_Object_Ageing(Fbk_Age_Ctr_T *fbk_obj_ages, Pa_Data_T *p_pa_data);


   /**
    * @brief Resets the aging of each object to its default.
    *
    * @return void
    *
    * @SRS{SF-296}
    * @SAE{}
    * @SDD{SF-4150}
    * @verification{}
    */
   void Fbk_Reset_Object_Ageing(Fbk_Age_Ctr_T *fbk_obj_ages);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FBK_OBJ_AGING_H */
