#include "sg_stationary_geometries.h"

#include <cassert>
#include <limits>

#include "sg_host_props.h"
#include "sg_stationary_geometries_impl.h"


sg::Stationary_Geometries::Stationary_Geometries(TimerBase *m_clock)
{
   static Stationary_Geometries_Impl implementation(m_clock);

   m_implementation = &implementation;
}

void sg::Stationary_Geometries::initialize(const SG_Internals_Dump_T &sg_internal)
{
   m_implementation->initialize(sg_internal);
}

void sg::Stationary_Geometries::step(const uint64_t timestamp_us,
                                     const rot::F360_Detection_Log_Output_T &rot_detections,
                                     const rspp::RSPP_Detection_List_T &rspp_detections,
                                     const rspp::F360_Radar_Sensor_T (&sensors)[rspp::MAX_NUMBER_OF_SENSORS],
                                     const RSPP_Host_T &host)
{
   m_implementation->step(timestamp_us, rot_detections, rspp_detections, sensors, host);
}


void sg::Stationary_Geometries::get_internals(SG_Internals_Dump_T &sg_internal)
{
   m_implementation->get_internals(sg_internal);
}

void sg::Stationary_Geometries::get_timing(SG_Timing_Dump_T &sg_timing)
{
   m_implementation->get_timing(sg_timing);
}

void sg::Stationary_Geometries::get_output(SG_Output_T &sg_output) const
{
   m_implementation->get_output(sg_output);
   sg_output.software_version = m_version;
}

void sg::Stationary_Geometries::get_reduced_output(SG_ReducedOutput_T &sg_output) const
{
   m_implementation->get_reduced_output(sg_output);
   sg_output.software_version = m_version;
}
