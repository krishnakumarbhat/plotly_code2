#ifndef CED_IFACE_H
#define CED_IFACE_H

/**
 * @file ced_iface.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function declarations for the CED interface.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
  * Includes
\*===========================================================================*/

#include "ced_input_t.h"
#include "ced_instance.h"
#include "ced_output_t.h"
#include "ced_public_calibration_t.h"
#include "fbk_output.h"
#include "pa_reuse.h"
#include "pt_output_t.h"
#include "sfl_status.h"

/*===========================================================================*\
 * Global Functions	Definition
\*===========================================================================*/
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

   /**
    * @brief Initializes and resets the CED feature.
    *
    * @return Sfl_Status_T
    *
    * @SRS{}
    * @SAE{}
    * @SDD{CSCSA-186572}
    * @verification{Create a test which checks whether CED is initialized correctly with its default parameters.}
    */
   Sfl_Status_T Ced_Init_Platform(Ced_Instance_T *p_ced_instance);


   /**
    * @brief Runs the main CED feature including pre- and post-run.
    *
    * @return Sfl_Status_T
    *
    * @SRS{}
    * @SAE{}
    * @SDD{CSCSA-186570}
    * @verification{Create a test to verify that Ced is giving an alert for a critical object. Also verify that Ced is not giving
    * an alert for non critical objects.}
    */
   Sfl_Status_T Ced_Run_Platform(Ced_Instance_T *p_ced_instance,
                                 const Ced_Input_T *p_ced_input,
                                 Ced_Output_T *p_ced_output,
                                 const Fbk_Output_T *p_fbk_output,
                                 const Pt_Output_T *p_pt_output);

   /**
    * @brief Change CED calibration and reset ced
    *
    * @return true if sucess
    *
    * @SRS{}
    * @SAE{}
    * @SDD{CSCSA-186571}
    * @verification{Check if the calibration values are updated and CED reset}
    */
   boolean_T Ced_Update_Calibration_Platform(Ced_Instance_T *p_ced_instance, const Ced_Public_Calibration_T *cal_src);

   /**
    * @brief Returns Ced_Sw_Major_Version.
    *
    * @return Ced_Sw_Major_Version
    *
    * @SRS{}
    * @SAE{}
    * @SDD{SF-3629}
    * @verification{Check that the correct software major version is returned.}
    */
   uint16_t Ced_Get_Sw_Major_Version(void);

   /**
    * @brief Returns Ced_Sw_Minor_Version.
    *
    * @return Ced_Sw_Minor_Version
    *
    * @SRS{}
    * @SAE{}
    * @SDD{SF-3630}
    * @verification{Check that the correct software minor version is returned.}
    */
   uint16_t Ced_Get_Sw_Minor_Version(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CED_IFACE_H */
