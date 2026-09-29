/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef OBJECT_CLASS_T_H
#define OBJECT_CLASS_T_H

/** \ingroup Enumerations */

typedef enum
{
   OBJECT_CLASS_UNKNOWN    = (0), /**< 0*/
   OBJECT_CLASS_PEDESTRIAN = (1), /**< 1*/
   OBJECT_CLASS_2WHEEL     = (2), /**< 2*/
   OBJECT_CLASS_CAR        = (3), /**< 3*/
   OBJECT_CLASS_TRUCK      = (4)  /**< 4*/
} object_class_T;

#endif

