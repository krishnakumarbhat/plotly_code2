import uuid
from constants import Constants

class Config:
   def __init__(self):      
      self.FILE_VERSION: str = "v1.5.0"
      self.MAX_NUM_OF_SI_TO_PROCESS: int = 0
      self.DET_FILE_TITLE: str = "Detection KPIs and Plots"
      self.DS_FILE_TITLE: str = "DownSelection KPIs and Plots"
      self.ALIGN_FILE_TITLE: str = "Alignment KPIs and Plots"
      self.ID_FILE_TITLE: str = "ID KPIs and Plots"
      self.RC_FILE_TITLE: str = "RC KPIs and Plots"
      self.UNIQUE_KEY: str = str(uuid.uuid4())
      self.DET_FILE_SUFFIX: str = '_UDP_GEN7_DET_CORE.csv'
      self.DS_FILE_SUFFIX: str = '_UDP_GEN7_DOWN_SELECTION_STREAM.csv'
      self.RDD_FILE_SUFFIX: str = '_UDP_GEN7_RDD_CORE.csv'
      self.CDC_FILE_SUFFIX: str = '_UDP_CDC.csv'
      self.VSE_FILE_SUFFIX: str = '_UDP_GEN7_VSE_CORE.csv'
      self.ALIGN_FILE_SUFFIX: str = '_UDP_GEN7_DYNAMIC_ALIGNMENT_STREAM.csv'
      self.ID_FILE_SUFFIX: str = '_UDP_GEN7_ID_STREAM.csv'
      self.RC_FILE_SUFFIX: str = '_UDP_GEN7_RADAR_CAPABILITY_STREAM.csv'
      self.DET_DEBUG_FILE_NAME: str = 'detection_kpi_debug'
      self.DET_HTML_FILE_NAME: str = 'detection_kpi_report'
      self.DS_DEBUG_FILE_NAME: str = 'ds_kpi_debug'
      self.DS_HTML_FILE_NAME: str = 'ds_kpi_report'
      self.ALIGN_DEBUG_FILE_NAME: str = 'alignment_kpi_debug'
      self.ALIGN_HTML_FILE_NAME: str = 'alignment_kpi_report'
      self.ALIGN_ADDITIONAL_PLOTS: bool = False
      self.ID_DEBUG_FILE_NAME: str = 'id_kpi_debug'
      self.ID_HTML_FILE_NAME: str = 'id_kpi_report'
      self.RC_DEBUG_FILE_NAME: str = 'rc_kpi_debug'
      self.RC_HTML_FILE_NAME: str = 'rc_kpi_report'
      self.DEFAULT_ACC_THRESHOLD: float = 99.0
      self.MAX_LOGS_IN_ONE_REPORT: int = 20
      self.MAX_NUM_OF_AF_DETS_FRONT_RADAR: int = 768
      self.MAX_NUM_OF_AF_DETS_CORNER_RADAR: int = 680
      self.MAX_NUM_OF_DS_DETS_FRONT_RADAR: int = 128
      self.MAX_NUM_OF_DS_DETS_CORNER_RADAR: int = 64
      self.MAX_NUM_OF_RDD_DETS: int = 512
      self.RADAR_CYCLE_S: float = 0.05
      self.RAN_THRESHOLD: float = 0.01 + Constants.EPSILON
      self.VEL_THRESHOLD: float = 0.015 + Constants.EPSILON
      self.THETA_THRESHOLD: float = 0.00873 + Constants.EPSILON
      self.PHI_THRESHOLD: float = 0.00873 + Constants.EPSILON
      self.AZ_MISALIGNMENT_THRESHOLD: float = 0.01 + Constants.EPSILON
      self.EL_MISALIGNMENT_THRESHOLD: float = 0.01 + Constants.EPSILON
      self.ID_THRESHOLD: float = 0
      self.RC_STATUS_THRESHOLD: float = 0
      # Modified via meta data file, below is init value
      self.RESIM_MODE: str = "CDC"
      self.MAX_CDC_RECORDS: int = 5016
      self.RANGE_SATURATION_THRESHOLD_FRONT_RADAR: float = 135.0
      self.RANGE_SATURATION_THRESHOLD_CORNER_RADAR: float = 135.0
      # Modified based on sensor position, below is init value
      self.MAX_NUM_OF_AF_DETS: int = 768
      self.MAX_NUM_OF_DS_DETS: int = 128
      self.RANGE_SATURATION_THRESHOLD: float = 135.0
      
config = Config()