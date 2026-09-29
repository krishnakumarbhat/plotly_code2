#ifndef FBK_IFACE_H
#define FBK_IFACE_H

/**
 * @file fbk_iface.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Feature building kit configuration file with the interface for shared functions.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*===========================================================================*\
 * Includes
\*===========================================================================*/

#include "fbk_instance.h"
#include "fbk_output.h"
#include "pa_context.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "sfl_status.h"

/*===========================================================================*\
 * Global Functions	Definition
\*===========================================================================*/
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

   /**
    * @brief Initializes internals of the fbk interface.
    *
    * @return Sfl_Status_T
    *
    * @SRS{}
    * @SAE{}
    * @SDD{SF-4147}
    * @verification{}
    */
   Sfl_Status_T Fbk_Init_Platform(Fbk_Instance_T *p_fbk_instance, Pa_Data_T *p_pa_data);


   /**
    * @brief Main routine of feature building kit which needs to be called before any other srf feature.
    *
    * @return Sfl_Status_T
    *
    * @SRS{}
    * @SAE{}
    * @SDD{SF-4148}
    * @verification{}
    */
   Sfl_Status_T Fbk_Run_Platform(Fbk_Instance_T *p_fbk_instance, Fbk_Output_T *p_fbk_output, Pa_Context_T *p_context);


   /**
    * @brief Get the feature building kit software major version.
    *
    * @return Feature building kit major version
    *
    * @SRS{}
    * @SAE{}
    * @SDD{SF-4149}
    * @verification{}
    */
   uint16_t Fbk_Get_Sw_Major_Version(void);

   /**
    * @brief Get the feature building kit software minor version.
    *
    * @return Feature building kit minor version
    *
    * @SRS{}
    * @SAE{}
    * @SDD{SF-4146}
    * @verification{}
    */
   uint16_t Fbk_Get_Sw_Minor_Version(void);

   /**
    * @brief Checks the current FBK version against the given required minimum version.
    *
    * @return true when minimum version is sufficient
    *
    * @SRS{}
    * @SAE{}
    * @SDD{SF-4158}
    * @verification{}
    */
   boolean_T Fbk_Is_Version_Compatible(const uint16_t major_version_min /**< Minimum Major Version */,
                                       const uint16_t minor_version_min /**< Minimum Minor Version */);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FBK_IFACE_H */
