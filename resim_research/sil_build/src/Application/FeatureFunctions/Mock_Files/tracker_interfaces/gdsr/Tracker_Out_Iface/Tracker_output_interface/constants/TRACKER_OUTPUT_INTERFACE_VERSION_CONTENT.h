#ifndef TRACKER_OUTPUT_INTERFACE_CONTENT_H
#define TRACKER_OUTPUT_INTERFACE_CONTENT_H

/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/

/*********** List of solved jira tickets ************************************/ 
#define AYF
#define AYF_6 /*Create tracker output api project*/
#define AYF_7 /*Do not set FAST_MATH_TABLES in tracker*/
#define AYF_8 /*Only show api targets in MSVS if TRACKER_DEVELOPER_MODE*/
#define AYF_10 /*Update api files to latest tracker*/
#define AYF_11 /*Create interface for detection users*/
#define AYF_12 /*rename f_redevided into f_redivided*/
#define AYF_13 /*Clean up file name and folder structure*/
#define AYF_15 /*Add TRACKER_OUTPUT_T.h to an api*/
#define AYF_16 /*Make RADAR_PARAMETER_BUFFER_SIZE dependant on DETECTION_BUFFER_SIZ*/
#define AYF_17 /*integrate ABX-694*/
#define AYF_18 /*JLR - decrease number of objects to 48*/
#define AYF_19 /*Clean up includes*/
#define AYF_20 /*Improve comments for innovation variance and accuracy*/
#define AYF_21 /*move heading into Space_Time_Derivative_Vector_2d_T struct*/
#define AYF_24 /*Adding Renault customer*/
#define AYF_25 /*Add covariances to variance structure*/
#define AYF_26 /*Remove tracker internal data from DETECTION_FLT_T*/
#define AYF_27 /*Modification of the Renault customer to RNA*/
#define AYF_28 /*Fix includes of Reuse.h => reuse.h*/
#define AYF_29 /*remove neighbor_sharing_priority from tracker_output*/
#define AYF_32 /*Add documenting comment to track status ENUM*/
#define AYF_33 /*Add size to variance structure*/
#define AYF_34 /*introduce detections flag - valid_level_bit_mixer_bias*/
#define AYF_35 /*distinct_id needs to have type uint8_t*/
#define AYF_36 /*Only use types allowed by coding guidelines*/
#define AYF_37 /*Remove speed_mode_T and tracker_mode_T from VEHIUCLE_DATA_FLT_T*/
#define AYF_40 /*Do not define FIXED_POINT_MATH within tracker*/
#define AYF_41 /*Remove definition of MAX_CLOUD_DETS*/
#define AYF_42 /*add moving state ambiguous stationary*/
#define AYF_44 /*update TRACKER_ERRORS_T description*/
#define AYF_45 /*Provide f_behind_guardrail*/
#define AYF_47 /*Rolling count and tracker configuration */
#define AYF_48 /*Add error flags for unset alignment and v_un*/
#define AYF_49 /*Add name to enumerations*/
#define AYF_50 /*Macro to disable id field in tracker output*/
#define AYF_51 /*SIL_Customer_Specific needs TRACKER_OUTPUT*/
#define AYF_52 /*Setting max number detection for DB scan algortithm to 1*/
#define AYF_55 /*add origin sensor to tracker output*/
#define AYF_56 /*DETECTION_FLT_T store association as index*/
#define AYF_58 /*remove not-existing requirement reference from VEHICLE_DATA_FLT_T*/
#define AYF_59 /*remove definition of TRACKS_PER_OBJECT in all customer constants files*/
#define AYF_60 /*Append information for documentation of f_just_merged_with*/
#define AYF_61 /*Rework input internal output separation in tracker structures*/
#define AYF_62 /*DETECTION_FLT_T: adjust comments of f_multi_bounce/_dealiased*/
#define AYF_63 /*Add errors for additional host vehicle member checks*/
#define AYF_65 /*Refactor Tracker Constants*/
#define AYF_66 /*Remove MAX_WS_DETS*/
#define AYF_67 /*Add f_on_trailer_box flag to the detection output structure*/
#define AYF_68 /*Add f_intersects_guardrail to object output*/
#define AYF_69 /*Fix doxygen in LINE_HESSE header*/
#define AYF_70 /*Refactor error output handling*/
#define AYF_71 /*Revert single error output*/
#define AYF_72 /*clean up some requirements*/
#define AYF_73 /*Add Tracker constants file for BMW SRR5*/
#define AYF_78 /*rawInputProcessing does not filter host acceleration*/
#define AYF_80 /*reduce DBSCAN_MAX_NO_CLUSTER*/
#define AYF_81 /*Extend guardrail output to contain age*/
#define AYF_82 /*remove DBSCAN_MAX_NO_CLUSTER*/
#define AYF_85 /*Add error flag detection_with_range_below_min_present*/
#define AYF_86 /*Adding azimuth confidence to detection structure DETECTION_FLT_T*/
#define AYF_90 /*re-introduce typo for compatibility reasons*/
#define AYF_91 /*remove cta_trigger_count */
#define AYF_92 /*Remove station_keeping_object_info from vehicle data*/
#define AYF_97 /*add signal to vehicle data struct: wheel direction*/
#define AYF_98 /*Do not call AS_add_header*/
#define AYF_99 /*Support GDSRTracker_PROJECT_VARIANT*/
#define AYF_100 /*Doxygen for mounting location enumeration*/
#define AYF_101 /*Doxygen for output iterator*/
#define AYF_102 /*Add existence probability to guardrail output*/
#define AYF_103 /*Decrease DETECTION_BUFFER_SIZE to 3*/
#define AYF_106 /*Requirements for DETECTION_FLT_T*/
#define AYF_107 /*Write SRD entries for mandatory GDSR Tracker input signals*/
#define AYF_108 /*cmake: Allow including project to set IDE folders*/
#define AYF_109 /*cmake do not use PUBLIC when adding sources*/
#define AYF_110 /*Fix doxygen for Tracker_api_vehicle_output*/
#define AYF_111 /*SRD Linkage for host_wheel_direction*/
#define AYF_112 /*Remove LINE_HESSE_T from tracker api*/
#define AYF_113 /*Reduce number of objects from 64 to 32 for BMW SRR5 Low/B-pillar*/
#define AYF_114 /*change NUMBER_OF_DETECTIONS to 128 for Geely CX11 A20*/
#define AYF_115 /*Solve QAC warnings*/
#define AYF_116 /*Remove single_sensor_fusion_fov_lines from RADAR_PARAMS_FLT_T*/
#define AYF_118 /*Remove unused variants from tracker_constants*/
#define AYF_119 /*Offer harmonized tracker interface copy*/
#define AYF_120 /*add a constant for stationary bounce algo*/
#define AYF_121 /*Tracker API changes for concrete guardrail detection implementation*/
#define AYF_122 /*update Geely SRR5 project variants*/
#define AYF_125 /*rangeRegionObstructed_probability is no longer computed*/
#define AYF_126 /*Suppress QAC warning for inverse include guard in tracker_constants_default.h*/
#define AYF_128 /*Remove outdated requirement linkage*/
#define AYF_129 /*investigate potential reduction of the cyclomatic complexity*/
#define AYF_130 /*Add guardrail-det-buffer-size SDD doxygen link*/
#define AYF_131 /*add new project variant for ecu*/
#define AYF_132 /*remove vcs velocities from Tracker input*/
#define AYF_133 /*Wrong implementation of f_updated flag from tracker output*/
#define AYF_134 /*bugfix and refactor the detection exclusion zones - Tracker API changes*/
#define AYF_135 /*Remove TRACKER_OUTPUT_INTERFACE_VERSION_DATE.c*/
#define AYF_136 /*Solve MSVC warning C4121: alignment of a member was sensitive to packing*/
#define AYF_139 /*add f_super_resolution flag to detection structure*/
#define AYF_140 /*add Detection properties for additional flags*/
#define AYF_142 /*Prepare to remove conf_overall*/
#define AYF_143 /*Delete updateKalmanCurvature*/
#define AYF_144 /*Remove Tracker_Output_Iterator*/
#define AYF_145 /*add error flag for inconsistency between and rear axle position vs. ego length*/
#define AYF_146 /*remove timestamp from detection struct*/
#define AYF_147 /*Add standard deviations and unambiguous range rate interval*/
#define AYF_148 /*Remove purely internal structures from the output*/
#define AYF_149 /*add a unit test variant*/
#define AYF_151 /*Remove standard heading members from RADAR_PARAMS_FLT_T*/
#define AYF_152 /*Document range_coverage member in RADAR_PARAMS_FLT_T*/
#define AYF_153 /*Remove arm_throughput from RADAR_PARAMS_FLT_T*/
#define AYF_154 /*Deprecate OTHER_SENSOR_RECEIVED_DATA_FLT_T*/
#define AYF_155 /*add new tracker_errors for incorrect setting of min/max_range_rate*/
#define AYF_156 /*add new signal noise_degradation_level to the radar params*/
#define AYF_157 /*add velocity variance as member to radar parameter structure*/
#define AYF_158 /*[BMW] tracker constans change for concrete guardrail*/
#define AYF_161 /*Add Tracker constants file for Nissan SRR6*/
#define ABX_3349 /*Modify SAD containing separated lists of Input/Output Interfaces*/
/*********** End of list of solved jira tickets *****************************/ 

#endif
