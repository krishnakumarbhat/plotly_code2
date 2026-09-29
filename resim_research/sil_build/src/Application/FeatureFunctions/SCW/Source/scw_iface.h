#ifndef SCW_IFACE_H
#define SCW_IFACE_H

/**
 * @file scw_iface.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief SCW configuration file with the interface for the state machine.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

/*===========================================================================*\
 * Includes
\*===========================================================================*/

#include "fbk_output.h"
#include "pa_reuse.h"
#include "scw_input_t.h"
#include "scw_instance_t.h"
#include "scw_output_t.h"
#include "scw_public_calibration_t.h"
#include "sfl_status.h"

/*===========================================================================*\
 * Global Functions	Definition
\*===========================================================================*/
#ifdef __cplusplus
extern "C"
{
#endif

   /**
    * @brief Initializes the SCW feature function.
    *
    * @return SFL status
    *
    * @SRS{}
    * @SAE{}
    * @SDD{CSCSA-186568}
    * @verification{}
    */
   Sfl_Status_T Scw_Init_Platform(Scw_Instance_T *p_scw_instance /**< Scw instance */);

   /**
    * @brief Runs the SCW feature function.
    *
    * @return SFL status
    *
    * @SRS{}
    * @SAE{}
    * @SDD{CSCSA-186566}
    * @verification{}
    */
   Sfl_Status_T Scw_Run_Platform(Scw_Instance_T *p_scw_instance /**< Scw instance */,
                                 const Scw_Input_T *p_scw_input /**< Scw input */,
                                 const Fbk_Output_T *p_fbk_output /**< Fbk output */,
                                 Scw_Output_T *p_scw_output /**< Scw output */);

   /**
    * @brief Change SCW calibration and reset SCW.
    *
    * @return true if success
    *
    * @SRS{}
    * @SAE{}
    * @SDD{CSCSA-186567}
    * @verification{Check whether the calibrations are updated correctly.}
    */
   boolean_T Scw_Update_Calibration_Platform(Scw_Instance_T *p_scw_instance /**< Scw instance */,
                                             const Scw_Public_Calibration_T *cal_src /**< Scw calibrations */);

   /**
    * @brief Get the SCW software major version.
    *
    * @return SCW major version
    *
    * @SRS{}
    * @SAE{}
    * @SDD{SF-8079}
    * @verification{}
    */
   uint8_t Scw_Get_Sw_Major_Version(void);

   /**
    * @brief Get the SCW software minor version.
    *
    * @return SCW minor version
    *
    * @SRS{}
    * @SAE{}
    * @SDD{SF-8080}
    * @verification{}
    */
   uint8_t Scw_Get_Sw_Minor_Version(void);

#ifdef __cplusplus
}
#endif

#endif /* SCW_IFACE_H */
