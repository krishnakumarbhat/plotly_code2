#pragma once

#ifdef _WIN32
#include <direct.h> // for _mkdir
#endif
#ifdef __GNUC__
#include <sys/stat.h>
#endif
#include <math.h>
#include <vector>
#include <fstream>
#include <map>
#include <string>
#include "osi_sensordata.pb.h"
#include "GenericSMIface.h"
#include "SensorModelIface.h"
#include "DGPS_Eth_decoding.h"
#include "SMValidationInterfaceDefinition.h"
#include <vector>
#include <unordered_map>

class SMValidationIface : private GenericSMIface {
public:
	SMValidationIface();
	~SMValidationIface();

	void init(const SMValidationInputConfig_T* dgps_config);
	void run(const SMValidationRunConfig_T* dgps_run_config, SMValidationOutput_T* dgps_output_ptr);
	void reset();

	void setHostInfo(SMValidationHostVehicleInfo_T host);
	void setSensorView(std::string osiBuffer);
	void getRadarSM2Version(int& major, int& moinor, int& patch);
	int getDGPSScanIndex();
	void copyDGPSData(DGPS_Data_T* dgps_data_out) const;

private:
	bool loadSensorDlls();
	void setSVVersion();
	void setSVTimestamp(SMValidationTimestamp_T timestamp);
	void setSVSensorID(std::string sensor_str);
	void setSVMountingPosition(int radar_index);
	void setSVHostVehicleData();
	void setSVHostVehicleID();
	void setSVGlobalGroundTruth();
	void setGTMovingObject();
	void setGTStationaryObject();
	void setGTLaneBoundary();
	void setGTTrafficSign();
	void setRadarSM2PositionInfo(const SMValidationSensorInfo_T sensorinfo[]);
	void setSensorType(const SMValidationSensorInfo_T sensorinfo[]);
	void setSM2ConfigPath(const std::string sm_path);
	void setOutputpath(const std::string path);
	void setSM2BinaryPath(const std::string sm2_binary_path);
	void setCustomer(const SMValidationCustomer cust);
	void setNumberOfSensors(const uint8_t num_of_sensors);
	void setBusType(const DGPSBusType bus_type);
	void ClearAlignData();
	void CheckInputConfigHeader(SMValidationInputConfigHeader_T header);
	void OpenOSITextFile(void);
	void findHost(const osi3::SensorView& current_in, osi3::MovingObject& host_);
	void opendebugfiles();
	void fillSensorView(SMValidationTimestamp_T timestamp);
	void fillOutputInterfaceData(SMValidationOutput_T* dgps_output_ptr);
	void writeSVOsiTracefile(const osi3::SensorView& l_sensor_view, int radar_index);
	void writeSDOsiTracefile();
	void matplot_osi_groundtruth(const osi3::SensorView& sensor_view);
	osi3::Vector3d aptivVCSPosnToOSIWorldFrame(osi3::Vector3d aptiv_vcs_posn);
	osi3::Vector3d aptivVCSVelToOSIWorldFrame(osi3::Vector3d aptiv_vcs_vel);
	osi3::Vector3d aptivVCSAccToOSIWorldFrame(osi3::Vector3d aptiv_vcs_acc);
	osi3::Orientation3d aptivVCSOrientToOSIWorldFrame(osi3::Orientation3d aptiv_vcs_orient);
	osi3::Orientation3d aptivVCSOrientRateToOSIWorldFrame(osi3::Orientation3d aptiv_vcs_orient_rate);
	void GenerateConfigFiles();
	

#if 1
	std::map<std::string, int> radar_pos_str_to_index = {
			{ "FRONT_CENTER",SMVALIDATION_FRONT_CENTER},
			{ "FRONT_LEFT", SMVALIDATION_FRONT_LEFT},
			{ "FRONT_RIGHT", SMVALIDATION_FRONT_RIGHT },
			{ "CENTER_LEFT", SMVALIDATION_CENTER_LEFT },
			{ "CENTER_RIGHT", SMVALIDATION_CENTER_RIGHT },
			{ "REAR_CENTER", SMVALIDATION_REAR_CENTER },
			{ "REAR_LEFT", SMVALIDATION_REAR_LEFT },
			{ "REAR_RIGHT", SMVALIDATION_REAR_RIGHT }
	};
#endif
	std::map<int, std::string> radar_pos_to_str = {
			{ SMVALIDATION_FRONT_CENTER, "FRONT_CENTER" },
			{ SMVALIDATION_FRONT_LEFT, "FRONT_LEFT"},
			{ SMVALIDATION_FRONT_RIGHT, "FRONT_RIGHT" },
			{ SMVALIDATION_CENTER_LEFT, "CENTER_LEFT" },
			{ SMVALIDATION_CENTER_RIGHT, "CENTER_RIGHT" },
			{ SMVALIDATION_REAR_CENTER, "REAR_CENTER" },
			{ SMVALIDATION_REAR_LEFT, "REAR_LEFT" },
			{ SMVALIDATION_REAR_RIGHT, "REAR_RIGHT" }
	};

	std::vector<SMValidationMountingPose_T> radar_mounting_position = {
		/*positionX, positiony, positionZ, OrientationRoll, OrientationPitch, OrientationYaw,*/
		{2.012, 0.0, -0.182, 0.0, 0.0, 0.0},
		{2.012, 0.86, -0.182, 3.141593, 0.0, 1.0472},
		{2.012, -0.86, -0.192, 0.0, 0.0, -1.0472},
		{0.0, 1.0, -0.25, 3.141593, 0.0, 1.570796},
		{0.0, -1.0, -0.25, 0.0, 0.0, -1.570796},
		{-2.085, 0.0, -0.12, 0.0, 0.0, 3.141593},
		{-2.085, 0.818, -0.12, 3.141593, 0.0, 2.094395},
		{-2.085, -0.818, -0.12, 0.0, 0.0, -2.094395}
	};

	std::vector<int> radar_sensortype = { 
		SMVALIDATION_SENSOR_TYPE_SRR5_PLUS,
		SMVALIDATION_SENSOR_TYPE_SRR5_PLUS,
		SMVALIDATION_SENSOR_TYPE_SRR5_PLUS,
		SMVALIDATION_SENSOR_TYPE_SRR5_PLUS,
		SMVALIDATION_SENSOR_TYPE_SRR5_PLUS,
		SMVALIDATION_SENSOR_TYPE_SRR5_PLUS,
		SMVALIDATION_SENSOR_TYPE_SRR5_PLUS,
		SMVALIDATION_SENSOR_TYPE_SRR5_PLUS
	};

	double Base_Latitude;
	double Base_Longitude;
	double Base_Orientation;
	double matplot_Number_of_DGPS_Objects;
	int scan_index;
	boolean_t first_call;
	boolean_t if_stationary;
	boolean_t sensormodelrun;
	uint8_t number_of_sensors;
	std::string config_folder_path;
	std::string dll_name;

	std::vector<std::ofstream> dump_sv_osi_trace_file;
	std::ofstream dump_sd_trace_file;
	std::string Outputpath;
	std::string serialised_sensor_view;
	std::string sm2_binary_path;
	DGPS_Data_T DGPS_Data;
	DGPSBusType bus_select; 
	DGPSEthernetDecoder dgps_ethernet;
	//DGPS_Can_class dgps_can;
	SM2SensorMessage pSensorView;
	osi3::SensorView sensor_view;
	osi3::SensorView* p_sensor_view;
	osi3::SensorData sensor_data;
	std::vector<std::string> sensor_data_str_vector;
	std::vector<std::string> rad_pos_str;
	SMValidationHostVehicleInfo_T host_info;
	SMValidationCustomer customer;
};



