#ifndef OLP_CORE_TYPES_H
#define OLP_CORE_TYPES_H

#include "olp_iface.h"
#include "olp_calibration.h"

/**************************************************************************************************\
* Preprocessor Constants
\**************************************************************************************************/
#define OLP_CYCLE_TIME (0.05f)
#define OLP_MIN_CURVATURE (0.0001f)
#define OLP_CURV_TRHESHOLD (1.0e-3f)
#define NUMBER_OF_SNAIL_POINTS    (20)
#define TRACKER_THRESHOLD_IS_ZERO_WHEN_SQUARED (1E-5f)
#define OLP_TRACKER_THRESHOLD_IS_ZERO (TRACKER_THRESHOLD_IS_ZERO_WHEN_SQUARED * TRACKER_THRESHOLD_IS_ZERO_WHEN_SQUARED)

/**************************************************************************************************\
* typedefs
\**************************************************************************************************/

typedef struct Olp_Angle_Tag
{
   float angle; /**< Angle in [radiant](https://en.wikipedia.org/wiki/Radian)*/
   float sin;   /**< [sine](https://en.wikipedia.org/wiki/Sin) of angle*/   /* PRQA S 0781 */
   float cos;   /**< [cosine](https://en.wikipedia.org/wiki/Cos) of angle*/ /* PRQA S 0781 */
} Olp_Angle_T;

typedef struct Curvature_Info_Tag
{
  float host_curvature_slow;
  unsigned char f_host_curvature_calculated;
  unsigned char f_abs_curvature_LT_cal;
}Curvature_Info_T;

/**
* A snail trail point and its properties.
* \ingroup vp_snail_trail
*/
typedef struct Snail_Trail_State_Tag
{
   Olp_Vector_2d_T point;               /**< [m] coordinate of snail point in world coordinates*/
   float       distance_traveled;   /**< [m] distance traveled*/
   float       dist_between_points; /**< [m] distance (Pythagorean) to the next older snail point*/
   float       heading;             /**< [rad] heading angle of host at snail point in world coordinates*/
} Snail_Trail_State_T;

/**
* The collected snail trail.
* \ingroup vp_snail_trail
*/
typedef struct Snail_Trail_Tag
{
   Snail_Trail_State_T snail_trail_states[NUMBER_OF_SNAIL_POINTS]; /**< Points (and their properties) of the snail trail */
   float               snail_diff_dist;                            /**< [m] distance traveled since last snail point*/
   Olp_Vector_2d_T     snail_host_position;                        /**< [m] host position in world coordinates*/
   float               snail_host_dist;                            /**< [m] total host distance traveled*/
   float               snail_diff_heading;                         /**< [m] heading change since last snail point*/
   Olp_Angle_T         snail_host_heading;                         /**< host heading angle in world coordinates*/
   short int           snail_index;                                /**< index into next slot in snail trail buffer*/
   short int           oldest_snail_trail_index;                   /**< index into the slot in snail trail buffer where the oldest point is stored*/
   unsigned char       f_snail_full_buffer;                        /**< true if buffer is full*/
} Snail_Trail_T;

/**
* A match on the snail trail. \sa vp_snail_trail_properties
* \ingroup vp_snail_trail
*/
typedef struct Snail_Trail_Match_Tag
{
   float                best_match_interpolation_factor;        /**< the best matching snail point's data interpolation factor*/
   float                second_best_match_interpolation_factor; /**< the second best matching snail point's data interpolation factor*/
   float                best_match_dist2;                       /**< the best matching snail point's distance squared*/
   float                second_best_match_dist2;                /**< the second best matching snail point's distance squared*/
   const Snail_Trail_T *p_snail_trail;                          /**< pointer to the snail trail data used to derive this match */
   short int            best_match_index;                       /**< the best matching index into the snail trail, 0 if none was found*/
   short int            second_best_match_index;                /**< the second best matching index into the snail trail, 0 if none was found*/
   unsigned char        f_best_match_found : 1;          /**< flag indicating if a best matching snail point was found*/
   unsigned char        f_second_best_match_found : 1;          /**< flag indicating if a second best matching snail point was found*/
} Snail_Trail_Match_T;

/**
* Snail trail property heading.
* \ingroup vp_snail_trail
*/
typedef struct Snail_Trail_Heading_Rate_Tag
{
   float         heading_rate_value;  /**< [rad/s] found heading rate value*/
   unsigned char f_found;             /**< flag indicating if a heading rate was found*/
} Snail_Trail_Heading_Rate_T;

typedef struct Cog_Input_Tag
{
   float vcs_xposn;
   float vcs_yposn;
   float vcs_xvel;
   float vcs_yvel;
   float vcs_xaccel;
   float vcs_yaccel;
   float vcs_pointing;
   float len1;
   float len2;
   float wid1;
   float wid2;
   float tang_accel;
   float curvature;
   float speed;
} Cog_Input_T;

typedef struct Cog_Dynamics_Tag
{
   float cog_long_pos_vcs;
   float cog_lat_pos_vcs;
   float cog_long_vel_vcs;
   float cog_lat_vel_vcs;
   float cog_speed;
   float cog_heading;
   float cog_long_acc_vcs;
   float cog_lat_acc_vcs;
} Cog_Dynamics_T;

typedef struct Curvi_Data_Tag
{
   Olp_Obj_Curvi_Calc_Method_T curvi_coordinates_calc_method; /**< Curvi coordinate calculation method. Set to 2 by default */
   Olp_Vector_2d_T curvi_pos;                             /**< [m] position of the object in curvi coordinates.*/
   Olp_Vector_2d_T curvi_vel;                             /**< [m/s] velocity of the object in curvi coordinates.*/
   Olp_Vector_2d_T curvi_vel_rel;                         /**< [m/s] relative velocity of the object in curvi coordinates.*/
   float curvi_heading;                                   /**< [rad] heading of the object in curvi coordinates.*/
}Curvi_Data_T;

typedef struct Object_Vcs_Tag
{
   Olp_Angle_T heading;                               /**< [Rad] heading relative to longitudinal axis of VCS */
   Olp_Vector_2d_T center_position;               /**< [m] coordinates of the objects center position */
   Olp_Vector_2d_T velocity;                      /**< [m/s] velocity vector OTG of this object in VCS coordinates */
   Olp_Vector_2d_T relative_velocity;             /**< [m/s] velocity vector relative to ego velocity in VCS coordinates */
   float speed;                                   /**< [m/s] speed of the object */
   unsigned char f_updated;                       /**< flag indicating the data in this structure has been updated already in the current cycle */
   float curvature;
} Object_Vcs_T;

#endif
