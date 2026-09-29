# -*- mode: python ; coding: utf-8 -*-

import os
from PyInstaller.utils.hooks import collect_submodules

block_cipher = None

project_root = os.path.abspath(os.getcwd())

# Bundle the default config + sample HDFs (optional but makes the dist runnable out-of-box).
# If your kpi.json references other absolute paths, pass a config path at runtime instead.
datas = [
    (os.path.join(project_root, "kpi.json"), "."),
]

hdf_db_dir = os.path.join(project_root, "hdf_db")
if os.path.isdir(hdf_db_dir):
    # PyInstaller expects hook-style 2-tuples: (source_dir_or_glob, target_dir)
    datas.append((hdf_db_dir, "hdf_db"))

# Plotly sometimes needs explicit submodules in frozen apps.
hiddenimports = [
    "h5py",
    "numpy",
    "plotly",
    "plotly.graph_objects",
    "plotly.express",
    "plotly.offline",
    "plotly.subplots",
]

# If you hit missing-module errors at runtime, uncomment the next line.
# hiddenimports += collect_submodules("plotly")


a = Analysis(
    [os.path.join(project_root, "kpi_main.py")],
    pathex=[project_root],
    binaries=[],
    datas=datas,
    hiddenimports=hiddenimports,
    hookspath=[],
    hooksconfig={},
    runtime_hooks=[],
    excludes=[],
    win_no_prefer_redirects=False,
    win_private_assemblies=False,
    cipher=block_cipher,
    noarchive=False,
)

pyz = PYZ(a.pure, a.zipped_data, cipher=block_cipher)

exe = EXE(
    pyz,
    a.scripts,
    a.binaries,
    a.zipfiles,
    a.datas,
    [],
    name="can_kpi",
    debug=False,
    bootloader_ignore_signals=False,
    strip=False,
    upx=False,
    upx_exclude=[],
    runtime_tmpdir=None,
    console=True,
    disable_windowed_traceback=False,
    argv_emulation=False,
    target_arch=None,
    codesign_identity=None,
    entitlements_file=None,
)

coll = COLLECT(
    exe,
    a.binaries,
    a.zipfiles,
    a.datas,
    strip=False,
    upx=False,
    upx_exclude=[],
    name="can_kpi",
)
