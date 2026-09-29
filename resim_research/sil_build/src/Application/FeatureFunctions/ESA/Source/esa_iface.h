#ifndef ESA_IFACE_H
#define ESA_IFACE_H

/**
 * @file esa_iface.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function declarations for the ESA interface.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

/*===========================================================================*\
  * Includes
\*===========================================================================*/

#include "esa_input_t.h"
#include "esa_instance_t.h"
#include "esa_output_t.h"
#include "esa_public_calibration_t.h"
#include "fbk_output.h"
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
    * @brief Initializes and resets the ESA feature.
    *
    * @return SFL status
    *
    * @SRD{}
    * @SAD{}
    * @SDD{CSCSA-216521}
    * @verification{Create a test which checks whether ESA is initialized correctly with its default parameters.}
    */
   Sfl_Status_T Esa_Init_Platform(Esa_Instance_T *p_esa_instance /**< ESA instance */);

   /**
    * @brief Runs the main ESA feature including pre- and post-run.
    *
    * @return SFL status
    *
    * @SRD{}
    * @SAD{}
    * @SDD{CSCSA-216522}
    * @verification{Create a test to verify that Esa is giving an alert for a critical object. Also verify that Esa is not giving
    * an alert for non critical objects.}
    */
   Sfl_Status_T Esa_Run_Platform(Esa_Instance_T *p_esa_instance /**< ESA instance */,
                                 const Esa_Input_T *p_esa_input /**< ESA input */,
                                 const Fbk_Output_T *p_fbk_output /**< FBK output */,
                                 Esa_Output_T *p_esa_output /**< ESA output */);


   /**
    * @brief Returns the reference to the Esa_Core_Calibration_T struct.
    *
    * @return true if success.
    *
    * @SRD{}
    * @SAD{}
    * @SDD{CSCSA-216523}
    * @verification{Check whether the calibrations are updated correctly.}
    */
   boolean_T Esa_Update_Calibration_Platform(Esa_Instance_T *p_esa_instance /**< ESA instance */,
                                             const Esa_Public_Calibration_T *cal_src /**< ESA calibrations */);

   /**
    * @brief Returns Esa_Sw_Major_Version.
    *
    * @return Esa_Sw_Major_Version
    *
    * @SRD{}
    * @SAD{}
    * @SDD{CSCSA-66000}
    * @verification{Check that the correct software major version is returned.}
    */
   uint16_t Esa_Get_Sw_Major_Version(void);

   /**
    * @brief Returns Esa_Sw_Minor_Version.
    *
    * @return Esa_Sw_Minor_Version
    *
    * @SRD{}
    * @SAD{}
    * @SDD{CSCSA-66001}
    * @verification{Check that the correct software minor version is returned.}
    */
   uint16_t Esa_Get_Sw_Minor_Version(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ESA_IFACE_H */
