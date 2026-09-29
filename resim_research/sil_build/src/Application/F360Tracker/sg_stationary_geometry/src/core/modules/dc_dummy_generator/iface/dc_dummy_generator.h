#ifndef DC_DUMMY_GENERATOR_H
#define DC_DUMMY_GENERATOR_H

#include "dc_contour_storage.h"
#include "dc_fused_contour_storage.h"

namespace sg
{
   namespace dc
   {
      /**
       * @brief      Generate random number in a given interval
       * @param[in]  min_value - left end of interval
       * @param[in]  max_value - right end of interval
       *
       * @return     random float number in range [min_value; max_value]
       **/
      float random_float(const float min_value, const float max_value);

      /**
       * @brief      Generate random petrurbation
       *
       * @return     random float number in range [-0.1; 0.1] with two digits decimal precision
       **/
      float random_perturbation();

      /**
       * @brief    Fill DC contour storage with dummy data
       *
       * @param    dc_contours - contours container
       **/
      void fill_dc_contours_with_dummy_data(DCContourStorage &dc_contours);

      /**
       * @brief    Fill input detections with dummy data
       *
       * @param    input_detections - contours container
       **/
      void dc_fill_input_detections_with_dummy_data(SG_Input_Detections_T &iput_detections);

      /**
       * @brief    Fill fused contour storage with dummy data
       *
       * @param    fused_contours - fused contours container
       **/
      void fill_fused_contours_with_dummy_data(FusedContourStorage &fused_contours);
   }
}
#endif