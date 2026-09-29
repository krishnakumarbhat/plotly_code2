#ifndef LCDA_IFACE_H
#define LCDA_IFACE_H

/**
 * @file lcda_iface.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Exported functions of the Lane Change Decision Aid (LCDA) Function Interface
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
  * Includes
\*===========================================================================*/

#include "fbk_output.h"
#include "lcda_input_t.h"
#include "lcda_instance.h"
#include "lcda_output_t.h"
#include "lcda_public_calibration_t.h"
#include "pa_reuse.h"
#include "sfl_status.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

   /*===========================================================================*\
 * Global Functions	Definition
\*===========================================================================*/
   /**
    * @brief Initializes and resets the Lcda feature.
    *
    * @return Sfl_Status_T
    *
    * @SRS{}
    * @SAE{}
    * @SDD{CSCSA-186590}
    * @verification{Create a test which checks whether Lcda is initialized correctly with its default parameters.}
    */
   Sfl_Status_T Lcda_Init_Platform(Lcda_Instance_T *p_lcda_instance);

   /**
    * @brief Runs the main Lcda feature including pre- and post-run.
    *
    * @return Sfl_Status_T
    *
    * @SRS{}
    * @SAE{}
    * @SDD{CSCSA-186592}
    * @verification{Create a test to verify that Lcda is giving an alert for a critical object. Also verify that Lcda is not giving
    * an alert for non critical objects.}
    */
   Sfl_Status_T Lcda_Run_Platform(Lcda_Instance_T *p_lcda_instance,
                                  const Lcda_Input_T *p_lcda_input,
                                  const Fbk_Output_T *p_fbk_output,
                                  Lcda_Output_T *p_lcda_output);

   /**
    * @brief Change LCDA calibration and reset LCDA.
    *
    * @return true if sucess
    *
    * @SRS{}
    * @SAE{}
    * @SDD{CSCSA-186593}
    * @verification{}
    *
    */
   boolean_T Lcda_Update_Calibration_Platform(Lcda_Instance_T *p_lcda_instance, const Lcda_Public_Calibration_T *cal_src);


   /**
    * @brief Returns Lcda_Sw_Major_Version.
    *
    * @return Lcda_Sw_Major_Version
    *
    * @SRS{CSCSA-68380}
    * @SAE{CSCSA-68379}
    * @SDD{SF-6637}
    * @verification{Check that the correct software major version is returned.}
    */
   uint16_t Lcda_Get_Sw_Major_Version(void);

   /**
    * @brief Returns Lcda_Sw_Minor_Version.
    *
    * @return Lcda_Sw_Minor_Version
    *
    * @SRS{CSCSA-68380}
    * @SAE{CSCSA-68379}
    * @SDD{SF-6638}
    * @verification{Check that the correct software minor version is returned.}
    */
   uint16_t Lcda_Get_Sw_Minor_Version(void);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LCDA_IFACE_H */
