import configparser
from xml.etree.ElementTree import Element

config_data = {}

def readconfig():
    RNconfig = configparser.ConfigParser()
    RNconfig.read('VV_Config_File.ini')

    config_data['Sprint'] = RNconfig.get('VVReleaseNotes', 'Sprint')
    config_data['VV_Version'] = RNconfig.get('VVReleaseNotes', 'VV_Version')
    config_data['Release_Date'] = RNconfig.get('VVReleaseNotes', 'Release_Date')
    config_data['VV_Release_CS'] = RNconfig.get('VVReleaseNotes', 'VV_Release_CS')
    
    config_data['SIL_ENG_SW_Version'] = RNconfig.get('SIL_ENGINE_Details', 'SIL_ENG_SW_Version')
    config_data['SIL_ENG_Date'] = RNconfig.get('SIL_ENGINE_Details', 'SIL_ENG_Date')
    config_data['SIL_ENG_CS'] = RNconfig.get('SIL_ENGINE_Details', 'SIL_ENG_CS')
    config_data['SIL_ENG_Repo'] = RNconfig.get('SIL_ENGINE_Details', 'SIL_ENG_Repo')
        
    config_data['LM2_SW_Version'] = RNconfig.get('LM2_Details', 'LM2_SW_Version')
    config_data['LM2_Date'] = RNconfig.get('LM2_Details', 'LM2_Date')
    config_data['LM2_CS'] = RNconfig.get('LM2_Details', 'LM2_CS')
    config_data['LM2_Repo'] = RNconfig.get('LM2_Details', 'LM2_Repo')
    
    config_data['SM2_SW_Version'] = RNconfig.get('SM2_Details', 'SM2_SW_Version')
    config_data['SM2_Date'] = RNconfig.get('SM2_Details', 'SM2_Date')
    config_data['SM2_CS'] = RNconfig.get('SM2_Details', 'SM2_CS')
    config_data['SM2_Repo'] = RNconfig.get('SM2_Details', 'SM2_Repo')
      
    config_data['VV_ENG_SW_Version'] = RNconfig.get('VV_ENGINE_Details', 'VV_ENG_SW_Version')
    config_data['VV_ENG_Date'] = RNconfig.get('VV_ENGINE_Details', 'VV_ENG_Date')
    config_data['VV_ENG_CS'] = RNconfig.get('VV_ENGINE_Details', 'VV_ENG_CS')
    config_data['VV_ENG_Repo'] = RNconfig.get('VV_ENGINE_Details', 'VV_ENG_Repo')
      
    config_data['Emb_Lib_BMW_SP21_LOW_SW_Version'] = RNconfig.get('Sensor_Library_Info',
                                                                  "Emb_Lib_BMW_SP21_LOW_SW_Version")
    config_data['Emb_Lib_BMW_SP21_LOW_Date'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_BMW_SP21_LOW_Date')
    config_data['Emb_Lib_BMW_SP21_LOW_CS'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_BMW_SP21_LOW_CS')
    config_data['Emb_Lib_BMW_SP21_LOW_Linux'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_BMW_SP21_LOW_Linux')
    config_data['Emb_Lib_BMW_SP21_LOW_Windows'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_BMW_SP21_LOW_Windows')
    config_data['Emb_Lib_BMW_SP21_LOW_Repo'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_BMW_SP21_LOW_Repo')
        
    config_data['Emb_Lib_BMW_SP21_MID_SW_Version'] = RNconfig.get('Sensor_Library_Info',
                                                                  'Emb_Lib_BMW_SP21_MID_SW_Version')
    config_data['Emb_Lib_BMW_SP21_MID_Date'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_BMW_SP21_MID_Date')
    config_data['Emb_Lib_BMW_SP21_MID_CS'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_BMW_SP21_MID_CS')
    config_data['Emb_Lib_BMW_SP21_MID_Linux'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_BMW_SP21_MID_Linux')
    config_data['Emb_Lib_BMW_SP21_MID_Windows'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_BMW_SP21_MID_Windows')
    config_data['Emb_Lib_BMW_SP21_MID_Repo'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_BMW_SP21_MID_Repo')
        
    config_data['Emb_Lib_BMW_SP21_HIGH_SW_Version'] = RNconfig.get('Sensor_Library_Info',
                                                                   'Emb_Lib_BMW_SP21_HIGH_SW_Version')
    config_data['Emb_Lib_BMW_SP21_HIGH_Date'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_BMW_SP21_HIGH_Date')
    config_data['Emb_Lib_BMW_SP21_HIGH_CS'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_BMW_SP21_HIGH_CS')
    config_data['Emb_Lib_BMW_SP21_HIGH_Linux'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_BMW_SP21_HIGH_Linux')
    config_data['Emb_Lib_BMW_SP21_HIGH_Windows'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_BMW_SP21_HIGH_Windows')
    config_data['Emb_Lib_BMW_SP21_HIGH_Repo'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_BMW_SP21_HIGH_Repo')
        
    config_data['Emb_Lib_BMW_SP25_L2_SW_Version'] = RNconfig.get('Sensor_Library_Info',
                                                                 'Emb_Lib_BMW_SP25_L2_SW_Version')
    config_data['Emb_Lib_BMW_SP25_L2_Date'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_BMW_SP25_L2_Date')
    config_data['Emb_Lib_BMW_SP25_L2_CS'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_BMW_SP25_L2_CS')
    config_data['Emb_Lib_BMW_SP25_L2_Linux'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_BMW_SP25_L2_Linux')
    config_data['Emb_Lib_BMW_SP25_L2_Windows'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_BMW_SP25_L2_Windows')
    config_data['Emb_Lib_BMW_SP25_L2_Repo'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_BMW_SP25_L2_Repo')
    
    config_data['Emb_Lib_BMW_SP25_L3_SW_Version'] = RNconfig.get('Sensor_Library_Info',
                                                                 'Emb_Lib_BMW_SP25_L3_SW_Version')
    config_data['Emb_Lib_BMW_SP25_L3_Date'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_BMW_SP25_L3_Date')
    config_data['Emb_Lib_BMW_SP25_L3_CS'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_BMW_SP25_L3_CS')
    config_data['Emb_Lib_BMW_SP25_L3_Linux'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_BMW_SP25_L3_Linux')
    config_data['Emb_Lib_BMW_SP25_L3_Windows'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_BMW_SP25_L3_Windows')
    config_data['Emb_Lib_BMW_SP25_L3_Repo'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_BMW_SP25_L3_Repo')
     
    config_data['Emb_Lib_STLA_SCALE1_SW_Version'] = RNconfig.get('Sensor_Library_Info',
                                                                 'Emb_Lib_STLA_SCALE1_SW_Version')
    config_data['Emb_Lib_STLA_SCALE1_Date'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_STLA_SCALE1_Date')
    config_data['Emb_Lib_STLA_SCALE1_CS'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_STLA_SCALE1_CS')
    config_data['Emb_Lib_STLA_SCALE1_Linux'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_STLA_SCALE1_Linux')
    config_data['Emb_Lib_STLA_SCALE1_Windows'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_STLA_SCALE1_Windows')
    config_data['Emb_Lib_STLA_SCALE1_Repo'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_STLA_SCALE1_Repo')
        
    config_data['Emb_Lib_STLA_SCALE3_SW_Version'] = RNconfig.get('Sensor_Library_Info',
                                                                 'Emb_Lib_STLA_SCALE3_SW_Version')
    config_data['Emb_Lib_STLA_SCALE3_Date'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_STLA_SCALE3_Date')
    config_data['Emb_Lib_STLA_SCALE3_CS'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_STLA_SCALE3_CS')
    config_data['Emb_Lib_STLA_SCALE3_Linux'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_STLA_SCALE3_Linux')
    config_data['Emb_Lib_STLA_SCALE3_Windows'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_STLA_SCALE3_Windows')
    config_data['Emb_Lib_STLA_SCALE3_Repo'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_STLA_SCALE3_Repo')
        
    config_data['Emb_Lib_STLA_SCALE4_SW_Version'] = RNconfig.get('Sensor_Library_Info',
                                                                 'Emb_Lib_STLA_SCALE4_SW_Version')
    config_data['Emb_Lib_STLA_SCALE4_Date'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_STLA_SCALE4_Date')
    config_data['Emb_Lib_STLA_SCALE4_CS'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_STLA_SCALE4_CS')
    config_data['Emb_Lib_STLA_SCALE4_Linux'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_STLA_SCALE4_Linux')
    config_data['Emb_Lib_STLA_SCALE4_Windows'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_STLA_SCALE4_Windows')
    config_data['Emb_Lib_STLA_SCALE4_Repo'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_STLA_SCALE4_Repo')
    
    config_data['Emb_Lib_HONDA_SW_Version'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_HONDA_SW_Version')
    config_data['Emb_Lib_HONDA_Date'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_HONDA_Date')
    config_data['Emb_Lib_HONDA_CS'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_HONDA_CS')
    config_data['Emb_Lib_HONDA_Linux'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_HONDA_Linux')
    config_data['Emb_Lib_HONDA_Windows'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_HONDA_Windows')
    config_data['Emb_Lib_HONDA_Repo'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_HONDA_Repo')
    
    config_data['Emb_Lib_TRATON_SW_Version'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_TRATON_SW_Version')
    config_data['Emb_Lib_TRATON_Date'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_TRATON_Date')
    config_data['Emb_Lib_TRATON_CommitID'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_TRATON_CommitID')
    config_data['Emb_Lib_TRATON_Linux'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_TRATON_Linux')
    config_data['Emb_Lib_TRATON_Windows'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_TRATON_Windows')
    config_data['Emb_Lib_TRATON_Repo'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_TRATON_Repo')
    
    config_data['Emb_Lib_GPO_GEN7_V1_SW_Version'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_GPO_GEN7_V1_SW_Version')
    config_data['Emb_Lib_GPO_GEN7_V1_Date'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_GPO_GEN7_V1_Date')
    config_data['Emb_Lib_GPO_GEN7_V1_CommitID'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_GPO_GEN7_V1_CommitID')
    config_data['Emb_Lib_GPO_GEN7_V1_Linux'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_GPO_GEN7_V1_Linux')
    config_data['Emb_Lib_GPO_GEN7_V1_Windows'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_GPO_GEN7_V1_Windows')
    config_data['Emb_Lib_GPO_GEN7_V1_Repo'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_GPO_GEN7_V1_Repo')
    
    config_data['Emb_Lib_RNA_GEN7_V1_SW_Version'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_RNA_GEN7_V1_SW_Version')
    config_data['Emb_Lib_RNA_GEN7_V1_Date'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_RNA_GEN7_V1_Date')
    config_data['Emb_Lib_RNA_GEN7_V1_CommitID'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_RNA_GEN7_V1_CommitID')
    config_data['Emb_Lib_RNA_GEN7_V1_Linux'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_RNA_GEN7_V1_Linux')
    config_data['Emb_Lib_RNA_GEN7_V1_Windows'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_RNA_GEN7_V1_Windows')
    config_data['Emb_Lib_RNA_GEN7_V1_Repo'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_RNA_GEN7_V1_Repo')
        
    config_data['Emb_Lib_NISSAN_SW_Version'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_NISSAN_SW_Version')
    config_data['Emb_Lib_NISSAN_Date'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_NISSAN_Date')
    config_data['Emb_Lib_NISSAN_CS'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_NISSAN_CS')
    config_data['Emb_Lib_NISSAN_Linux'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_NISSAN_Linux')
    config_data['Emb_Lib_NISSAN_Windows'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_NISSAN_Windows')
    config_data['Emb_Lib_NISSAN_Repo'] = RNconfig.get('Sensor_Library_Info', 'Emb_Lib_NISSAN_Repo')
        
    config_data['Emb_Lib_PLATFORM_GPO_SRR6P_SW_Version'] = RNconfig.get('Sensor_Library_Info',
                                                                        "Emb_Lib_PLATFORM_GPO_SRR6P_SW_Version")
    config_data['Emb_Lib_PLATFORM_GPO_SRR6P_Date'] = RNconfig.get('Sensor_Library_Info',
                                                                  'Emb_Lib_PLATFORM_GPO_SRR6P_Date')
    config_data['Emb_Lib_PLATFORM_GPO_SRR6P_CS'] = RNconfig.get('Sensor_Library_Info',
                                                                'Emb_Lib_PLATFORM_GPO_SRR6P_CS')
    config_data['Emb_Lib_PLATFORM_GPO_SRR6P_Linux'] = RNconfig.get('Sensor_Library_Info',
                                                                   'Emb_Lib_PLATFORM_GPO_SRR6P_Linux')
    config_data['Emb_Lib_PLATFORM_GPO_SRR6P_Windows'] = RNconfig.get('Sensor_Library_Info',
                                                                     'Emb_Lib_PLATFORM_GPO_SRR6P_Windows')
    config_data['Emb_Lib_PLATFORM_GPO_SRR6P_Repo'] = RNconfig.get('Sensor_Library_Info',
                                                                  'Emb_Lib_PLATFORM_GPO_SRR6P_Repo')
        
    config_data['Emb_Lib_PLATFORM_GPO_FLR4_SW_Version'] = RNconfig.get('Sensor_Library_Info',
                                                                       'Emb_Lib_PLATFORM_GPO_FLR4_SW_Version')
    config_data['Emb_Lib_PLATFORM_GPO_FLR4_Date'] = RNconfig.get('Sensor_Library_Info',
                                                                 'Emb_Lib_PLATFORM_GPO_FLR4_Date')
    config_data['Emb_Lib_PLATFORM_GPO_FLR4_CS'] = RNconfig.get('Sensor_Library_Info',
                                                               'Emb_Lib_PLATFORM_GPO_FLR4_CS')
    config_data['Emb_Lib_PLATFORM_GPO_FLR4_Linux'] = RNconfig.get('Sensor_Library_Info',
                                                                  'Emb_Lib_PLATFORM_GPO_FLR4_Linux')
    config_data['Emb_Lib_PLATFORM_GPO_FLR4_Windows'] = RNconfig.get('Sensor_Library_Info',
                                                                    'Emb_Lib_PLATFORM_GPO_FLR4_Windows')
    config_data['Emb_Lib_PLATFORM_GPO_FLR4_Repo'] = RNconfig.get('Sensor_Library_Info',
                                                                 'Emb_Lib_PLATFORM_GPO_FLR4_Repo')
        
    config_data['Emb_Lib_PLATFORM_GPO_FLR4P_SW_Version'] = RNconfig.get('Sensor_Library_Info',
                                                                        'Emb_Lib_PLATFORM_GPO_FLR4P_SW_Version')
    config_data['Emb_Lib_PLATFORM_GPO_FLR4P_Date'] = RNconfig.get('Sensor_Library_Info',
                                                                  'Emb_Lib_PLATFORM_GPO_FLR4P_Date')
    config_data['Emb_Lib_PLATFORM_GPO_FLR4P_CS'] = RNconfig.get('Sensor_Library_Info',
                                                                'Emb_Lib_PLATFORM_GPO_FLR4P_CS')
    config_data['Emb_Lib_PLATFORM_GPO_FLR4P_Linux'] = RNconfig.get('Sensor_Library_Info',
                                                                   'Emb_Lib_PLATFORM_GPO_FLR4P_Linux')
    config_data['Emb_Lib_PLATFORM_GPO_FLR4P_Windows'] = RNconfig.get('Sensor_Library_Info',
                                                                     'Emb_Lib_PLATFORM_GPO_FLR4P_Windows')
    config_data['Emb_Lib_PLATFORM_GPO_FLR4P_Repo'] = RNconfig.get('Sensor_Library_Info',
                                                                  'Emb_Lib_PLATFORM_GPO_FLR4P_Repo')
            
    config_data['DC_BMW_SP21_SW_Version'] = RNconfig.get('Domain_Controller_Lib_Info', 'DC_BMW_SP21_SW_Version')
    config_data['DC_BMW_SP21_Date'] = RNconfig.get('Domain_Controller_Lib_Info', 'DC_BMW_SP21_Date')
    config_data['DC_BMW_SP21_CS'] = RNconfig.get('Domain_Controller_Lib_Info', 'DC_BMW_SP21_CS')
    config_data['DC_BMW_SP21_Linux'] = RNconfig.get('Domain_Controller_Lib_Info', 'DC_BMW_SP21_Linux')
    config_data['DC_BMW_SP21_Windows'] = RNconfig.get('Domain_Controller_Lib_Info', 'DC_BMW_SP21_Windows')
    config_data['DC_BMW_SP21_Repo'] = RNconfig.get('Domain_Controller_Lib_Info', 'DC_BMW_SP21_Repo')
            
    config_data['DC_BMW_SP25_SRR_SW_Version'] = RNconfig.get('Domain_Controller_Lib_Info', 'DC_BMW_SP25_SRR_SW_Version')
    config_data['DC_BMW_SP25_SRR_Date'] = RNconfig.get('Domain_Controller_Lib_Info', 'DC_BMW_SP25_SRR_Date')
    config_data['DC_BMW_SP25_SRR_CS'] = RNconfig.get('Domain_Controller_Lib_Info', 'DC_BMW_SP25_SRR_CS')
    config_data['DC_BMW_SP25_SRR_Linux'] = RNconfig.get('Domain_Controller_Lib_Info', 'DC_BMW_SP25_SRR_Linux')
    config_data['DC_BMW_SP25_SRR_Windows'] = RNconfig.get('Domain_Controller_Lib_Info', 'DC_BMW_SP25_SRR_Windows')
    config_data['DC_BMW_SP25_SRR_Repo'] = RNconfig.get('Domain_Controller_Lib_Info', 'DC_BMW_SP25_SRR_Repo')
            
    config_data['DC_BMW_SP25_MRR_SW_Version'] = RNconfig.get('Domain_Controller_Lib_Info', 'DC_BMW_SP25_MRR_SW_Version')
    config_data['DC_BMW_SP25_MRR_Date'] = RNconfig.get('Domain_Controller_Lib_Info', 'DC_BMW_SP25_MRR_Date')
    config_data['DC_BMW_SP25_MRR_CS'] = RNconfig.get('Domain_Controller_Lib_Info', 'DC_BMW_SP25_MRR_CS')
    config_data['DC_BMW_SP25_MRR_Linux'] = RNconfig.get('Domain_Controller_Lib_Info', 'DC_BMW_SP25_MRR_Linux')
    config_data['DC_BMW_SP25_MRR_Windows'] = RNconfig.get('Domain_Controller_Lib_Info', 'DC_BMW_SP25_MRR_Windows')
    config_data['DC_BMW_SP25_MRR_Repo'] = RNconfig.get('Domain_Controller_Lib_Info', 'DC_BMW_SP25_MRR_Repo')
            
    config_data['DC_STLA_IFV600_SRR_SW_Version'] = RNconfig.get('Domain_Controller_Lib_Info',
                                                                'DC_STLA_IFV600_SRR_SW_Version')
    config_data['DC_STLA_IFV600_SRR_Date'] = RNconfig.get('Domain_Controller_Lib_Info', 'DC_STLA_IFV600_SRR_Date')
    config_data['DC_STLA_IFV600_SRR_CS'] = RNconfig.get('Domain_Controller_Lib_Info', 'DC_STLA_IFV600_SRR_CS')
    config_data['DC_STLA_IFV600_SRR_Linux'] = RNconfig.get('Domain_Controller_Lib_Info', 'DC_STLA_IFV600_SRR_Linux')
    config_data['DC_STLA_IFV600_SRR_Windows'] = RNconfig.get('Domain_Controller_Lib_Info', 'DC_STLA_IFV600_SRR_Windows')
    config_data['DC_STLA_IFV600_SRR_Repo'] = RNconfig.get('Domain_Controller_Lib_Info', 'DC_STLA_IFV600_SRR_Repo')
            
    config_data['DC_STLA_IFV600_MRR_SW_Version'] = RNconfig.get('Domain_Controller_Lib_Info',
                                                                'DC_STLA_IFV600_MRR_SW_Version')
    config_data['DC_STLA_IFV600_MRR_Date'] = RNconfig.get('Domain_Controller_Lib_Info', 'DC_STLA_IFV600_MRR_Date')
    config_data['DC_STLA_IFV600_MRR_CS'] = RNconfig.get('Domain_Controller_Lib_Info', 'DC_STLA_IFV600_MRR_CS')
    config_data['DC_STLA_IFV600_MRR_Linux'] = RNconfig.get('Domain_Controller_Lib_Info', 'DC_STLA_IFV600_MRR_Linux')
    config_data['DC_STLA_IFV600_MRR_Windows'] = RNconfig.get('Domain_Controller_Lib_Info', 'DC_STLA_IFV600_MRR_Windows')
    config_data['DC_STLA_IFV600_MRR_Repo'] = RNconfig.get('Domain_Controller_Lib_Info', 'DC_STLA_IFV600_MRR_Repo')
                
    config_data['BMW_LOW_Linux'] = RNconfig.get('Test_Summary', 'BMW_LOW_Linux')
    config_data['BMW_LOW_Windows'] = RNconfig.get('Test_Summary', 'BMW_LOW_Windows')
                
    config_data['BMW_MID_Linux'] = RNconfig.get('Test_Summary', 'BMW_MID_Linux')
    config_data['BMW_MID_Windows'] = RNconfig.get('Test_Summary', 'BMW_MID_Windows')
                
    config_data['BMW_HIGH_Linux'] = RNconfig.get('Test_Summary', 'BMW_HIGH_Linux')
    config_data['BMW_HIGH_Windows'] = RNconfig.get('Test_Summary', 'BMW_HIGH_Windows')
    
    config_data['STLA_IFV600DC_Linux'] = RNconfig.get('Test_Summary', 'STLA_IFV600DC_Linux')
    config_data['STLA_IFV600DC_Windows'] = RNconfig.get('Test_Summary', 'STLA_IFV600DC_Windows')
                
    config_data['STLA_SCALE1_Linux'] = RNconfig.get('Test_Summary', 'STLA_SCALE1_Linux')
    config_data['STLA_SCALE1_Windows'] = RNconfig.get('Test_Summary', 'STLA_SCALE1_Windows')
                
    config_data['STLA_SCALE3_Linux'] = RNconfig.get('Test_Summary', 'STLA_SCALE3_Linux')
    config_data['STLA_SCALE3_Windows'] = RNconfig.get('Test_Summary', 'STLA_SCALE3_Windows')
                
    config_data['STLA_SCALE4_Linux'] = RNconfig.get('Test_Summary', 'STLA_SCALE4_Linux')
    config_data['STLA_SCALE4_Windows'] = RNconfig.get('Test_Summary', 'STLA_SCALE4_Windows')
                
    config_data['HONDA_Linux'] = RNconfig.get('Test_Summary', 'HONDA_Linux')
    config_data['HONDA_Windows'] = RNconfig.get('Test_Summary', 'HONDA_Windows')
                
    config_data['TRATON_Linux'] = RNconfig.get('Test_Summary', 'TRATON_Linux')
    config_data['TRATON_Windows'] = RNconfig.get('Test_Summary', 'TRATON_Windows')
                
    config_data['GPO_GEN7_V1_Linux'] = RNconfig.get('Test_Summary', 'GPO_GEN7_V1_Linux')
    config_data['GPO_GEN7_V1_Windows'] = RNconfig.get('Test_Summary', 'GPO_GEN7_V1_Windows')
                
    config_data['RNA_GEN7_V1_Linux'] = RNconfig.get('Test_Summary', 'RNA_GEN7_V1_Linux')
    config_data['RNA_GEN7_V1_Windows'] = RNconfig.get('Test_Summary', 'RNA_GEN7_V1_Windows')
                
    config_data['NISSAN_Linux'] = RNconfig.get('Test_Summary', 'NISSAN_Linux')
    config_data['NISSAN_Windows'] = RNconfig.get('Test_Summary', 'NISSAN_Windows')
                
    config_data['BMW_SP25_L2_Linux'] = RNconfig.get('Test_Summary', 'BMW_SP25_L2_Linux')
    config_data['BMW_SP25_L2_Windows'] = RNconfig.get('Test_Summary', 'BMW_SP25_L2_Windows')
               
    config_data['BMW_SP25_L3_Linux'] = RNconfig.get('Test_Summary', 'BMW_SP25_L3_Linux')
    config_data['BMW_SP25_L3_Windows'] = RNconfig.get('Test_Summary', 'BMW_SP25_L3_Windows')
                
    config_data['PLATFORM_GPO_SRR6P_Linux'] = RNconfig.get('Test_Summary', 'PLATFORM_GPO_SRR6P_Linux')
    config_data['PLATFORM_GPO_SRR6P_Windows'] = RNconfig.get('Test_Summary', 'PLATFORM_GPO_SRR6P_Windows')
                
    config_data['PLATFORM_GPO_FLR4_Linux'] = RNconfig.get('Test_Summary', 'PLATFORM_GPO_FLR4_Linux')
    config_data['PLATFORM_GPO_FLR4_Windows'] = RNconfig.get('Test_Summary', 'PLATFORM_GPO_FLR4_Windows')
                
    config_data['PLATFORM_GPO_FLR4P_Linux'] = RNconfig.get('Test_Summary', 'PLATFORM_GPO_FLR4P_Linux')
    config_data['PLATFORM_GPO_FLR4P_Windows'] = RNconfig.get('Test_Summary', 'PLATFORM_GPO_FLR4P_Windows')
                    
    config_data['Changelog_details'] = RNconfig.get('Changelog', 'Changelog_details')

    config_data['JIRA_Tickets'] = RNconfig.get('JIRA', 'JIRA_Tickets')
		   
    config_data['Known_Issues'] = RNconfig.get('Known_Issues', 'Known_Issues')


def writeXMLfile():
    import xml.etree.ElementTree as Rnotes
    from xml.dom import minidom

    vv_notes = "RELEASE_DESCRIPTION_SPRINT_" + config_data['Sprint'] + "_VV_Release"

    root: Element = Rnotes.Element(vv_notes)

    vv_version = Rnotes.SubElement(root, "Virtual_Validation_Version")
    vv_version.text = str(config_data['VV_Version'])

    release_date = Rnotes.SubElement(root, "Release_Date")
    release_date.text = config_data['Release_Date']

    release_CS = Rnotes.SubElement(root, "VV_Release_CS")
    release_CS.text = str(config_data['VV_Release_CS'])

    apv_module = Rnotes.SubElement(root, "APTIVE_MODULE_Info")

    SIL_en_info = Rnotes.SubElement(apv_module, "SIL_ENGINE_Details")
    SIL_en_info.set("SW_Version", config_data['SIL_ENG_SW_Version'])
    SIL_en_info.set("Date", config_data['SIL_ENG_Date'])
    SIL_en_info.set("CS", str(config_data['SIL_ENG_CS']))
    SIL_en_info.set("Repo", config_data['SIL_ENG_Repo'])

    LM2_info = Rnotes.SubElement(apv_module, "LM2_Details")
    LM2_info.set("SW_Version", config_data['LM2_SW_Version'])
    LM2_info.set("Date", config_data['LM2_Date'])
    LM2_info.set("CS", str(config_data['LM2_CS']))
    LM2_info.set("Repo", config_data['LM2_Repo'])

    SM2_info = Rnotes.SubElement(apv_module, "SM2_Details")
    SM2_info.set("SW_Version", config_data['SM2_SW_Version'])
    SM2_info.set("Date", config_data['SM2_Date'])
    SM2_info.set("CS", str(config_data['SM2_CS']))
    SM2_info.set("Repo", config_data['SM2_Repo'])

    vv_eng_info = Rnotes.SubElement(apv_module, "VV_ENGINE_Details")
    vv_eng_info.set("SW_Version", config_data['VV_ENG_SW_Version'])
    vv_eng_info.set("Date", config_data['VV_ENG_Date'])
    vv_eng_info.set("CS", str(config_data['VV_ENG_CS']))
    vv_eng_info.set("Repo", config_data['VV_ENG_Repo'])

    emb_lib_info = Rnotes.SubElement(root , "Sensor_Library_Info")

    bmw_sp21_low = Rnotes.SubElement(emb_lib_info, "BMW_SP21_LOW")
    bmw_sp21_low.set("SW_Version", config_data['Emb_Lib_BMW_SP21_LOW_SW_Version'])
    bmw_sp21_low.set("Date", config_data['Emb_Lib_BMW_SP21_LOW_Date'])
    bmw_sp21_low.set("CS", str(config_data['Emb_Lib_BMW_SP21_LOW_CS']))
    bmw_sp21_low.set("Linux", config_data['Emb_Lib_BMW_SP21_LOW_Linux'])
    bmw_sp21_low.set("Windows", config_data['Emb_Lib_BMW_SP21_LOW_Windows'])
    bmw_sp21_low.set("Repo", config_data['Emb_Lib_BMW_SP21_LOW_Repo'])

    bmw_sp21_mid = Rnotes.SubElement(emb_lib_info, "BMW_SP21_MID")
    bmw_sp21_mid.set("SW_Version", config_data['Emb_Lib_BMW_SP21_MID_SW_Version'])
    bmw_sp21_mid.set("Date", config_data['Emb_Lib_BMW_SP21_MID_Date'])
    bmw_sp21_mid.set("CS", str(config_data['Emb_Lib_BMW_SP21_MID_CS']))
    bmw_sp21_mid.set("Linux", config_data['Emb_Lib_BMW_SP21_MID_Linux'])
    bmw_sp21_mid.set("Windows", config_data['Emb_Lib_BMW_SP21_MID_Windows'])
    bmw_sp21_mid.set("Repo", config_data['Emb_Lib_BMW_SP21_MID_Repo'])

    bmw_sp21_high = Rnotes.SubElement(emb_lib_info, "BMW_SP21_HIGH")
    bmw_sp21_high.set("SW_Version", config_data['Emb_Lib_BMW_SP21_HIGH_SW_Version'])
    bmw_sp21_high.set("Date", config_data['Emb_Lib_BMW_SP21_HIGH_Date'])
    bmw_sp21_high.set("CS", str(config_data['Emb_Lib_BMW_SP21_HIGH_CS']))
    bmw_sp21_high.set("Linux", config_data['Emb_Lib_BMW_SP21_HIGH_Linux'])
    bmw_sp21_high.set("Windows", config_data['Emb_Lib_BMW_SP21_HIGH_Windows'])
    bmw_sp21_high.set("Repo", config_data['Emb_Lib_BMW_SP21_HIGH_Repo'])

    bmw_sp25_L2 = Rnotes.SubElement(emb_lib_info, "BMW_SP25_L2")
    bmw_sp25_L2.set("SW_Version", config_data['Emb_Lib_BMW_SP25_L2_SW_Version'])
    bmw_sp25_L2.set("Date", config_data['Emb_Lib_BMW_SP25_L2_Date'])
    bmw_sp25_L2.set("CS", str(config_data['Emb_Lib_BMW_SP25_L2_CS']))
    bmw_sp25_L2.set("Linux", config_data['Emb_Lib_BMW_SP25_L2_Linux'])
    bmw_sp25_L2.set("Windows", config_data['Emb_Lib_BMW_SP25_L2_Windows'])
    bmw_sp25_L2.set("Repo", config_data['Emb_Lib_BMW_SP25_L2_Repo'])

    bmw_sp25_L3 = Rnotes.SubElement(emb_lib_info, "BMW_SP25_L3")
    bmw_sp25_L3.set("SW_Version", config_data['Emb_Lib_BMW_SP25_L3_SW_Version'])
    bmw_sp25_L3.set("Date", config_data['Emb_Lib_BMW_SP25_L3_Date'])
    bmw_sp25_L3.set("CS", str(config_data['Emb_Lib_BMW_SP25_L3_CS']))
    bmw_sp25_L3.set("Linux", config_data['Emb_Lib_BMW_SP25_L3_Linux'])
    bmw_sp25_L3.set("Windows", config_data['Emb_Lib_BMW_SP25_L3_Windows'])
    bmw_sp25_L3.set("Repo", config_data['Emb_Lib_BMW_SP25_L3_Repo'])

    stla_scale1 = Rnotes.SubElement(emb_lib_info, "STLA_SCALE1")
    stla_scale1.set("SW_Version", config_data['Emb_Lib_STLA_SCALE1_SW_Version'])
    stla_scale1.set("Date", config_data['Emb_Lib_STLA_SCALE1_Date'])
    stla_scale1.set("CS", str(config_data['Emb_Lib_STLA_SCALE1_CS']))
    stla_scale1.set("Linux", config_data['Emb_Lib_STLA_SCALE1_Linux'])
    stla_scale1.set("Windows", config_data['Emb_Lib_STLA_SCALE1_Windows'])
    stla_scale1.set("Repo", config_data['Emb_Lib_STLA_SCALE1_Repo'])

    stla_scale3 = Rnotes.SubElement(emb_lib_info, "STLA_SCALE3")
    stla_scale3.set("SW_Version", config_data['Emb_Lib_STLA_SCALE3_SW_Version'])
    stla_scale3.set("Date", config_data['Emb_Lib_STLA_SCALE3_Date'])
    stla_scale3.set("CS", str(config_data['Emb_Lib_STLA_SCALE3_CS']))
    stla_scale3.set("Linux", config_data['Emb_Lib_STLA_SCALE3_Linux'])
    stla_scale3.set("Windows", config_data['Emb_Lib_STLA_SCALE3_Windows'])
    stla_scale3.set("Repo", config_data['Emb_Lib_STLA_SCALE3_Repo'])

    stla_scale4 = Rnotes.SubElement(emb_lib_info, "STLA_SCALE4")
    stla_scale4.set("SW_Version", config_data['Emb_Lib_STLA_SCALE4_SW_Version'])
    stla_scale4.set("Date", config_data['Emb_Lib_STLA_SCALE4_Date'])
    stla_scale4.set("CS", str(config_data['Emb_Lib_STLA_SCALE4_CS']))
    stla_scale4.set("Linux", config_data['Emb_Lib_STLA_SCALE4_Linux'])
    stla_scale4.set("Windows", config_data['Emb_Lib_STLA_SCALE4_Windows'])
    stla_scale4.set("Repo", config_data['Emb_Lib_STLA_SCALE4_Repo'])

    honda_srr6p = Rnotes.SubElement(emb_lib_info, "HONDA_SRR6P")
    honda_srr6p.set("SW_Version", config_data['Emb_Lib_HONDA_SW_Version'])
    honda_srr6p.set("Date", config_data['Emb_Lib_HONDA_Date'])
    honda_srr6p.set("CS", str(config_data['Emb_Lib_HONDA_CS']))
    honda_srr6p.set("Linux", config_data['Emb_Lib_HONDA_Linux'])
    honda_srr6p.set("Windows", config_data['Emb_Lib_HONDA_Windows'])
    honda_srr6p.set("Repo", config_data['Emb_Lib_HONDA_Repo'])
    
    traton = Rnotes.SubElement(emb_lib_info, "TRATON")
    traton.set("SW_Version", config_data['Emb_Lib_TRATON_SW_Version'])
    traton.set("Date", config_data['Emb_Lib_TRATON_Date'])
    traton.set("CommitID", config_data['Emb_Lib_TRATON_CommitID'])
    traton.set("Linux", config_data['Emb_Lib_TRATON_Linux'])
    traton.set("Windows", config_data['Emb_Lib_TRATON_Windows'])
    traton.set("Repo", config_data['Emb_Lib_TRATON_Repo'])
    
    gpo_gen7_v1 = Rnotes.SubElement(emb_lib_info, "GPO_GEN7_V1")
    gpo_gen7_v1.set("SW_Version", config_data['Emb_Lib_GPO_GEN7_V1_SW_Version'])
    gpo_gen7_v1.set("Date", config_data['Emb_Lib_GPO_GEN7_V1_Date'])
    gpo_gen7_v1.set("CommitID", config_data['Emb_Lib_GPO_GEN7_V1_CommitID'])
    gpo_gen7_v1.set("Linux", config_data['Emb_Lib_GPO_GEN7_V1_Linux'])
    gpo_gen7_v1.set("Windows", config_data['Emb_Lib_GPO_GEN7_V1_Windows'])
    gpo_gen7_v1.set("Repo", config_data['Emb_Lib_GPO_GEN7_V1_Repo'])
    
    rna_gen7_v1 = Rnotes.SubElement(emb_lib_info, "RNA_GEN7_V1")
    rna_gen7_v1.set("SW_Version", config_data['Emb_Lib_RNA_GEN7_V1_SW_Version'])
    rna_gen7_v1.set("Date", config_data['Emb_Lib_RNA_GEN7_V1_Date'])
    rna_gen7_v1.set("CommitID", config_data['Emb_Lib_RNA_GEN7_V1_CommitID'])
    rna_gen7_v1.set("Linux", config_data['Emb_Lib_RNA_GEN7_V1_Linux'])
    rna_gen7_v1.set("Windows", config_data['Emb_Lib_RNA_GEN7_V1_Windows'])
    rna_gen7_v1.set("Repo", config_data['Emb_Lib_RNA_GEN7_V1_Repo'])
    
    nissan_srr6= Rnotes.SubElement(emb_lib_info, "NISSAN_SRR6")
    nissan_srr6.set("SW_Version", config_data['Emb_Lib_NISSAN_SW_Version'])
    nissan_srr6.set("Date", config_data['Emb_Lib_NISSAN_Date'])
    nissan_srr6.set("CS", str(config_data['Emb_Lib_NISSAN_CS']))
    nissan_srr6.set("Linux", config_data['Emb_Lib_NISSAN_Linux'])
    nissan_srr6.set("Windows", config_data['Emb_Lib_NISSAN_Windows'])
    nissan_srr6.set("Repo", config_data['Emb_Lib_NISSAN_Repo'])

    gpo_srr6p= Rnotes.SubElement(emb_lib_info, "PLATFORM_GPO_SRR6P")
    gpo_srr6p.set("SW_Version", config_data['Emb_Lib_PLATFORM_GPO_SRR6P_SW_Version'])
    gpo_srr6p.set("Date", config_data['Emb_Lib_PLATFORM_GPO_SRR6P_Date'])
    gpo_srr6p.set("CS", str(config_data['Emb_Lib_PLATFORM_GPO_SRR6P_CS']))
    gpo_srr6p.set("Linux", config_data['Emb_Lib_PLATFORM_GPO_SRR6P_Linux'])
    gpo_srr6p.set("Windows", config_data['Emb_Lib_PLATFORM_GPO_SRR6P_Windows'])
    gpo_srr6p.set("Repo", config_data['Emb_Lib_PLATFORM_GPO_SRR6P_Repo'])

    gpo_flr4= Rnotes.SubElement(emb_lib_info, "PLATFORM_GPO_FLR4")
    gpo_flr4.set("SW_Version", config_data['Emb_Lib_PLATFORM_GPO_FLR4_SW_Version'])
    gpo_flr4.set("Date", config_data['Emb_Lib_PLATFORM_GPO_FLR4_Date'])
    gpo_flr4.set("CS", str(config_data['Emb_Lib_PLATFORM_GPO_FLR4_CS']))
    gpo_flr4.set("Linux", config_data['Emb_Lib_PLATFORM_GPO_FLR4_Linux'])
    gpo_flr4.set("Windows", config_data['Emb_Lib_PLATFORM_GPO_FLR4_Windows'])
    gpo_flr4.set("Repo", config_data['Emb_Lib_PLATFORM_GPO_FLR4_Repo'])

    gpo_flr4p= Rnotes.SubElement(emb_lib_info, "PLATFORM_GPO_FLR4P")
    gpo_flr4p.set("SW_Version", config_data['Emb_Lib_PLATFORM_GPO_FLR4P_SW_Version'])
    gpo_flr4p.set("Date", config_data['Emb_Lib_PLATFORM_GPO_FLR4P_Date'])
    gpo_flr4p.set("CS", str(config_data['Emb_Lib_PLATFORM_GPO_FLR4P_CS']))
    gpo_flr4p.set("Linux", config_data['Emb_Lib_PLATFORM_GPO_FLR4P_Linux'])
    gpo_flr4p.set("Windows", config_data['Emb_Lib_PLATFORM_GPO_FLR4P_Windows'])
    gpo_flr4p.set("Repo", config_data['Emb_Lib_PLATFORM_GPO_FLR4P_Repo'])
    
    DC_lib_info = Rnotes.SubElement(root, "Domain_Controller_Lib_Info")
    
    bmw_sp21_recu = Rnotes.SubElement(DC_lib_info, "BMW_SP21_RECU")
    bmw_sp21_recu.set("SW_Version", config_data['DC_BMW_SP21_SW_Version'])
    bmw_sp21_recu.set("Date", config_data['DC_BMW_SP21_Date'])
    bmw_sp21_recu.set("CS", str(config_data['DC_BMW_SP21_CS']))
    bmw_sp21_recu.set("Linux", config_data['DC_BMW_SP21_Linux'])
    bmw_sp21_recu.set("Windows", config_data['DC_BMW_SP21_Windows'])
    bmw_sp21_recu.set("Repo", config_data['DC_BMW_SP21_Repo'])
    
    bmw_sp25_srr = Rnotes.SubElement(DC_lib_info, "BMW_SP25_IPNext_SRR")
    bmw_sp25_srr.set("SW_Version", config_data['DC_BMW_SP25_SRR_SW_Version'])
    bmw_sp25_srr.set("Date", config_data['DC_BMW_SP25_SRR_Date'])
    bmw_sp25_srr.set("CS", str(config_data['DC_BMW_SP25_SRR_CS']))
    bmw_sp25_srr.set("Linux", config_data['DC_BMW_SP25_SRR_Linux'])
    bmw_sp25_srr.set("Windows", config_data['DC_BMW_SP25_SRR_Windows'])
    bmw_sp25_srr.set("Repo", config_data['DC_BMW_SP25_SRR_Repo'])
    
    bmw_sp25_mrr = Rnotes.SubElement(DC_lib_info, "BMW_SP25_IPNext_MRR")
    bmw_sp25_mrr.set("SW_Version", config_data['DC_BMW_SP25_MRR_SW_Version'])
    bmw_sp25_mrr.set("Date", config_data['DC_BMW_SP25_MRR_Date'])
    bmw_sp25_mrr.set("CS", str(config_data['DC_BMW_SP25_MRR_CS']))
    bmw_sp25_mrr.set("Linux", config_data['DC_BMW_SP25_MRR_Linux'])
    bmw_sp25_mrr.set("Windows", config_data['DC_BMW_SP25_MRR_Windows'])
    bmw_sp25_mrr.set("Repo", config_data['DC_BMW_SP25_MRR_Repo'])
    
    stl_my24_srr = Rnotes.SubElement(DC_lib_info, "STLA_MY24_IFV600_SRR")
    stl_my24_srr.set("SW_Version", config_data['DC_STLA_IFV600_SRR_SW_Version'])
    stl_my24_srr.set("Date", config_data['DC_STLA_IFV600_SRR_Date'])
    stl_my24_srr.set("CS", str(config_data['DC_STLA_IFV600_SRR_CS']))
    stl_my24_srr.set("Linux", config_data['DC_STLA_IFV600_SRR_Linux'])
    stl_my24_srr.set("Windows", config_data['DC_STLA_IFV600_SRR_Windows'])
    stl_my24_srr.set("Repo", config_data['DC_STLA_IFV600_SRR_Repo'])
    
    stl_my24_mrr = Rnotes.SubElement(DC_lib_info, "STLA_MY24_IFV600_MRR")
    stl_my24_mrr.set("SW_Version", config_data['DC_STLA_IFV600_MRR_SW_Version'])
    stl_my24_mrr.set("Date", config_data['DC_STLA_IFV600_MRR_Date'])
    stl_my24_mrr.set("CS", str(config_data['DC_STLA_IFV600_MRR_CS']))
    stl_my24_mrr.set("Linux", config_data['DC_STLA_IFV600_MRR_Linux'])
    stl_my24_mrr.set("Windows", config_data['DC_STLA_IFV600_MRR_Windows'])
    stl_my24_mrr.set("Repo", config_data['DC_STLA_IFV600_MRR_Repo'])
    
    test_summary = Rnotes.SubElement(root, "Test_Summary")
    
    bmw_low = Rnotes.SubElement(test_summary, "BMW_LOW")
    bmw_low.set("Linux", config_data['BMW_LOW_Linux'])
    bmw_low.set("Windows", config_data['BMW_LOW_Windows'])
    
    bmw_mid = Rnotes.SubElement(test_summary, "BMW_MID")
    bmw_mid.set("Linux", config_data['BMW_MID_Linux'])
    bmw_mid.set("Windows", config_data['BMW_MID_Windows'])
    
    bmw_high = Rnotes.SubElement(test_summary, "BMW_HIGH")
    bmw_high.set("Linux", config_data['BMW_HIGH_Linux'])
    bmw_high.set("Windows", config_data['BMW_HIGH_Windows'])
    
    stl_ifv600dc = Rnotes.SubElement(test_summary, "STLA_IFV600DC")
    stl_ifv600dc.set("Linux", config_data['STLA_IFV600DC_Linux'])
    stl_ifv600dc.set("Windows", config_data['STLA_IFV600DC_Windows'])
    
    stl_scale1 = Rnotes.SubElement(test_summary, "STLA_SCALE1")
    stl_scale1.set("Linux", config_data['STLA_SCALE1_Linux'])
    stl_scale1.set("Windows", config_data['STLA_SCALE1_Windows'])
    
    stl_scale3 = Rnotes.SubElement(test_summary, "STLA_SCALE3")
    stl_scale3.set("Linux", config_data['STLA_SCALE3_Linux'])
    stl_scale3.set("Windows", config_data['STLA_SCALE3_Windows'])
    
    stl_scale4 = Rnotes.SubElement(test_summary, "STLA_SCALE4")
    stl_scale4.set("Linux", config_data['STLA_SCALE4_Linux'])
    stl_scale4.set("Windows", config_data['STLA_SCALE4_Windows'])
    
    honda = Rnotes.SubElement(test_summary, "HONDA")
    honda.set("Linux", config_data['HONDA_Linux'])
    honda.set("Windows", config_data['HONDA_Windows'])
    
    traton = Rnotes.SubElement(test_summary, "TRATON")
    traton.set("Linux", config_data['TRATON_Linux'])
    traton.set("Windows", config_data['TRATON_Windows'])
    
    gpo_gen7_v1 = Rnotes.SubElement(test_summary, "GPO_GEN7_V1")
    gpo_gen7_v1.set("Linux", config_data['GPO_GEN7_V1_Linux'])
    gpo_gen7_v1.set("Windows", config_data['GPO_GEN7_V1_Windows'])
    
    rna_gen7_v1 = Rnotes.SubElement(test_summary, "RNA_GEN7_V1")
    rna_gen7_v1.set("Linux", config_data['RNA_GEN7_V1_Linux'])
    rna_gen7_v1.set("Windows", config_data['RNA_GEN7_V1_Windows'])
    
    nissan = Rnotes.SubElement(test_summary, "NISSAN")
    nissan.set("Linux", config_data['NISSAN_Linux'])
    nissan.set("Windows", config_data['NISSAN_Windows'])
    
    bmw_sp25_L2 = Rnotes.SubElement(test_summary, "BMW_SP25_L2")
    bmw_sp25_L2.set("Linux", config_data['BMW_SP25_L2_Linux'])
    bmw_sp25_L2.set("Windows", config_data['BMW_SP25_L2_Windows'])
    
    bmw_sp25_L3 = Rnotes.SubElement(test_summary, "BMW_SP25_L3")
    bmw_sp25_L3.set("Linux", config_data['BMW_SP25_L3_Linux'])
    bmw_sp25_L3.set("Windows", config_data['BMW_SP25_L3_Windows'])
    
    gpo_srr6p = Rnotes.SubElement(test_summary, "PLATFORM_GPO_SRR6P")
    gpo_srr6p.set("Linux", config_data['PLATFORM_GPO_SRR6P_Linux'])
    gpo_srr6p.set("Windows", config_data['PLATFORM_GPO_SRR6P_Windows'])
    
    gpo_flr4 = Rnotes.SubElement(test_summary, "PLATFORM_GPO_FLR4")
    gpo_flr4.set("Linux", config_data['PLATFORM_GPO_FLR4_Linux'])
    gpo_flr4.set("Windows", config_data['PLATFORM_GPO_FLR4_Windows'])
    
    gpo_flr4p = Rnotes.SubElement(test_summary, "PLATFORM_GPO_FLR4P")
    gpo_flr4p.set("Linux", config_data['PLATFORM_GPO_FLR4P_Linux'])
    gpo_flr4p.set("Windows", config_data['PLATFORM_GPO_FLR4P_Windows'])
    
    changelog = Rnotes.SubElement(root, "ChangeLog_Details")
    changelog.text = config_data['Changelog_details']
    
    jira_tickets = Rnotes.SubElement(root, "JIRA_Tickets")
    jira_tickets.text = config_data['JIRA_Tickets']
    
    known_issues = Rnotes.SubElement(root, "Known_Issues")
    known_issues.text = config_data['Known_Issues']

    xml_str = Rnotes.tostring(root)
    
    xml_dom = minidom.parseString(xml_str)

    with open("VV_Release_Notes.xml", "w") as file:
        file.write(xml_dom.toprettyxml(indent="    "))

    print("[INFO] : VV Release Notes written successfully")

readconfig()
writeXMLfile()
