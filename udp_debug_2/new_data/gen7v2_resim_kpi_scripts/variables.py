# Session variables are initialized only once
class Session_Vars:
   def __init__(self):
      self.html_header = ""
      self.html_content = ""
      self.output_folder = ""
      self.is_cdc_mode = False
      self.script_start_timestamp = ""
      
# Log variables need to be reset for every log
class Log_Vars:
   def __init__(self):
      self._initialize()
      
   def reset(self):
      self._initialize()
      
   def _initialize(self):
      self.num_of_SI_in_veh_af = 0
      self.num_of_SI_in_sim_af = 0
      self.num_of_SI_in_veh_ds = 0
      self.num_of_SI_in_sim_ds = 0
      self.num_of_SI_in_veh_rdd = 0
      self.num_of_SI_in_sim_rdd = 0
      self.num_of_SI_in_veh_cdc = 0
      self.num_of_SI_in_sim_cdc = 0
      self.num_of_SI_in_veh_merged = 0
      self.num_of_SI_in_sim_merged = 0
      self.num_of_SI_in_veh_vse = 0
      self.num_of_SI_in_sim_vse = 0
      self.num_of_SI_in_veh_and_sim_af = 0
      self.num_of_SI_in_veh_and_sim_ds = 0
      self.num_of_SI_in_veh_and_sim_rdd = 0
      self.num_of_SI_in_veh_and_sim_merged = 0
      self.num_of_SI_in_veh_and_sim_vse = 0
      self.num_of_SI_with_same_num_of_dets_af = 0
      self.num_of_SI_with_same_num_of_dets_ds = 0
      self.num_of_SI_with_same_num_of_dets_rdd = 0
      self.ran_diff_list = []
      self.vel_diff_list = []
      self.theta_diff_list = []
      self.phi_diff_list = []
      self.snr_diff_list = []
      self.rcs_diff_list = []
      self.scan_index_list = []
      self.accuracy_list = []
      self.num_af_det_veh_list = []
      self.num_af_det_sim_list = []
      self.num_ds_det_veh_list = []
      self.num_ds_det_sim_list = []
      self.max_range_veh_list = []
      self.max_range_sim_list = []
      self.saturated_list = []
      self.min_accuracy = 0.0
      self.overall_accuracy = 0.0
      self.overall_accuracy_excluding_cdc_saturation = 0.0
      self.cdc_data_available = False
      self.vse_data_available = False
      self.perc_of_scans_with_cdc_saturation = 0.0
      self.perc_of_scans_with_range_saturation = 0.0
      self.dist_travelled_by_veh = 0.0
      self.dist_travelled_by_sim = 0.0
      