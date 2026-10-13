import sys, os
sys.path.insert(0, r'C:\Users\ouymc2\Desktop\sdafasdf\output\KPI')
sys.path.insert(0, r'C:\Users\ouymc2\Desktop\sdafasdf\output\KPI\intplot_kpi')

from UDP_KPI.a_persistence_layer.hdf_wrapper import parse_for_kpi
from UDP_KPI.runtime_utils import ensure_dir
from InteractivePlot.b_persistence_layer.prerun_hdf_parser import PreRun

base = r'C:\Users\ouymc2\Desktop\sdafasdf\output'
files = [
    'CCA_9010_3100099_DEBUG_20260612_162336_0023_r00100012.h5',
    'CCA_9010_3100099_DEBUG_20260612_170521_0190.h5',
    'CCA_9010_3100099_DEBUG_20260708_144737_0000.h5',
    'IFV7XX_WBATR95060NC89209_20250919_080721_0000_b05.h5',
]
fpaths = [os.path.join(base, f) for f in files]
short = ['CCA_162336','CCA_170521','CCA_144737','IFV7XX_b05']

results = []
for i_in in range(4):
    for i_out in range(4):
        pair_num = i_in * 4 + i_out + 1
        tag = 'pair{:02d}_{}_vs_{}'.format(pair_num, short[i_in], short[i_out])
        out_dir = ensure_dir(os.path.join(base, tag))
        inp = fpaths[i_in]
        out = fpaths[i_out]
        print('--- {} ---'.format(tag))
        try:
            prerun = PreRun(inp, out)
            sensors = prerun.sensor_list or []
            if not sensors:
                results.append((tag, 'NO_SENSORS', ''))
                continue
            for sensor in sensors:
                html = parse_for_kpi(sensor, inp, out_dir, tag, 'KPI', out)
                print('  {}: {}'.format(sensor, html))
            results.append((tag, 'OK', out_dir))
        except Exception as e:
            print('  ERROR: {}'.format(e))
            results.append((tag, 'ERROR: {}'.format(e), ''))

print()
print('=' * 110)
print('{:<45s} {:<35s} {:<30s}'.format('Combination', 'Status', 'Output Dir'))
print('=' * 110)
for tag, status, out_dir in results:
    print('{:<45s} {:<35s} {:<30s}'.format(tag, str(status), str(out_dir)))
print('=' * 110)
