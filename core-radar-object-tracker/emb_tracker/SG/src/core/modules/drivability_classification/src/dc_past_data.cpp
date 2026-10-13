/*===================================================================================*\
* FILE: dc_past_data.cpp
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

#include "dc_past_data.h"

#include "sg_variant.h"

namespace sg
{
namespace dc
{
Signals_T::Signals_T(const float value) : m_rcs(value), m_z_scs_abs_ewma025(value), m_z_scs_abs_recur(value)
{
}

float Signals_T::get_rcs() const
{
   return m_rcs;
}

void Signals_T::set_rcs(const float &rcs)
{
   m_rcs = rcs;
}

float Signals_T::get_z_scs_abs_ewma025() const
{
   return m_z_scs_abs_ewma025;
}

void Signals_T::set_z_scs_abs_ewma025(const float &z_scs_abs_ewma025)
{
   m_z_scs_abs_ewma025 = z_scs_abs_ewma025;
}

float Signals_T::get_z_scs_abs_recur() const
{
   return m_z_scs_abs_recur;
}

void Signals_T::set_z_scs_abs_recur(const float &z_scs_abs_recur)
{
   m_z_scs_abs_recur = z_scs_abs_recur;
}

float Signals_T::get_snr() const
{
   return m_snr;
}

void Signals_T::set_snr(const float snr)
{
   m_snr = snr;
}

float Signals_T::get_snr_recur() const
{
   return m_snr_recur;
}

void Signals_T::set_snr_recur(const float snr_recur)
{
   m_snr_recur = snr_recur;
}

float Signals_T::get_z_scs_recur() const
{
   return m_z_scs_recur;
}

void Signals_T::set_z_scs_recur(const float z_scs_recur)
{
   m_z_scs_recur = z_scs_recur;
}

float Signals_T::get_z_scs() const
{
   return m_z_scs;
}

void Signals_T::set_z_scs(const float z_scs)
{
   m_z_scs = z_scs;
}

Signal_Extremes_T::Signal_Extremes_T(const float value) : m_z_scs_abs(value)
{
}

float Signal_Extremes_T::get_z_scs_abs() const
{
   return m_z_scs_abs;
}

void Signal_Extremes_T::set_z_scs_abs(const float &z_scs_abs)
{
   m_z_scs_abs = z_scs_abs;
}

float Signals_Height_Bins_T::get_dets_freq_bins_mean() const
{
   return m_dets_freq_bins_mean;
}

void Signals_Height_Bins_T::set_dets_freq_bins_mean(const float dets_freq_bins_mean)
{
   m_dets_freq_bins_mean = dets_freq_bins_mean;
}

float Signals_Height_Bins_T::get_low_bin_detections_sum() const
{
   return m_low_bin_detections_sum;
}

void Signals_Height_Bins_T::set_low_bin_detections_sum(const float low_bin_detections_sum)
{
   m_low_bin_detections_sum = low_bin_detections_sum;
}

float Signals_Height_Bins_T::get_high_bin_detections_sum() const
{
   return m_high_bin_detections_sum;
}

void Signals_Height_Bins_T::set_high_bin_detections_sum(const float high_bin_detections_sum)
{
   m_high_bin_detections_sum = high_bin_detections_sum;
}

uint32_t Signals_Height_Bins_T::get_denominator() const
{
   return m_denominator;
}

void Signals_Height_Bins_T::set_denominator(const uint32_t denominator)
{
   m_denominator = denominator;
}

uint32_t Signals_Height_Bins_T::get_age_last_update() const
{
   return m_age_last_update;
}

void Signals_Height_Bins_T::set_age_last_update(const uint32_t age_last_update)
{
   m_age_last_update = age_last_update;
}

const Signal_Extremes_T &Past_Data_T::get_maxes() const
{
   return m_maxes;
}

void Past_Data_T::set_maxes(const Signal_Extremes_T &maxes)
{
   m_maxes = maxes;
}

const Signals_T &Past_Data_T::get_means() const
{
   return m_means;
}

void Past_Data_T::set_means(const Signals_T &means)
{
   m_means = means;
}

uint32_t Past_Data_T::get_detections_sum() const
{
   return m_detections_sum;
}

void Past_Data_T::add_to_detections_sum(const uint32_t detections_to_add)
{
   if ((DC_PAST_DATA_MAX_DETECTIONS_SUM - m_detections_sum) < detections_to_add)
   {
      m_detections_sum = DC_PAST_DATA_MAX_DETECTIONS_SUM;
   }
   else
   {
      m_detections_sum = m_detections_sum + detections_to_add;
   }
}

void Past_Data_T::set_detections_sum(const uint32_t detections_sum)
{
   if (detections_sum > DC_PAST_DATA_MAX_DETECTIONS_SUM)
   {
      m_detections_sum = DC_PAST_DATA_MAX_DETECTIONS_SUM;
   }
   else
   {
      m_detections_sum = detections_sum;
   }
}

uint32_t Past_Data_T::get_age() const
{
   return m_age;
}

void Past_Data_T::increment_age()
{
   if (m_age < DC_PAST_DATA_MAX_AGE)
   {
      m_age++;
   }
}

Signals_Height_Bins_T &Past_Data_T::get_height_bins_under_nondr()
{
   return m_height_bins_under_nondr;
}

Signals_Height_Bins_T &Past_Data_T::get_height_bins_over_nondr()
{
   return m_height_bins_over_nondr;
}

float Past_Data_T::get_z_scs_std_sum() const
{
   return m_z_scs_std_sum;
}

void Past_Data_T::set_z_scs_std_sum(const float z_scs_std_sum)
{
   m_z_scs_std_sum = z_scs_std_sum;
}

float Past_Data_T::get_z_scs_abs_std_sum() const
{
   return m_z_scs_abs_std_sum;
}

void Past_Data_T::set_z_scs_abs_std_sum(const float z_scs_abs_std_sum)
{
   m_z_scs_abs_std_sum = z_scs_abs_std_sum;
}

float Past_Data_T::get_z_scs_std_squared_sum() const
{
   return m_z_scs_std_squared_sum;
}

void Past_Data_T::set_z_scs_std_squared_sum(const float z_scs_std_squared_sum)
{
   m_z_scs_std_squared_sum = z_scs_std_squared_sum;
}
} // namespace dc
} // namespace sg
