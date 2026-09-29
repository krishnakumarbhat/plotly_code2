#ifndef TA_IFACE_H
#define TA_IFACE_H

/**
 * @file ta_iface.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief TA configuration file with the interface for the state machine.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
 * Includes
\*===========================================================================*/

#include "fbk_output.h"
#include "pa_reuse.h"
#include "sfl_status.h"
#include "ta_input_t.h"
#include "ta_instance_t.h"
#include "ta_output_t.h"
#include "ta_public_calibration_t.h"

/*===========================================================================*\
 * Global Functions	Definition
\*===========================================================================*/
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

   /**
    * @brief Change TA calibration and reset TA.
    *
    * @return true if success
    *
    * @SRS{SF-2298}
    * @SAE{}
    * @SDD{CSCSA-122773}
    * @verification{Check whether the calibrations are updated correctly.}
    */
   boolean_T Ta_Update_Calibration_Platform(Ta_Instance_T *p_ta_instance, const Ta_Public_Calibration_T *cal_src);
   /**
    * @brief Initializes the TA feature function.
    *
    * @return void
    *
    * @SRS{SF-2361,SF-2300,SF-2298,SF-2297}
    * @SAE{}
    * @SDD{CSCSA-216770}
    * @verification{Check whether Ta is initialized correctly with its default parameters.}
    */
   Sfl_Status_T Ta_Init_Platform(Ta_Instance_T *p_ta_instance);

   /**
    * @brief Runs the TA feature function.
    *
    * @return void
    *
    * @SRS{SF-2352,SF-2278}
    * @SAE{}
    * @SDD{CSCSA-216769}
    * @verification{Check that the main function is not giving an alert level when no valid object data is available.}
    */
   Sfl_Status_T Ta_Run_Platform(Ta_Instance_T *p_ta_instance,
                                const Ta_Input_T *p_ta_input,
                                const Fbk_Output_T *p_fbk_output,
                                Ta_Output_T *p_ta_output);

   /**
    * @brief Get the TA software major version.
    *
    * @return TA major version
    *
    * @SRS{}
    * @SAE{}
    * @SDD{SF-8715}
    * @verification{Check that the correct software major version is returned.}
    */
   uint16_t Ta_Get_Sw_Major_Version(void);

   /**
    * @brief Get the TA software minor version.
    *
    * @return TA minor version
    *
    * @SRS{}
    * @SAE{}
    * @SDD{SF-8716}
    * @verification{Check that the correct software minor version is returned.}
    */
   uint16_t Ta_Get_Sw_Minor_Version(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TA_IFACE_H */
