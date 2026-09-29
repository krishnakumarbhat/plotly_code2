/**
 * @file lane_model.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Lane model based on camera and navigation data.
 *
 * @copyright Copyright (C) 2019 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "lane_model.h"
#include "fbk_macros.h"
#include "fbk_vehicle_data_t.h"
#include "lane_model_camera_data.h"
#include "lcda_bmw_sp25_debug_interface.h"
#include "ml_saturated_math.h"
#include <assert.h>

/*===========================================================================*\
* Local Defines
\*===========================================================================*/

#define LCDA_VALID_SPEED_LOWER_THRESH (0.0f)
#define LCDA_VALID_SPEED_UPPER_THRESH (100.0f)
#define LCDA_VALID_YAWRATE_LOWER_THRESH (-5.0f)
#define LCDA_VALID_YAWRATE_UPPER_THRESH (5.0f)

typedef enum
{
   NAV_ROAD_TYPE_UNKNOWN              = 0u,
   NAV_TRAFFIC_CALMED_ZONE_BIDIR      = 1u,
   NAV_TRAFFIC_CALMED_ZONE_ONEWAY     = 2u,
   NAV_RESIDENTIAL_AREA_BIDIR         = 3u,
   NAV_RESIDENTIAL_AREA_ONEWAY        = 4u,
   NAV_URBAN_BIDIR                    = 5u,
   NAV_URBAN_ONEWAY                   = 6u,
   NAV_URBAN_WITH_MEDIAN_STRIP        = 7u,
   NAV_CITY_FREEWAY                   = 8u,
   NAV_CITY_FREEWAY_WITH_MEDIAN_STRIP = 9u,
   NAV_COUNTRY_ROAD                   = 10u,
   NAV_COUNTRY_ROAD_WITH_MEDIAN_STRIP = 11u,
   NAV_MAIN_ROAD                      = 12u,
   NAV_MAIN_ROAD_WITH_MEDIAN_STRIP    = 13u,
   NAV_MAIN_ROAD_ON_RAMP              = 14u,
   NAV_MAIN_ROAD_EXIT                 = 15u,
   NAV_MAIN_ROAD_ON_RAMP_AND_EXIT     = 16u,
   NAV_HIGHWAY                        = 17u,
   NAV_HIGHWAY_ON_RAMP                = 18u,
   NAV_HIGHWAY_ON_RAMP_AND_EXIT       = 19u
} Navigation_Road_Type_T;

typedef enum
{
   ROAD_TYPE_UNKNOWN = 0,
   ROAD_TYPE_CITY    = 1,
   ROAD_TYPE_HIGHWAY = 2
} Road_Type_T;

typedef struct
{
   uint16_t vdyn_count_city2hway;
   uint16_t vdyn_count_hway2city;

   uint16_t navi_count_city;
   uint16_t navi_count_hway;

   Road_Type_T vdyn_road_type;
   Road_Type_T navi_road_type;

} Lane_Model_Persistent_T;

/*===========================================================================*\
* File Scope variables
\*===========================================================================*/

static Lane_Model_Persistent_T Lane_Model_Persistent;

/*===========================================================================*\
* Local Function Prototypes
\*===========================================================================*/

/**
 * @brief Fills the lane model output with default values.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2780}
 * @SDD{SF-6895}
 * @verification{}
 */
static void Lcda_Fill_Default_Output(const Lcda_Core_Calibration_T *p_cals /**< Lcda calibration data */,
                                     Lane_Model_Output_T *p_lane_model_output /**< Lcda lane model output */,
                                     const Country_Type_T country_type /**< Country type */);

/**
 * @brief Initializes all the persistence variables related to vehicle dynamic based lane model.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2780}
 * @SDD{SF-6894}
 * @verification{}
 */
static void Lcda_Init_Veh_Dyn_Pers(void);

/**
 * @brief Initializes all the persistence variables related to navigation data based lane model.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2780}
 * @SDD{SF-6893}
 * @verification{}
 */
static void Lcda_Init_Navi_Pers(void);

/**
 * @brief Populates the lane model output with the lane width (from cals) using the following priority
 * 1. camera data based lane model
 * 2. navigation based lane model
 * 3. vehicle driving dynamics based lane model
 * 3. If the outputs from 1, 2 and 3 are UNKNOWN, then the default value for lane width is populated.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2780}
 * @SDD{SF-6892}
 * @verification{}
 */
static void Lcda_Fill_Lane_Model_Output(const Lane_Model_Output_Camera_T *p_camera_result /**< Lcda lane model camera output */,
                                        const Road_Type_T navi_result /**< Navigation road type */,
                                        const Road_Type_T vdyn_result /**< Vehicle dynamics road type */,
                                        const Lcda_Core_Calibration_T *p_cals /**< Lcda calibration data */,
                                        Lane_Model_Output_T *p_lane_model_output /**< Lcda lane model output */,
                                        const Country_Type_T country_type /**< Country type */);

/**
 * @brief This function checks if the vehicle speed and yawrate signals are within a plausible range.
 *
 * @return true if vehicle data is valid, false otherwise
 *
 * @SRS{}
 * @SAE{SF-2780}
 * @SDD{SF-6891}
 * @verification{}
 */
static boolean_T Lcda_Is_Veh_Data_Valid(const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */);

/**
 * @brief This function returns true if the navigation data is equal to one of the values related to city driving.
 *
 * @return true if navi road type is city, false otherwise
 *
 * @SRS{}
 * @SAE{SF-2780}
 * @SDD{SF-6890}
 * @verification{}
 */
static boolean_T Lcda_Is_Navi_Road_Type_City(const Navigation_Road_Type_T navi_road_data /**< Navigation road data type */);

/**
 * @brief This function returns true if the navigation data is equal to one of the values related to highway driving.
 *
 * @return true if navi road type is highway, false otherwise
 *
 * @SRS{}
 * @SAE{SF-2780}
 * @SDD{SF-6889}
 * @verification{}
 */
static boolean_T Lcda_Is_Navi_Road_Type_Hway(const Navigation_Road_Type_T navi_road_data /**< Navigation road data type */);

/**
 * @brief This function returns the road type as determined using the navigation data. The navigation data should be related to a
 * given road type (city/highway) for a given number of consecutive cycles before the road type is changed. Otherwise the previous
 * state is maintained. The state information is maintained in the persistence variable Lane_Model_Persistent.navi_road_type.
 *
 * @return Navigation data lane model
 *
 * @SRS{}
 * @SAE{SF-2780}
 * @SDD{SF-6896}
 * @verification{}
 */
static Road_Type_T Lcda_Get_Navi_Data_Lane_Model(const Navigation_Road_Type_T navi_road_data /**< Navigation road data type */,
                                                 const Lcda_Core_Calibration_T *p_cals /**< Lcda calibration data */);

/**
 * @brief This function returns the road type as determined using the vehicle driving dynamics data. The road type switched between
 * city and highway.
 *
 * @return Vehicle dynamic lane model
 *
 * @SRS{}
 * @SAE{SF-2780}
 * @SDD{SF-6897}
 * @verification{}
 */
static Road_Type_T Lcda_Get_Veh_Dyn_Lane_Model(const float32_T speed /**< Host vehicle speed */,
                                               const float32_T yawrate /**< Host vehicle yawrate */,
                                               const Lcda_Core_Calibration_T *p_cals /**< Lcda calibration data */);

/**
 * @brief This function checks if the conditions for a transition from CITY to HIGHWAY are satisfied. If the speed is greater than
 * a threshold, then the count is incremented. If the count > 0 then it indicates that the speed has been above the given threshold
 * before, hence we now check if the speed is greater than a slightly lower threshold (hysteresis). When the vdyn_count_city2hway
 * exceeds a given threshold, then the road type can transition to type HIGHWAY. Otherwise, the road type remains as CITY. Note
 * that the counts are saturated as the max value.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2780}
 * @SDD{SF-6898}
 * @verification{}
 */
static void Lcda_Vd_Process_City(const float32_T speed /**< Host vehicle speed */,
                                 const Lcda_Core_Calibration_T *p_cals /**< Lcda calibration data */);

/**
 * @brief This function checks if the conditions for a transition from HIGHWAY to CITY are satisfied. If the speed is lower than
 * the hysteresis threshold, and the yawrate is greater than a threshold then the count is incremented. If the count > 0 then it
 * indicates that the speed and yawrate conditions have been satisfied before, so the threholds are now reduced by the hysteresis
 * value. When the vdyn_count_hway2city exceeds a given threshold, then the road type can transition to type CITY. Otherwise, the
 * road type remains as HIGHWAY. Note that the counts are saturated as the max value.
 *
 * IMPORTANT NOTE:
 * Only the absolute value of yaw rate is checked against the threshold. In case the yaw rate toggles between for example -2.0 and
 * +2.0 such that the abs value of the yawrate is greater than the threshold, this function will still consider that the condition
 * for city is met. However this is in fact not a plausible yawrate. Hence in the future consider implementing an additional check
 * that delta yawrate should be below a threshold.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2780}
 * @SDD{SF-6899}
 * @verification{}
 */
static void Lcda_Vd_Process_Highway(const float32_T speed /**< Host vehicle speed */,
                                    const float32_T yawrate /**< Host vehicle yawrate */,
                                    const Lcda_Core_Calibration_T *p_cals /**< Lcda calibration data */);

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/


void Lcda_Initialize_Lane_Model(const Lcda_Core_Calibration_T *p_cals, Lane_Model_Output_T *p_lane_model_output)
{
   /* Initialize camera lane model */
   Lcda_Init_Lane_Model_Camera();

   /* Initilialize the persistence */
   Lcda_Init_Veh_Dyn_Pers();
   Lcda_Init_Navi_Pers();

   /* Initialize the outputs */
   Lcda_Fill_Default_Output(p_cals, p_lane_model_output, COUNTRY_TYPE_DEFAULT);
}


void Lcda_Process_Lane_Model(const Lcda_Input_T *p_lcda_input,
                             const Fbk_Vehicle_Data_T *p_vehicle_data,
                             const Lcda_Core_Calibration_T *p_cals,
                             Lane_Model_Output_T *p_lane_model_output)
{
   Road_Type_T navi_result;
   Road_Type_T vdyn_result;
   Lane_Model_Output_Camera_T lane_output_camera;
   /* coverity[misra_c_2012_rule_10_5_violation][Intentional cast from unsigned integer to matching enum type]  */
   Country_Type_T country_type = (Country_Type_T) p_lcda_input->country_type;
   /* coverity[misra_c_2012_rule_10_5_violation][Intentional cast from unsigned integer to matching enum type]  */
   Navigation_Road_Type_T navigation_road_type = (Navigation_Road_Type_T) p_lcda_input->navigation_data_road_type;

   /*Init lane_output_camera*/
   Lcda_Init_Lane_Output(&lane_output_camera);
   lane_output_camera.lane_lateral_speed[FBK_SIDE_LEFT]  = FBK_ZERO_F;
   lane_output_camera.lane_lateral_speed[FBK_SIDE_RIGHT] = FBK_ZERO_F;


   if (Fbk_Is_True(p_cals->k_lm_use_default_lane_information))
   {
      Lcda_Fill_Default_Output(p_cals, p_lane_model_output, country_type);
   }
   else
   {
      if (Fbk_Is_True(p_cals->k_lm_enable_use_camera_data) && Fbk_Is_True(p_lcda_input->f_lcda_enable_extended_lane_model))
      {
         Lcda_Get_Camera_Data_Lane_Model(p_lcda_input, p_vehicle_data, p_cals, &lane_output_camera);
      }
      else
      {
         Lcda_Init_Lane_Model_Camera();
         Lcda_Init_Lane_Output(&lane_output_camera);
      }

      if (Fbk_Is_True(p_cals->k_lm_enable_use_navigation_data) && Fbk_Is_True(p_lcda_input->f_lcda_enable_basic_lane_model))
      {
         navi_result = Lcda_Get_Navi_Data_Lane_Model(navigation_road_type, p_cals);
      }
      else
      {
         navi_result = ROAD_TYPE_UNKNOWN;
         Lcda_Init_Navi_Pers();
      }

      if (Fbk_Is_True(p_cals->k_lm_enable_use_vehicle_dyn) && Lcda_Is_Veh_Data_Valid(p_vehicle_data)
          && Fbk_Is_True(p_lcda_input->f_lcda_enable_basic_lane_model))
      {
         vdyn_result = Lcda_Get_Veh_Dyn_Lane_Model(p_vehicle_data->host_speed, p_vehicle_data->yawrate, p_cals);
      }
      else
      {
         vdyn_result = ROAD_TYPE_UNKNOWN;
         Lcda_Init_Veh_Dyn_Pers();
      }

      /* Populate the output*/
      Lcda_Fill_Lane_Model_Output(&lane_output_camera, navi_result, vdyn_result, p_cals, p_lane_model_output, country_type);

      /* Write debug data. */
      Binary_Pass_Lcda_Debug_Bmw_Sp25_Lane_Model(&lane_output_camera, p_lane_model_output, Lane_Model_Persistent.vdyn_count_city2hway,
                                                 Lane_Model_Persistent.vdyn_count_hway2city, Lane_Model_Persistent.navi_count_city,
                                                 Lane_Model_Persistent.navi_count_hway, Lane_Model_Persistent.vdyn_road_type,
                                                 Lane_Model_Persistent.navi_road_type);
   }
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static void Lcda_Fill_Default_Output(const Lcda_Core_Calibration_T *p_cals,
                                     Lane_Model_Output_T *p_lane_model_output,
                                     const Country_Type_T country_type)
{
   switch (country_type)
   {
      case COUNTRY_TYPE_US:
         p_lane_model_output->lane_width = p_cals->k_lcda_lm_lane_width_us_default;
         break;
      case COUNTRY_TYPE_JAPAN:
         p_lane_model_output->lane_width = p_cals->k_lcda_lm_lane_width_japan_default;
         break;
      case COUNTRY_TYPE_CHINA:
         p_lane_model_output->lane_width = p_cals->k_lcda_lm_lane_width_china_default;
         break;
      case COUNTRY_TYPE_KOREA:
         p_lane_model_output->lane_width = p_cals->k_lcda_lm_lane_width_korea_default;
         break;
      case COUNTRY_TYPE_GERMANY:
         p_lane_model_output->lane_width = p_cals->k_lcda_lm_lane_width_germany_default;
         break;
      default:
         p_lane_model_output->lane_width = p_cals->k_lcda_lm_lane_width_defaultcountry_default;
         break;
   }

   p_lane_model_output->lane_center_offset                 = p_cals->k_lm_lane_center_offset_default;
   p_lane_model_output->lane_lateral_speed[FBK_SIDE_LEFT]  = FBK_ZERO_F;
   p_lane_model_output->lane_lateral_speed[FBK_SIDE_RIGHT] = FBK_ZERO_F;
   p_lane_model_output->calculation_method                 = CALC_METHOD_DEFAULT_VALUE;
}

static void Lcda_Init_Veh_Dyn_Pers(void)
{
   Lane_Model_Persistent.vdyn_road_type       = ROAD_TYPE_CITY;
   Lane_Model_Persistent.vdyn_count_city2hway = FBK_ZERO_UINT;
   Lane_Model_Persistent.vdyn_count_hway2city = FBK_ZERO_UINT;
}

static void Lcda_Init_Navi_Pers(void)
{
   Lane_Model_Persistent.navi_road_type  = ROAD_TYPE_UNKNOWN;
   Lane_Model_Persistent.navi_count_city = FBK_ZERO_UINT;
   Lane_Model_Persistent.navi_count_hway = FBK_ZERO_UINT;
}

static void Lcda_Fill_Lane_Model_Output(const Lane_Model_Output_Camera_T *p_camera_result,
                                        const Road_Type_T navi_result,
                                        const Road_Type_T vdyn_result,
                                        const Lcda_Core_Calibration_T *p_cals,
                                        Lane_Model_Output_T *p_lane_model_output,
                                        const Country_Type_T country_type)
{
   /* Fill Lateral Speed for left and right lane markings*/
   p_lane_model_output->lane_lateral_speed[FBK_SIDE_LEFT]  = p_camera_result->lane_lateral_speed[FBK_SIDE_LEFT];
   p_lane_model_output->lane_lateral_speed[FBK_SIDE_RIGHT] = p_camera_result->lane_lateral_speed[FBK_SIDE_RIGHT];

   if ((LM_CAMERA_STATUS_AVAILABLE == p_camera_result->status) || (LM_CAMERA_STATUS_DEGRADED == p_camera_result->status))
   {
      p_lane_model_output->lane_width         = p_camera_result->lane_width;
      p_lane_model_output->lane_center_offset = p_camera_result->lane_center_offset;
      p_lane_model_output->calculation_method = CALC_METHOD_CAMERA_DATA;
   }
   else if (ROAD_TYPE_CITY == navi_result)
   {
      p_lane_model_output->lane_width         = p_cals->k_lm_lane_width_city;
      p_lane_model_output->lane_center_offset = p_cals->k_lm_lane_center_offset_default;
      p_lane_model_output->calculation_method = CALC_METHOD_NAVIGATION_DATA;
   }
   else if (ROAD_TYPE_HIGHWAY == navi_result)
   {
      p_lane_model_output->lane_width         = p_cals->k_lm_lane_width_highway;
      p_lane_model_output->lane_center_offset = p_cals->k_lm_lane_center_offset_default;
      p_lane_model_output->calculation_method = CALC_METHOD_NAVIGATION_DATA;
   }
   else if (ROAD_TYPE_CITY == vdyn_result)
   {
      p_lane_model_output->lane_width         = p_cals->k_lm_lane_width_city;
      p_lane_model_output->lane_center_offset = p_cals->k_lm_lane_center_offset_default;
      p_lane_model_output->calculation_method = CALC_METHOD_DRIVING_DYNAMICS;
   }
   else if (ROAD_TYPE_HIGHWAY == vdyn_result)
   {
      p_lane_model_output->lane_width         = p_cals->k_lm_lane_width_highway;
      p_lane_model_output->lane_center_offset = p_cals->k_lm_lane_center_offset_default;
      p_lane_model_output->calculation_method = CALC_METHOD_DRIVING_DYNAMICS;
   }
   else
   {
      Lcda_Fill_Default_Output(p_cals, p_lane_model_output, country_type);
   }
}

static boolean_T Lcda_Is_Veh_Data_Valid(const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   /* TODO: This should be updated to consider the quality factor of the vehicle speed and
    * yawrate signals when they are available.
    * Currently we simply check if the speed and yaw rate are within some plausible range */
   return (boolean_T) ((p_vehicle_data->host_speed > LCDA_VALID_SPEED_LOWER_THRESH)
                       && (p_vehicle_data->host_speed < LCDA_VALID_SPEED_UPPER_THRESH)
                       && (Fbk_Abs_F(p_vehicle_data->yawrate) > LCDA_VALID_YAWRATE_LOWER_THRESH)
                       && (Fbk_Abs_F(p_vehicle_data->yawrate) < LCDA_VALID_YAWRATE_UPPER_THRESH));
}

static boolean_T Lcda_Is_Navi_Road_Type_City(const Navigation_Road_Type_T navi_road_data)
{
   return (boolean_T) ((NAV_TRAFFIC_CALMED_ZONE_BIDIR == navi_road_data) || (NAV_TRAFFIC_CALMED_ZONE_ONEWAY == navi_road_data)
                       || (NAV_RESIDENTIAL_AREA_BIDIR == navi_road_data) || (NAV_RESIDENTIAL_AREA_ONEWAY == navi_road_data)
                       || (NAV_URBAN_BIDIR == navi_road_data) || (NAV_URBAN_ONEWAY == navi_road_data)
                       || (NAV_URBAN_WITH_MEDIAN_STRIP == navi_road_data));
}

static boolean_T Lcda_Is_Navi_Road_Type_Hway(const Navigation_Road_Type_T navi_road_data)
{
   return (boolean_T) ((NAV_COUNTRY_ROAD == navi_road_data) || (NAV_COUNTRY_ROAD_WITH_MEDIAN_STRIP == navi_road_data)
                       || (NAV_MAIN_ROAD == navi_road_data) || (NAV_MAIN_ROAD_WITH_MEDIAN_STRIP == navi_road_data)
                       || (NAV_MAIN_ROAD_ON_RAMP == navi_road_data) || (NAV_MAIN_ROAD_EXIT == navi_road_data)
                       || (NAV_MAIN_ROAD_ON_RAMP_AND_EXIT == navi_road_data) || (NAV_HIGHWAY == navi_road_data)
                       || (NAV_HIGHWAY_ON_RAMP == navi_road_data) || (NAV_HIGHWAY_ON_RAMP_AND_EXIT == navi_road_data)
                       || (NAV_CITY_FREEWAY == navi_road_data) || (NAV_CITY_FREEWAY_WITH_MEDIAN_STRIP == navi_road_data));
}

static Road_Type_T Lcda_Get_Navi_Data_Lane_Model(const Navigation_Road_Type_T navi_road_data, const Lcda_Core_Calibration_T *p_cals)
{
   if (Lcda_Is_Navi_Road_Type_City(navi_road_data))
   {
      Sat_Inc_Uint16(&Lane_Model_Persistent.navi_count_city);
      Lane_Model_Persistent.navi_count_hway = FBK_ZERO_UINT;
   }
   else if (Lcda_Is_Navi_Road_Type_Hway(navi_road_data))
   {
      Sat_Inc_Uint16(&Lane_Model_Persistent.navi_count_hway);
      Lane_Model_Persistent.navi_count_city = FBK_ZERO_UINT;
   }
   else
   {
      Lane_Model_Persistent.navi_count_hway = FBK_ZERO_UINT;
      Lane_Model_Persistent.navi_count_city = FBK_ZERO_UINT;
      Lane_Model_Persistent.navi_road_type  = ROAD_TYPE_UNKNOWN;
   }

   if (Lane_Model_Persistent.navi_count_city > p_cals->k_lm_min_count_in_state)
   {
      Lane_Model_Persistent.navi_road_type  = ROAD_TYPE_CITY;
      Lane_Model_Persistent.navi_count_hway = FBK_ZERO_UINT;
   }
   else if (Lane_Model_Persistent.navi_count_hway > p_cals->k_lm_min_count_in_state)
   {
      Lane_Model_Persistent.navi_road_type  = ROAD_TYPE_HIGHWAY;
      Lane_Model_Persistent.navi_count_city = FBK_ZERO_UINT;
   }
   else
   {
      /* do nothing */
   }

   return Lane_Model_Persistent.navi_road_type;
}

static Road_Type_T Lcda_Get_Veh_Dyn_Lane_Model(const float32_T speed, const float32_T yawrate, const Lcda_Core_Calibration_T *p_cals)
{

   switch (Lane_Model_Persistent.vdyn_road_type)
   {
      case ROAD_TYPE_CITY:
      {
         Lcda_Vd_Process_City(speed, p_cals);
         break;
      }

      case ROAD_TYPE_HIGHWAY:
      {
         Lcda_Vd_Process_Highway(speed, yawrate, p_cals);
         break;
      }

      default:
      {
         /* We should never get here */
         assert(Lane_Model_Persistent.vdyn_road_type > ROAD_TYPE_HIGHWAY);
         break;
      }
   }

   return Lane_Model_Persistent.vdyn_road_type;
}

static void Lcda_Vd_Process_City(const float32_T speed, const Lcda_Core_Calibration_T *p_cals)
{
   if (speed > p_cals->k_lm_min_speed_hway)
   {
      Sat_Inc_Uint16(&Lane_Model_Persistent.vdyn_count_city2hway);
   }
   else if ((Lane_Model_Persistent.vdyn_count_city2hway > FBK_ZERO_UINT)
            && (speed > (p_cals->k_lm_min_speed_hway - p_cals->k_lm_hys_delta_speed_hway)))
   {
      Sat_Inc_Uint16(&Lane_Model_Persistent.vdyn_count_city2hway);
   }
   else
   {
      Lane_Model_Persistent.vdyn_count_city2hway = FBK_ZERO_UINT;
   }

   if (Lane_Model_Persistent.vdyn_count_city2hway > p_cals->k_lm_min_count_in_state)
   {
      Lane_Model_Persistent.vdyn_road_type = ROAD_TYPE_HIGHWAY;
   }
   else
   {
      Lane_Model_Persistent.vdyn_road_type = ROAD_TYPE_CITY;
   }
}

static void Lcda_Vd_Process_Highway(const float32_T speed, const float32_T yawrate, const Lcda_Core_Calibration_T *p_cals)
{
   if ((speed < (p_cals->k_lm_min_speed_hway - p_cals->k_lm_hys_delta_speed_hway))
       && (Fbk_Abs_F(yawrate) > p_cals->k_lm_min_yawrate_city_abs))
   {
      Sat_Inc_Uint16(&Lane_Model_Persistent.vdyn_count_hway2city);
   }
   else if ((Lane_Model_Persistent.vdyn_count_hway2city > FBK_ZERO_UINT)
            && (speed < (p_cals->k_lm_min_speed_hway - p_cals->k_lm_hys_delta_speed_hway))
            && (Fbk_Abs_F(yawrate) > (p_cals->k_lm_min_yawrate_city_abs - p_cals->k_lm_hys_delta_yawrate_city_abs)))
   {
      Sat_Inc_Uint16(&Lane_Model_Persistent.vdyn_count_hway2city);
   }
   else
   {
      Lane_Model_Persistent.vdyn_count_hway2city = 0;
   }

   if (Lane_Model_Persistent.vdyn_count_hway2city > p_cals->k_lm_min_count_in_state)
   {
      Lane_Model_Persistent.vdyn_road_type = ROAD_TYPE_CITY;
   }
   else
   {
      Lane_Model_Persistent.vdyn_road_type = ROAD_TYPE_HIGHWAY;
   }
}
