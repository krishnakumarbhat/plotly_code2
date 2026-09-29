#ifndef RECW_IFACE_H
#define RECW_IFACE_H

/**
 * @file recw_iface.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is RECW interface header file.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_output.h"
#include "pa_reuse.h"
#include "recw_input_t.h"
#include "recw_instance.h"
#include "recw_output_t.h"
#include "recw_public_calibration_t.h"
#include "sfl_status.h"

#ifdef __cplusplus
extern "C"
{
#endif

   /*===========================================================================*\
       * Global Function Prototypess
   \*===========================================================================*/

   /**
    * @brief Main routine for Recw algorithm
    *
    * @return Sfl_Status_T
    *
    * @SRS{}
    * @SAE{}
    * @SDD{CSCSA-186514}
    * @verification{Verify that function can be called without any fatal failure.}
    */
   Sfl_Status_T Recw_Run_Platform(Recw_Instance_T *p_recw_instance,
                                  const Recw_Input_T *p_recw_input,
                                  const Fbk_Output_T *p_fbk_output,
                                  Recw_Output_T *p_recw_output);
   /**
    * @brief Initializes Recw
    *
    * @return Sfl_Status_T
    *
    * @SRS{}
    * @SAE{}
    * @SDD{CSCSA-186512}
    * @verification{Verify that tracker output and vehicle pointer are set in RECW input.}
    */
   Sfl_Status_T Recw_Init_Platform(Recw_Instance_T *p_recw_instance);

   /**
    * @brief Change Recw calibration and reset recw.
    *
    * @return true if sucess
    *
    * @SRS{}
    * @SAE{}
    * @SDD{CSCSA-186513}
    * @verification{Test that calibration updating workflow works properly in case of valid cal pointer.}
    */
   boolean_T Recw_Update_Calibration_Platform(Recw_Instance_T *p_recw_instance, const Recw_Public_Calibration_T *cal_src);

   /**
    * @brief Returns software major version
    *
    * @return uint16_t
    *
    * @SRS{}
    * @SAE{}
    * @SDD{SF-7908}
    * @verification{}
    */
   uint16_t Recw_Get_Sw_Major_Version(void);

   /**
    * @brief Returns software minor version
    *
    * @return uint16_t
    *
    * @SRS{}
    * @SAE{}
    * @SDD{SF-7909}
    * @verification{}
    */
   uint16_t Recw_Get_Sw_Minor_Version(void);

#ifdef __cplusplus
}
#endif


#endif /* RECW_IFACE_H */
