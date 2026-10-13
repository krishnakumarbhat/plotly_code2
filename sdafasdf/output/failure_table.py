import os, glob, re

base = r'C:\Users\ouymc2\Desktop\sdafasdf\output'
pairs = sorted([d for d in os.listdir(base) if d.startswith('pair') and os.path.isdir(os.path.join(base, d))])

print('Pair  Combination                                    Det    Aln    Detection Failure              Alignment Failure')
print('=' * 120)
for p in pairs:
    pnum = p.split('_')[0].replace('pair','')
    det_files = glob.glob(base + '/' + p + '/**/*detection_kpi.html', recursive=True)
    aln_files = glob.glob(base + '/' + p + '/**/*alignment_kpi.html', recursive=True)
    
    det_ok = 0
    for hf in det_files:
        c = open(hf, 'r', encoding='utf-8').read()
        if 'Failed' not in c:
            det_ok += 1
    
    aln_ok = 0
    for hf in aln_files:
        c = open(hf, 'r', encoding='utf-8').read()
        if 'Failed' not in c:
            aln_ok += 1
    
    d_reasons = set()
    for hf in det_files:
        c = open(hf, 'r', encoding='utf-8').read()
        if 'Failed' in c:
            m = re.search(r'<p>(.*?)</p>', c)
            if m:
                d_reasons.add(m.group(1))
    
    a_reasons = set()
    for hf in aln_files:
        c = open(hf, 'r', encoding='utf-8').read()
        if 'Failed' in c:
            m = re.search(r'<p>(.*?)</p>', c)
            if m:
                a_reasons.add(m.group(1))
    
    det_str = str(det_ok) + '/' + str(len(det_files)) if det_files else '0/0'
    aln_str = str(aln_ok) + '/' + str(len(aln_files)) if aln_files else '0/0'
    
    det_fail = '; '.join(sorted(d_reasons))[:48] if d_reasons else ''
    aln_fail = '; '.join(sorted(a_reasons))[:48] if a_reasons else ''
    
    if not det_fail and det_ok == 0 and len(det_files) > 0:
        det_fail = 'Error processing'
    if not aln_fail and aln_ok == 0 and len(aln_files) > 0:
        aln_fail = 'Error processing'
    if len(det_files) == 0 and len(aln_files) == 0:
        det_fail = 'NO common sensors (PreRun)'
        aln_fail = 'NO common sensors (PreRun)'
    
    print('{:<5s} {:<42s} {:<7s} {:<7s} {:<30s} {:<30s}'.format(
        pnum, p, det_str, aln_str, det_fail, aln_fail))
    print()

print()
print('=' * 120)
print('ROOT CAUSE SUMMARY')
print('=' * 120)
print('  Pairs 01-20 (CCA/CEER input):')
print('    Detection FAIL: DETECTION_STREAM for FC missing in CCA/CEER files;')
print('      for FL/FR/RL/RR: stream exists but has NO scan_index header -> skipped')
print()
print('  Pairs 21-24 (IFV7XX input, CCA/CEER output):')
print('    Detection FAIL: PreRun excludes FC (IFV7XX has FC/DET but CCA/CEER does not);')
print('      for FL/FR/RL/RR: scan_index mismatch between files -> DET_STREAM skipped')
print()
print('  Pairs 01-24 (ALL):')
print('    Alignment FAIL: DYNAMIC_ALIGNMENT_STREAM not present in ANY file (0/5 files)')
print()
print('  Pair 25 (IFV7XX self-pair):')
print('    Detection PASS: 5/5 sensors have DETECTION_STREAM + RDD_STREAM with scan_index')
print('    Alignment FAIL: DYNAMIC_ALIGNMENT_STREAM not present (as with all files)')
