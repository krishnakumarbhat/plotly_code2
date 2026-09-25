# -*- coding: utf-8 -*-
"""
Created on Created on Tue Apr 15 16:38:57 2024

@author: kfupyw (Megha.Tandur@aptiv.com)
"""

###########################################################

###########################################################



import os
import sys
import csv


def get_staging_dir():
    """
    Directory used to stage the transient .resim_copy_list_*/.resim_source_list_*
    files consumed by the Slurm array/zip jobs. Kept alongside the pipeline scripts
    (not inside the destination path_name) so job-tracking files never show up in
    the Release/deliverable directory, even while jobs are still running.
    """
    staging_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), '.job_staging')
    os.makedirs(staging_dir, exist_ok=True)
    return staging_dir


def find_move_path(src):
    move_paths = []
    for root, dirs, _files in os.walk(src):
        for directory in sorted(dirs):
            if directory.startswith('rR'):
                move_paths.append(os.path.join(root, directory))
    return move_paths


def build_dest_from_move_path(base_dest, move_path):
    """Append the path portion after 'output' from move_path to base_dest."""
    normalized = os.path.normpath(move_path)
    parts = normalized.split(os.sep)

    try:
        output_index = parts.index('output')
        suffix_parts = parts[output_index + 1:]
    except ValueError:
        suffix_parts = []

    if suffix_parts:
        return os.path.join(base_dest, *suffix_parts)
    return base_dest


def read_template_csv(csv_file, path_name):
    """
    Read the Template.csv file and create source-destination-movePath triplets.
    
    Args:
        csv_file (str): Path to the Template.csv file
        path_name (str): Base destination directory
        
    Returns:
        list: List of tuples (src, dest, move_path)
    """
    triplets = []
    try:
        with open(csv_file, 'r') as file:
            reader = csv.DictReader(file)
            for row in reader:
                src = row.get('RunPath', '').strip()
                identifier = row.get('Identifier Name', '').strip()

                if not src or not identifier:
                    print(f"[WARN] : Skipping row with missing RunPath/Identifier Name: {row}")
                    continue

                base_dest = os.path.join(path_name, identifier)
                move_paths = find_move_path(src)

                if not move_paths:
                    final_dest = base_dest
                    final_move = ""
                    all_pairs = [(final_dest, final_move)]
                else:
                    all_pairs = [
                        (build_dest_from_move_path(base_dest, mp), mp)
                        for mp in move_paths
                    ]

                for dest, move_path in all_pairs:
                    os.makedirs(dest, exist_ok=True)
                    triplets.append((src, dest, move_path))
                    print(f"SRC  : {src}")
                    print(f"DEST : {dest}")
                    print(f"MOVE : {move_path}")

            print(f"✓ Successfully read {len(triplets)} valid row(s) from {csv_file}")
            return triplets
    except FileNotFoundError:
        print(f"✗ Error: File '{csv_file}' not found")
        sys.exit(1)
    except Exception as e:
        print(f"✗ Error reading CSV file: {e}")
        sys.exit(1)


def validate_argv():
    # sys.argv[0] is the script name. We need 2 additional arguments.
    if len(sys.argv) != 3:
        print("Error: Invalid number of arguments.")
        print("Usage: python script.py <file.csv> <directory_path>")
        sys.exit(1)

    csv_file = sys.argv[1]
    path_name = sys.argv[2]

    # Validate that the first argument ends with .csv
    if not csv_file.lower().endswith(".csv"):
        print(f"Error: '{csv_file}' is not a CSV file.")
        sys.exit(1)

    # Validate that the second argument is an existing directory
    if not os.path.isdir(path_name):
        print(f"Error: '{path_name}' is not a valid directory path.")
        sys.exit(1)

    print("Arguments validated successfully!")
    return csv_file, path_name


def write_copy_list(triplets, list_path):
    """
    Write src -> move_path -> dest tuples to a file for SLURM array consumption.
    Skips triplets where move_path is empty (no rR* folder found).

    Args:
        triplets (list): List of (src, dest, move_path) tuples
        list_path (str): Output file path

    Returns:
        int: Number of copy pairs written
    """
    count = 0
    with open(list_path, 'w') as f:
        for src, dest, move_path in triplets:
            if not move_path:
                print(f"[WARN] : No move_path for {src}, skipping copy entry")
                continue
            f.write(f"{src}\t{move_path}\t{dest}\n")
            count += 1
    print(f"✓ Written {count} copy pair(s) to {list_path}")
    return count


def get_user_operation():
    prompt = (
        "Select operation:\n"
        "1. File Transfer\n"
        "2. Zip Run Paths\n"
        "3. Both 1 and 2\n"
        "Enter your choice (1/2/3): "
    )

    valid_choices = {"1", "2", "3"}
    while True:
        choice = input(prompt).strip()
        if choice in valid_choices:
            return choice
        print("[ERROR] : Invalid choice. Please enter 1, 2, or 3.")


def write_source_list(triplets, list_path):
    unique_sources = []
    seen_sources = set()

    for src, _dest, _move_path in triplets:
        if src in seen_sources:
            continue
        seen_sources.add(src)
        unique_sources.append(src)

    with open(list_path, 'w') as file_handle:
        for src in unique_sources:
            file_handle.write(f"{src}\n")

    print(f"[INFO] : Written {len(unique_sources)} source path(s) to {list_path}")
    return len(unique_sources)


def submit_copy_job(copy_list, count):
    script = os.path.join(os.path.dirname(__file__), 'Copy_Logs.sh')
    cmd = f"sbatch --parsable -A 8k3p89 -a 1-{count}%20 {script} {copy_list}"
    print(f"[INFO] : Submitting SLURM array: {cmd}")
    return os.popen(cmd).read().strip().split(';')[0]


def cleanup_local_files(files_to_remove):
    """Remove staging file(s) synchronously (no Slurm job was submitted for them),
    then remove the staging directory itself if it is now empty."""
    for f in files_to_remove:
        try:
            os.remove(f)
        except OSError:
            pass

    staging_dir = os.path.dirname(files_to_remove[0])
    try:
        os.rmdir(staging_dir)
    except OSError:
        pass


def submit_cleanup_job(files_to_remove, dependency_job_id):
    """Submit a lightweight job to remove the given file(s) once the copy array job succeeds.

    Also removes the staging directory itself afterwards, if it has become empty
    (i.e. no other concurrent run is still using it).
    """
    rm_targets = " ".join(f'"{f}"' for f in files_to_remove)
    staging_dir = os.path.dirname(files_to_remove[0])
    cleanup_cmd = (
        f"sbatch -A 8k3p89 -p 8k3 -o /dev/null -e /dev/null "
        f"--dependency=afterok:{dependency_job_id} "
        f"--wrap='rm -f {rm_targets}; rmdir --ignore-fail-on-non-empty \"{staging_dir}\"'"
    )
    print(f"[INFO] : Submitting cleanup job: {cleanup_cmd}")
    os.system(cleanup_cmd)


def submit_zip_job(source_list, path_name, dependency_job_id=None):
    post_script = os.path.join(os.path.dirname(__file__), 'PostCopy_Zip.sh')

    if dependency_job_id:
        post_cmd = (
            f"sbatch -A 8k3p89 -p 8k3 --dependency=afterok:{dependency_job_id} "
            f"{post_script} {source_list} {path_name}"
        )
    else:
        post_cmd = f"sbatch -A 8k3p89 -p 8k3 {post_script} {source_list} {path_name}"

    print(f"[INFO] : Submitting post-copy zip job: {post_cmd}")
    os.system(post_cmd)