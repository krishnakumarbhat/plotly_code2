import os
import sys
import xml.etree.ElementTree as ET

name_of_script = sys.argv[0]
customer = int((sys.argv[1]))
radarVariant = int((sys.argv[2]))
sil_config_path = sys.argv[3]
mode = sys.argv[4]

def update_xml(sil_config_path, mode, customer, radarVariant):
    try:
        tree = ET.parse(sil_config_path)
        root = tree.getroot()
        
        sensor_rl = root.find('.//SENSOR_00')
        sensor_rr = root.find('.//SENSOR_01')
        sensor_fr = root.find('.//SENSOR_02')
        sensor_fl = root.find('.//SENSOR_03')
        sensor_fc = root.find('.//SENSOR_04')
        srr_tag = root.find('.//ECU_00')
        mrr_tag = root.find('.//ECU_01')
        
        LOG_REPLAY_MODE = root.find('.//LOG_REPLAY_MODE')
        if LOG_REPLAY_MODE is not None:
                LOG_REPLAY_MODE.text="Continuous_FILE_Input"
                
        srr_mrr_tag=root.find('.//SRR_MRR_ORCAS_OUTPUT_FILE_ENABLE')
        if srr_mrr_tag is not None:
                srr_mrr_tag.text="0"
                
        sil_injection_mode_tag=root.find('.//SIL_INJECTION_MODE')
        if sil_injection_mode_tag is not None:
                sil_injection_mode_tag.text="VEHICLE_EXPEDITION_FILE"
                
        mode_tag=root.find('.//SIL_Entrypoint')
        if mode_tag is not None and mode != "DGPS":
                mode_tag.text=mode

        recu_config_00 = root.find('.//RECU_CONFIG_00')
        recu_config_01 = root.find('.//RECU_CONFIG_01')

        if customer == 1 and radarVariant == 1:
                sensor_rl.text="1"
                sensor_rr.text="1"
                sensor_fr.text="1"
                sensor_fl.text="1"
                sensor_fc.text="0"
                srr_tag.text="1"
                mrr_tag.text="0"
                if recu_config_00 is not None:
                        recu_config_00.text="/Binaries/SRR_DC_Lib_Control.xml"
                if recu_config_01 is not None:
                        recu_config_01.text="NONE"

        elif customer == 1 and radarVariant == 2:
                sensor_rl.text="0"
                sensor_rr.text="0"
                sensor_fr.text="0"
                sensor_fl.text="0"
                sensor_fc.text="1"
                srr_tag.text="0"
                mrr_tag.text="1"
                if recu_config_00 is not None:
                        recu_config_00.text="NONE"
                if recu_config_01 is not None:
                        recu_config_01.text="/Binaries/MRR_DC_Lib_Control.xml"

        elif customer == 1 and radarVariant == 3:
                sensor_rl.text="1"
                sensor_rr.text="1"
                sensor_fr.text="1"
                sensor_fl.text="1"
                sensor_fc.text="1"
                srr_tag.text="1"
                mrr_tag.text="1"
                if recu_config_00 is not None:
                        recu_config_00.text="/Binaries/SRR_DC_Lib_Control.xml"
                if recu_config_01 is not None:
                        recu_config_01.text="/Binaries/MRR_DC_Lib_Control.xml"


        # Write the updated XML back to the file
        tree.write(sil_config_path, encoding="utf-8", xml_declaration=True)
        print(f"Successfully updated XML file: {sil_config_path}")
    except Exception as e:
        print(f"Error processing XML file: {e}")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) < 4:
        print("Usage: python script.py <customer_id> <variant_id> <config_path>")
        sys.exit(1)
        
update_xml(sil_config_path, mode, customer, radarVariant)



        

