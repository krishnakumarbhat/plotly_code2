#ifndef CTA_IFACE_H
#define CTA_IFACE_H

/**
 * @file cta_iface.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Exports interface function definitions of Cta.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "cta_input_t.h"
#include "cta_instance.h"
#include "cta_output_t.h"
#include "cta_public_calibration_t.h"
#include "fbk_output.h"
#include "pa_reuse.h"
#include "pt_output_t.h"
#include "sfl_status.h"

/*===========================================================================*\
* Global Function Prototypes
\*===========================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

   /**
    * @brief Initializes the CTA specific structs needed for the CTA feature function.
    *
    * @return void
    *
    * @SRS{}
    * @SAE{}
    * @SDD{CSCSA-186416}
    * @verification{Check that CTA initializes correctly.}
    */
   Sfl_Status_T Cta_Init_Platform(Cta_Instance_T *p_cta_instance);

   /**
    * @brief This function is the superordinate function which calls the CTA procedures, if all pointers are valid.
    *
    * @return void
    *
    * @SRS{}
    * @SAE{}
    * @SDD{CSCSA-186415}
    * @verification{Check that the main function is not giving an alert level when no valid object data is available.}
    */
   Sfl_Status_T Cta_Run_Platform(const Cta_Input_T *p_cta_input,
                                 const Fbk_Output_T *p_fbk_output,
                                 const Pt_Output_T *p_pt_output,
                                 Cta_Instance_T *p_cta_instance,
                                 Cta_Output_T *p_cta_output);


   /**
    * @brief Returns the CTA SW major version.
    *
    * @return Major CTA software version
    *
    * @SRS{CSCSA-27716}
    * @SAE{CSCSA-27718}
    * @SDD{SF-3842}
    * @verification{Check that the returned address equals to the address of the internal static stored one.}
    */
   uint16_t Cta_Get_Sw_Major_Version(void);

   /**
    * @brief Returns the CTA_SW_Minor_Version.
    *
    * @return Minor CTA software version
    *
    * @SRS{CSCSA-27716}
    * @SAE{CSCSA-27718}
    * @SDD{SF-3843}
    * @verification{Check that the returned address equals to the address of the internal static stored one.}
    */
   uint16_t Cta_Get_Sw_Minor_Version(void);

   /**
    * @brief Change CTA calibration and reset cta.
    *
    * @return true if sucess
    *
    * @SRS{}
    * @SAE{}
    * @SDD{CSCSA-186425}
    * @verification{check if Cta_Update_Calibration is updating calibration.}
    */
   boolean_T Cta_Update_Calibration_Platform(Cta_Instance_T *p_cta_instance, const Cta_Public_Calibration_T *p_cal_src);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CTA_IFACE_H */
