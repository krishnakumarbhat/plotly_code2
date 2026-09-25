#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
MUDP + UDP_KPI Standalone Pipeline  -  Main Orchestration
Triggered by: ./MUDP_KPI.sh <input_txt_path>

<input_txt_path>  Text file containing one resim output path per line.
                  Each path points to RESULTS-RESIM/YYYY-MM-DD/Timestamp/
"""

from mudp_kpi_staticdata import *


# ====================================================================
#  STEP 1 – Validate command-line arguments
# ====================================================================
def validate_argv():
    """Validate the input .txt file passed on the command line."""
    global input_parameter
    print('[INFO] : Inputs - Validating ', end='\r')

    if len(sys.argv) < 2:
        print(
            '[ERROR] : Missing argument.\n'
            '  Usage: ./MUDP_KPI.sh <input_txt_path>\n'
            '  <input_txt_path> must be a .txt file containing resim output paths.'
        )
        sys.exit(1)

    input_txt = sys.argv[1]

    if not input_txt.endswith('.txt'):
        print(f'[ERROR] : {input_txt} is not a .txt file.')
        sys.exit(1)

    if not os.path.isfile(input_txt):
        print(f'[ERROR] : {input_txt} does not exist.')
        sys.exit(1)

    # Derive SLURM project based on cluster (not from path text)
    if cluster == 'Southfield':
        project = 'GPO-IFV7XX'
    elif cluster == 'Helios':
        # Helios uses a dedicated SLURM account/partition regardless of the
        # customer/project routing used on Southfield/Krakow.
        project = '8k3p89'
    else:  # Krakow
        project = 'CEER-PROGRAM'

    # [0] = orchestrator shell, [1] = input txt, [2] = project
    input_parameter.append(os.path.join(repo_root, 'Shell', 'mudp_kpi_orchestrator.sh'))
    input_parameter.append(input_txt)
    input_parameter.append(project)

    print(f'[INFO] : Inputs - Validated  (project={project})')


# ====================================================================
#  STEP 2 – Create RESULTS-KPI output directories
# ====================================================================
def create_output_dirs():
    """
    Create RESULTS-KPI/{date}/{time}/ at the same level as RESULTS-RESIM.
    Base path is derived from the first resim path listed in the input txt
    (splitting on '/2-Sim/'), which is more robust than relying on cwd.
    Stores the execution path in input_parameter[3].
    """
    global input_parameter

    # Read the first valid resim path from the input txt to anchor base path
    first_resim_path = None
    with open(input_parameter[1], 'r') as fh:
        for line in fh:
            stripped = line.strip().rstrip('/')
            if stripped:
                first_resim_path = stripped
                break

    if not first_resim_path:
        print('[ERROR] : Input txt file is empty – cannot determine output base path.')
        sys.exit(1)

    sim_marker = '/2-Sim/'
    if sim_marker in first_resim_path:
        base_path = first_resim_path.split(sim_marker)[0]
    else:
        # Fallback: derive from cwd using the cluster-specific server token
        path_parts = os.getcwd().split(server)
        if len(path_parts) > 1:
            base_path = path_parts[0] + server + path_parts[1].split('/')[0]
        else:
            base_path = os.getcwd()

    curr_user = getpass.getuser()
    date = str(datetime.datetime.now()).split(' ')[0]
    time = str(datetime.datetime.now()).split(' ')[1].replace(':', '-').replace('.', '-')

    exec_path  = f'{base_path}/2-Sim/USER_DATA/{curr_user}/RESULTS-KPI/{date}/{time}'
    jobout_dir = f'{exec_path}/jobout'
    output_dir = f'{exec_path}/output'

    os.makedirs(jobout_dir)
    os.makedirs(output_dir)

    print(f'[INFO] : Output directories created at:\n         {exec_path}')
    input_parameter.append(exec_path)   # index [3]


# ====================================================================
#  STEP 3 – Collect b05 files from caches and ORCAS .mf4 from output
# ====================================================================
def collect_b05_and_orcas():
    """
    For every resim output path listed in the input .txt:
      1.  Find *b05_cache*.txt inside {path}/jobout/
      2.  Read all b05 log paths from those cache files
      3.  Walk {path}/output/ recursively for all ORCAS directories
          and collect every .mf4 file found inside them.
    Populates all_b05_files and all_orcas_files globals.
    """
    global all_b05_files, all_orcas_files

    with open(input_parameter[1], 'r') as fh:
        resim_paths = [ln.strip().rstrip('/') for ln in fh if ln.strip()]

    print(f'[INFO] : Found {len(resim_paths)} resim path(s) in input file')

    for resim_path in resim_paths:
        if not os.path.isdir(resim_path):
            print(f'[WARN] : Resim path does not exist, skipping: {resim_path}')
            continue

        # ---- collect b05 paths from cache file(s) or SIL session file ----
        jobout_dir = os.path.join(resim_path, 'jobout')
        if not os.path.isdir(jobout_dir):
            print(f'[WARN] : jobout directory not found in: {resim_path}')
            continue

        cache_files = [
            os.path.join(jobout_dir, fn)
            for fn in os.listdir(jobout_dir)
            if 'b05_cache' in fn and fn.endswith('.txt')
        ]

        if cache_files:
            for cache_file in sorted(cache_files):
                with open(cache_file, 'r') as fh:
                    for line in fh:
                        b05_path = line.strip()
                        if b05_path and os.path.isfile(b05_path):
                            all_b05_files.append(b05_path)
                        elif b05_path:
                            print(f'[WARN] : b05 file listed in cache not found: {b05_path}')
            print(f'[INFO] : Collected {len(all_b05_files)} b05 file(s) from {resim_path}')
        else:
            # Fall back to SIL files.
            # IMPORTANT: SIL_input_session.txt / SIL_Input_all.txt only list a
            # SAMPLE of b05 files (e.g. every 15th recording per session) -
            # column 1 is NOT the complete set. Column 3 is the raw session
            # directory (e.g. .../20250617/102329); we must recursively scan
            # that directory for every *_b05.MF4 file to avoid missing logs.
            sil_candidates = ['SIL_input_session.txt', 'SIL_Input_all.txt']
            sil_files_found = []
            for candidate in sil_candidates:
                candidate_path = os.path.join(jobout_dir, candidate)
                if os.path.isfile(candidate_path):
                    sil_files_found.append(candidate_path)

            if not sil_files_found:
                print(f'[WARN] : No b05_cache*.txt or SIL input file found in {jobout_dir}')
            else:
                count_before = len(all_b05_files)
                raw_dirs = set()
                for sil_file in sil_files_found:
                    with open(sil_file, 'r') as fh:
                        for line in fh:
                            line = line.strip()
                            if not line:
                                continue
                            fields = line.split(',')
                            if len(fields) >= 3:
                                raw_dirs.add(fields[2].strip())

                for raw_dir in sorted(raw_dirs):
                    if not os.path.isdir(raw_dir):
                        print(f'[WARN] : Raw session directory not found: {raw_dir}')
                        continue
                    for root, dirs, files in os.walk(raw_dir, followlinks=True):
                        for fn in files:
                            if fn.lower().endswith('_b05.mf4'):
                                b05_path = os.path.join(root, fn)
                                if b05_path not in all_b05_files:
                                    all_b05_files.append(b05_path)

                added = len(all_b05_files) - count_before
                print(f'[INFO] : Collected {added} b05 file(s) by scanning {len(raw_dirs)} raw '
                      f'session dir(s) from {len(sil_files_found)} SIL file(s) in {resim_path}')

        # ---- walk output/ for ORCAS directories and .mf4 files ----
        output_dir = os.path.join(resim_path, 'output')
        if not os.path.isdir(output_dir):
            print(f'[WARN] : output directory not found in: {resim_path}')
            continue

        orcas_found_here = 0
        for root, dirs, files in os.walk(output_dir, followlinks=True):
            # Only collect files that are inside a directory named "ORCAS" (case-insensitive)
            if os.path.basename(root).upper() == 'ORCAS':
                for fn in files:
                    if fn.lower().endswith('.mf4'):
                        all_orcas_files.append(os.path.join(root, fn))
                        orcas_found_here += 1

        print(f'[INFO] : Found {orcas_found_here} ORCAS .mf4 file(s) in {output_dir}')

    print(f'\n[INFO] : Total b05  files collected : {len(all_b05_files)}')
    print(f'[INFO] : Total ORCAS files collected : {len(all_orcas_files)}')

    if not all_b05_files:
        print('[ERROR] : No b05 files found – cannot continue.')
        sys.exit(1)

    if not all_orcas_files:
        print('[ERROR] : No ORCAS .mf4 files found – cannot continue.')
        sys.exit(1)


# ====================================================================
#  STEP 4 – Pair b05 ↔ ORCAS, save orcas_files_all.txt, create
#           per-task iList / oList files in jobout
# ====================================================================
def _get_session_stem(filepath: str) -> str:
    """
    Extract the common recording stem from a filename by stripping the
    file-type tag (_b05, _b04, _g03, _g02, _ORCAS, _orcas) and everything
    after it - e.g. ORCAS outputs append a resim run-id suffix following
    the type tag:
      CEER_S12_20260701_151002_0000_b05.MF4              -> ceer_s12_20260701_151002_0000
      CEER_S12_20260701_151002_0000_b05_r00100012.mf4     -> ceer_s12_20260701_151002_0000
    Returns a lower-case string for case-insensitive comparison.
    """
    name = os.path.splitext(os.path.basename(filepath))[0]
    name = re.sub(r'_(b05|b04|g03|g02|orcas).*$', '', name, flags=re.IGNORECASE)
    return name.lower()


def pair_and_create_lists(batch_size: int = 15):
    """
    1.  Write all discovered ORCAS paths to orcas_files_all.txt.
    2.  Match each b05 file with its ORCAS counterpart by session stem.
    3.  Write master MUDP_iList.txt and MUDP_oList.txt (paired only).
    4.  Split pairs into batches of `batch_size` and write per-task
        {n}_iList.txt / {n}_oList.txt files in jobout.
    5.  Store total task count in input_parameter[4].
    """
    global paired_b05, paired_orcas, input_parameter

    exec_path  = input_parameter[3]
    jobout_dir = os.path.join(exec_path, 'jobout')

    # -- Write orcas_files_all.txt (before pairing) --
    orcas_all_path = os.path.join(jobout_dir, 'orcas_files_all.txt')
    all_orcas_sorted = sorted(all_orcas_files)
    with open(orcas_all_path, 'w') as fh:
        fh.write('\n'.join(all_orcas_sorted) + '\n')
    print(f'[INFO] : All ORCAS files saved to {orcas_all_path}')

    # -- Build orcas lookup keyed by session stem --
    orcas_by_stem: dict[str, str] = {}
    stem_collisions: list[str] = []
    for orcas_path in all_orcas_sorted:
        stem = _get_session_stem(orcas_path)
        if stem in orcas_by_stem:
            stem_collisions.append(stem)
        orcas_by_stem[stem] = orcas_path

    if stem_collisions:
        print(f'[WARN] : {len(stem_collisions)} ORCAS stem collision(s) – last file wins per stem.')

    # -- Pair b05 with ORCAS --
    unmatched_b05: list[str] = []
    for b05_path in sorted(all_b05_files):
        stem = _get_session_stem(b05_path)
        if stem in orcas_by_stem:
            paired_b05.append(b05_path)
            paired_orcas.append(orcas_by_stem[stem])
        else:
            unmatched_b05.append(b05_path)

    print(f'[INFO] : Paired {len(paired_b05)} / {len(all_b05_files)} b05 files with ORCAS outputs')
    if unmatched_b05:
        print(f'[WARN] : {len(unmatched_b05)} b05 file(s) had no ORCAS match:')
        for f in unmatched_b05[:5]:
            print(f'         - {os.path.basename(f)}')
        if len(unmatched_b05) > 5:
            print(f'         ... and {len(unmatched_b05) - 5} more')

    if not paired_b05:
        print('[ERROR] : No paired files after matching – cannot submit any SLURM tasks.')
        sys.exit(1)

    # -- Write master paired lists --
    ilist_master = os.path.join(jobout_dir, 'MUDP_iList.txt')
    olist_master = os.path.join(jobout_dir, 'MUDP_oList.txt')
    with open(ilist_master, 'w') as fi, open(olist_master, 'w') as fo:
        for b05, orcas in zip(paired_b05, paired_orcas):
            fi.write(b05 + '\n')
            fo.write(orcas + '\n')
    print(f'[INFO] : Master lists written:')
    print(f'         {ilist_master}')
    print(f'         {olist_master}')

    # -- Create per-task iList / oList files (batched) --
    total_pairs = len(paired_b05)
    total_tasks = (total_pairs + batch_size - 1) // batch_size  # ceiling division

    for task_num in range(1, total_tasks + 1):
        start_idx = (task_num - 1) * batch_size
        end_idx   = min(start_idx + batch_size, total_pairs)

        task_ilist = os.path.join(jobout_dir, f'{task_num}_iList.txt')
        task_olist = os.path.join(jobout_dir, f'{task_num}_oList.txt')

        with open(task_ilist, 'w') as fi, open(task_olist, 'w') as fo:
            for b05, orcas in zip(paired_b05[start_idx:end_idx],
                                  paired_orcas[start_idx:end_idx]):
                fi.write(b05 + '\n')
                fo.write(orcas + '\n')

        print(f'[INFO] : Task {task_num:>4}/{total_tasks} – '
              f'{end_idx - start_idx} pair(s)  '
              f'[{start_idx + 1} – {end_idx}]',
              end='\r')

    print(f'\n[INFO] : {total_tasks} SLURM task(s) prepared  '
          f'(batch_size={batch_size}, total_pairs={total_pairs})')

    input_parameter.append(total_tasks)   # index [4]


# ====================================================================
#  STEP 5 – Write the SLURM child script to jobout/.mudp_kpi_child.sh
# ====================================================================
def create_child_script():
    """
    Write the SLURM child script (with correct #SBATCH -o/-e paths)
    into jobout/.mudp_kpi_child.sh so it is hidden from ls by default.
    """
    exec_path  = input_parameter[3]
    jobout_dir = os.path.join(exec_path, 'jobout')

    new_out = f'#SBATCH -o {jobout_dir}/%A_%a.out'
    new_err = f'#SBATCH -e {jobout_dir}/%A_%a.out'

    script_lines = mudp_kpi_child_script(config_file).split('\n')
    child_path   = os.path.join(jobout_dir, '.mudp_kpi_child.sh')

    with open(child_path, 'w') as fh:
        for line in script_lines:
            if 'PLACEHOLDER_OUT' in line and line.strip().startswith('#SBATCH'):
                fh.write(new_out + '\n')
            elif 'PLACEHOLDER_ERR' in line and line.strip().startswith('#SBATCH'):
                fh.write(new_err + '\n')
            else:
                fh.write(line + '\n')

    os.chmod(child_path, 0o755)
    print(f'[INFO] : SLURM child script written to {child_path}')


# ====================================================================
#  Entry point
# ====================================================================
if __name__ == '__main__':
    print(banner)
    validate_argv()
    create_output_dirs()
    collect_b05_and_orcas()
    pair_and_create_lists(batch_size=15)
    create_child_script()

    print(f'\n[INFO] : Output path  : {input_parameter[3]}')
    print(f'[INFO] : Total tasks  : {input_parameter[4]}')
    print(f'[INFO] : Launching orchestrator ...\n')

    # Call orchestrator shell: <shell> <csv_of_all_params>
    cmd = f'{input_parameter[0]} {",".join(str(x) for x in input_parameter)}'
    os.system(cmd)
