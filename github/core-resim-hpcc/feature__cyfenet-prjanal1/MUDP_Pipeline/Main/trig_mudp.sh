#!/usr/bin/env bash
echo "[ver-3.0] : Starting Resim Execution"

if [[ $1 == *'/projects/'* ]]; then
    source /mnt/usmidet/projects/GPO-IFV7XX/4-Checkout/Environment/env/bin/activate

    python /mnt/usmidet/projects/GPO-IFV7XX/4-Checkout/Resim_Pipeline/Core_RESIM_HPCC/MUDP_Pipeline/Support/mudp_kpi_main.py "$@"

    deactivate
else
    source /net/8k3/e0fs01/irods/PLKRA-PROJECTS/CEER-PROGRAM/4-Checkout/Environment/env/bin/activate

    module load slurm

    python /net/8k3/e0fs01/irods/PLKRA-PROJECTS/CEER-PROGRAM/4-Checkout/Resim_Pipeline/Core_RESIM_HPCC/MUDP_Pipeline/Support/mudp_kpi_main.py "$@"

    deactivate
fi