#ifndef PT_VERSION_FORD_H
#define PT_VERSION_FORD_H

/**
 * @file pt_version_ford.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function declarations for the Ford_DAT2_1 versioning of PT.
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
    * @SDD{SF-7648}
    * @verification{}
    */
   uint16_t Pt_Get_Ford_Sw_Major_Version(void);

   /**
    * @brief Returns Ford_Sw_Minor_Version.
    *
    * @return Ford_Sw_Minor_Version
    *
    * @SRS{}
    * @SAE{}
    * @SDD{SF-7647}
    * @verification{}
    */
   uint16_t Pt_Get_Ford_Sw_Minor_Version(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PT_VERSION_FORD_H */
