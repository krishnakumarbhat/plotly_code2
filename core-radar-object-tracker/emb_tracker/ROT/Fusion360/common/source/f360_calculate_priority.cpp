/*===================================================================================*\
 * FILE:  f360_calculate_priority.cpp
 *====================================================================================
 * Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose."
 *-----------------------------------------------------------------------------------------
 * DESCRIPTION:
 * This file contains function shared between object and cluster for calculating priority value
 *
 *
 * Applicable Standards (in order of precedence: highest first):
 * ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
 * ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
 ***/

#include "f360_calculate_priority.h"
#include "f360_calculate_curvi_position.h"
#include "f360_math.h"
#include "f360_math_func.h"
#include "f360_sort_priority.h"
#include <algorithm>

namespace f360_variant_A
{
// File-scope state for front polar zone priority, updated once per scan
static Priority_Zone_Params_T s_priority_zone_params;

// File-scope constants shared across priority functions
static const float32_t K_MAX_RANGE          = 600.0F;
static const float32_t K_MIN_VEL_THRESH     = 0.2F;
static const float32_t K_TTC_PRIORITY_FLOOR = 0.3F;
static const float32_t K_MAX_MOVABLE_BONUS  = 1.0F;

/*===========================================================================*\
* FUNCTION: Update_Priority_Zone_Params()
*===========================================================================
* RETURN VALUE:
* None
*
* PARAMETERS:
* const float32_t host_speed
* const float32_t cipv_long_pos
*
* EXTERNAL REFERENCES:
* F360_Linear_Equation_With_Saturation()
*
* DEVIATIONS FROM STANDARDS:
* None.
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* Precomputes parameters for priority zones from host speed and cipv position into the
* file-scope static s_priority_zone_params. Must be called once per scan
* before any calls to Calculate_Priority_Zone_Priority.
*
* PRECONDITIONS:
* None
*
* POSTCONDITIONS:
* s_priority_zone_params is populated.
*
\*===========================================================================*/
void Update_Priority_Zone_Params(const float32_t host_speed, const float32_t cipv_long_pos)
{
   const float32_t k_deg_to_rad = F360_PI / 180.0F;
   const float32_t az_rad       = F360_Linear_Equation_With_Saturation(host_speed, 0.0F, 30.0F, 60.0F, 20.0F) * k_deg_to_rad;
   const float32_t radius       = F360_Linear_Equation_With_Saturation(host_speed, 0.0F, 30.0F, 100.0F, 200.0F);
   const float32_t range_step   = radius * 0.3F;
   const Angle az_step(az_rad * 0.3F);

   const Angle polar_zone_az(az_rad);

   s_priority_zone_params.polar_zone_radius       = radius;
   s_priority_zone_params.polar_zone_range_step   = range_step;
   s_priority_zone_params.polar_zone_outer_radius = radius + range_step;
   s_priority_zone_params.polar_zone_az_sin       = polar_zone_az.Sin();
   s_priority_zone_params.polar_zone_az_cos       = polar_zone_az.Cos();
   s_priority_zone_params.polar_zone_az_threshold = radius * az_step.Sin();

   // Precompute per-scan constants for distance and corridor priority
   static const float32_t ext_factor_bp_host_speed[] = {0.0F, 10.0F, 30.0F};
   static const float32_t ext_factor_bp_factor[]     = {1.0F, 2.5F, 5.0F};
   s_priority_zone_params.ellipse_extend_long_factor =
      F360_Piecewise_Linear_Equation(host_speed, ext_factor_bp_host_speed, ext_factor_bp_factor);

   static const float32_t corridor_bp_host_speed[] = {0.0F, 10.0F, 30.0F};
   static const float32_t corridor_bp_range[]      = {100.0F, 200.0F, K_MAX_RANGE};
   const float32_t default_range = F360_Piecewise_Linear_Equation(host_speed, corridor_bp_host_speed, corridor_bp_range);
   s_priority_zone_params.corridor_default_range = (cipv_long_pos > 0.0F) ? cipv_long_pos : default_range;
}

/*===========================================================================*\
* FUNCTION: Calculate_Distance_Priority()
*===========================================================================
* RETURN VALUE:
* float32_t
*
* PARAMETERS:
* const float32_t longitudinal_pos
* const float32_t curvi_lat_pos
*
* EXTERNAL REFERENCES:
* F360_Piecewise_Linear_Equation(), F360_Sqrtf()
*
* DEVIATIONS FROM STANDARDS:
* None.
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* Computes a piecewise-linear distance priority in elliptical distance
* space. The lateral component is stretched by the precomputed speed-dependent
* factor so that at higher speeds the ellipse elongates longitudinally, giving
* more priority to objects ahead.
*
* PRECONDITIONS:
* Update_Priority_Zone_Params must have been called for the current scan.
*
* POSTCONDITIONS:
* Result is in [0, 1]
*
\*===========================================================================*/
float32_t Calculate_Distance_Priority(const float32_t longitudinal_pos, const float32_t curvi_lat_pos)
{
   // Speed-dependent lateral stretch for elliptical distance metric (precomputed)
   const float32_t scaled_lat       = curvi_lat_pos * s_priority_zone_params.ellipse_extend_long_factor;
   const float32_t ellipse_distance = F360_Sqrtf(longitudinal_pos * longitudinal_pos + scaled_lat * scaled_lat);

   // Distance priority: piecewise linear in distance space
   static const float32_t dist_bp_distance[] = {0.0F, 20.0F, 50.0F, 100.0F, 300.0F, K_MAX_RANGE};
   static const float32_t dist_bp_priority[] = {0.99F, 0.9F, 0.8F, 0.6F, 0.2F, 0.0F};
   const float32_t distance_priority = F360_Piecewise_Linear_Equation(ellipse_distance, dist_bp_distance, dist_bp_priority);
   return distance_priority;
}

/*===========================================================================*\
* FUNCTION: Calculate_Front_Polar_Zone_Priority()
*===========================================================================
* RETURN VALUE:
* float32_t
*
* PARAMETERS:
* const float32_t range
* const float32_t longitudinal_pos
* const float32_t curvi_lat_pos
*
* EXTERNAL REFERENCES:
* F360_Linear_Equation_With_Saturation()
*
* DEVIATIONS FROM STANDARDS:
* None.
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* Computes the front detection zone priority using precomputed
* speed-dependent parameters from s_priority_zone_params (populated by
* Update_Priority_Zone_Params).
*
* Range priority: 1.0 inside zone radius, linear decay to 0.0 over
* a transition band of 30% of the radius.
*
* Azimuth priority: cross-product test against the half-angle boundary,
* with a transition band of 30% of the azimuth. Folded via |y| for
* symmetry about the x-axis.
*
* PRECONDITIONS:
* Update_Priority_Zone_Params must have been called for the current scan.
*
* POSTCONDITIONS:
* Result is in [0, 1]
*
\*===========================================================================*/
float32_t Calculate_Front_Polar_Zone_Priority(const float32_t range, const float32_t longitudinal_pos,
                                              const float32_t curvi_lat_pos)
{
   // Range priority: 1.0 inside polar_zone_radius, linear decay to 0.0 at polar_zone_outer_radius
   const float32_t range_priority = F360_Linear_Equation_With_Saturation(
      range, s_priority_zone_params.polar_zone_radius, s_priority_zone_params.polar_zone_outer_radius, 1.0F, 0.0F);

   // Azimuth priority via cross-product
   const float32_t abs_y = std::abs(curvi_lat_pos);
   const float32_t s =
      (longitudinal_pos * s_priority_zone_params.polar_zone_az_sin) - (abs_y * s_priority_zone_params.polar_zone_az_cos);
   const float32_t azimuth_priority =
      F360_Linear_Equation_With_Saturation(s, -s_priority_zone_params.polar_zone_az_threshold, 0.0F, 0.0F, 1.0F);

   const float32_t front_polar_priority = range_priority * azimuth_priority;
   return front_polar_priority;
}

/*===========================================================================*\
* FUNCTION: Calculate_Corridor_Priority()
*===========================================================================
* RETURN VALUE:
* float32_t
*
* PARAMETERS:
* const float32_t longitudinal_pos
* const float32_t curvi_lat_pos
*
* EXTERNAL REFERENCES:
* F360_Linear_Equation_With_Saturation(),
* Indicator_Function_Smoothed()
*
* DEVIATIONS FROM STANDARDS:
* None.
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* Computes the in-path corridor priority using a narrow
* rectangular band along the host's projected driving path. The corridor
* has a lateral half-width of 1 m (core) plus 5 m linear decay.
* Longitudinally, it extends from behind the host (10 m decay) to either
* the precomputed corridor range (10 m decay beyond).
*
* PRECONDITIONS:
* Update_Priority_Zone_Params must have been called for the current scan.
*
* POSTCONDITIONS:
* Result is in [0, 1]
*
\*===========================================================================*/
float32_t Calculate_Corridor_Priority(const float32_t longitudinal_pos, const float32_t curvi_lat_pos)
{
   const float32_t corridor_long_step = 10.0F;
   const float32_t corridor_lat_step  = 5.0F;

   // Longitudinal indicator: linear decay behind host and beyond range
   const float32_t corridor_x_left = F360_Linear_Equation_With_Saturation(longitudinal_pos, -corridor_long_step, 0.0F, 0.0F, 1.0F);
   const float32_t corridor_x_right =
      F360_Linear_Equation_With_Saturation(longitudinal_pos, s_priority_zone_params.corridor_default_range,
                                           s_priority_zone_params.corridor_default_range + corridor_long_step, 1.0F, 0.0F);
   const float32_t corridor_x_priority = std::min(corridor_x_left, corridor_x_right);
   const float32_t corridor_y_priority = Indicator_Function_Smoothed(curvi_lat_pos, -1.0F, 1.0F, corridor_lat_step);
   const float32_t corridor_priority   = corridor_x_priority * corridor_y_priority;
   return corridor_priority;
}

/*===========================================================================*\
* FUNCTION: Calculate_Position_Priority()
*===========================================================================
* RETURN VALUE:
* float32_t
*
* PARAMETERS:
* const float32_t longitudinal_pos
* const float32_t curvi_lat_pos
* const float32_t range
*
* EXTERNAL REFERENCES:
* Calculate_Distance_Priority(), Calculate_Front_Polar_Zone_Priority(),
* Calculate_Corridor_Priority()
*
* DEVIATIONS FROM STANDARDS:
* None.
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* Computes a continuous geometric position priority score that maps a VCS
* position to [0, 1] reflecting spatial relevance to the host vehicle.
*
* The score is computed in two layers:
*
* Layer 1 -- Base (distance + front polar zone blend):
* - Distance priority: piecewise-linear decay in elliptical distance
*   space, with precomputed speed-dependent lateral stretching (weight 0.7)
* - Front polar zone: precomputed speed-dependent forward cone (weight 0.3)
*
* Layer 2 -- Corridor boost via probabilistic sum:
* - Corridor (in-path corridor): narrow rectangular corridor along the
*   host's projected driving path using curvilinear lateral position.
*   Applied as: priority = base + k_boost * corridor * (1 - base)
*
* PRECONDITIONS:
* Update_Priority_Zone_Params must have been called for the current scan.
*
* POSTCONDITIONS:
* Result is in [0, 1]
*
\*===========================================================================*/
float32_t Calculate_Position_Priority(const float32_t longitudinal_pos, const float32_t curvi_lat_pos, const float32_t range)
{
   const float32_t distance_priority    = Calculate_Distance_Priority(longitudinal_pos, curvi_lat_pos);
   const float32_t front_polar_priority = Calculate_Front_Polar_Zone_Priority(range, longitudinal_pos, curvi_lat_pos);
   const float32_t corridor_priority    = Calculate_Corridor_Priority(longitudinal_pos, curvi_lat_pos);

   // Base: distance (70%) + front polar zone (30%) blend
   const float32_t k_front_polar_weight = 0.3F;
   const float32_t base_priority =
      (distance_priority * (1.0F - k_front_polar_weight)) + (front_polar_priority * k_front_polar_weight);

   // Corridor boost via probabilistic sum: fills remaining headroom toward 1.0
   const float32_t k_corridor_boost = 0.7F;
   const float32_t priority         = base_priority + (k_corridor_boost * corridor_priority * (1.0F - base_priority));

   return priority;
}

/*===========================================================================*\
 * FUNCTION: Compute_Detection_Position_Priority()
 *===========================================================================
 * RETURN VALUE:
 * float32_t
 *
 * PARAMETERS:
 * const F360_Detection_T& detection
 * const F360_Detection_Props_T& det_prop
 * const float32_t host_speed
 * const float32_t cipv_long_pos
 * const float32_t host_curvature
 *
 * EXTERNAL REFERENCES:
 * Calculate_Curvi_Lat_Pos(),
 * Calculate_Position_Priority()
 *
 * DEVIATIONS FROM STANDARDS:
 * None.
 *
 * --------------------------------------------------------------------------
 * ABSTRACT:
 * --------------------------------------------------------------------------
 * Computes position-based priority for a single detection from its VCS
 * position. Populates the priority cache locally (since this function is
 * called during clustering, before Object_Track_Initialization populates it),
 * then delegates to Calculate_Position_Priority.
 *
 * PRECONDITIONS:
 * det_prop.vcs_position must be set.
 *
 * POSTCONDITIONS:
 * s_priority_zone_params is updated with parameters for the given host_speed
 * and cipv_long_pos.
 *
 \*===========================================================================*/
float32_t Compute_Detection_Position_Priority(const F360_Detection_T &detection, const F360_Detection_Props_T &det_prop,
                                              const float32_t host_speed, const float32_t cipv_long_pos,
                                              const float32_t host_curvature)
{
   // Populate cache locally (clustering occurs before Object_Track_Initialization
   // has populated it for this scan)
   Update_Priority_Zone_Params(host_speed, cipv_long_pos);

   const float32_t det_curvi_lat_pos = Calculate_Curvi_Lat_Pos(host_curvature, det_prop.vcs_position.x, det_prop.vcs_position.y);

   const float32_t priority = Calculate_Position_Priority(det_prop.vcs_position.x, det_curvi_lat_pos, detection.range);

   return priority;
}

/*===========================================================================*\
 * FUNCTION: Calculate_And_Apply_Movable_Bonus()
 *===========================================================================
 * RETURN VALUE:
 * None
 *
 * PARAMETERS:
 * const int32_t rank
 * const uint32_t max_num_reduced_tracks
 * float32_t& priority
 *
 * EXTERNAL REFERENCES:
 * None.
 *
 * DEVIATIONS FROM STANDARDS:
 * None.
 *
 * --------------------------------------------------------------------------
 * ABSTRACT:
 * --------------------------------------------------------------------------
 * Computes a rank-based decayed movable bonus and adds it to the provided
 * priority. Objects ranked 1 through N_full (50% of max_num_reduced_tracks)
 * receive the maximum bonus (1.0). Objects ranked N_full+1 through N_zero
 * (75% of max_num_reduced_tracks) receive a linearly decaying bonus. Objects
 * ranked above N_zero receive no bonus.
 *
 * PRECONDITIONS:
 * rank >= 1
 *
 * POSTCONDITIONS:
 * priority is increased by [0, 1]
 *
\*===========================================================================*/
void Calculate_And_Apply_Movable_Bonus(const int32_t rank, const uint32_t max_num_reduced_tracks, float32_t &priority)
{
   const int32_t n_full  = static_cast<int32_t>(max_num_reduced_tracks) / 2;
   const int32_t n_zero  = (static_cast<int32_t>(max_num_reduced_tracks) * 3) / 4;
   const int32_t n_decay = n_zero - n_full;

   float32_t bonus;
   if (rank <= n_full)
   {
      bonus = K_MAX_MOVABLE_BONUS;
   }
   else if ((n_decay > 0) && (rank <= n_zero))
   {
      const int32_t n_remaining = n_zero - rank;
      bonus                     = K_MAX_MOVABLE_BONUS * static_cast<float32_t>(n_remaining) / static_cast<float32_t>(n_decay);
   }
   else
   {
      bonus = 0.0F;
   }
   priority += bonus;
}

/*===========================================================================*\
 * FUNCTION: Map_Curvature_For_Priority()
 *===========================================================================
 * RETURN VALUE:
 * float32_t
 *
 * PARAMETERS:
 * const float32_t host_curvature
 *
 * EXTERNAL REFERENCES:
 * F360_Linear_Equation_With_Saturation()
 *
 * DEVIATIONS FROM STANDARDS:
 * None.
 *
 * --------------------------------------------------------------------------
 * ABSTRACT:
 * --------------------------------------------------------------------------
 * Maps curvature through a dead-zone with linear ramp for use in
 * priority calculations:
 *   |curvature| < 0.0025        -> 0
 *   |curvature| in [0.0025,0.005) -> linear ramp from 0 to +/-0.005
 *   |curvature| >= 0.005        -> curvature unchanged
 *
 * PRECONDITIONS:
 * None
 *
 * POSTCONDITIONS:
 * None
 *
\*===========================================================================*/
float32_t Map_Curvature_For_Priority(const float32_t host_curvature)
{
   const float32_t abs_curvature = std::abs(host_curvature);
   float32_t mapped_curvature;

   const float32_t upper_bound = 0.005F;
   if (abs_curvature >= upper_bound)
   {
      mapped_curvature = host_curvature;
   }
   else
   {
      // Dead-zone below lower_bound (saturates to 0) + linear ramp from 0 to upper_bound in [lower_bound, upper_bound]
      const float32_t lower_bound = 0.0025F;
      const float32_t ramp = F360_Linear_Equation_With_Saturation(abs_curvature, lower_bound, upper_bound, 0.0F, upper_bound);
      const float32_t sign = (host_curvature >= 0.0F) ? 1.0F : -1.0F;
      mapped_curvature     = sign * ramp;
   }

   return mapped_curvature;
}

/*===========================================================================*\
 * FUNCTION: Calculate_Priority_From_Pos_Vel()
 *===========================================================================
 * RETURN VALUE:
 * float32_t
 *
 * PARAMETERS:
 * const F360_Priority_Pos_Vel_T& pos_vel
 * const float32_t host_speed
 * const Angle& host_sideslip
 * const float32_t host_curvature
 *
 * EXTERNAL REFERENCES:
 * Calculate_Curvi_Lat_Pos(), Calculate_Position_Priority(),
 * Calculate_Object_TTC_Priority(), F360_Get_Hypotenuse(),
 * Probabilistic_Sum()
 *
 * DEVIATIONS FROM STANDARDS:
 * None.
 *
 * --------------------------------------------------------------------------
 * ABSTRACT:
 * --------------------------------------------------------------------------
 * Lightweight implementation of the base priority computation that takes
 * a F360_Priority_Pos_Vel_T instead of a full F360_Object_Track_T.
 * Computes the probabilistic sum of Position and TTC factors:
 *    P_base = 1 - (1 - position_score) * (1 - ttc_score)
 *
 * PRECONDITIONS:
 * None
 *
 * POSTCONDITIONS:
 * Result is in [0, 1]
 *
\*===========================================================================*/
float32_t Calculate_Priority_From_Pos_Vel(const F360_Priority_Pos_Vel_T &pos_vel, const float32_t host_speed,
                                          const Angle &host_sideslip, const float32_t host_curvature)
{
   const float32_t range             = F360_Get_Hypotenuse(pos_vel.vcs_pos_x, pos_vel.vcs_pos_y);
   const float32_t curvi_lat_pos     = Calculate_Curvi_Lat_Pos(host_curvature, pos_vel.vcs_pos_x, pos_vel.vcs_pos_y);
   const float32_t position_priority = Calculate_Position_Priority(pos_vel.vcs_pos_x, curvi_lat_pos, range);

   const float32_t ttc_priority = Calculate_Object_TTC_Priority(pos_vel, host_speed, host_sideslip, range, curvi_lat_pos);

   const float32_t priority = Probabilistic_Sum(position_priority, ttc_priority);
   return priority;
}

/*===========================================================================*\
 * FUNCTION: Calculate_Priority_For_Object()
 *===========================================================================
 * RETURN VALUE:
 * float32_t
 *
 * PARAMETERS:
 * const F360_Object_Track_T& obj_trk
 * const float32_t host_speed
 * const Angle& host_sideslip
 * const float32_t host_curvature
 *
 * EXTERNAL REFERENCES:
 * Calculate_Priority_From_Pos_Vel()
 *
 * DEVIATIONS FROM STANDARDS:
 * None.
 *
 * --------------------------------------------------------------------------
 * ABSTRACT:
 * --------------------------------------------------------------------------
 * Thin wrapper over Calculate_Priority_From_Pos_Vel that unpacks fields
 * from an object track. Computes base priority using the probabilistic
 * sum of Position and TTC factors.
 *
 * Movable bonus is NOT included here; it is applied externally via
 * Apply_Movable_Bonus_For_All_Objects() based on rank among movable objects.
 *
 * PRECONDITIONS:
 * None
 *
 * POSTCONDITIONS:
 * Result is in [0, 1]
 *
\*===========================================================================*/
float32_t Calculate_Priority_For_Object(const F360_Object_Track_T &obj_trk, const float32_t host_speed, const Angle &host_sideslip,
                                        const float32_t host_curvature)
{
   const F360_Priority_Pos_Vel_T pos_vel = {obj_trk.vcs_position.x, obj_trk.vcs_position.y, obj_trk.vcs_velocity.longitudinal,
                                            obj_trk.vcs_velocity.lateral, obj_trk.movable_prob};
   const float32_t priority              = Calculate_Priority_From_Pos_Vel(pos_vel, host_speed, host_sideslip, host_curvature);
   return priority;
}

/*===========================================================================*\
* FUNCTION: Stationary_Lateral_Attenuation()
*===========================================================================
* RETURN VALUE:
* float32_t
*
* PARAMETERS:
* const float32_t curvi_lat_pos
*
* EXTERNAL REFERENCES:
* F360_Linear_Equation_With_Saturation()
*
* DEVIATIONS FROM STANDARDS:
* None.
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* Computes a lateral attenuation factor for stationary objects/clusters
* to suppress off-path targets (e.g. guardrails).
*   |curvi_lat| < 1 m  -> 1.0 (full score)
*   |curvi_lat| > 5 m  -> 0.5 (capped at 50%)
*
* PRECONDITIONS:
* None
*
* POSTCONDITIONS:
* Result is in [0.5, 1.0]
*
\*===========================================================================*/
float32_t Stationary_Lateral_Attenuation(const float32_t curvi_lat_pos)
{
   const float32_t lat_scale = F360_Linear_Equation_With_Saturation(std::abs(curvi_lat_pos), 1.0F, 3.0F, 1.0F, 0.5F);
   return lat_scale;
}

/*===========================================================================*\
* FUNCTION: Map_TTC_To_Priority()
*===========================================================================
* RETURN VALUE:
* float32_t
*
* PARAMETERS:
* const float32_t ttc
*
* EXTERNAL REFERENCES:
* F360_Piecewise_Linear_Equation(), F360_Linear_Equation_With_Saturation()
*
* DEVIATIONS FROM STANDARDS:
* None.
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* Saturates TTC to [-20, 20] s and maps it to a priority score.
* For non-negative TTC (upcoming collision): piecewise linear over
* breakpoints ttc = [0, 2, 5, 20] s -> priority = [0.99, 0.9, 0.75, 0.3],
* extrapolating linearly beyond 20 s.
* For negative TTC (collision already passed): maps [-20, 0] to [0.3, 0.0].
*
* PRECONDITIONS:
* None
*
* POSTCONDITIONS:
* Result is in [0, 1]
*
\*===========================================================================*/
float32_t Map_TTC_To_Priority(const float32_t ttc)
{
   const float32_t k_ttc_sat_min = -20.0F;
   const float32_t k_ttc_sat_max = 20.0F;
   const float32_t ttc_sat       = std::max(k_ttc_sat_min, std::min(k_ttc_sat_max, ttc));

   float32_t priority;
   if (ttc_sat < 0.0F)
   {
      priority = F360_Linear_Equation_With_Saturation(ttc_sat, k_ttc_sat_min, 0.0F, K_TTC_PRIORITY_FLOOR, 0.0F);
   }
   else
   {
      static const float32_t bp_ttc[]      = {0.0F, 2.0F, 5.0F, 20.0F};
      static const float32_t bp_priority[] = {0.99F, 0.9F, 0.75F, K_TTC_PRIORITY_FLOOR};
      priority                             = F360_Piecewise_Linear_Equation(ttc_sat, bp_ttc, bp_priority);
   }

   return priority;
}

/*===========================================================================*\
* FUNCTION: Compute_CPA_Attenuation()
*===========================================================================
* RETURN VALUE:
* float32_t
*
* PARAMETERS:
* const float32_t vcs_pos_x
* const float32_t vcs_pos_y
* const float32_t rel_vel_x
* const float32_t rel_vel_y
* const float32_t range_sq
*
* EXTERNAL REFERENCES:
* F360_Sqrtf(), F360_Linear_Equation_With_Saturation()
*
* DEVIATIONS FROM STANDARDS:
* None.
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* Computes the CPA (Closest Point of Approach) attenuation scale factor
* for TTC priority. CPA represents the minimum distance the target will
* reach relative to the host assuming constant 2D velocities.
*
* Returns 1.0 (no attenuation) if relative speed is below 0.5 m/s
* (unreliable CPA). Otherwise:
*   d_cpa in [0, 5] m   -> 1.0  (in-path or adjacent lane)
*   d_cpa in [5, 15] m  -> linear decay to 0.3
*   d_cpa > 15 m        -> 0.3  (passing-by object)
*
* PRECONDITIONS:
* range = sqrt(vcs_pos_x^2 + vcs_pos_y^2)
*
* POSTCONDITIONS:
* Result is in [0.3, 1.0]
*
\*===========================================================================*/
float32_t Compute_CPA_Attenuation(const float32_t vcs_pos_x, const float32_t vcs_pos_y, const float32_t rel_vel_x,
                                  const float32_t rel_vel_y, const float32_t range)
{
   const float32_t range_sq     = range * range;
   const float32_t vel_sq       = (rel_vel_x * rel_vel_x) + (rel_vel_y * rel_vel_y);
   const float32_t k_min_vel_sq = 0.25F;
   float32_t cpa_scale          = 1.0F;
   if (vel_sq > k_min_vel_sq)
   {
      const float32_t dot      = (vcs_pos_x * rel_vel_x) + (vcs_pos_y * rel_vel_y);
      const float32_t d_cpa_sq = std::max(0.0F, range_sq - ((dot * dot) / vel_sq));
      const float32_t d_cpa    = F360_Sqrtf(d_cpa_sq);
      cpa_scale                = F360_Linear_Equation_With_Saturation(d_cpa, 5.0F, 15.0F, 1.0F, K_TTC_PRIORITY_FLOOR);
   }
   return cpa_scale;
}

/*===========================================================================*\
* FUNCTION: Compute_TTC_2D()
*===========================================================================
* RETURN VALUE:
* float32_t
*
* PARAMETERS:
* const float32_t vcs_pos_x
* const float32_t vcs_pos_y
* const float32_t rel_vel_x
* const float32_t rel_vel_y
* const float32_t range
*
* EXTERNAL REFERENCES:
* None.
*
* DEVIATIONS FROM STANDARDS:
* None.
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* Computes time to collision from 2D position and relative velocity:
*    ttc = -(range^2) / dot(rel_pos, rel_vel)
*
* When the projected relative velocity (dot product) is near zero
* (|dot| < 0.2 * range), the velocity is clamped to 0.2 m/s to avoid
* division by a near-zero value.
* Returns 0 if range is zero.
*
* PRECONDITIONS:
* range_sq = vcs_pos_x^2 + vcs_pos_y^2
*
* POSTCONDITIONS:
* None
*
\*===========================================================================*/
float32_t Compute_TTC_2D(const float32_t vcs_pos_x, const float32_t vcs_pos_y, const float32_t rel_vel_x,
                         const float32_t rel_vel_y, const float32_t range)
{
   float32_t ttc;
   if (range > 0.0F)
   {
      const float32_t dot = (vcs_pos_x * rel_vel_x) + (vcs_pos_y * rel_vel_y);
      if (std::abs(dot) < (K_MIN_VEL_THRESH * range))
      {
         ttc = (dot >= 0.0F) ? (-range / K_MIN_VEL_THRESH) : (range / K_MIN_VEL_THRESH);
      }
      else
      {
         ttc = -(range * range) / dot;
      }
   }
   else
   {
      ttc = 0.0F;
   }
   return ttc;
}

/*===========================================================================*\
* FUNCTION: Compute_TTC_1D()
*===========================================================================
* RETURN VALUE:
* float32_t
*
* PARAMETERS:
* const float32_t range
* const float32_t rdot_relative
*
* EXTERNAL REFERENCES:
* None.
*
* DEVIATIONS FROM STANDARDS:
* None.
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* Computes time to collision from 1D radial range and relative radial
* velocity:
*    ttc = -range / rdot_relative
*
* When |rdot_relative| is below 0.2 m/s, the velocity is clamped
* (preserving sign) to avoid division by a near-zero value.
* Returns 0 if range is zero.
*
* PRECONDITIONS:
* None
*
* POSTCONDITIONS:
* None
*
\*===========================================================================*/
float32_t Compute_TTC_1D(const float32_t range, const float32_t rdot_relative)
{
   float32_t ttc;
   if (range > 0.0F)
   {
      if (std::abs(rdot_relative) < K_MIN_VEL_THRESH)
      {
         ttc = (rdot_relative >= 0.0F) ? (-range / K_MIN_VEL_THRESH) : (range / K_MIN_VEL_THRESH);
      }
      else
      {
         ttc = -range / rdot_relative;
      }
   }
   else
   {
      ttc = 0.0F;
   }
   return ttc;
}

/*===========================================================================*\
* FUNCTION: Calculate_Object_TTC_Priority()
*===========================================================================
* RETURN VALUE:
* float32_t
*
* PARAMETERS:
* const F360_Priority_Pos_Vel_T& pos_vel
* const float32_t host_speed
* const Angle& host_sideslip
* const float32_t range
* const float32_t curvi_lat_pos
*
* EXTERNAL REFERENCES:
* Compute_TTC_2D(), Map_TTC_To_Priority(),
* Compute_CPA_Attenuation(), Stationary_Lateral_Attenuation()
*
* DEVIATIONS FROM STANDARDS:
* None.
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* Computes a priority score from time to collision (ttc) in [0, 1].
* Relative velocity is computed from the provided velocity and
* host speed/sideslip. TTC is computed via Compute_TTC_2D from position
* and relative velocity and mapped to a base priority via
* Map_TTC_To_Priority.
*
* For positive TTC, the priority is further attenuated by CPA distance
* via Compute_CPA_Attenuation (see that function for details).
*
* For stationary targets (movable_prob <= MOVABLE_PROB_THRESHOLD), TTC is additionally
* attenuated by a lateral-dependent scale factor via
* Stationary_Lateral_Attenuation.
*
* PRECONDITIONS:
* None
*
* POSTCONDITIONS:
* Result is in [0, 1]
*
\*===========================================================================*/
float32_t Calculate_Object_TTC_Priority(const F360_Priority_Pos_Vel_T &pos_vel, const float32_t host_speed,
                                        const Angle &host_sideslip, const float32_t range, const float32_t curvi_lat_pos)
{
   const float32_t rel_vel_x = pos_vel.vcs_vel_long - (host_sideslip.Cos() * host_speed);
   const float32_t rel_vel_y = pos_vel.vcs_vel_lat - (host_sideslip.Sin() * host_speed);
   const float32_t ttc       = Compute_TTC_2D(pos_vel.vcs_pos_x, pos_vel.vcs_pos_y, rel_vel_x, rel_vel_y, range);

   float32_t priority = Map_TTC_To_Priority(ttc);

   if (ttc >= 0.0F)
   {
      const float32_t cpa_scale = Compute_CPA_Attenuation(pos_vel.vcs_pos_x, pos_vel.vcs_pos_y, rel_vel_x, rel_vel_y, range);
      priority *= cpa_scale;
   }

   if (pos_vel.movable_prob <= MOVABLE_PROB_THRESHOLD)
   {
      priority *= Stationary_Lateral_Attenuation(curvi_lat_pos);
   }

   return priority;
}

/*===========================================================================*\
* FUNCTION: Calculate_Cluster_TTC_Priority()
*===========================================================================
* RETURN VALUE:
* float32_t
*
* PARAMETERS:
* const F360_Cluster_T& cluster
* const float32_t host_speed
* const Angle& host_sideslip
* const float32_t range
* const float32_t curvi_lat_pos
* const bool f_moving
*
* EXTERNAL REFERENCES:
* Compute_TTC_1D(), Map_TTC_To_Priority(), Stationary_Lateral_Attenuation()
*
* DEVIATIONS FROM STANDARDS:
* None.
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* Computes a TTC-based priority score for a cluster in [0, 1].
*
* If the cluster is not dealiased, returns a fixed fallback of 0.3.
* Otherwise, the 1D closing speed is computed as rep_rdotcomp minus the
* host radial velocity component, and TTC is computed via Compute_TTC_1D
* and mapped via Map_TTC_To_Priority.
*
* CPA attenuation is intentionally not applied because clusters only have
* 1D radial velocity -- a target with zero radial closing could still be
* approaching on a collision course laterally.
*
* For stationary clusters (!f_moving), the result is attenuated by
* Stationary_Lateral_Attenuation.
*
* PRECONDITIONS:
* None
*
* POSTCONDITIONS:
* Result is in [0, 1]
*
\*===========================================================================*/
float32_t Calculate_Cluster_TTC_Priority(const F360_Cluster_T &cluster, const float32_t host_speed, const Angle &host_sideslip,
                                         const float32_t range, const float32_t curvi_lat_pos, const bool f_moving)
{
   float32_t ttc_priority = 0.0F;
   if (!cluster.f_dealiased)
   {
      ttc_priority = K_TTC_PRIORITY_FLOOR;
   }
   else
   {
      // Closing speed: target world-frame radial velocity minus host radial velocity component
      const float32_t v_host_radial =
         host_speed * ((cluster.cos_vcs_az * host_sideslip.Cos()) + (cluster.sin_vcs_az * host_sideslip.Sin()));
      const float32_t rdot_relative = cluster.rep_rdotcomp - v_host_radial;
      const float32_t ttc           = Compute_TTC_1D(range, rdot_relative);

      ttc_priority = Map_TTC_To_Priority(ttc);
   }

   if (!f_moving)
   {
      ttc_priority *= Stationary_Lateral_Attenuation(curvi_lat_pos);
   }

   return ttc_priority;
}

/*===========================================================================*\
 * FUNCTION: Apply_Conservative_Movable_Bonus()
 *===========================================================================
 * RETURN VALUE:
 * float32_t
 *
 * PARAMETERS:
 * const float32_t base_priority
 * const bool f_moving
 * const F360_Tracker_Info_T& tracker_info
 *
 * EXTERNAL REFERENCES:
 * Calculate_And_Apply_Movable_Bonus()
 *
 * DEVIATIONS FROM STANDARDS:
 * None.
 *
 * --------------------------------------------------------------------------
 * ABSTRACT:
 * --------------------------------------------------------------------------
 * Adds a conservative rank-based movable bonus to a base priority for
 * entities (clusters, init objects, motion-flip objects) whose final
 * rank is not yet known. The conservative rank is num_movable_objs + 1
 * (i.e. worst-case rank among current movers).
 *
 * PRECONDITIONS:
 * tracker_info.num_movable_objs must be set.
 *
 * POSTCONDITIONS:
 * None
 *
\*===========================================================================*/
float32_t Apply_Conservative_Movable_Bonus(const float32_t base_priority, const bool f_moving,
                                           const F360_Tracker_Info_T &tracker_info)
{
   float32_t priority = base_priority;
   if (f_moving)
   {
      const int32_t conservative_rank = tracker_info.num_movable_objs + 1;
      Calculate_And_Apply_Movable_Bonus(conservative_rank, tracker_info.variant.num_reduced_tracks, priority);
   }
   return priority;
}

/*===========================================================================*\
 * FUNCTION: Is_Cluster_Moving()
 *===========================================================================
 * RETURN VALUE:
 * bool
 *
 * PARAMETERS:
 * const F360_Cluster_T& cluster
 *
 * EXTERNAL REFERENCES:
 * None.
 *
 * DEVIATIONS FROM STANDARDS:
 * None.
 *
 * --------------------------------------------------------------------------
 * ABSTRACT:
 * --------------------------------------------------------------------------
 * Classifies a cluster as moving if it has at least one moving-type
 * detection, or if it is dealiased with a representative compensated
 * range-rate magnitude exceeding 0.8 m/s.
 *
 * PRECONDITIONS:
 * None
 *
 * POSTCONDITIONS:
 * None
 *
\*===========================================================================*/
bool Is_Cluster_Moving(const F360_Cluster_T &cluster)
{
   const bool f_moving = (cluster.num_types_of_dets[0] > 0) || (cluster.f_dealiased && (std::abs(cluster.rep_rdotcomp) > 0.8F));
   return f_moving;
}

/*===========================================================================*\
 * FUNCTION: Calculate_Priority_For_Cluster()
 *===========================================================================
 * RETURN VALUE:
 * float32_t
 *
 * PARAMETERS:
 * const F360_Cluster_T& cluster
 * const float32_t host_speed
 * const float32_t host_curvature
 * const Angle& host_sideslip
 * const F360_Tracker_Info_T& tracker_info
 *
 * EXTERNAL REFERENCES:
 * Calculate_Curvi_Lat_Pos(), Calculate_Position_Priority(),
 * Calculate_Cluster_TTC_Priority(), F360_Get_Hypotenuse(),
 * Apply_Conservative_Movable_Bonus(), Is_Cluster_Moving(),
 * Probabilistic_Sum()
 *
 * DEVIATIONS FROM STANDARDS:
 * None.
 *
 * --------------------------------------------------------------------------
 * ABSTRACT:
 * --------------------------------------------------------------------------
 * Computes base priority using the probabilistic sum of Position and TTC
 * factors:
 *    P_base = 1 - (1 - position_score) * (1 - ttc_score)
 *
 * Position score is computed from the cluster VCS position via
 * Calculate_Position_Priority. TTC score is computed independently:
 * if the cluster is not dealiased, ttc_score is fixed at 0.3. Otherwise,
 * the 1D closing speed is computed as rep_rdotcomp minus the host radial
 * velocity component, and TTC is computed via Compute_TTC_1D and mapped
 * via Map_TTC_To_Priority.
 *
 * CPA attenuation is intentionally not applied here because clusters only
 * have 1D radial velocity - a target that appears to have zero radial
 * closing could still be approaching on a collision course laterally.
 *
 * PRECONDITIONS:
 * None
 *
 * POSTCONDITIONS:
 * Result is in [0, 1]
 *
\*===========================================================================*/
float32_t Calculate_Priority_For_Cluster(const F360_Cluster_T &cluster, const float32_t host_speed, const float32_t host_curvature,
                                         const Angle &host_sideslip, const F360_Tracker_Info_T &tracker_info)
{
   const float32_t range             = F360_Get_Hypotenuse(cluster.vcs_position_x, cluster.vcs_position_y);
   const float32_t curvi_lat_pos     = Calculate_Curvi_Lat_Pos(host_curvature, cluster.vcs_position_x, cluster.vcs_position_y);
   const float32_t position_priority = Calculate_Position_Priority(cluster.vcs_position_x, curvi_lat_pos, range);

   const bool f_moving = Is_Cluster_Moving(cluster);
   const float32_t ttc_priority =
      Calculate_Cluster_TTC_Priority(cluster, host_speed, host_sideslip, range, curvi_lat_pos, f_moving);

   const float32_t base_priority = Probabilistic_Sum(position_priority, ttc_priority);

   const float32_t priority = Apply_Conservative_Movable_Bonus(base_priority, f_moving, tracker_info);
   return priority;
}

/*===========================================================================*\
 * FUNCTION: Calculate_Priority_For_Init()
 *===========================================================================
 * RETURN VALUE:
 * float32_t
 *
 * PARAMETERS:
 * const float32_t host_speed
 * const Angle& host_sideslip
 * const F360_Cluster_T& cluster
 * const float32_t longvel_estimate
 * const float32_t latvel_estimate
 * const float32_t speed_thresh
 * const float32_t host_curvature
 * const F360_Tracker_Info_T& tracker_info
 *
 * EXTERNAL REFERENCES:
 * Calculate_Priority_From_Pos_Vel(), Apply_Conservative_Movable_Bonus()
 *
 * DEVIATIONS FROM STANDARDS:
 * None.
 *
 * --------------------------------------------------------------------------
 * ABSTRACT:
 * --------------------------------------------------------------------------
 * Computes priority for a cluster at init time using the 2D velocity
 * estimate from the initialization algorithm.
 *
 * A lightweight F360_Priority_Pos_Vel_T is built from the cluster position,
 * velocity estimate and a movable_prob derived from the speed threshold,
 * then passed to Calculate_Priority_From_Pos_Vel for the base priority.
 *
 * Moving classification is derived from movable_prob. Moving objects
 * receive a conservative rank-based bonus via
 * Apply_Conservative_Movable_Bonus.
 *
 * PRECONDITIONS:
 * None
 *
 * POSTCONDITIONS:
 * Result is in [0, 2]
 *
\*===========================================================================*/
float32_t Calculate_Priority_For_Init(const float32_t host_speed, const Angle &host_sideslip, const F360_Cluster_T &cluster,
                                      const float32_t longvel_estimate, const float32_t latvel_estimate,
                                      const float32_t speed_thresh, const float32_t host_curvature,
                                      const F360_Tracker_Info_T &tracker_info)
{
   const float32_t speed_sq = (longvel_estimate * longvel_estimate) + (latvel_estimate * latvel_estimate);
   const bool f_moving      = (speed_sq > (speed_thresh * speed_thresh));

   const F360_Priority_Pos_Vel_T pos_vel = {cluster.vcs_position_x, cluster.vcs_position_y, longvel_estimate, latvel_estimate,
                                            f_moving ? 1.0F : 0.0F};

   const float32_t base_priority = Calculate_Priority_From_Pos_Vel(pos_vel, host_speed, host_sideslip, host_curvature);

   const float32_t priority = Apply_Conservative_Movable_Bonus(base_priority, f_moving, tracker_info);
   return priority;
}

/*===========================================================================*\
 * FUNCTION: Calculate_Priority_For_All_Clusters()
 *===========================================================================
 * RETURN VALUE:
 * None
 *
 * PARAMETERS:
 * const float32_t host_speed
 * const float32_t host_curvature
 * const Angle& host_sideslip
 * F360_Tracker_Info_T& tracker_info
 * F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS]
 *
 * EXTERNAL REFERENCES:
 * Calculate_Priority_For_Cluster()
 *
 * DEVIATIONS FROM STANDARDS:
 * None.
 *
 * --------------------------------------------------------------------------
 * ABSTRACT:
 * --------------------------------------------------------------------------
 * Computes priority scores for all active clusters, including movable bonus,
 * via Calculate_Priority_For_Cluster.
 *
 * PRECONDITIONS:
 * tracker_info.num_movable_objs must be set before calling.
 *
 * POSTCONDITIONS:
 * clusters[*].priority is set for all active clusters.
 *
\*===========================================================================*/
void Calculate_Priority_For_All_Clusters(const float32_t host_speed, const float32_t host_curvature, const Angle &host_sideslip,
                                         const F360_Tracker_Info_T &tracker_info, F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS])
{
   // Compute priority scores for all active clusters (cache already updated by caller)
   for (int16_t i = 0; i < tracker_info.num_active_clusters; i++)
   {
      const int16_t cluster_idx = tracker_info.active_cluster_ids[i] - 1;
      F360_Cluster_T &cluster   = clusters[cluster_idx];
      cluster.priority          = Calculate_Priority_For_Cluster(cluster, host_speed, host_curvature, host_sideslip, tracker_info);
   }
}

/*===========================================================================*\
 * FUNCTION: Apply_Movable_Bonus_For_All_Objects()
 *===========================================================================
 * RETURN VALUE:
 * None
 *
 * PARAMETERS:
 * const int32_t(&movable_indices)[NUMBER_OF_OBJECT_TRACKS]
 * const int32_t num_movable
 * const uint32_t max_num_reduced_tracks
 * F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
 *
 * EXTERNAL REFERENCES:
 * Calculate_And_Apply_Movable_Bonus(), F360_Sort()
 *
 * DEVIATIONS FROM STANDARDS:
 * None.
 *
 * --------------------------------------------------------------------------
 * ABSTRACT:
 * --------------------------------------------------------------------------
 * Applies rank-based decayed movable bonus to all movable objects using
 * pre-collected movable indices.
 *
 * If num_movable <= N_full (50% of max_num_reduced_tracks), all movable objects
 * receive the full bonus (1.0) and sorting of movable objects is skipped
 * for efficiency.
 *
 * Otherwise, movable objects are sorted by base priority descending,
 * assigned ranks 1..N_movable, and each receives a bonus that decays
 * linearly from 1.0 (rank 1) to 0.0 (rank = 75% of max_num_reduced_tracks).
 *
 * PRECONDITIONS:
 * object_tracks[*].priority must contain the base priority (no bonus).
 * movable_indices must contain num_movable valid track indices.
 *
 * POSTCONDITIONS:
 * object_tracks[*].priority includes the movable bonus for movable objects.
 *
\*===========================================================================*/
void Apply_Movable_Bonus_For_All_Objects(const int32_t (&movable_indices)[NUMBER_OF_OBJECT_TRACKS], const int32_t num_movable,
                                         const uint32_t max_num_reduced_tracks,
                                         F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS])
{
   const int32_t n_full = static_cast<int32_t>(max_num_reduced_tracks) / 2;

   if (num_movable <= n_full)
   {
      // All movable objects get full bonus, no sorting needed
      for (int32_t i = 0; i < num_movable; i++)
      {
         object_tracks[movable_indices[i]].priority += K_MAX_MOVABLE_BONUS;
      }
   }
   else
   {
      // Sort movable objects by base priority descending, apply rank-based bonus
      float32_t movable_base_prios[NUMBER_OF_OBJECT_TRACKS];
      for (int32_t i = 0; i < num_movable; i++)
      {
         movable_base_prios[i] = object_tracks[movable_indices[i]].priority;
      }

      uint32_t perm[NUMBER_OF_OBJECT_TRACKS];
      (void)F360_Sort(static_cast<uint32_t>(num_movable), false, movable_base_prios, perm);

      for (int32_t i = 0; i < num_movable; i++)
      {
         const int32_t rank    = i + 1;
         const int32_t trk_idx = movable_indices[perm[i]];
         Calculate_And_Apply_Movable_Bonus(rank, max_num_reduced_tracks, object_tracks[trk_idx].priority);
      }
   }
}

/*===========================================================================*\
 * FUNCTION: Calculate_Priority_For_All_Objects_And_Sort()
 *===========================================================================
 * RETURN VALUE:
 * None
 *
 * PARAMETERS:
 * const float32_t host_speed
 * const float32_t host_curvature
 * const Angle& host_sideslip
 * F360_Tracker_Info_T& tracker_info
 * F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
 *
 * EXTERNAL REFERENCES:
 * Calculate_Priority_For_Object(), Apply_Movable_Bonus_For_All_Objects(),
 * Quick_Sort_Track_Priority()
 *
 * DEVIATIONS FROM STANDARDS:
 * None.
 *
 * --------------------------------------------------------------------------
 * ABSTRACT:
 * --------------------------------------------------------------------------
 * Computes new priority scores for all active object tracks, including
 * rank-based decayed movable bonus.
 *
 * Computes base priority (position + TTC probabilistic sum) for each
 * object while counting movable objects, then applies rank-based decayed
 * movable bonus via Apply_Movable_Bonus_For_All_Objects and sorts the
 * priority linked list.
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * object_tracks[*].priority is set for all active objects,
 * tracker_info.num_movable_objs is updated,
 * and priority linked list pointers are updated.
 *
\*===========================================================================*/
void Calculate_Priority_For_All_Objects_And_Sort(const float32_t host_speed, const float32_t host_curvature,
                                                 const Angle &host_sideslip, F360_Tracker_Info_T &tracker_info,
                                                 F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS])
{
   // Compute base priority scores for all active objects (without movable bonus)
   // and collect movable object indices for the bonus computation
   // Note: cache already updated by caller via Update_Priority_Zone_Params
   int32_t movable_indices[NUMBER_OF_OBJECT_TRACKS];
   int32_t num_movable = 0;
   for (int32_t iobj = 0; iobj < tracker_info.num_active_objs; iobj++)
   {
      const int32_t trk_idx       = tracker_info.active_obj_ids[iobj] - 1;
      F360_Object_Track_T &object = object_tracks[trk_idx];
      object.priority             = Calculate_Priority_For_Object(object, host_speed, host_sideslip, host_curvature);

      if (object.movable_prob > MOVABLE_PROB_THRESHOLD)
      {
         movable_indices[num_movable] = trk_idx;
         num_movable++;
      }
   }
   tracker_info.num_movable_objs = num_movable;

   // Apply rank-based decayed movable bonus
   Apply_Movable_Bonus_For_All_Objects(movable_indices, num_movable, tracker_info.variant.num_reduced_tracks, object_tracks);

   // Re-sort priority linked list
   Quick_Sort_Track_Priority(tracker_info, object_tracks);
}

/*===========================================================================*\
* FUNCTION: Adjust_Priority_On_Motion_Flip()
*===========================================================================
* RETURN VALUE:
* None
*
* PARAMETERS:
* const F360_Host_T& host
* const bool f_became_movable
* F360_Object_Track_T& object
* F360_Tracker_Info_T& tracker_info
*
* EXTERNAL REFERENCES:
* Map_Curvature_For_Priority(), Calculate_Priority_For_Object(),
* Apply_Conservative_Movable_Bonus(), Remove_Obj_From_Sorted_Prio_List(),
* Sort_Priority_With_New_Track()
*
* DEVIATIONS FROM STANDARDS:
* None.
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* Recalculates priority for an object whose movable_prob crossed the 0.5
* threshold. If the object became movable, a conservative movable bonus
* is added via Apply_Conservative_Movable_Bonus and num_movable_objs is
* incremented. If it became stationary, no bonus is applied and
* num_movable_objs is decremented. The track is then removed from the
* priority linked list and re-inserted at its new position.
*
* PRECONDITIONS:
* object must be a member of the priority linked list.
*
* POSTCONDITIONS:
* object.priority is updated.
* tracker_info.num_movable_objs is adjusted by +1 or -1.
* Priority linked list is updated.
*
\*===========================================================================*/
void Adjust_Priority_On_Motion_Flip(const F360_Host_T &host, const bool f_became_movable, F360_Object_Track_T &object,
                                    F360_Tracker_Info_T &tracker_info)
{
   const float32_t curvature = Map_Curvature_For_Priority(host.curvature_rear);
   const Angle host_sideslip(host.vcs_sideslip);
   const float32_t base_priority = Calculate_Priority_For_Object(object, host.vcs_speed, host_sideslip, curvature);
   object.priority               = Apply_Conservative_Movable_Bonus(base_priority, f_became_movable, tracker_info);
   if (f_became_movable)
   {
      tracker_info.num_movable_objs++;
   }
   else
   {
      tracker_info.num_movable_objs--;
   }
   Remove_Obj_From_Sorted_Prio_List(tracker_info, object);
   Sort_Priority_With_New_Track(tracker_info, &object);
}
} // namespace f360_variant_A
