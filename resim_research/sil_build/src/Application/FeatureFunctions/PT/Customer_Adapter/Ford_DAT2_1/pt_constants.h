#ifndef PT_CONSTANTS_H
#define PT_CONSTANTS_H

/**
 * @file pt_constants.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Ford_Dat2_1 customer specific macros and functions
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pa_reuse.h"


/** Constants used by the path tracking algorithm.
 * Different customers demand different sizes of the arrays.
 * Since array sizes are stored as #define constants each supported customer project gets its own .h file to set these.
 */

/*===========================================================================*\
* Global Defines
\*===========================================================================*/

/** Maximum number of paths*/
#define PT_NUMBER_OF_PATHS (25u)

/** Maximum number of lateral reference points*/
#define PT_NUM_GRID_POINTS (25u)

/** Single Array Index Offset for lateral reference point*/
#define PT_SINGLE_GRID_POINT_OFFSET (1u)

/** Index Offset between two lateral points used for extrapolation of paths*/
#define PT_EXTRAPOL_INDEX_OFFSET (2u)

/** Default value for path points */
#define PT_PATH_POINTS_DEFAULT_VAL (0.0f)

/** Array Index for lateral reference point at zero*/
#define PT_LOWEST_GRID_POINT_INDEX (0u)

/** Array Index for lateral reference point at right edge relative to ego*/
#define PT_MID_GRID_POINT_INDEX (12u)

/** Array Index for lateral reference point at left edge relative to ego*/
#define PT_HIGHEST_GRID_POINT_INDEX (24u)

/** Invalid Array Index*/
#define PT_INVALID_GRID_POINT_INDEX (255u)

/*===========================================================================*\
* Global Function Prototypes
\*===========================================================================*/

/**
 *  Returns pointer to the grid point array.
 *
 * \return pointer to the grid point array
 *
 */
void Pt_Update_Grid_Array_Defaults(float32_T grid_pt_array[PT_NUM_GRID_POINTS]);

#endif /* PT_CONSTANTS_H */
