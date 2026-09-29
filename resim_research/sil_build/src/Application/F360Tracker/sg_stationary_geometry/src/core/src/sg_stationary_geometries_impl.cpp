#include "sg_stationary_geometries_impl.h"

#include <algorithm>

namespace sg
{
   Stationary_Geometries_Impl::Stationary_Geometries_Impl(TimerBase *const external_clock)
       : m_timing{external_clock}, m_core{m_timing}
   {
   }

   void Stationary_Geometries_Impl::initialize(const SG_Internals_Dump_T &sg_internals)
   {
      m_core.initialize(sg_internals);
   }

   void Stationary_Geometries_Impl::step(const uint64_t timestamp_us,
                                         const rot::F360_Detection_Log_Output_T &rot_detections,
                                         const rspp::RSPP_Detection_List_T &rspp_detections,
                                         const rspp::F360_Radar_Sensor_T (&sensors)[rspp::MAX_NUMBER_OF_SENSORS],
                                         const RSPP_Host_T &host)
   {
      const auto sg_start_time = m_timing.elapsed();

      m_safety_analyzer.clear_faults();

      const SG_Input_T grouped_input{timestamp_us, rot_detections, rspp_detections, sensors, host};
      m_safety_analyzer.diagnose(grouped_input);

      if (m_safety_analyzer.is_critical_fault_detected())
      {
         reset();
      }
      else
      {
         m_core.step(grouped_input);

         m_core.get_output(m_output);
         m_safety_analyzer.diagnose(m_output);

         m_core.get_reduced_output(m_reduced_output);
         m_safety_analyzer.diagnose(m_reduced_output);

         if (m_safety_analyzer.is_critical_fault_detected())
         {
            reset();
         }
      }

      m_timing.total = m_timing.elapsed() - sg_start_time;
   }

   void Stationary_Geometries_Impl::get_internals(SG_Internals_Dump_T &sg_internals) const
   {
      m_core.get_internals(sg_internals);
   }

   void Stationary_Geometries_Impl::get_timing(SG_Timing_Dump_T &sg_timing) const
   {
      this->dump_timing(sg_timing);
   }

   void Stationary_Geometries_Impl::get_output(SG_Output_T &sg_output) const
   {
      sg_output = m_output;
   }

   void Stationary_Geometries_Impl::get_reduced_output(SG_ReducedOutput_T &sg_output) const
   {
      sg_output = m_reduced_output;
   }

   void Stationary_Geometries_Impl::dump_timing(SG_Timing_Dump_T &timing_dump) const
   {
      timing_dump.total = m_timing.total;
      std::copy(m_timing.main_steps, &m_timing.main_steps[static_cast<uint8_t>(SG_AlgorithmStep_T::NUM_OF_ALGO_STEPS)],
                timing_dump.main_steps);
      std::copy(m_timing.details.dc_steps, &m_timing.details.dc_steps[static_cast<uint8_t>(DC_AlgorithmStep_T::NUM_OF_ALGO_STEPS)],
                timing_dump.details.dc_steps);
   }

   void Stationary_Geometries_Impl::reset()
   {
      m_core.reset();
      m_output         = SG_Output_T{};
      m_reduced_output = SG_ReducedOutput_T{};
   }
}
