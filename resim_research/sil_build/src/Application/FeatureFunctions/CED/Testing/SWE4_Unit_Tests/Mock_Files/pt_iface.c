/**
 * @file pt_iface.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Source file for PT interface.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

#include "pt_iface.h"

static Pt_Output_T pt_output;

Pt_Output_T *Pt_Get_Output_Ptr(void)
{
   return &pt_output;
}
