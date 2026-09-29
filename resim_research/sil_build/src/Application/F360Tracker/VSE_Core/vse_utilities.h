#ifndef VSE_UTILITIES_H
#define VSE_UTILITIES_H

#include "T360_Types.h"
#include "VSEOutputLog.h"
#include "VSEInternalDataLog.h"

#include "f360_host.h"
#include "f360_host_calib.h"

#include "f360_circular_buffer.h"

#include "VSE_Master_Model_L2.h"

#include "f360_radar_sensor.h"
#include "f360_radar_sensor_calib.h"
#include "f360_constants.h"

namespace vse_core
{
	typedef f360_variant_A::cmn::Circular_Buffer<VSE_OUT, VSE_OUTPUT_BUFFER_LENGTH> VSE_buffer;

	enum_quality_factor_T Map_QF_F360_VSE(const F360_QUALITY_FACTOR qf);
	F360_QUALITY_FACTOR Map_QF_VSE_F360(const enum_quality_factor_T qf);

	float Map_VCS_Sideslip(const float vse_out_sideslip);

	void Map_VSE_OUT_To_VSE_Output_Log(
		const VSE_OUT& r_VSE_Output,
		VSE_Output_Log_T& r_VSE_Output_log);

	void Map_VSE_Output_Log_To_VSE_OUT(
		const VSE_Output_Log_T& r_VSE_Output_log,
		VSE_OUT& r_VSE_Output);

	void F360_Map_VSE_OUT_to_Host_T(
		const VSE_OUT& r_VSE_Ouput,
		const float rear_cornering_compliance,
		f360_variant_A::F360_Host_T& r_host);

	uint64_t F360_Get_Middle_Sensor_Timestamp(const f360_variant_A::F360_Radar_Sensor_T(&r_sensors)[f360_variant_A::MAX_NUMBER_OF_SENSORS]);

	uint32_t F360_Get_Closest_VSE_Output_Index(const uint64_t timestamp_us, VSE_buffer& r_VSE_Output_buffer);
}
#endif
