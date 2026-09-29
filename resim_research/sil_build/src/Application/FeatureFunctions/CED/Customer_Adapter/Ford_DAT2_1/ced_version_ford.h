#ifndef CED_VERSION_FORD_H
#define CED_VERSION_FORD_H

/**
 * @file ced_version_ford.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function declarations for the Ford_DAT2_1 versioning of CED.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*===========================================================================*\
  * Includes
\*===========================================================================*/

#include "pa_reuse.h"

/*===========================================================================*\
 * Global Functions	Definition
\*===========================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

   /**
    * @brief Returns Ford_Sw_Major_Version.
    *
    * @return Ford_Sw_Major_Version
    *
    * @SRS{}
    * @SAE{}
    * @SDD{SF-3507}
    * @verification{}
    */
   uint16_t Ced_Get_Ford_Sw_Major_Version(void);

   /**
    * @brief Returns Ford_Sw_Minor_Version.
    *
    * @return Ford_Sw_Minor_Version
    *
    * @SRS{}
    * @SAE{}
    * @SDD{SF-3508}
    * @verification{}
    */
   uint16_t Ced_Get_Ford_Sw_Minor_Version(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CED_VERSION_FORD_H */
