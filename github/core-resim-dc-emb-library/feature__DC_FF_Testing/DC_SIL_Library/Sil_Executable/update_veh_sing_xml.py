import os
import sys
import xml.etree.ElementTree as ET

name_of_script = sys.argv[0]
customer = int((sys.argv[1]))
radarVariant = int((sys.argv[2]))
sil_config_path = sys.argv[3]

def update_xml(sil_config_path, customer, radarVariant):
    try:
        tree = ET.parse(sil_config_path)
        root = tree.getroot()
        
        srr_path_tag = root.find('.//RECU_CONFIG_00')
        mrr_path_tag = root.find('.//RECU_CONFIG_01')
        sensor_rl = root.find('.//SENSOR_00')
        sensor_rr = root.find('.//SENSOR_01')
        sensor_fr = root.find('.//SENSOR_02')
        sensor_fl = root.find('.//SENSOR_03')
        sensor_fc = root.find('.//SENSOR_05')
        srr_tag = root.find('.//ECU_00')
        mrr_tag = root.find('.//ECU_01')
        
        emb_lib_config = root.find('.//SENSOR_CONFIG')
        if emb_lib_config is not None:
                emb_lib_config.text="./Emb_Lib_Config.xml"
                
        srr_mrr_tag=root.find('.//SRR_MRR_ORCAS_OUTPUT_FILE_ENABLE')
        if srr_mrr_tag is not None:
                srr_mrr_tag.text="1"
        
        if customer == 1 and radarVariant == 1:
            if srr_path_tag is not None:
                srr_path_tag.text="./SRR_DC_Lib_Control.xml"
                sensor_rl.text="1"
                sensor_rr.text="1"
                sensor_fr.text="1"
                sensor_fl.text="1"
                sensor_fc.text="0"
                srr_tag.text="1"
                mrr_tag.text="0"

        elif customer == 1 and radarVariant == 2:
            if mrr_path_tag is not None:
                mrr_path_tag.text="./MRR_DC_Lib_Control.xml"
                sensor_rl.text="0"
                sensor_rr.text="0"
                sensor_fr.text="0"
                sensor_fl.text="0"
                sensor_fc.text="1"
                srr_tag.text="0"
                mrr_tag.text="1"
        
        elif customer == 1 and radarVariant == 3:
            if srr_path_tag is not None:
                srr_path_tag.text="./SRR_DC_Lib_Control.xml"
            elif mrr_path_tag is not None:
                mrr_path_tag.text="./MRR_DC_Lib_Control.xml"
                sensor_rl.text="1"
                sensor_rr.text="1"
                sensor_fr.text="1"
                sensor_fl.text="1"
                sensor_fc.text="1"
                srr_tag.text="1"
                mrr_tag.text="1"

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
        
update_xml(sil_config_path, customer, radarVariant)



        

