#!/usr/bin/env bash
echo " "
echo "ACTIVATING VIRTUAL ENVIRONMENT !!!!"
source /mnt/usmidet/projects/STLA-THUNDER/2-Sim/USER_DATA/qj743z/virtual_env/thunder_env/DC_venv/bin/activate
echo "ACTIVATED VIRTUAL ENVIRONMENT !!!!"
echo " "
echo "CALLING PYTHON SCRIPT TO EXTRACT RESIM YIELD DETAILS !!!!"
python /mnt/usmidet/projects/STLA-THUNDER/7-Tools/ReSimAutoMng/Support/PostProcessing/extract_resim_yeild.py $1 $2 $3 
deactivate
echo "DEACTIVATED VIRTUAL ENVIRONMENT !!!!"
echo " "