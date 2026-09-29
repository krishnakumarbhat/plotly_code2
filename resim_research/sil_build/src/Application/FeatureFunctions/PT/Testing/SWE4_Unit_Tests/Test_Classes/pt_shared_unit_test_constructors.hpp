#ifndef PT_SHARED_UNIT_TEST_CONSTRUCTORS_HPP
#define PT_SHARED_UNIT_TEST_CONSTRUCTORS_HPP

/**
 * @file pt_shared_unit_test_constructors.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Helper class used for code reduction accross all test fixture classes.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/*Gtest include is needed here, because of the internal mathlib/shared toolbox redefinition of Max*/
#include <gtest/gtest.h> // IWYU pragma: keep

extern "C"
{
#include "fbk_macros.h"
#include "ml_interval.h"
#include "pt_common_functions.h"
#include "pt_constants.h"
#include "pt_core_calibration_t.h"
#include "pt_types.h"
}

typedef struct
{
   float32_T factor_cubic;
   float32_T factor_quadr;
   float32_T factor_lin;
   float32_T factor_const;
   uint8_t first_p;
   uint8_t last_p;
   Pt_Path_Direction_T direction;
   Pt_Path_Status_T path_state;
} Pt_Path_Creation_Props_T;

class Pt_Shared_Unit_Test_Constructors
{
 protected:
   /* Initializes a path point array and temporary grid values to their given inputs. This can be used for a standard
    * initialization or even for temporary datatypes of path rotation functionality within the unit test framework*/
   void Pt_Init_Path_Points_Linearly(float32_T *p_path_points,
                                     float32_T *p_grid_vals,
                                     const uint8_t lower_path_index_border,
                                     const uint8_t upper_path_index_border,
                                     const float32_T slope,
                                     const float32_T offset,
                                     const float32_T grid_array[PT_NUM_GRID_POINTS]);

   /* Initializes a given path structure*/
   void Pt_Init_Path_Linearly(Pt_Path_T *p_path,
                              float32_T grid_array[PT_NUM_GRID_POINTS],
                              const Pt_Path_Direction_T path_direction,
                              const uint8_t lower_path_index_border,
                              const uint8_t upper_path_index_border,
                              const uint8_t num_groupings,
                              const Vector_2d_T first_path_point,
                              const Vector_2d_T last_path_point,
                              const float32_T slope,
                              const float32_T offset);

   /*Used for initialization of constant, linear, quadratic or cubic pt_persistent.paths*/
   void Pt_Create_Path(Pt_Path_T *p_path,
                       Pt_Path_Creation_Props_T *p_path_creation_props,
                       float32_T temp_grid_array[PT_NUM_GRID_POINTS]);

   /*Initialization function for creation properties of pt_persistent.paths*/
   void Pt_Init_Creation_Prop(Pt_Path_Creation_Props_T *p_path_creation_props,
                              uint8_t lower_border,
                              uint8_t upper_border,
                              float32_T cubic_part,
                              float32_T quadr_part,
                              float32_T lin_part,
                              float32_T const_part,
                              Pt_Path_Direction_T direction,
                              Pt_Path_Status_T path_state);

   /*Used for further path extension by a defined value in a given point interval*/
   void Pt_Expand_Path(Pt_Path_T *p_path, uint8_t lower_border, uint8_t upper_border, float32_T value);
};

inline void Pt_Shared_Unit_Test_Constructors::Pt_Init_Path_Points_Linearly(float32_T *p_path_points,
                                                                           float32_T *p_grid_vals,
                                                                           const uint8_t lower_path_index_border,
                                                                           const uint8_t upper_path_index_border,
                                                                           const float32_T slope,
                                                                           const float32_T offset,
                                                                           const float32_T grid_array[PT_NUM_GRID_POINTS])
{
   Int_Range_T range = Create_Int_Range(lower_path_index_border, upper_path_index_border);
   for (uint8_t idx = PT_LOWEST_GRID_POINT_INDEX; idx <= PT_HIGHEST_GRID_POINT_INDEX; idx++)
   {
      if (Is_Int_Contained_In_Int_Range(idx, &range))
      {
         p_path_points[idx] = (float32_T) slope * idx + offset;
      }
      else
      {
         p_path_points[idx] = PT_PATH_POINTS_DEFAULT_VAL;
      }
      p_grid_vals[idx] = grid_array[idx];
   }
}


inline void Pt_Shared_Unit_Test_Constructors::Pt_Init_Path_Linearly(Pt_Path_T *p_path,
                                                                    float32_T grid_array[PT_NUM_GRID_POINTS],
                                                                    const Pt_Path_Direction_T path_direction,
                                                                    const uint8_t lower_path_index_border,
                                                                    const uint8_t upper_path_index_border,
                                                                    const uint8_t num_groupings,
                                                                    const Vector_2d_T first_path_point,
                                                                    const Vector_2d_T last_path_point,
                                                                    const float32_T slope,
                                                                    const float32_T offset)
{
   /* Set attributes of given path */
   p_path->direction                       = path_direction;
   p_path->first                           = first_path_point;
   p_path->last_mat                        = last_path_point;
   p_path->first_p                         = lower_path_index_border;
   p_path->last_p                          = upper_path_index_border;
   p_path->num_groupings                   = num_groupings;
   p_path->path_state                      = PATH_STATUS_MATURE;
   p_path->obj_curr_used_for_path_build.id = 0u;

   /* Set path points in given range */
   Pt_Init_Path_Points_Linearly(p_path->path_points, grid_array, p_path->first_p, p_path->last_p, slope, offset, grid_array);
}

inline void Pt_Shared_Unit_Test_Constructors::Pt_Create_Path(Pt_Path_T *p_path,
                                                             Pt_Path_Creation_Props_T *p_path_creation_props,
                                                             float32_T temp_grid_array[PT_NUM_GRID_POINTS])
{
   float32_T x;

   p_path->path_state = p_path_creation_props->path_state;
   p_path->direction  = p_path_creation_props->direction;
   p_path->first_p    = p_path_creation_props->first_p;
   p_path->last_p     = p_path_creation_props->last_p;

   for (uint8_t idx = PT_LOWEST_GRID_POINT_INDEX; idx <= PT_HIGHEST_GRID_POINT_INDEX; idx++)
   {
      if (idx >= p_path_creation_props->first_p && idx <= p_path_creation_props->last_p)
      {
         x                        = temp_grid_array[idx];
         p_path->path_points[idx] = p_path_creation_props->factor_cubic * pow(x, 3.0f)
                                    + p_path_creation_props->factor_quadr * pow(x, 2.0f) + p_path_creation_props->factor_lin * x
                                    + p_path_creation_props->factor_const;
      }
      else
      {
         p_path->path_points[idx] = 0.0f;
      }
   }
}


inline void Pt_Shared_Unit_Test_Constructors::Pt_Init_Creation_Prop(Pt_Path_Creation_Props_T *p_path_creation_props,
                                                                    uint8_t lower_border,
                                                                    uint8_t upper_border,
                                                                    float32_T cubic_part,
                                                                    float32_T quadr_part,
                                                                    float32_T lin_part,
                                                                    float32_T const_part,
                                                                    Pt_Path_Direction_T direction,
                                                                    Pt_Path_Status_T path_state)
{
   p_path_creation_props->first_p      = lower_border;
   p_path_creation_props->last_p       = upper_border;
   p_path_creation_props->factor_cubic = cubic_part;
   p_path_creation_props->factor_quadr = quadr_part;
   p_path_creation_props->factor_lin   = lin_part;
   p_path_creation_props->factor_const = const_part;
   p_path_creation_props->direction    = direction;
   p_path_creation_props->path_state   = path_state;
}


inline void Pt_Shared_Unit_Test_Constructors::Pt_Expand_Path(Pt_Path_T *p_path, uint8_t lower_border, uint8_t upper_border, float32_T value)
{
   /*Set union of borders*/
   p_path->first_p = Fbk_Min(p_path->first_p, lower_border);
   p_path->last_p  = Fbk_Max(p_path->last_p, upper_border);

   for (uint8_t idx = lower_border; idx <= upper_border; idx++)
   {
      p_path->path_points[idx] = value;
   }
}

#endif /* PT_SHARED_UNIT_TEST_CONSTRUCTORS_HPP */
