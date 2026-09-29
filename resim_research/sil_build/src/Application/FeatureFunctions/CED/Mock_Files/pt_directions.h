#ifndef PT_DIRECTIONS_H
#define PT_DIRECTIONS_H
/**
 * @file pt_directions.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains type definitions for object moving directions.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */
/*===========================================================================*\
* Includes
\*===========================================================================*/

/*Fbk includes*/
#include "pa_reuse.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/*
 * @brief Describes the direction of the target in relation to the host vcs-coordinate system.
 * This is a subset of information when compared to the object moving direction.
 */
typedef enum
{
   PT_OBJECT_ORIENTATION_NONE,        /**< Object has no orientation yet. */
   PT_OBJECT_ORIENTATION_LATERAL,     /**< Object has a lateral orientation. */
   PT_OBJECT_ORIENTATION_LONGITUDINAL /**< Object has a longitudinal orientation. */
} Pt_Object_Orientation_T;


/**
 * @brief classification of objects moving direction
 */
typedef enum
{
   PT_OBJECT_MOV_DIR_NONE /**< object moving direction classified as none*/,
   PT_OBJECT_MOV_DIR_LAT_LEFT /**< object moving direction classified as lateral left*/,
   PT_OBJECT_MOV_DIR_LAT_RIGHT /**< object moving direction classified as lateral right*/,
   PT_OBJECT_MOV_DIR_LONG_FORWARD /**< object moving direction classified as longitudinal forward*/,
   PT_OBJECT_MOV_DIR_LONG_BACKWARD /**< object moving direction classified as longitudinal backward*/
} Pt_Object_Mov_Direction_T;


#endif
