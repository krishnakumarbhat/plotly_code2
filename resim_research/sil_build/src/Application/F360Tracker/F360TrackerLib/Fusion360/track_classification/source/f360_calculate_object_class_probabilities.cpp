/*===================================================================================*\
* FILE: f360_calculate_object_class_probabilities.cpp
*====================================================================================
* Copyright (C) 2020-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains the function to calculate object class probabilities of a track.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/

#include "f360_math.h"
#include "f360_calculate_object_class_probabilities.h"

namespace f360_variant_A
{
   typedef struct ClassProb_Tag
   {
      float32_t probability;
      F360_Object_Class_T object_class;
   }ClassProb_T;

   /*===========================================================================*\
   * FUNCTION: Determine_To_Freeze_Vehicular_Class
   *===========================================================================
   * RETURN VALUE:
   * bool - true if vehicular class should be frozen, false otherwise
   *
   * PARAMETERS:
   * const F360_Tracker_Info_T& tracker_info - tracker information containing elapsed_time_s
   * F360_Object_Track_T& object - object being analyzed (time_since_veh_class_high_prob is updated)
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
   * Determines whether to freeze (block) VRU class probabilities in favor of 
   * vehicular classes based on speed, RCS, and temporal evidence.
   * 
   * When an object exhibits high speed (only after at least 0.5s from initialization)
   * or RCS confirming vehicle class this function returns true to prevent erroneous VRU classification.
   * It maintains a timer (time_since_obj_considered_veh_for_class_freeze) to track how long the freeze 
   * should persist.
   *
   * PRECONDITIONS:
   * tracker_info.elapsed_time_s should be positive and represent actual cycle time.
   *
   * POSTCONDITIONS:
   * object.time_since_obj_considered_veh_for_class_freeze may be updated:
   *   - Set to 0.0 when vehicle confirmation occurs
   *   - Incremented by tracker_info.elapsed_time_s when freeze is active
   *   - Set to -1.0 when freeze expires or insufficient evidence
   *
   \*===========================================================================*/
   bool Determine_To_Freeze_Vehicular_Class(
      const F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T& object
      )
   {
      const float32_t elapsed_time_s = tracker_info.elapsed_time_s;
      
      constexpr float32_t max_speed_vru_classes = 8.0F; // [m/s] maximum speed for pedestrian and bicycle
      constexpr float32_t max_rcs_vru_classes = 12.0F; // [dBm^2] maximum RCS for pedestrian and bicycle
      const float32_t obj_max_rcs = std::fmaxf(object.average_rcs, object.maximum_rcs); // Use maximum of average and maximum RCS for VRU class blocking
      constexpr float32_t max_class_freeze_time_allowed = 30.0F;

      bool f_freeze_vehicular_class = false;

      // determine to block VRU classes in favor for vehicle classes
      // we consider speed after 0.5s from objects initialization to make sure Kalman filter is converged
      if (((max_speed_vru_classes < object.speed) && (object.time_since_initialization > 0.5F)) || 
          (max_rcs_vru_classes < obj_max_rcs)) 
      {
         // object is confirmed to be of vehicle class, block VRU classes and reset timer
         f_freeze_vehicular_class = true;
         object.time_since_obj_considered_veh_for_class_freeze = 0.0F;
      }
      else if ((object.time_since_obj_considered_veh_for_class_freeze < max_class_freeze_time_allowed) &&
               (object.time_since_obj_considered_veh_for_class_freeze >= 0.0F) &&
               (object.movable_prob > 0.5F))
      {
         // object class should now already be vehicular, freeze vehicular class
         f_freeze_vehicular_class = true;
         object.time_since_obj_considered_veh_for_class_freeze += elapsed_time_s;
      }
      else
      {
         // there is not enough evidence for freezing vehicular class, or the timer has maxed out: invalidate timer
         object.time_since_obj_considered_veh_for_class_freeze = -1.0F;
      }

      return f_freeze_vehicular_class;
   }

   /*===========================================================================*\
   * FUNCTION: calcAprioriProbability
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * OBJ_CLASS_A_PRIORI_PROBABILITIES_T& apriori,
   * F360_Object_Track_T& object,
   * const F360_Tracker_Info_T& tracker_info
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
   * Function assigns apriori probabilities of object being specific class.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void calcAprioriProbability(
      OBJ_CLASS_A_PRIORI_PROBABILITIES_T& apriori,
      F360_Object_Track_T& object,
      const F360_Tracker_Info_T& tracker_info)
   {
      // Initialize apriories to same for all classes
      apriori.pedestrian = 0.2F;
      apriori.bicycle = 0.2F;
      apriori.motorcycle = 0.2F;
      apriori.car = 0.2F;
      apriori.truck = 0.2F;

      // Determine whether to freeze vehicular class (block VRU classes) based on speed/RCS evidence
      const bool f_freeze_vehicular_class = Determine_To_Freeze_Vehicular_Class(tracker_info, object);

      // Decrease apriori based on object speed
      const float32_t abs_obj_speed = std::abs(object.speed);
      apriori.truck *= F360_Linear_Equation_With_Saturation(abs_obj_speed, 30.0F, 35.0F, 1.0F, 0.001F);  // Decrease the likelihood of objects traveling above 100kph being classifed as trucks.

      if (f_freeze_vehicular_class)
      {
         apriori.pedestrian = 0.0F;
         apriori.bicycle = 0.0F;
         apriori.motorcycle = 0.0F;
      }
      else
      {
         // Decrease apriori based on object speed
         apriori.pedestrian *= ((abs_obj_speed > 4.0F) ? 0.0F : 1.0F); // Make it impossible for objects with speed above 4m/s to be classified as pedestrians
         apriori.bicycle *= ((abs_obj_speed > 8.0F) ? 0.0F : 1.0F); // Make it impossible for objects with speed above 8m/s to be classified as bicycles

         // Decrease apriori based on object RCS
         const float32_t obj_max_rcs = std::fmaxf(object.average_rcs, object.maximum_rcs);
         apriori.pedestrian *= ((obj_max_rcs > 12.0F) ? 0.0F : 1.0F); // Make it impossible for objects with RCS above 12dBm^2 to be classified as pedestrians
         apriori.bicycle *= ((obj_max_rcs > 12.0F) ? 0.0F : 1.0F); // Make it impossible for objects with RCS above 12dBm^2 to be classified as bicycles

         // Decrease apriori based on object num_members_in_slow_moving_obj_cluster
         apriori.pedestrian *= ((object.num_members_in_slow_moving_obj_cluster > 2U) ? 0.0F : 1.0F); // Make it impossible for objects in a cluster with 3 or more objects to be classifed as pedestrians
         const float32_t two_wheeler_factor = ((object.num_members_in_slow_moving_obj_cluster > 3U) ? 0.0F : 1.0F); // Make it impossible for objects in a cluster with 4 or more objects to be classifed as a two wheeler (bicyvle and motorcycle)
         apriori.bicycle *= two_wheeler_factor;
         apriori.motorcycle *= two_wheeler_factor;
      }
      



      // Normalize apriory probabilities to sum up to 1
      const float32_t sum = apriori.pedestrian + apriori.bicycle + apriori.motorcycle + apriori.car + apriori.truck;
      if (sum > F360_EPSILON)
      {
         const float32_t normalization_constant = 1.0F / sum;
         apriori.pedestrian *= normalization_constant;
         apriori.bicycle *= normalization_constant;
         apriori.motorcycle *= normalization_constant;
         apriori.car *= normalization_constant;
         apriori.truck *= normalization_constant;
      }
      else
      {
         // Something has gone wrong. Use default uniform apriori
         apriori.pedestrian = 0.2F;
         apriori.bicycle = 0.2F;
         apriori.motorcycle = 0.2F;
         apriori.car = 0.2F;
         apriori.truck = 0.2F;
      }
   }

   /*===========================================================================*\
   * FUNCTION: evaluateNormalDistribution
   *===========================================================================
   * RETURN VALUE:
   * float32_t - calculated pdf value
   *
   * PARAMETERS:
   * const float32_t value - tested value
   * const float32_t mean - mean value of tested value
   * float32_t inv_standard_deviation - inverse of std of tested value (inverse is used for code optimization)
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
   * Function calculates pdf value using normal distribution
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float32_t evaluateNormalDistribution(const float32_t value,
      const float32_t mean,
      float32_t inv_standard_deviation)
   {
      const float32_t tracker_threshold_is_inf = 1e10F;

      const  float32_t inv_sqrt2pi = 0.398942280401433F;
      if (inv_standard_deviation > tracker_threshold_is_inf)
      {
         inv_standard_deviation = 1.0F; // Use 1 as default STD in ase inputted inv_standard devitiaon indicates SDT is close to zero
      }
      const float32_t exponent = (value - mean) * inv_standard_deviation;
      return ((1.0F * inv_sqrt2pi * inv_standard_deviation) * F360_Expf(-0.5F * exponent * exponent));
   }


   /*===========================================================================*\
   * FUNCTION: calcProbability_Criteria_Bayes
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const OBJ_CLASS_A_PRIORI_PROBABILITIES_T& apriori - apriori probabilities
   * OBJECT_CLASS_PROBABILITY_T& criteria_Bayes - struct containing information about calculated probabilities
   * const F360_Object_Track_T& object - analysed object
   * const F360_Calibrations_T& calib - tracker calibrations
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
   * Function calculates pdf value using normal distribution
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void calcProbability_Criteria_Bayes(
      const OBJ_CLASS_A_PRIORI_PROBABILITIES_T& apriori,
      OBJECT_CLASS_PROBABILITY_T& criteria_Bayes,
      const F360_Object_Track_T& object,
      const F360_Calibrations_T& calib)
   {
      /****************
      * evaluate the probabiliy density function at the current values for length, width and speed
      * assuming that the random variables length, width and speed are independent p(length, with, speed|c_i) simplifies to
      * p(length, with, speed|c_i)=p(length|c_i)*p(width|c_i)*p(speed|c_i)
      **********************/

      // Pedestrian
      const float32_t pdf_value_pedestrian = evaluateNormalDistribution(object.bbox.Get_Length(), calib.k_ad_oc_mean_length_pedestrian, calib.k_ad_oc_inv_standard_deviation_length_pedestrian);

      // Bicycle
      const float32_t pdf_value_2wheeler = evaluateNormalDistribution(object.bbox.Get_Length(), calib.k_ad_oc_mean_length_2wheel, calib.k_ad_oc_inv_standard_deviation_length_2wheel);

      const float32_t pdf_value_bicycle = pdf_value_2wheeler;

      // Motorcycle
      const float32_t pdf_value_motorcycle = pdf_value_2wheeler;

      // Car
      const float32_t pdf_value_car = evaluateNormalDistribution(object.bbox.Get_Length(), calib.k_ad_oc_mean_length_car, calib.k_ad_oc_inv_standard_deviation_length_car);

      // Truck
      const float32_t pdf_value_truck = evaluateNormalDistribution(object.bbox.Get_Length(), calib.k_ad_oc_mean_length_truck, calib.k_ad_oc_inv_standard_deviation_length_truck);

      /*********************************************************
      *  Bayes Rule:
      *  c_i class i,  m current measurement,  N number of classes,
      *  P(c_i|m) a posteriori probability of class c_i, p(m|c_i) probability density function of class c_i, P(c_i) a priori probability of class c_i
      *  P(c_i|m)=p(m|c_i)P(c_i)/(sum j=1:N p(m|c_j)P(c_j))
      ************************************************************/
      const float32_t prob_pedestrian = pdf_value_pedestrian * apriori.pedestrian;
      const float32_t prob_bicycle = pdf_value_bicycle * apriori.bicycle;
      const float32_t prob_motorcycle = pdf_value_motorcycle * apriori.motorcycle;
      const float32_t prob_car = pdf_value_car * apriori.car;
      const float32_t prob_truck = pdf_value_truck * apriori.truck;
      float32_t sum = prob_pedestrian + prob_bicycle + prob_motorcycle + prob_car + prob_truck;

      // prevent division by zero if sum == 0.
      if (TRACKER_SUM_OF_PDF_THRESHOLD_IS_ZERO > sum)
      {
         sum = 1.0F;
      }

      const float32_t inv_sum = 1.0F / sum;

      criteria_Bayes.probability_pedestrian = prob_pedestrian * inv_sum;
      criteria_Bayes.probability_bicycle = prob_bicycle * inv_sum;
      criteria_Bayes.probability_motorcycle = prob_motorcycle * inv_sum;
      criteria_Bayes.probability_car = prob_car * inv_sum;
      criteria_Bayes.probability_truck = prob_truck * inv_sum;
      criteria_Bayes.probability_unknown = 0.0F;
   }

   /*===========================================================================*\
   * FUNCTION: calcUndetProb
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * F360_Object_Track_T& object - analysed object
   * const F360_Calibrations_T& cals - tracker calibrations
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
   * function calculates probability that object class is undetermined.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void calcUndetProb(
      F360_Object_Track_T& object,
      const F360_Calibrations_T& cals)
   {
      // if object is new, we don't have enough information to be sure of the classification,
      // decrease the probability for unknown if we have seen the object longer
      if ((F360_OBJECT_STATUS_NEW == object.status) || (F360_OBJECT_STATUS_INVALID == object.status) ||
         (F360_OBJECT_STATUS_NEW_UPDATED == object.status) || (F360_OBJECT_STATUS_NEW_COASTED == object.status))
      {
         object.probability_undet = 1.0F;
      }
      else
      {
         object.probability_undet = object.probability_undet - cals.k_ad_oc_step_decrease_prob_unknown;
      }

      if (0.0F > object.probability_undet)
      {
         object.probability_undet = 0.0F;
      }
      else if (1.0F < object.probability_undet)
      {
         object.probability_undet = 1.0F;
      }
      else
      {
         // do nothing
      }
   }

   /*===========================================================================*\
   * FUNCTION: Set_Object_Class
   *===========================================================================
   * RETURN VALUE:
   * F360_Object_Track_T& object - object with modified parameters
   *
   * PARAMETERS:
   * F360_Object_Track_T& object - analysed object
   * const float32_t min_prob - probability value threshold
   * const float32_t undet_scaling_factor - scaling factor used to inrease undetermined probability value
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
   * Function sets object class basing on calculated probabilities values.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Set_Object_Class(
      F360_Object_Track_T& object)
   {
      constexpr uint8_t num_classes = 6U;
      const ClassProb_T classes[num_classes] = {
          {object.probability_pedestrian, F360_OBJ_CLASS_PEDESTRIAN},
          {object.probability_bicycle, F360_OBJ_CLASS_BICYCLE},
          {object.probability_motorcycle, F360_OBJ_CLASS_MOTORCYCLE},
          {object.probability_car, F360_OBJ_CLASS_CAR},
          {object.probability_truck, F360_OBJ_CLASS_TRUCK},
          {object.probability_undet, F360_OBJ_CLASS_UNDETERMINED}
      };

      F360_Object_Class_T max_class = F360_OBJ_CLASS_UNDETERMINED;
      float32_t max_prob = 0.0F;

      // Choose class with the highest probability and assign it to object
      for (uint8_t i = 0U; i < num_classes; i++)
      {
          if (classes[i].probability > max_prob)
          {
              max_prob = classes[i].probability;
              max_class = classes[i].object_class;
          }
      }
      object.object_class = max_class;
   }

   /*===========================================================================*\
   * FUNCTION: filteringAndNormalizationOfProbabilities
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * F360_Object_Track_T& object - analysed object
   * const OBJECT_CLASS_PROBABILITY_T& criteria_Bayes - calculated probabilities innovation
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
   * Function filters probabilities using first order filter and normalizes them so they sum up to 1.0F
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void filteringAndNormalizationOfProbabilities(
      F360_Object_Track_T& object,
      const OBJECT_CLASS_PROBABILITY_T& criteria_Bayes)
   {
      const float32_t alpha = 0.6F;

      // use a low pass filter to prevent quick switches between classes.
      const float32_t one_minus_alpha = 1.0F - alpha;
      object.probability_pedestrian = alpha * object.probability_pedestrian + one_minus_alpha * criteria_Bayes.probability_pedestrian;
      object.probability_car = alpha * object.probability_car + one_minus_alpha * criteria_Bayes.probability_car;
      object.probability_truck = alpha * object.probability_truck + one_minus_alpha * criteria_Bayes.probability_truck;
      object.probability_bicycle = alpha * object.probability_bicycle + one_minus_alpha * criteria_Bayes.probability_bicycle;
      object.probability_motorcycle = alpha * object.probability_motorcycle + one_minus_alpha * criteria_Bayes.probability_motorcycle;

      // normalize
      float32_t sum = object.probability_pedestrian + object.probability_motorcycle
         + object.probability_bicycle + object.probability_car + object.probability_truck;

      // prevent division by zero if sum == 0.
      if (TRACKER_SUM_OF_PDF_THRESHOLD_IS_ZERO > sum)
      {
         sum = 1.0F;
      }

      /* /sum to make sum(ped, 2wheel, car, truck)=1
      factor (1.0f - object.object_class_probability.reserved_value_5) to decrease (reserved_value_1, 2wheel, car, truck) so that
      sum(reserved_value_1, 2wheel, car, truck, unknown)=1 without changing reserved_value_5*/
      const float32_t normalize_factor = (1.0F - object.probability_undet) / sum;

      object.probability_pedestrian *= normalize_factor;
      object.probability_bicycle *= normalize_factor;
      object.probability_motorcycle *= normalize_factor;
      object.probability_car *= normalize_factor;
      object.probability_truck *= normalize_factor;
   }

   /*===========================================================================*\
   * FUNCTION: calcProbability
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * F360_Object_Track_T& object - analysed object
   * const OBJ_CLASS_A_PRIORI_PROBABILITIES_T& apriori - apriori probabilities
   * const F360_Calibrations_T& calib - tracker calibrations
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
   * Function calculates probabilities of object belonging to defined class and assigns
   * winner class to object.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void calcProbability(
      F360_Object_Track_T& object,
      const OBJ_CLASS_A_PRIORI_PROBABILITIES_T& apriori,
      const F360_Calibrations_T& calib)
   {
      OBJECT_CLASS_PROBABILITY_T criteria_Bayes{};

      // Compute probability based on each criteria
      calcProbability_Criteria_Bayes(apriori, criteria_Bayes, object, calib);

      // Set the probability for undetermined class, depening on for how long we have been tracking this object
      calcUndetProb(object, calib);

      // Reason about if we have enough info to be able to update classes or if we want to increase prob for undetermined instead
      constexpr uint8_t num_classes = 5U;
      const ClassProb_T classes[num_classes] = {
          {criteria_Bayes.probability_pedestrian, F360_OBJ_CLASS_PEDESTRIAN},
          {criteria_Bayes.probability_bicycle, F360_OBJ_CLASS_BICYCLE},
          {criteria_Bayes.probability_motorcycle, F360_OBJ_CLASS_MOTORCYCLE},
          {criteria_Bayes.probability_car, F360_OBJ_CLASS_CAR},
          {criteria_Bayes.probability_truck, F360_OBJ_CLASS_TRUCK}
      };

      F360_Object_Class_T max_class = F360_OBJ_CLASS_UNDETERMINED;
      float32_t max_prob = 0.0F;

      // Find class with the highest current probability
      for (uint8_t i = 0U; i < num_classes; i++)
      {
         if (classes[i].probability > max_prob)
         {
            max_prob = classes[i].probability;
            max_class = classes[i].object_class;
         }
      }

      const bool f_possible_VRU = (max_class == F360_OBJ_CLASS_PEDESTRIAN) ||
         (max_class == F360_OBJ_CLASS_BICYCLE) || 
         ((max_class == F360_OBJ_CLASS_MOTORCYCLE) && (std::abs(object.speed) < 8.0F));
      
      const bool f_likely_vehicle_takeoff = (F360_OBJ_CLASS_UNDETERMINED == object.object_class) && ((object.time_since_started_move < 0.55F) || (object.tang_accel > 0.5F));
      
      const bool f_moving_in_long_cluster = (object.num_members_in_slow_moving_obj_cluster > 0U) && (object.length_of_slow_moving_obj_cluster > 2.5F);

      const bool f_delay_classification_due_to_VRU_veh_confusion = f_possible_VRU && (f_likely_vehicle_takeoff || f_moving_in_long_cluster);

      const bool f_object_speed_in_range_stationary = (object.movable_prob < 0.5F) && (object.speed < calib.k_ad_oc_max_stationary_speed);

      const bool f_increase_prob_undetermined = f_delay_classification_due_to_VRU_veh_confusion || f_object_speed_in_range_stationary;
      if (!f_increase_prob_undetermined)
      {
         // Filter and normalize the probabilities
         filteringAndNormalizationOfProbabilities(object, criteria_Bayes);
      }
      else
      {
         // Increase prob for undetermined class while freezing the ratio between other classes
         // (i.e. keep current classification until prob undet becomes large enough to set UNKNOWN class)
         Update_Probability_Undetermined(calib, object);
      }

      Set_Object_Class(object);
   }


   /*===========================================================================*\
   * FUNCTION: Update_Probability_Undetermined
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const F360_Calibrations_T& calibs - tracker calibrations
   * F360_Object_Track_T& object - analysed object
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
   * Function improves classification of slow moving object performing
   * speed-based recognition of pedestrians and stationary objects.
   * 
   * PRECONDITIONS:
   * The function expects probability of pedestrian not to be greater than 1.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Update_Probability_Undetermined(
      const F360_Calibrations_T& calib,
      F360_Object_Track_T& object)
   {
      object.probability_car *= calib.k_ad_oc_prob_decrease;
      object.probability_pedestrian *= calib.k_ad_oc_prob_decrease;
      object.probability_motorcycle *= calib.k_ad_oc_prob_decrease;
      object.probability_bicycle *= calib.k_ad_oc_prob_decrease;
      object.probability_truck *= calib.k_ad_oc_prob_decrease;

      object.probability_undet = 1.0F - (object.probability_pedestrian + object.probability_car + object.probability_motorcycle +
         object.probability_bicycle + object.probability_truck);
   }

   /*===========================================================================*\
   * FUNCTION: run_obj_class
   *===========================================================================
   * RETURN VALUE:
   * F360_Object_Track_T& object - object with modified parameters
   *
   * PARAMETERS:
   * F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS] - object tracks array
   * const F360_Tracker_Info_T& tracker_info - struct containing information about tracker
   * const F360_Calibrations_T& calibs - tracker calibrations
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
   * Function sets object class basing on calculated probabilities values.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void run_obj_class(
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      const F360_Tracker_Info_T& tracker_info,
      const F360_Calibrations_T& calibs)
   {

      for (int32_t num_obj_active = 0; num_obj_active < tracker_info.num_active_objs; num_obj_active++)
      {
         const int32_t obj_trk_idx = tracker_info.active_obj_ids[num_obj_active] - 1;

         OBJ_CLASS_A_PRIORI_PROBABILITIES_T apriori;
         calcAprioriProbability(apriori, object_tracks[obj_trk_idx], tracker_info);

         calcProbability(object_tracks[obj_trk_idx], apriori, calibs);
      }
   }
}
