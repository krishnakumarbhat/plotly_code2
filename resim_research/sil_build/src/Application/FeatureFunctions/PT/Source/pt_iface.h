#ifndef PT_IFACE_H
#define PT_IFACE_H
/**
 * @file pt_iface.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the declarations of the interface of path tracking algorithm which are exported to any project.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

/* Internal includes */
#include "fbk_output.h"
#include "pt_instance.h"
#include "pt_output_t.h"
#include "pt_public_calibration_t.h"
#include "sfl_status.h"

/*Fbk includes*/
#include "pa_reuse.h"

/*===========================================================================*\
* Global Function Prototypess
\*===========================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif

   /**
    * @brief This function initializes the path tracking structs
    *
    * @SRS{SF-1522,SF-1520,SF-1521}
    * @SAE{SF-2918}
    * @SDD{SF-7425}
    * @verification{}
    * @return     filled input pointers needed for path tracking algorithm
    *
    */
   Sfl_Status_T Pt_Init_Platform(Pt_Instance_T *p_pt_instance);

   /**
    * This high level function is the interface for controls when the procedures Pt_Init_Platform
    * and Pt_Run_Platform are called
    *
    * @SRS{SF-1522,SF-1520,SF-1521}
    * @SAE{SF-2918}
    * @SDD{SF-7424}
    * @verification{}
    * @return		output struct of path algorithm with information considering object to path matches
    */
   Sfl_Status_T Pt_Run_Platform(Pt_Instance_T *p_pt_instance, Pt_Output_T *p_pt_output, const Fbk_Output_T *p_fbk_output);


   /**
    * This function returns the nearest path information by a given index.
    *
    * @SRS{}
    * @SAE{}
    * @SDD{SF-7574}
    * @verification{}
    * @return		nearest path information for an input index
    */
   const Pt_Nearest_Path_T *Pt_Get_Nearest_Path_Info(const Pt_Output_T *p_pt_output,
                                                     uint8_t index /**< index of the output struct which shall be returned*/);

   /**
    * This function returns the match information for a given index.
    *
    * @SRS{}
    * @SAE{}
    * @SDD{SF-7575}
    * @verification{}
    * @return		match information for an input index
    */
   const Pt_Path_Object_Pair_Output_T *Pt_Get_Match_Information(
      const Pt_Output_T *p_pt_output, uint8_t index /**< index of the output struct which shall be returned*/);

   /**
    * @brief Get the PT software major version.
    *
    * @return PT major version
    *
    * @SRS{}
    * @SAE{}
    * @SDD{SF-7422}
    * @verification{Check that the correct software major version is returned.}
    */
   uint16_t Pt_Get_Sw_Major_Version(void);

   /**
    * @brief Get the PT software minor version.
    *
    * @return PT minor version
    *
    * @SRS{}
    * @SAE{}
    * @SDD{SF-7423}
    * @verification{Check that the correct software minor version is returned.}
    */
   uint16_t Pt_Get_Sw_Minor_Version(void);


   /**
    * @brief Change CTA calibration and reset cta.
    *
    * @return true if sucess
    *
    */
   boolean_T Pt_Update_Calibration_Platform(Pt_Instance_T *p_pt_instance, const Pt_Public_Calibration_T *cal_src);


#ifdef __cplusplus
}
#endif


#endif
