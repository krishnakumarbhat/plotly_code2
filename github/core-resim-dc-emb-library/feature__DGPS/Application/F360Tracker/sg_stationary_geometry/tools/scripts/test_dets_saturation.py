import glob
import pandas as pd
import os
from pathlib import Path
from aspe.extractors.API.keg import extract_env_from_keg


keg_directory = r"C:\Logs\SG_Stationary_Geometries\KPI_compare\test_tmp"
keg_list = glob.glob(f"{keg_directory}/*.keg")
data = pd.DataFrame(columns=['age', 'count', '[%]'])

for keg_path in keg_list:
    extracted_data = extract_env_from_keg(keg_path, False, False, False, )

    internal_dets = extracted_data.stationary_geometries_detections_internal.signals
    det_age = extracted_data.stationary_geometries_detections_internal.signals.age
    min_det_age_value = min(det_age)
    max_det_age_value = max(det_age)

    dets_goupped_by_scn_idx = internal_dets.groupby(['scan_index'])

    dets_num_in_scan = dets_goupped_by_scn_idx.age.count()

    dets_goupped_by_age = dets_goupped_by_scn_idx.age.value_counts()
    dets_goupped_by_age_normalize = dets_goupped_by_scn_idx.age.value_counts(normalize=True)

    idx_age = dets_goupped_by_age.index.values

    dets_age_in_scan_idxs = pd.DataFrame(0, index=range(0, len(dets_goupped_by_scn_idx)), columns=range(1, max_det_age_value+1))
    dets_age_in_scan_idxs_norm = pd.DataFrame(0, index=range(0, len(dets_goupped_by_scn_idx)), columns=range(1, max_det_age_value+1))

    for idx in range(0, len(idx_age)):
        dets_age_in_scan_idxs.loc[idx_age[idx]] = dets_goupped_by_age.values[idx]
        dets_age_in_scan_idxs_norm.loc[idx_age[idx]] = dets_goupped_by_age_normalize.values[idx]

    dets_age_in_scan_idxs['count'] = dets_num_in_scan

    filename = os.path.basename(keg_path)
    new_filename = Path(filename).stem + ".xlsx"

    with pd.ExcelWriter(new_filename) as writer:
        dets_age_in_scan_idxs.to_excel(writer, sheet_name='age numbers', index=False)
        dets_age_in_scan_idxs_norm.to_excel(writer, sheet_name='age normalized %', index=False)

    det_age_data = pd.DataFrame(columns=["count", "[%]", "age"])
    det_age_data['count'] = det_age.value_counts()
    det_age_data['[%]'] = det_age.value_counts(normalize=True)
    det_age_data['age'] = pd.Series(range(len(det_age_data['count'])+1))

    data = pd.concat([data, pd.concat([pd.Series(keg_path), det_age_data])], axis=0)

# Export the DataFrame to an Excel file
output_file_name = 'summary.xlsx'
with pd.ExcelWriter(output_file_name) as writer:
    data.to_excel(writer, sheet_name='summary', index=False)
