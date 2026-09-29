#ifndef CED_HONDA_INSTANCE_H
#define CED_HONDA_INSTANCE_H

/**
 * @file Ced_honda_instance.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module contains the CED honda custom adapter instance data types.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */
#include "reuse.h"
/**
 * @brief Ced_Honda_Instance_T structure
 *
 * @SDD{}
 * @verification{}
 */
/* coverity[misra_c_2012_rule_2_4_violation][Type has tag "Ced_Instance_T" but that tag is never used] */
typedef struct Ced_Honda_Instance_T
{
   uint8_t timer_for_ced_enable;
} Ced_Honda_Instance_T;

Ced_Honda_Instance_T *Ced_Get_Ced_Honda_Instance(void);
#endif /* CED_HONDA_INSTANCE_H */
