import sys, os, glob, json, re
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
    'CEER_S12_20260701_151002_0000_b05.h5',
    'IFV7XX_WBATR95060NC89209_20250919_080721_0000_b05.h5',
]
fpaths = [os.path.join(base, f) for f in files]
short = ['CCA_162336','CCA_170521','CCA_144737','CEER_S12','IFV7XX_b05']

# Clean old pair dirs
for d in os.listdir(base):
    if d.startswith('pair') and os.path.isdir(os.path.join(base, d)):
        import shutil
        shutil.rmtree(os.path.join(base, d))

rows = []
for i_in in range(5):
    for i_out in range(5):
        pair_num = i_in * 5 + i_out + 1
        tag = 'pair{:02d}_{}_vs_{}'.format(pair_num, short[i_in], short[i_out])
        out_dir = ensure_dir(os.path.join(base, tag))
        inp = fpaths[i_in]
        out = fpaths[i_out]
        print('--- {} ---'.format(tag))
        try:
            prerun = PreRun(inp, out)
            sensors = prerun.sensor_list or []
            if not sensors:
                rows.append({'pair':pair_num, 'tag':tag, 'in':short[i_in], 'out':short[i_out],
                             'sensors_found':0, 'sensors_ok':0, 'sensors_align_ok':0,
                             'det_files':[], 'aln_files':[], 'accuracies':[]})
                continue
            det_ok = 0; aln_ok = 0; accuracies = []
            for sensor in sensors:
                try:
                    html = parse_for_kpi(sensor, inp, out_dir, tag, 'KPI', out)
                    if html and os.path.exists(html):
                        content = open(html, 'r', encoding='utf-8').read()
                        if 'Failed' not in content and 'plotly' in content:
                            aln_ok += 1
                        # Also check detection_kpi.html in same dir
                        det_path = html.replace('_alignment_kpi', '_detection_kpi')
                        if os.path.exists(det_path):
                            det_content = open(det_path, 'r', encoding='utf-8').read()
                            if 'Failed' not in det_content and 'plotly' in det_content:
                                det_ok += 1
                                # Extract accuracy
                                m = re.search(r'Accuracy[:\s]*([\d.]+)%', det_content)
                                if m:
                                    accuracies.append({'sensor':sensor, 'accuracy':float(m.group(1))})
                                else:
                                    m2 = re.search(r'detection_accuracy_percentage[":\s]+([\d.]+)', det_content)
                                    if m2:
                                        accuracies.append({'sensor':sensor, 'accuracy':float(m2.group(1))})
                                    else:
                                        accuracies.append({'sensor':sensor, 'accuracy':'present'})
                except Exception as e:
                    print('  {} ERROR: {}'.format(sensor, e))
            rows.append({'pair':pair_num, 'tag':tag, 'in':short[i_in], 'out':short[i_out],
                         'sensors_found':len(sensors), 'det_ok':det_ok, 'aln_ok':aln_ok,
                         'accuracies':accuracies})
        except Exception as e:
            rows.append({'pair':pair_num, 'tag':tag, 'in':short[i_in], 'out':short[i_out],
                         'sensors_found':0, 'det_ok':0, 'aln_ok':0, 'accuracies':[], 'error':str(e)})

# Print results
print()
print('=' * 160)
print('RESULTS: 25 Pairs (5x5)')
print('=' * 160)
print('{:<5s} {:<40s} {:<12s} {:<12s} {:<10s} {:<10s} {:<60s}'.format(
    'Pair', 'Combination', 'Input', 'Output', 'Det_OK', 'Aln_OK', 'Detection Accuracy (per-sensor)'))
print('-' * 160)
for r in rows:
    acc_str = ', '.join(['{}: {}%'.format(a['sensor'], a['accuracy']) if isinstance(a['accuracy'], float) else '{}: {}'.format(a['sensor'], a['accuracy']) for a in r.get('accuracies',[])])
    if 'error' in r:
        acc_str = 'ERROR: ' + r['error']
    print('{:<5d} {:<40s} {:<12s} {:<12s} {:<10s} {:<10s} {:<60s}'.format(
        r['pair'], r['tag'], r['in'], r['out'],
        str(r.get('det_ok',0)) + '/5', str(r.get('aln_ok',0)) + '/5', acc_str))
print('=' * 160)

# Save as JSON
json_path = os.path.join(base, 'udp_kpi_25_results.json')
json.dump(rows, open(json_path, 'w', encoding='utf-8'), indent=2)
print('Results saved to: ' + json_path)
