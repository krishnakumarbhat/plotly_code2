#ifndef LCDA_INPUT_T_H
#define LCDA_INPUT_T_H
/*===================================================================*\
* Copyright 2019, Aptiv, All Rights Reserved.
* Aptiv Confidential.
*--------------------------------------------------------------------
*
* Description: Implements input structure for Renault
* Applicable Standards (in order of precedence: highest first):
* Aptiv C Coding Standards
*
*
\*===================================================================*/
#include "pa_data.h"
#include "pa_reuse.h"

typedef struct
{
   bitfield32_t f_lcda_switch : 8; /* Flag indicating if the LCMA feature should be operational */
   bitfield32_t LDW_Enabled : 1;
   bitfield32_t LDW_Side : 1;
   bitfield32_t BV1_FahrSzen_Baustelle : 1;
   /* Unused bits = 21. Note update this line when new fields are added above */
   float32_T LDW_distance_to_line;
   float32_T LDW_time_to_line;
   float32_T BV1_LIN_01_AbstandY;
   float32_T BV1_LIN_02_AbstandY;
   float32_T BV1_LIN_03_AbstandY;
   float32_T BV1_LIN_04_AbstandY;
   float32_T BV1_LIN_01_ExistMass;
   float32_T BV1_LIN_02_ExistMass;
   float32_T BV1_LIN_03_ExistMass;
   float32_T BV1_LIN_04_ExistMass;
   uint8_t LDW_front_camera_Enabled_over_coding;
   uint8_t dyn_LW_front_camera_Enabled_over_coding;

} Lcda_Input_T;


/*===========================================================================*\
* Global Function Prototypess
\*===========================================================================*/

#endif /* LCDA_INPUT_T_H */
