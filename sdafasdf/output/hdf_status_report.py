import os, h5py

base = r'C:\Users\ouymc2\Desktop\sdafasdf\output'

# MF4 -> HDF conversion from logs
print('=' * 110)
print('MF4 to HDF Conversion Status')
print('=' * 110)
print('{:<55s} {:<30s} {:<25s}'.format('MF4 File', 'HDF Output', 'Status'))
print('-' * 110)

# From log4: SUCCESS but weird duration
print('{:<55s} {:<30s} SUCCESS (duration=1782911401s, possible corrupt log)'.format(
    'CEER_S12_20260701_151002_0000_b05_rR00100012.MF4',
    'CEER_S12_20260701_151002_0000_b05.h5'))
# From log5: SUCCESS with H5 errors
print('{:<55s} {:<30s} SUCCESS but H5Dclose FAILED at end (data may be corrupt)'.format(
    'CEER_S12_20260701_151002_0000_b05.MF4',
    'CEER_S12_20260701_151002_0000_b05.h5'))
# From log6: FAILED
print('{:<55s} {:<30s} FAILED - un-finalized MF4 file'.format(
    'IFV7XX_WBATR95060NC89209_20250919_080721_0000_b05_r00100012.mf4',
    '(none)'))
# Unknown sources
print('{:<55s} {:<30s} SUCCESS (no log in output dir)'.format(
    '(unknown MF4)',
    'CCA_9010_3100099_DEBUG_20260612_162336_0023_r00100012.h5'))
print('{:<55s} {:<30s} SUCCESS (no log in output dir)'.format(
    '(unknown MF4)',
    'CCA_9010_3100099_DEBUG_20260612_170521_0190.h5'))
print('{:<55s} {:<30s} SUCCESS (no log in output dir)'.format(
    '(unknown MF4)',
    'CCA_9010_3100099_DEBUG_20260708_144737_0000.h5'))
print('{:<55s} {:<30s} SUCCESS (no log in output dir, may be different MF4 than r00100012)'.format(
    '(unknown MF4)',
    'IFV7XX_WBATR95060NC89209_20250919_080721_0000_b05.h5'))

print()

# HDF Quality per sensor
print('=' * 150)
print('HDF File - Per-Sensor Content Quality')
print('=' * 150)

h5files = sorted([f for f in os.listdir(base) if f.endswith('.h5')])
header = '{:<52s}'.format('File')
for sensor in ['FC','FL','FR','RL','RR']:
    header += '  {:<30s}'.format(sensor)
print(header)
print('-' * 150)

for f in h5files:
    fp = os.path.join(base, f)
    size_mb = os.path.getsize(fp) / (1024*1024)
    line = '{:<52s}'.format('{} ({:.0f}MB)'.format(f, size_mb))
    h = h5py.File(fp, 'r')
    for sensor in ['FC','FL','FR','RL','RR']:
        if sensor not in h:
            line += '  {:<30s}'.format('MISSING')
            continue
        streams = list(h[sensor].keys())
        det = 'DET' if 'DETECTION_STREAM' in streams else '--'
        rdd = 'RDD' if 'RDD_STREAM' in streams else '--'
        aln = 'ALN' if 'DYNAMIC_ALIGNMENT_STREAM' in streams else '--'
        trk = 'TRK' if 'TRACKER_STREAM' in streams else '--'
        vse = 'VSE' if 'VSE_STREAM' in streams else '--'
        cdc = 'CDC' if 'CDC_STREAM' in streams else '--'
        ds  = 'DS' if 'DOWN_SELECTION_STREAM' in streams else '--'
        # Check scan_index in DETECTION_STREAM
        si = '?'
        if 'DETECTION_STREAM' in streams:
            grp = h[sensor + '/DETECTION_STREAM']
            hdrs = [k for k in grp.keys() if 'hdr' in k.lower() or 'header' in k.lower()]
            for hdr in hdrs:
                if 'scan_index' in grp[hdr]:
                    si = 'S'
                    break
            if 'scan_index' in grp:
                si = 'S'
            if si == '?':
                si = 'X'
        tag = '{}-{}-{}-{}-{}-{}-{} si={}'.format(det, rdd, aln, trk, vse, cdc, ds, si)
        line += '  {:<30s}'.format(tag)
    print(line)
    h.close()

print('-' * 150)
print('Legend: DET=DETECTION_STREAM  RDD=RDD_STREAM  ALN=DYNAMIC_ALIGNMENT_STREAM')
print('        TRK=TRACKER_STREAM     VSE=VSE_STREAM  CDC=CDC_STREAM  DS=DOWN_SELECTION_STREAM')
print('        si=S=has scan_index  si=X=stream exists but NO scan_index')
print()

# Summary of issues
print('=' * 80)
print('Summary of Issues Per HDF File')
print('=' * 80)
for f in h5files:
    fp = os.path.join(base, f)
    h = h5py.File(fp, 'r')
    issues = []
    for sensor in ['FC','FL','FR','RL','RR']:
        if sensor not in h:
            issues.append(sensor + ' sensor group MISSING')
            continue
        streams = list(h[sensor].keys())
        if 'DETECTION_STREAM' not in streams:
            issues.append(sensor + ' missing DETECTION_STREAM')
        else:
            grp = h[sensor + '/DETECTION_STREAM']
            hdrs = [k for k in grp.keys() if 'hdr' in k.lower() or 'header' in k.lower()]
            has_si = False
            for hdr in hdrs:
                if 'scan_index' in grp[hdr]:
                    has_si = True
                    break
            if 'scan_index' in grp:
                has_si = True
            if not has_si:
                issues.append(sensor + ' DETECTION_STREAM missing scan_index')
        if 'DYNAMIC_ALIGNMENT_STREAM' not in streams:
            issues.append(sensor + ' missing DYNAMIC_ALIGNMENT_STREAM')
        if 'TRACKER_STREAM' not in streams:
            issues.append(sensor + ' missing TRACKER_STREAM')
    if not issues:
        issues.append('ALL STREAMS PRESENT')
    print('{}:'.format(f))
    for issue in issues:
        print('  - ' + issue)
    print()
    h.close()
