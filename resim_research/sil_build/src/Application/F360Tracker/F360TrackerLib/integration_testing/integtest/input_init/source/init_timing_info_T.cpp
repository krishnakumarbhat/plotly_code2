#include "f360_constants.h"
#include "f360_reuse.h"
#include "f360_timing_info.h"
#include "init_timing_info_T.h"
namespace f360_variant_A
{
void init_timing_info_T(F360_TRKR_TIMING_INFO_T & timing_info)
{
   // modules
   timing_info.core_tracker=0.569581985;
   timing_info.sensor_preprocessing=0.000597953796;
   timing_info.time_update_tracks=0.569581985;
   timing_info.pre_association_track_management=1.43051147e-06;
   timing_info.detection_to_track_association=7.62939453e-06;
   timing_info.clustering=0.000350475311;
   timing_info.cluster_grouping=0.000607967377;
   timing_info.track_grouping=0.0;
   timing_info.measurement_update_tracks=0.0;
   timing_info.initialize_tracks=0.000404834747;
   timing_info.post_update_track_adjustments=4.00543213e-05;
   timing_info.track_classification=0.000133514404;
   timing_info.track_validity=3.67164612e-05;
   timing_info.track_downselection=3.33786011e-06;
   timing_info.e2e_protection=4.76837158e-07;
   timing_info.lsc_module =9.44137573e-05;
   timing_info.concrete_wall_detector = 9.44137573e-05;

   // sensor preprocessing
   timing_info.host_state_update=5.7220459e-06;
   timing_info.calc_obj_mov_stat_thresh=0.0;
   timing_info.update_sensor_valid_info=0.0;
   timing_info.configure_rdot_interval_compability=1.28746033e-05;
   timing_info.sensor_motion=8.10623169e-06;
   timing_info.update_det_hist=9.01222229e-05;
   timing_info.ego_motion_compensate_dets=2.67028809e-05;
   timing_info.detect_wheelspin=1.09672546e-05;
   timing_info.check_az_el_conf=1.90734863e-06;
   timing_info.mark_out_det_pairs=4.76837158e-07;
   timing_info.sensor_capability_module=0.000102519989;
   timing_info.double_bounce_detection_countermeasure=2.24113464e-05;

    // time update tracks
   timing_info.time_update_obj_trks_cca=0.0;
   timing_info.time_update_obj_trks_ctca=4.76837158e-07;

    // pre association track management
   timing_info.split_tracks=0.0;
   timing_info.kill_coasted_tracks=0.0;

   // association
   timing_info.assoc_dets_with_obj_trks=0.0;
   timing_info.assoc_countermeasure=0.0;
   timing_info.store_det_data=0.0;
   timing_info.det_downselect=0.0;
   timing_info.cluster_moving_detections=0.000109195709;
   timing_info.cluster_leftover_detections=0.000240325928;
   timing_info.m_initialize_clusters=0.0;
   timing_info.assoc_unconf_obj_trks=0.0;

   // existence probability 
   timing_info.predict_existence_probability=0.0;
   timing_info.update_existence_probability=1.90734863e-06;

   // pseudo estimations
   timing_info.pseudo_estimations=0.0;

   // track properties
   timing_info.obj_trk_status_bookkeeping=0.0;
   timing_info.adjust_fltr_type_dependent_params=4.76837158e-07;
   timing_info.obj_trk_properties=2.33650208e-05;
   timing_info.cancel_new_updated_trk_overlapping_confirmed_trks=2.86102295e-06;
   timing_info.adjust_overlapping_confirmed_trks=4.76837158e-07;
   timing_info.det_mean_var=0.0;
   timing_info.assign_underdrivability_status_to_tracks_ocg=0.000112533569;

   // downselect

   // static environment
   timing_info.mark_object_tracks_next_to_sensors=1.33514404e-05;

    // common
   timing_info.sanity_check=2.19345093e-05;

   // detailed timers
   timing_info.msmt_update_obj_trks_cca_non_moveable=0.0;
   timing_info.msmt_update_obj_trks_cca_moveable=0.0;
   timing_info.msmt_update_obj_trks_cca=0.0;
   timing_info.msmt_update_obj_trks_ctca=0.0;
   timing_info.assoc_unconf_obj_trks_sub_time_1=0.0;
   timing_info.assoc_unconf_obj_trks_sub_time_2=0.0;
   timing_info.assoc_unconf_obj_trks_sub_time_3=0.0;
   timing_info.assoc_unconf_obj_trks_sub_time_4=0.0;
   timing_info.assoc_unconf_obj_trks_sub_time_5=0.0;
   timing_info.assoc_unconf_obj_trks_sub_time_6=0.0;

}
}
