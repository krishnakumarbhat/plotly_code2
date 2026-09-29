/*===========================================================================*\
* FILE: f360_update_longi_stat_curves.cpp
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function definition of Update_Longi_Stat_Curves() and subfunctions
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_update_longi_stat_curves.h"
#include "f360_math.h"
#include "f360_matrix_vector_Init_real32_T.h"
#include "f360_math_func.h"
#include "f360_longi_stat_curve_init.h"
#include "f360_cluster_objects_for_lsc.h"
#include "f360_downselect_longi_stat_clusters.h"
#include "f360_post_process_longi_stat_clusters.h"
#include "f360_get_wall_time.h"
#include "f360_check_if_point_is_inside_box.h"
#include "f360_static_env_helpers.h"
#include "f360_vcs_long_sorted_dets_support_functions.h"


namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Update_Longi_Stat_Curves()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *   const F360_Tracker_Info_T& tracker_info,
   *   const F360_Calibrations_T& calibs,
   *   const F360_Host_T& host,
   *   const F360_Host_Props_T& host_props,
   *   const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
   *   F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS],
   *   F360_Longi_Stat_Curve_T(&longi_stat_curves)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES],
   *   Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
   *   F360_TRKR_TIMING_INFO_T& timing_info
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
   * This function clusters slow moving CCA objects and fits polynomial to clusters 
   * that fulfill the requirements to become longi stat curves
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Update_Longi_Stat_Curves(
      const F360_Tracker_Info_T& tracker_info,
      const F360_Calibrations_T& calibs,
      const F360_Host_T& host,
      const F360_Host_Props_T& host_props,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      const F360_Globals_T& globals,
      F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS],
      F360_Longi_Stat_Curve_T(&longi_stat_curves)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES],
      Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      F360_TRKR_TIMING_INFO_T& timing_info)
   {
      const float32_t start_time = get_wall_time();
      
      // remember old LSCs adjusted to host motion
      F360_Longi_Stat_Curve_T old_longi_stat_curves[MAX_NR_OF_LONGITUDINAL_STAT_CURVES] = {};

      if (tracker_info.f_highway_suspected)
      {
         Remember_Old_LSC(longi_stat_curves, host_props, old_longi_stat_curves);
      }
          
      // Reset all curves from previous tracker iteration, all curves are instantaneous estimates and not filtered in time
      F360_Longi_Stat_Curve_Init(longi_stat_curves);

      for (int32_t i = 0; i < tracker_info.num_active_objs; i++)
      {
         const int32_t obj_idx = tracker_info.active_obj_ids[i] - 1;

         objects[obj_idx].lsc_next_in_cluster = NULL;
         objects[obj_idx].lsc_prev_in_cluster = NULL;
      }

      // Init first iteration. Prepare data for start of clustering algo
      uint16_t nr_next_ids_of_interest;
      uint16_t next_ids_of_interest[NUMBER_OF_OBJECT_TRACKS] = {};
      const bool f_is_data_available = Arrange_First_Iteration(
         tracker_info,
         calibs,
         nr_next_ids_of_interest,
         next_ids_of_interest
         );

      if (f_is_data_available)
      {
         // Valid cluster array. Array of clusters that are candidates to be fitted to polynomial in the end of this function
         uint16_t nr_valid_clusters = 0U;
         F360_Longi_Stat_Cluster_T valid_clusters[NR_LONGI_STAT_CLUSTERS] = {};

         Cluster_Objects_For_LSC(
            calibs,
            tracker_info,
            host,
            nr_next_ids_of_interest,
            next_ids_of_interest,
            objects,
            nr_valid_clusters,
            valid_clusters);
         
         const float32_t host_turn_radius = std::abs(host.curvature_rear) > F360_EPSILON ? 1.0F / host.curvature_rear : INFTY;
         Post_Process_Longi_Stat_Clusters(
            calibs,
            tracker_info,
            host_turn_radius,
            nr_valid_clusters,
            valid_clusters);

         uint16_t nr_downselected_clusters;
         F360_Longi_Stat_Cluster_T downselected_clusters[MAX_NR_OF_LONGITUDINAL_STAT_CURVES] = {};
         Downselect_Longi_Stat_Clusters(
            nr_valid_clusters,
            valid_clusters,
            calibs,
            nr_downselected_clusters,
            downselected_clusters);

         F360_Longi_Stat_Curve_T new_longi_stat_curves[MAX_NR_OF_LONGITUDINAL_STAT_CURVES] = {};
         Fit_Second_Order_Polynomials_To_Clusters(
            nr_downselected_clusters,
            downselected_clusters,
            new_longi_stat_curves);

         Sanity_Check_And_Populate_LSC_Output(
            calibs,
            nr_downselected_clusters,
            new_longi_stat_curves,
            longi_stat_curves);

         // Extend LSC x_max using detections if only rear sensors are present
         const bool f_only_rear_sensors_present = (globals.f_rear_sensor_available) && (!(globals.f_front_or_front_corner_sensor_available));
         if ((f_only_rear_sensors_present) && (host.speed > 3.0F))
         {
            Extend_LSC_With_Detections(
               calibs,
               host,
               raw_detections,
               longi_stat_curves);
         }
         if (tracker_info.f_highway_suspected)
         {
            Extend_Old_LSC(old_longi_stat_curves, longi_stat_curves);
         }
      }

      for (uint8_t i = 0U; i < MAX_NR_OF_LONGITUDINAL_STAT_CURVES; i++)
      {
         Map_Single_LSC_To_Static_Env_Poly(longi_stat_curves[i], static_env_polys[i]);
      }

      timing_info.lsc_module = get_wall_time() - start_time;
   }

   /*===========================================================================*\
   * FUNCTION: Arrange_First_Iteration()
   * ===========================================================================
   * RETURN VALUE:
   * bool f_is_data_remaining
   *
   * PARAMETERS:
   *   const F360_Tracker_Info_T& tracker_info,
   *   const F360_Calibrations_T& calibs,
   *   const F360_Host_T& host,
   *   F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS],
   *   uint16_t& nr_next_ids_of_interest,
   *   uint16_t(&next_ids_of_interest)[NUMBER_OF_OBJECT_TRACKS]
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
   * This function arranges the data for the first clustering iteration.
   * It copies the longitudinal sorted objects array and does a sanity check that 
   * enough objects exists for clustering to be possbile.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Arrange_First_Iteration(
      const F360_Tracker_Info_T& tracker_info,
      const F360_Calibrations_T& calibs,
      uint16_t& nr_next_ids_of_interest,
      uint16_t(&next_ids_of_interest)[NUMBER_OF_OBJECT_TRACKS]
      )
   {
      // Fill sorted array of objects
      nr_next_ids_of_interest = 0U;
      const F360_Object_Track_T* curr_trk = tracker_info.vcslong_sorted_start; // Start with first object in list
      for (uint32_t i = 0U; i < static_cast<uint32_t>(tracker_info.num_active_objs); i++)
      {
         if (curr_trk->bbox.Get_Center().x < calibs.k_lsc_min_long_pos)
         {
            // Do nothing
         }
         else if (curr_trk->bbox.Get_Center().x > calibs.k_lsc_max_long_pos)
         {
            // Stop filling relevant objects
            break;
         }
         else
         {
            const bool f_non_movable_cca = (F360_TRACKER_TRKFLTR_CCA == curr_trk->trk_fltr_type) && (curr_trk->movable_prob < 0.5F);
            if (f_non_movable_cca)
            {
               next_ids_of_interest[nr_next_ids_of_interest] = static_cast<uint16_t>(curr_trk->id);
               nr_next_ids_of_interest++;
            }
         }

         // take next object from list
         curr_trk = tracker_info.vcslong_sorted_next_track[curr_trk->id - 1];
      }

      // Check that there are enough objects to form at least one cluster
      bool f_is_data_remaining;
      if (nr_next_ids_of_interest >= calibs.k_lsc_min_points_in_cluster)
      {
         f_is_data_remaining = true;
      }
      else
      {
         f_is_data_remaining = false;
      }

      return f_is_data_remaining;
   }

    /*===========================================================================*\
    * FUNCTION: Fit_Second_Order_Polynomials_To_Clusters()
    * ===========================================================================
    * RETURN VALUE:
    * None
    *
    * PARAMETERS:
    *   const uint16_t nr_valid_clusters
    *   const F360_Longi_Stat_Cluster_T(&valid_clusters)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES]
    *   F360_Longi_Stat_Curve_T(&all_curves)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES]
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
    * This function fits a second degree polynomial to valid clusters and populates the
    * longi stat curve structure.
    *
    * PRECONDITIONS:
    *
    * POSTCONDITIONS:
    * None
    *
    \*===========================================================================*/
   void Fit_Second_Order_Polynomials_To_Clusters(
      const uint16_t nr_valid_clusters,
      const F360_Longi_Stat_Cluster_T(&valid_clusters)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES],
      F360_Longi_Stat_Curve_T(&all_curves)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES])
   {

      for (uint32_t i = 0U; i < nr_valid_clusters; i++)
      {
         // Arrange matrix for least square fit
         const uint32_t nr_clustered_ids = valid_clusters[i].nr_objects;
         const int32_t nr_clustered_ids_signed = static_cast<int32_t>(nr_clustered_ids);

         const int32_t nr_poly_coeff_slots_signed = static_cast<int32_t>(LSC_NR_POLY_COEFF_SLOTS);
         F360_matrix_real32_LSC_T A;
         A.m_size[0] = nr_clustered_ids_signed;
         A.m_size[1] = nr_poly_coeff_slots_signed;
         A.numDimensions = nr_poly_coeff_slots_signed;

         F360_vector_real32_LSC_T B;
         B.m_size = nr_clustered_ids_signed;
         B.numDimensions = 1;

         F360_Object_Track_T* current_obj = valid_clusters[i].first_object;
         for (uint32_t k = 0U; k < nr_clustered_ids; k++)
         {

            // Compensate for center displacement
            const float32_t obj_aspect_angle = F360_Atan2f(current_obj->bbox.Get_Center().y, current_obj->bbox.Get_Center().x);
            const float32_t obj_radius = current_obj->bbox.Get_Length() * 0.5F;   // nonmoveable objects are in circular shape
            const float32_t x_vcs = current_obj->bbox.Get_Center().x - obj_radius * F360_Cosf(obj_aspect_angle);
            const float32_t y_vcs = current_obj->bbox.Get_Center().y - obj_radius * F360_Sinf(obj_aspect_angle);

            // Fill matrices for polynomial fit
            A.data[k][0] = x_vcs * x_vcs;
            A.data[k][1] = x_vcs;
            A.data[k][2] = 1.0F;

            B.data[k] = y_vcs;

            current_obj = current_obj->lsc_next_in_cluster;
         }

         float32_t x1; // Coefficient a 
         float32_t x2; // Coefficient b
         float32_t x3; // Coefficient c
         const bool f_poly_fit_ok = F360_Fit_Second_Degree_Polynomial(A, B, x1, x2, x3);

         if (f_poly_fit_ok)
         {
            all_curves[i].f_valid = true;
            all_curves[i].x_min = Get_Cluster_Min_Long_Pos(valid_clusters[i]);
            all_curves[i].x_max = Get_Cluster_Max_Long_Pos(valid_clusters[i]);
            all_curves[i].a = x1;
            all_curves[i].b = x2;
            all_curves[i].c = x3;
            all_curves[i].mean_lat_pos = valid_clusters[i].lat_mean;
         }
         else
         {
            all_curves[i].f_valid = false;
            all_curves[i].x_min = 0.0F;
            all_curves[i].x_max = 0.0F;
            all_curves[i].a = 0.0F;
            all_curves[i].b = 0.0F;
            all_curves[i].c = 0.0F;
            all_curves[i].mean_lat_pos = 0.0F;
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Sanity_Check_And_Populate_LSC_Output()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *   const F360_Calibrations_T & calibs
   *   const uint16_t nr_downselected_clusters
   *   const F360_Longi_Stat_Curve_T(new_longi_stat_curves)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES]
   *   F360_Longi_Stat_Curve_T(longi_stat_curves)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES]
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
   * This function sanity checks the found curves and populates the LSC
   * structure if the curve is valid.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Sanity_Check_And_Populate_LSC_Output(
      const F360_Calibrations_T & calibs,
      const uint16_t nr_downselected_clusters,
      const F360_Longi_Stat_Curve_T(&new_longi_stat_curves)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES],
      F360_Longi_Stat_Curve_T(&longi_stat_curves)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES])
   {
      // Sanity check found polynomials and fill output structure
      for (uint32_t i = 0U; i < nr_downselected_clusters; i++)
      {
         longi_stat_curves[i] = new_longi_stat_curves[i];

         if (std::abs(longi_stat_curves[i].a) > calibs.k_lsc_max_a_coeff)
         {
            longi_stat_curves[i].f_valid = false;
         }
      }
   }
   /*===========================================================================*\
   * FUNCTION: Remember_Old_LSC()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *   const F360_Longi_Stat_Curve_T(&longi_stat_curves)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES]
   *   const F360_Host_Props_T& host_props,
   *   F360_Longi_Stat_Curve_T(&old_longi_stat_curves)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES]
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
   * This function remmebers old LSC in highway scenario
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Remember_Old_LSC(
      const F360_Longi_Stat_Curve_T(&longi_stat_curves)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES],
      const F360_Host_Props_T& host_props,
      F360_Longi_Stat_Curve_T(&old_longi_stat_curves)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES])
   {
      const float32_t host_delta_x = host_props.delta_position_x;
      const float32_t host_delta_y = host_props.delta_position_y;
      for (uint8_t i = 0U; i < MAX_NR_OF_LONGITUDINAL_STAT_CURVES; i++)
      {
         F360_Longi_Stat_Curve_T lsc = longi_stat_curves[i];

         if (lsc.f_valid && (std::abs(lsc.a) < 0.001F) && (std::abs(lsc.b) < 0.1F))
         {
            lsc.b = lsc.b - 2.0F * lsc.a * host_delta_x;
            lsc.c = lsc.c + lsc.a * host_delta_x * host_delta_x - lsc.b * host_delta_x - host_delta_y;
            lsc.x_max = lsc.x_max - host_delta_x;
            lsc.x_min = lsc.x_min - host_delta_x;
            old_longi_stat_curves[i] = lsc;
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Extend_Old_LSC()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Longi_Stat_Curve_T(&old_longi_stat_curves)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES]
   * F360_Longi_Stat_Curve_T(&old_longi_stat_curves)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES]
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
   * This function extends old LSCs in highway scenario based on history.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Extend_Old_LSC(
      const F360_Longi_Stat_Curve_T(&old_longi_stat_curves)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES],
      F360_Longi_Stat_Curve_T(&longi_stat_curves)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES])
   {
      for (uint8_t i = 0U; i < MAX_NR_OF_LONGITUDINAL_STAT_CURVES; i++)
      {
         F360_Longi_Stat_Curve_T& new_lsc = longi_stat_curves[i];
         if ((new_lsc.f_valid) && (std::abs(new_lsc.a) < 0.001F) && (std::abs(new_lsc.b) < 0.1F) && (new_lsc.x_min < -15.0F))
         {
            for (uint8_t j = 0U; j < MAX_NR_OF_LONGITUDINAL_STAT_CURVES; j++)
            {
               const F360_Longi_Stat_Curve_T old_lsc = old_longi_stat_curves[j];
               if ((old_lsc.f_valid) && (std::abs(old_lsc.c - new_lsc.c) < 0.5F) && (new_lsc.x_min > old_lsc.x_min) && (old_lsc.x_max > new_lsc.x_min))
               {
                  new_lsc.x_min = std::max(old_lsc.x_min, -100.0F);
                  break;
               }
            }
         }
      }
   }

  /*===========================================================================*\
  * FUNCTION: Extend_LSC_With_Detections()
  * ===========================================================================
  * RETURN VALUE:
  * None.
  *
  * PARAMETERS:
  *   const F360_Calibrations_T& calibs,
  *   const F360_Host_T& host,
  *   const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
  *   F360_Longi_Stat_Curve_T(&longi_stat_curves)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES]
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
  * This function checks if the x_max of an LSC is behind the host (up to 35m
  * behind) and the mean lateral position is within 10m of the host origin.
  * If so, and if LSC is mostly linear with minimal curvature and is aligned parallel 
  * to host motion, it looks for stationary detections in front of the LSC's x_max
  * position and extends the LSC x_max using the furthest valid detection found + 5m 
  * further forward if the LSC now ends closer than 10m behind host (otherwise it
  * still extends upto closest detection if it is more than 10m behind host).
  * Only detections up to the host origin (vcs_position.x <= 0) are considered.
  * The same longitudinal and lateral position gates used for object clustering
  * are applied for detection clustering mostly except that the lateral gate is
  * smaller to account for handling only parallel guardrails in this case and there 
  * is no increase in logitudinal gate at high speed since detections do not lag as 
  * much as objects and do not need this gate extension. Uses the VCS-longitudinal
  * sorted detection list for efficient traversal.
  * 
  * 
  *
  * PRECONDITIONS:
  * LSCs should be populated before calling this function.
  * Detections must be sorted in VCS-longitudinal order.
  *
  * POSTCONDITIONS:
  * None
  *
  \*===========================================================================*/
   void Extend_LSC_With_Detections(
      const F360_Calibrations_T& calibs,
      const F360_Host_T& host,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      F360_Longi_Stat_Curve_T(&longi_stat_curves)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES])
   {
      const float32_t abs_host_speed = std::abs(host.speed);
      
      // Set up range of x_max values for LSC (i.e. where LSC begins in x distance from host) where this logic should be active. Upper limit is set to 0m always (i.e. behind host origin) while lower limit is set to a value between 15m and 30m behind host depending on host speed
      const float32_t lsc_x_max_upper_lim = 0.0F;
      const float32_t lsc_x_max_lower_lim = - F360_Linear_Equation_With_Saturation(
         abs_host_speed,
         3.0F,           // min abs host speed [m/s]
         30.0F,          // max abs host speed [m/s]
         15.0F,           // lower limit of lsc x_max value (i.e. limit of x value where lsc starts) when abs host speed is 0m/s (set up as positive value here and then converted to negative before Linear Equation function call since function expects positive slope)
         35.0F           // lower limit of lsc x_max value (i.e. limit of x value where lsc starts) when abs host speed is 25m/s or above (set up as positive value here and then converted to negative before Linear Equation function call since function expects positive slope)
      );

      for (uint8_t lsc_idx = 0U; lsc_idx < MAX_NR_OF_LONGITUDINAL_STAT_CURVES; lsc_idx++)
      {
         F360_Longi_Stat_Curve_T& lsc = longi_stat_curves[lsc_idx];
         const float32_t lsc_x_max = lsc.x_max;
         const float32_t lsc_x_min = lsc.x_min;
         const float32_t lsc_y_pos_at_x_max = lsc.LSC_Lateral_Pos_At(lsc_x_max); // Uses the polynomial formula of the LSC to get the y position of the LSC at the x_max position of the LSC
         const float32_t lsc_long_length = lsc_x_max - lsc_x_min;
         const float32_t lsc_lat_limit_lower = 1.5F; // Lower Limit of lateral position of lsc intercept and last object in formed lsc to allow lsc extension (to ensure lsc's very close are not extended on top of host)
         const float32_t lsc_lat_limit_upper = 10.0F; // Upper Limit of lateral position of lsc intercept and last object in formed lsc to allow lsc extension (to ensure lsc's very far are not extended as they are irrelevant to host mirror)

         const bool f_lsc_intercept_same_side_as_lsc_lat_mean = ((lsc.c * lsc.mean_lat_pos) > 0.0F); // Check that Y intercept of the LSC is on the same side of the host as the end of LSC so that it does not cross over to other side of LSC when extending
         const bool f_lsc_intercept_within_lateral_threshold = ((std::abs(lsc.c) < lsc_lat_limit_upper) && (std::abs(lsc.c) > lsc_lat_limit_lower)); // Checks that Y intercept of the LSC is not too close to host origin (which might indicate a false positive LSC fit to objects close to host) but also not too far from host origin
         const bool f_lsc_intercept_ok_for_extension = ((f_lsc_intercept_same_side_as_lsc_lat_mean) && (f_lsc_intercept_within_lateral_threshold));
         
         const bool f_lsc_end_y_pos_ok_for_extension = ((std::abs(lsc_y_pos_at_x_max) < lsc_lat_limit_upper) && (std::abs(lsc_y_pos_at_x_max) > lsc_lat_limit_lower)); //Check if lateral position last object used to form lsc is close enough to host origin but not too close to be a false positive extension on top of host

         // Check if LSC is valid for extension using detections ahead of it
         const bool lsc_valid_for_extension = ((lsc.f_valid) && // Check if lsc is valid
                                                (lsc_x_max < lsc_x_max_upper_lim) && // Check if lsc end point is behind host
                                                (lsc_x_max > lsc_x_max_lower_lim) && // Check if lsc end point is within max distance behind host
                                                (f_lsc_end_y_pos_ok_for_extension) && // Check if lateral position last object used to form lsc fulfills conditions for extension
                                                (std::abs(lsc.a) < 0.012F) && // Check that lsc is close to linear but allowing some light curvature
                                                (std::abs(lsc.b) < 0.27F) && // Check that lsc is parallel to host within +-15 degrees (approx.)
                                                (f_lsc_intercept_ok_for_extension) && // Check that Y intercept of LSC fulfills condition for extension
                                                (lsc_long_length > 10.0F)); // Check that lsc has minimum length of 10m to be considered for extension

         if (lsc_valid_for_extension)
         {
            // Determine clustering gates based on host speed (same logic as Cluster_Longi_Stat_Objects)
            float32_t lsc_long_pos_gate;
            float32_t lsc_lat_pos_gate;

            lsc_long_pos_gate = calibs.k_lsc_long_pos_gate;
            lsc_lat_pos_gate = calibs.k_lsc_lat_pos_gate * 0.75F; // Lateral gate is reduced to be more strict for detection based lsc extension as existing guardrail with good positioning data exists

            float32_t prev_clustered_x = lsc_x_max;
            float32_t max_det_x = lsc_x_max;

            // Get the first detection at or after the LSC's x_max position using sorted list
            int32_t det_idx = Get_First_Relevant_Long_Sorted_Det_Idx(lsc_x_max, raw_detections);

            // Iterate through sorted detections to find candidates in front of the LSC's x_max
            bool f_continue_loop = (det_idx > F360_INVALID_ID);
            for (uint32_t i = 0U; (i < raw_detections.number_of_valid_detections) && f_continue_loop; i++)
            {
               const rspp_variant_A::RSPP_Detection_T& det = raw_detections.detections[det_idx];
               const float32_t det_x = det.processed.vcs_position_x;
               const float32_t det_y = det.processed.vcs_position_y;

               // Stop processing if detection is beyond host origin (x > 0) or if longitudinal gate is exceeded
               if ((det_x > lsc_x_max_upper_lim) || ((det_x - prev_clustered_x) > lsc_long_pos_gate))
               {
                  f_continue_loop = false;
               }
               else
               {
                  // Only consider detections that are ok to use and stationary dets
                  if ((det.processed.f_ok_to_use) && (det.processed.motion_status == rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS))
                  {
                     // Check lateral gate against the LSC polynomial evaluated at the detection's x position
                     const float32_t lsc_y_at_det_x = lsc.LSC_Lateral_Pos_At(det_x);
                     const float32_t lat_diff = std::abs(det_y - lsc_y_at_det_x);
                     if (lat_diff <= lsc_lat_pos_gate)
                     {
                        // Detection matches clustering criteria, update tracking variables
                        prev_clustered_x = det_x;
                        // Track the maximum x position found
                        if (det_x > max_det_x)
                        {
                           max_det_x = det_x;
                        }
                     }
                  }
                  // Get next detection in sorted order
                  det_idx = det.processed.next_sorted_idx;
               }
               // Only continue if continue flag is not set to false above or if next det exists and is not invalid
               f_continue_loop = f_continue_loop && (det_idx > F360_INVALID_ID);
            }

            // Update LSC x_max only if valid detections were found (extend lsc further forward by 5m if dets found closer than 10m to host since this might mean guardrail continues in front of host)
            if (max_det_x > lsc_x_max)
            {
               if(max_det_x < -10.0F)
               {
                  lsc.x_max = max_det_x;
               }
               else
               {
                  lsc.x_max = max_det_x + 5.0F;
               }
            }
         }
      }
   }
}
