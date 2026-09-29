#ifndef LCDA_CAMERA_DATA_T_H
#define LCDA_CAMERA_DATA_T_H

/**
 * @file camera_data_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the definition of the camera data struct.
 *
 * @copyright Copyright (C) 2019 Aptiv. All rights reserved.
 */

#include "pa_reuse.h"

/**
 * @brief This structure holds all of the camera related data such as distance to lane markings, type of lane marking etc.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-6869}
 */
typedef struct
{
   float32_T lane_width_ego;             /* lane width of ego lane provided by camera */
   float32_T lane_width_left;            /* [m] lane width of left adjacent lane provided by camera */
   float32_T lane_width_right;           /* [m] lane width of right adjacent lane provided by camera */
   float32_T lane_center_offset;         /* lateral lane center offset provided by camera */
   float32_T quality_lane_width_ego;     /* quality of lane width of ego lane provided by camera */
   float32_T quality_lane_width_left;    /* quality of lane width of left adjacent lane provided by camera */
   float32_T quality_lane_width_right;   /* quality of lane width of right adjacent lane provided by camera */
   float32_T quality_lane_center_offset; /* quality of lateral lane center offset provided by camera */
   float32_T lane_distance_first_left;   /* [m] distance to first lane border on the left of the ego provided by camera */
   float32_T lane_distance_first_right;  /* [m] distance to first lane border on the right of the ego provided by camera */
   float32_T lane_distance_second_left;  /* [m] distance to second lane border on the left of the ego provided by camera */
   float32_T lane_distance_second_right; /* [m] distance to second lane border on the right of the ego provided by camera */
   float32_T lane_angle_first_left;  /* [rad] angle between ego long.axis and first lane border on the left of the ego provided by
                                        camera */
   float32_T lane_angle_first_right; /* [rad] angle between ego long axis and first lane border on the right of the ego provided by
                                        camera */
   float32_T lane_angle_second_left; /* [rad] angle between ego long axis and second lane border on the left of the ego provided by
                                        camera */
   float32_T lane_angle_second_right; /* [rad] angle between ego long axis and second lane border on the right of the ego provided
                                         by camera */
   float32_T lane_existance_probability_first_left;  /* [percent] existance probability of first lane border on the left of the ego
                                                        provided by camera */
   float32_T lane_existance_probability_first_right; /* [percent] existance probability of first lane border on the right of the
                                                        ego provided by camera */
   float32_T lane_existance_probability_second_left; /* [percent] existance probability of second lane border on the left of the
                                                        ego provided by camera */
   float32_T lane_existance_probability_second_right; /* [percent] existance probability of second lane border on the right of the
                                                         ego provided by camera */
   uint8_t lane_type_first_left;                      /* type of first lane border on the left of the ego provided by camera */
   uint8_t lane_type_first_right;                     /* type of first lane border on the right of the ego provided by camera */
   uint8_t lane_type_second_left;                     /* type of second lane border on the left of the ego provided by camera */
   uint8_t lane_type_second_right;                    /* type of second lane border on the right of the ego provided by camera */
   uint8_t lane_border_status_first_left; /* status (measured or predicted) of first lane border on the left of the ego provided by
                                             camera */
   uint8_t lane_border_status_first_right;  /* status (measured or predicted) of first lane border on the right of the ego provided
                                               by camera */
   uint8_t lane_border_status_second_left;  /* status (measured or predicted) of second lane border on the left of the ego provided
                                               by camera */
   uint8_t lane_border_status_second_right; /* status (measured or predicted) of second lane border on the right of the ego
                                               provided by camera */
   uint8_t lane_color_first_left;           /* color of first lane border on the left of the ego provided by camera */
   uint8_t lane_color_first_right;          /* color of first lane border on the right of the ego provided by camera */
   float32_T lane_curvature_first_left;     /* [1/m] curvature of first lane border on the left of the ego provided by camera */
   float32_T lane_curvature_first_right;    /* [1/m] curvature of first lane border on the right of the ego provided by camera */
   float32_T lane_curvature_second_left;    /* [1/m] curvature of second lane border on the left of the ego provided by camera */
   float32_T lane_curvature_second_right;   /* [1/m] curvature of second lane border on the right of the ego provided by camera */
   float32_T lane_curvature_change_first_left;  /* [1/m^2] curvature change of first lane border on the left of the ego provided by
                                                   camera */
   float32_T lane_curvature_change_first_right; /* [1/m^2] curvature change of first lane border on the right of the ego provided
                                                   by camera */
   float32_T lane_curvature_change_second_left; /* [1/m^2] curvature change of second lane border on the left of the ego provided
                                                   by camera */
   float32_T lane_curvature_change_second_right; /* [1/m^2] curvature change of second lane border on the right of the ego provided
                                                    by camera */

} Camera_Data_T;

#endif /* LCDA_CAMERA_DATA_T_H */
