#!/usr/bin/env bash

echo "[ver-1.0] : Starting Pipeline Execution"

USER_ID=$(whoami)

if [[ -d "/mnt/usmidet/projects/GPO-IFV7XX" ]]; then
    echo "Detected cluster: Southfield"

    source /mnt/usmidet/projects/GPO-IFV7XX/4-Checkout/Environment/env/bin/activate
    python /mnt/usmidet/projects/GPO-IFV7XX/4-Checkout/Resim_Pipeline/Core_RESIM_HPCC/VV_Resim_Pipeline/VV_main.py "$@"
    deactivate

elif [[ "${USER_ID}" == 8k3* ]]; then
    echo "Detected cluster: Helios"

    source /net/8k3/e0fs01/irods/PLKRA-PROJECTS/CEER-PROGRAM/4-Checkout/Environment/env/bin/activate
    module load slurm
    python /net/8k3/e0fs01/irods/PLKRA-PROJECTS/CEER-PROGRAM/4-Checkout/Resim_Pipeline/Core_RESIM_HPCC/VV_Resim_Pipeline/VV_main.py "$@"
    deactivate

else
    echo "Detected cluster: Krakow"

    source /net/8k3/e0fs01/irods/PLKRA-PROJECTS/CEER-PROGRAM/4-Checkout/Environment/env/bin/activate
    module load slurm
    python /net/8k3/e0fs01/irods/PLKRA-PROJECTS/CEER-PROGRAM/4-Checkout/Resim_Pipeline/Core_RESIM_HPCC/VV_Resim_Pipeline/VV_main.py "$@"
    deactivate
fi

# @author: pcmzxl (Pranjal.Singh@aptiv.com)