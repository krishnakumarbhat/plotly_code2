/*===================================================================================*\
* FILE: dc_past_data.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file implements Past_Data_T used to store data needed for feature calculation.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef DC_PAST_DATA_H
#define DC_PAST_DATA_H

#include <limits>

#include "sg_reuse.h"

namespace sg
{
   namespace dc
   {
      struct Signals_T
      {
        private:
         float m_rcs{0.0F};
         float m_z_scs_abs_ewma025{0.0F};
         float m_z_scs_abs_recur{0.0F};

        public:
         explicit Signals_T() = default;
         explicit Signals_T(const float value);

         float get_rcs() const;
         void set_rcs(const float &rcs);
         float get_z_scs_abs_ewma025() const;
         void set_z_scs_abs_ewma025(const float &z_scs_abs_ewma025);
         float get_z_scs_abs_recur() const;
         void set_z_scs_abs_recur(const float &z_scs_abs_recur);
      };

      struct Signal_Extremes_T
      {
        private:
         float m_z_scs_abs{0.0F};

        public:
         explicit Signal_Extremes_T() = default;
         explicit Signal_Extremes_T(const float value);

         float get_z_scs_abs() const;
         void set_z_scs_abs(const float &z_scs_abs);
      };

      class Signals_Height_Bins_T
      {
        private:
         float m_dets_freq_bins_mean{0.0F};
         float m_low_bin_detections_sum{0.0F};
         float m_high_bin_detections_sum{0.0F};
         uint32_t m_denominator{0U};
         uint32_t m_age_last_update{0U};

        public:
         Signals_Height_Bins_T() = default;
         float get_dets_freq_bins_mean() const;
         void set_dets_freq_bins_mean(const float dets_freq_bins_mean);
         float get_low_bin_detections_sum() const;
         void set_low_bin_detections_sum(const float low_bin_detections_sum);
         float get_high_bin_detections_sum() const;
         void set_high_bin_detections_sum(const float high_bin_detections_sum);
         uint32_t get_denominator() const;
         void set_denominator(const uint32_t denominator);
         uint32_t get_age_last_update() const;
         void set_age_last_update(const uint32_t age_last_update);
      };

      class Past_Data_T
      {
        private:
         Signal_Extremes_T m_maxes{-std::numeric_limits<float>::infinity()};
         Signals_T m_means{0.0F};
         uint32_t m_detections_sum{0U};
         uint32_t m_age{0U};
         Signals_Height_Bins_T m_height_bins_under_nondr{};
         Signals_Height_Bins_T m_height_bins_over_nondr{};

        public:
         Past_Data_T() = default;
         const Signal_Extremes_T &get_maxes() const;
         void set_maxes(const Signal_Extremes_T &maxes);
         const Signals_T &get_means() const;
         void set_means(const Signals_T &means);
         uint32_t get_detections_sum() const;
         void add_to_detections_sum(const uint32_t detections_to_add);
         void set_detections_sum(const uint32_t detections_sum);
         uint32_t get_age() const;
         void increment_age();
         Signals_Height_Bins_T &get_height_bins_under_nondr();
         Signals_Height_Bins_T &get_height_bins_over_nondr();
      };
   }
}
#endif
