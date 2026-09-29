/**
 * @file ced_honda_instance.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the Honda_SRR6 post run logic for CED.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ced_honda_instance.h"

Ced_Honda_Instance_T *Ced_Get_Ced_Honda_Instance(void)
{
   static Ced_Honda_Instance_T ced_honda_instance;
   return &ced_honda_instance;
}
