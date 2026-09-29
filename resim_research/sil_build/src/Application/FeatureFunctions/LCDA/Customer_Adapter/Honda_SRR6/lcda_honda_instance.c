/**
 * @file lcda_honda_instance.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the Honda_SRR6 post run logic for LCDA.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "lcda_honda_instance.h"

Lcda_Honda_Instance_T *Lcda_Get_Lcda_Honda_Instance(void)
{
   static Lcda_Honda_Instance_T lcda_honda_instance;
   return &lcda_honda_instance;
}
