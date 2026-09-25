#!/bin/bash

echo "Select the KPI script to run:"
echo "1. Detection KPI"
echo "2. Alignment KPI"
echo "3. Tracker KPI"
read -p "Enter your choice (1/2/3): " choice

case $choice in
    1)
        script_name="detection_matching_kpi_script.py"
        ;;
    2)
        script_name="alignment_matching_kpi_script.py"
        ;;
    3)
        script_name="tracker_matching_kpi_script.py"
        ;;
    *)
        echo "Invalid choice. Exiting."
        exit 1
        ;;
esac

python3 "$script_name" log_path.txt meta_data.txt "mnt/c/Project/Logs/Gen7/v2/ASTAZERO_GEN7v2_TESTING_w19/CDC_Cloud_MaxRangeTest"
