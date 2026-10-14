# -*- mode: python ; coding: utf-8 -*-

block_cipher = None

a = Analysis(
    ['ResimHTMLReport.py'],
    pathex=['.'],
    binaries=[],
    datas=[('ConfigInteractivePlots.xml', '.')],
    hiddenimports=[
        'InteractivePlot.d_business_layer.data_cal',
        'InteractivePlot.kpi_client.hdf_add_pb2',
        'InteractivePlot.kpi_client.hdf_pb2',
        'google.protobuf',
    ],
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
    name='ResimHTMLReport',
    debug=False,
    bootloader_ignore_signals=False,
    strip=False,
    upx=True,
    upx_exclude=[],
    runtime_tmpdir=None,
    console=True,
    disable_windowed_traceback=False,
)
