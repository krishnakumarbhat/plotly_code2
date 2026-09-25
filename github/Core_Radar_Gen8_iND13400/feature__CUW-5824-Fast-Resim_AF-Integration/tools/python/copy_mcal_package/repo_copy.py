"""Utility script for copying MCAL packages between repositories."""
# ...existing code...
import os
import shutil
from typing import Iterable, Optional


def copy_files_by_extensions(
    source_folder: str,
    destination_folder: str,
    extensions: Iterable[str],
    overwrite: bool = False,
    remove_path_component: Optional[str] = None,
    verbose: bool = False,
) -> int:
    """
    Copy files with given extensions from source to destination.

    - extensions: iterable of extensions including the leading dot (e.g. ['.c', '.h'])
    - overwrite: replace existing files when True
    - remove_path_component: if provided, remove any path component equal to this (case-insensitive)
      when building the destination relative path
    - verbose: print copied/skipped files
    Returns number of files copied.
    """
    if not os.path.isdir(source_folder):
        raise FileNotFoundError(f"Source folder does not exist: {source_folder}")

    dest_root = destination_folder
    os.makedirs(dest_root, exist_ok=True)

    ext_set = {ext.lower() for ext in extensions}
    removed_comp = remove_path_component.lower() if remove_path_component else None
    copied = 0

    for root, _, files in os.walk(source_folder):
        rel_path = os.path.relpath(root, source_folder)
        if rel_path == ".":
            new_rel = "."
        else:
            comps = rel_path.split(os.sep)
            if removed_comp:
                comps = [c for c in comps if c.lower() != removed_comp]
            new_rel = os.path.join(*comps) if comps else "."

        dest_dir = os.path.join(dest_root, new_rel) if new_rel != "." else dest_root

        # iterate files and copy only matching extensions; create dest dir lazily
        for fname in files:
            fname_l = fname.lower()
            if not any(fname_l.endswith(ext) for ext in ext_set):
                continue

            src_file = os.path.join(root, fname)
            dest_file = os.path.join(dest_dir, fname)

            os.makedirs(dest_dir, exist_ok=True)

            if os.path.exists(dest_file) and not overwrite:
                if verbose:
                    print(f"Skipped (exists): {dest_file}")
                continue

            shutil.copy2(src_file, dest_file)
            copied += 1
            if verbose:
                print(f"Copied: {dest_file}")

    if verbose:
        print(f"Total files copied: {copied}")
    return copied


# Convenience wrappers for previous behavior
def copy_sdk_folder(
    source_folder: str, destination_folder: str, overwrite: bool = False, verbose: bool = False
) -> int:
    """Copy SDK folder (.c and .h files)."""
    # copy only .c and .h and remove 'Implementation' from rel path
    return copy_files_by_extensions(
        source_folder,
        destination_folder,
        extensions=[".c", ".h"],
        overwrite=overwrite,
        remove_path_component="None",
        verbose=verbose,
    )


# Convenience wrappers for previous behavior
def copy_sip_folder(
    source_folder: str, destination_folder: str, overwrite: bool = False, verbose: bool = False
) -> int:
    """Copy SIP folder (.c and .h files, removes 'Implementation' from path)."""
    # copy only .c and .h and remove 'Implementation' from rel path
    return copy_files_by_extensions(
        source_folder,
        destination_folder,
        extensions=[".c", ".h"],
        overwrite=overwrite,
        remove_path_component="Implementation",
        verbose=verbose,
    )


def copy_sip_gen_folder(
    source_folder: str, destination_folder: str, overwrite: bool = False, verbose: bool = False
) -> int:
    """Copy SIP generator folder (.arxml and .jar files)."""
    # copy only .arxml and .jar
    return copy_files_by_extensions(
        source_folder,
        destination_folder,
        extensions=[".arxml", ".jar"],
        overwrite=overwrite,
        remove_path_component=None,
        verbose=verbose,
    )


# Example usage
if __name__ == "__main__":
    # Copy SDK folder
    source_dir_sdk = (
        r"C:\Gen8\46D_integration_automation\chandra_autobahn-main\R52\chandra-autosar\indie\CMSIS"
    )
    dest_dir_sdk = r"C:\Gen8\46D_integration_automation\Core_Radar_Gen8_iND13400_SDK\sdk\R52\cmsis-drivers\CMSIS\chandra"
    copy_sdk_folder(source_dir_sdk, dest_dir_sdk, overwrite=True, verbose=True)

    # Copy SIP folder & SIP generator folder
    source_dir = r"C:\Gen8\46D_integration_automation\chandra_autobahn-main\R52\chandra-autosar\indie\Components"
    dest_dir_sip = r"C:\Gen8\46D_integration_automation\Core_Radar_Gen8_iND13400_SIP\Components"
    dest_dir_sip_gen = r"C:\Gen8\46D_integration_automation\Core_Radar_Gen8_iND13400\software\r52\autosar\sip\Components"
    copy_sip_folder(source_dir, dest_dir_sip, overwrite=True, verbose=True)
    copy_sip_gen_folder(source_dir, dest_dir_sip_gen, overwrite=True, verbose=True)
# ...existing code...
