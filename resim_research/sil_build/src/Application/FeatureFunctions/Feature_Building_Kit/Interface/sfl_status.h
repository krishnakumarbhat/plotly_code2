#ifndef FBK_SFL_STATUS_H
#define FBK_SFL_STATUS_H

/**
 * @file sfl_status.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains types for fbk iface.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Typedefs
\*===========================================================================*/
typedef enum
{
   SFL_STATUS_OK             = (0), /**< No Error */
   SFL_STATUS_OBJ_DATA_ERROR = (1), /**< Object Data Error */
   SFL_STATUS_VEH_DATA_ERROR = (2)  /**< Vehicle Data Error */
} Sfl_Status_T;

#endif /*FBK_SFL_STATUS_H*/
