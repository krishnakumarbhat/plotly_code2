/*===================================================================================*\
* FILE: f360_calculate_object_class_probabilities.h
*====================================================================================
*-----------------------------------------------------------------------------------------
* %full_filespec: %
* %version: %
* %derived_by: %
* %date_created: %
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declarations for caclculating the probability vectors
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*  None
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#ifndef F360_CALCULATE_OBJECT_CLASS_H
#define F360_CALCULATE_OBJECT_CLASS_H

#include "f360_reuse.h"
#include "f360_globals.h"
#include "f360_host.h"
#include "f360_tracker_info.h"
#include "f360_calibrations.h"
#include "f360_object_class_probability_type.h"

namespace f360_variant_A
{
   const float32_t TRACKER_SUM_OF_PDF_THRESHOLD_IS_ZERO = 1E-20F;

   /**
   This structure is used to hold the a priori probabilities
   */
   struct OBJ_CLASS_A_PRIORI_PROBABILITIES_T
   {
      float32_t pedestrian;  // a priori probability for class pedestrian
      float32_t bicycle;     // a priori probability for class bicycle
      float32_t motorcycle;  // a priori probability for class motorcycle
      float32_t car;         // a priori probability for class car
      float32_t truck;       // a priori probability for class truck
   };

   void calcAprioriProbability(
      OBJ_CLASS_A_PRIORI_PROBABILITIES_T& apriori,
      F360_Object_Track_T& object,
      const F360_Tracker_Info_T& tracker_info
   );

   float32_t evaluateNormalDistribution(const float32_t value,
      const float32_t mean,
      float32_t inv_standard_deviation
   );

   void calcProbability_Criteria_Bayes(
      const OBJ_CLASS_A_PRIORI_PROBABILITIES_T& apriori,
      OBJECT_CLASS_PROBABILITY_T& criteria_Bayes,
      const F360_Object_Track_T& object,
      const F360_Calibrations_T& calib);

   void calcUndetProb(
      F360_Object_Track_T& object,
      const F360_Calibrations_T& cals);

   void Set_Object_Class(
      F360_Object_Track_T& object);

   void filteringAndNormalizationOfProbabilities(
      F360_Object_Track_T& object,
      const OBJECT_CLASS_PROBABILITY_T& criteria_Bayes);

   void calcProbability(
      F360_Object_Track_T& object,
      const OBJ_CLASS_A_PRIORI_PROBABILITIES_T& apriori,
      const F360_Calibrations_T& calib
   );

   void Update_Probability_Undetermined(
      const F360_Calibrations_T& calib,
      F360_Object_Track_T& object);

   bool Determine_To_Freeze_Vehicular_Class(
      const F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T& object);

   void run_obj_class(
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      const F360_Tracker_Info_T& tracker_info,
      const F360_Calibrations_T& calibs);
}
#endif
