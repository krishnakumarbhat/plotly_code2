#ifndef PT_IFACE_H
#define PT_IFACE_H
/**
 * @file pt_iface.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file for PT interface.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pa_reuse.h"
#include "pt_output_t.h"

/*===========================================================================*\
* Global Function Prototypess
\*===========================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
   Pt_Nearest_Path_T *Pt_Get_Nearest_Path_Info(uint8_t index);
   Pt_Path_Object_Pair_Output_T *Pt_Get_Match_Information(uint8_t index);
   Pt_Output_T *Pt_Get_Output_Ptr(void);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
