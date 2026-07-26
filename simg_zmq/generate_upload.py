import os, platform, shutil, subprocess, sys, json, hashlib, socket, threading, time
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path, PurePosixPath

ROOT = Path(__file__).resolve().parent
GEN = ROOT / 'generate_upload'
META = ROOT / '.metadata.json'
BUILD_ORDER = ['main_html.simg', 'rag.simg', 'kpi/can/can_kpi.simg', 'kpi/udp/udp_kpi.simg', 'kpi/int_plot/intplot_kpi.simg']

DEF_MAP = {
    'main_html.simg':          ROOT / 'Singularity.def',
    'rag.simg':                ROOT / 'rag' / 'Singularity_RAG.def',
    'kpi/can/can_kpi.simg':    ROOT / 'KPI' / 'can_kpi' / 'can_singularity_KPI.def',
    'kpi/udp/udp_kpi.simg':    ROOT / 'KPI' / 'UDP_KPI' / 'Singularity_KPI.def',
    'kpi/int_plot/intplot_kpi.simg': ROOT / 'KPI' / 'intplot_kpi' / 'singularity_interactiveplot.def',
}

SIMGG_SRC = {
    'main_html.simg':          [ROOT / 'Singularity.def', ROOT / 'main_html', ROOT / 'Hyperlink_tool', ROOT / 'KPI'],
    'rag.simg':                [ROOT / 'rag' / 'Singularity_RAG.def', ROOT / 'rag', ROOT / 'rag' / 'vlm_process.py'],
    'kpi/can/can_kpi.simg':    [ROOT / 'KPI' / 'can_kpi' / 'can_singularity_KPI.def', ROOT / 'KPI' / 'can_kpi'],
    'kpi/udp/udp_kpi.simg':    [ROOT / 'KPI' / 'UDP_KPI' / 'Singularity_KPI.def', ROOT / 'KPI' / 'UDP_KPI', ROOT / 'KPI' / 'intplot_kpi' / 'InteractivePlot'],
    'kpi/int_plot/intplot_kpi.simg': [ROOT / 'KPI' / 'intplot_kpi' / 'singularity_interactiveplot.def', ROOT / 'KPI' / 'intplot_kpi', ROOT / 'KPI' / 'UDP_KPI'],
}

SCRIPTS = ['bundle_common.sh', 'cleanup_memory.sh', 'kpi_runtime_launcher.sh']
BUNDLE_DIRS = ['main_html', 'Hyperlink_tool', 'KPI']


print('generate_upload.py starting...', flush=True)


def _p(*args, **kw):
    """Print with flush=True by default."""
    kw.setdefault('flush', True)
    print(*args, **kw)


def _auto_wsl():
    if platform.system() != 'Windows':
        return False
    # Only re-execute in WSL for 'generate' (needs apptainer)
    # 'upload' and 'deploy' run natively on Windows (has paramiko)
    if 'generate' not in sys.argv:
        return False
    if shutil.which('apptainer') or shutil.which('singularity'):
        return False
    if os.environ.get('GEN_NO_WSL') == '1':
        return False
    wsl = shutil.which('wsl') or shutil.which('wsl.exe')
    if not wsl:
        return False
    try:
        result = subprocess.run([wsl, '-d', 'Ubuntu', '--', 'true'],
                                capture_output=True, timeout=30)
        if result.returncode != 0:
            raise RuntimeError('Ubuntu distro not available')
    except Exception:
        print('WSL found but Ubuntu distro not available, running natively', flush=True)
        return False
    drive = ROOT.drive[0].lower()
    wsl_path = f'/mnt/{drive}{str(ROOT)[2:].replace(chr(92), "/")}/{Path(__file__).name}'
    cmd = [wsl, '-d', 'Ubuntu', '--', 'python3', wsl_path] + sys.argv[1:]
    _p(f'Re-executing in WSL: {" ".join(cmd)}')
    sys.exit(subprocess.call(cmd))


def _run_in_wsl(args=None):
    """Run generate_upload.py in WSL with given args (e.g. ['generate'])."""
    wsl = shutil.which('wsl') or shutil.which('wsl.exe')
    if not wsl:
        return False
    drive = ROOT.drive[0].lower()
    wsl_path = f'/mnt/{drive}{str(ROOT)[2:].replace(chr(92), "/")}/{Path(__file__).name}'
    cmd = [wsl, '-d', 'Ubuntu', '--', 'python3', wsl_path] + (args or ['generate'])
    _p(f'Running in WSL: {" ".join(cmd)}')
    ret = subprocess.call(cmd)
    if ret != 0:
        print(f'ERROR: WSL build failed (exit {ret}). Fix the build error and try again.', flush=True)
        raise SystemExit(1)
    return True


_auto_wsl()


def _sha256(path):
    h = hashlib.sha256()
    with open(path, 'rb') as f:
        for chunk in iter(lambda: f.read(65536), b''):
            h.update(chunk)
    return h.hexdigest()


def _rel_key(path) -> str:
    if isinstance(path, Path):
        return path.as_posix()
    return str(path).replace('\\', '/')


def _read_meta():
    if META.exists():
        try:
            return json.loads(META.read_text())
        except Exception:
            pass
    return {'build': {}, 'upload': {}}


def _save_meta(meta):
    META.write_text(json.dumps(meta, indent=2, sort_keys=True))


_SRC_EXTS = {'.py', '.html', '.htm', '.js', '.css', '.scss', '.def', '.sh', '.json', '.xml', '.yaml', '.yml', '.txt', '.cfg', '.env', '.md', '.spec', '.toml', '.jinja', '.jinja2'}
_DATA_EXTS = {'.h5', '.hdf5', '.gguf', '.mf4', '.csv', '.pyc', '.pyd', '.pyo', '.so', '.log', '.db', '.sqlite', '.simg', '.sif', '.zip', '.tar', '.gz', '.tar.gz', '.tgz', '.bin', '.pt', '.pth', '.onnx', '.npy', '.npz', '.wav', '.mp3', '.mp4', '.avi', '.png', '.jpg', '.jpeg', '.gif', '.ico', '.svg', '.woff', '.woff2', '.ttf', '.eot', '.pdf', '.o', '.a', '.lib', '.dll', '.dylib', '.exe'}

def _is_source_file(f: Path) -> bool:
    return f.suffix.lower() in _SRC_EXTS or (f.suffix.lower() not in _DATA_EXTS and not f.name.startswith('.'))

def _deps_hash(paths):
    h = hashlib.sha256()
    for p in paths:
        if p.is_file():
            if _is_source_file(p):
                h.update(p.read_bytes())
        elif p.is_dir():
            for f in sorted(p.rglob('*')):
                if f.is_file() and _is_source_file(f):
                    h.update(f.read_bytes())
    return h.hexdigest()


_meta_lock = threading.Lock()


def _check_runtime():
    return shutil.which('apptainer') or shutil.which('singularity') or ''


def build_simg(img_rel):
    dst = GEN / img_rel
    with _meta_lock:
        meta = _read_meta()
        deps = SIMGG_SRC.get(img_rel, [])
        current_dep_hash = _deps_hash(deps)
        prev_dep_hash = meta.get('build', {}).get(img_rel, {}).get('deps_hash', '')
        if dst.exists() and current_dep_hash == prev_dep_hash:
            _p(f'  up-to-date: {img_rel}')
            return False
    def_path = DEF_MAP.get(img_rel)
    if not def_path or not def_path.exists():
        alt = ROOT / 'Singularity.def'
        def_path = alt if alt.exists() else None
    if not def_path:
        _p(f'  ERROR: no .def for {img_rel}')
        raise SystemExit(1)
    _p(f'  building {img_rel}...')
    dst.parent.mkdir(parents=True, exist_ok=True)
    runtime = _check_runtime()
    if not runtime:
        _p(f'  ERROR: apptainer/singularity not found — cannot build {img_rel}')
        _p(f'  Run this command from WSL (Ubuntu) where apptainer is installed.')
        _p(f'  Or install WSL: wsl --install -d Ubuntu')
        raise SystemExit(1)
    build_env = os.environ.copy()
    build_env['APPTAINER_TMPDIR'] = '/tmp'
    build_env['APPTAINER_CACHEDIR'] = '/tmp'
    build_env['SINGULARITY_TMPDIR'] = '/tmp'
    build_env['SINGULARITY_CACHEDIR'] = '/tmp'
    # Use gzip compression to avoid mksquashfs SIGSEGV on WSL (zstd default can run out of memory)
    build_env['APPTAINER_SQUASHFS_COMPRESSION'] = 'gzip'
    build_env['SINGULARITY_SQUASHFS_COMPRESSION'] = 'gzip'
    # Build entirely on Linux tmpfs to avoid mksquashfs crash on /mnt/c/ (Windows 9P mount)
    import tempfile as _tf, shutil as _shutil
    with _tf.TemporaryDirectory(dir='/tmp') as _build_dir:
        _linux_root = Path(_build_dir) / 'src'
        _linux_root.mkdir()
        # Copy Singularity.def
        _shutil.copy2(def_path, _linux_root / def_path.name)
        _def_linux = _linux_root / def_path.name
        # Copy all source dependencies and app.py into the Linux temp root.
        # Sort so directories come first (avoids conflicts when a dir dep is
        # followed by an individual file that lives inside that same directory).
        _sorted_deps = sorted(deps, key=lambda d: (0 if d.is_dir() else 1))
        for dep in _sorted_deps:
            _rel = dep.relative_to(ROOT) if dep.is_relative_to(ROOT) else dep.name
            _dest = _linux_root / _rel
            if dep.is_dir():
                _shutil.copytree(dep, _dest, symlinks=True, dirs_exist_ok=True,
                                 ignore=lambda d, f: ['.git', '__pycache__', '.pytest_cache'])
            else:
                _dest.parent.mkdir(parents=True, exist_ok=True)
                _shutil.copy2(dep, _dest)
        _app_py = ROOT / 'app.py'
        if _app_py.exists():
            _shutil.copy2(_app_py, _linux_root / 'app.py')
        _tmp_img = Path(_build_dir) / dst.name
        proc = subprocess.Popen(
            [runtime, 'build', '--fakeroot', '--disable-cache', '--tmpdir', '/tmp',
             '--mksquashfs-args', '-mem 4G -processors 1 -no-exports -no-sparse',
             str(_tmp_img), str(_def_linux)],
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, env=build_env, cwd=str(_linux_root), bufsize=1,
        )
        stdout_lines = []
        for line in proc.stdout:
            _p(f'  [{img_rel}] {line}', end='')
            stdout_lines.append(line)
        proc.wait()
        result_stdout = ''.join(stdout_lines)
        result_stderr = ''
        result_returncode = proc.returncode
        if result_returncode != 0:
            _p(f'  ERROR: {runtime} build failed (exit {result_returncode})')
            if result_stdout:
                _p(f'  stdout:\n{result_stdout}')
            if result_stderr:
                _p(f'  stderr:\n{result_stderr}')
            _p(f'  Check:')
            _p(f'    - Is docker/podman running?  (apptainer needs it to pull the base image)')
            _p(f'    - Is there enough disk space in /tmp?  (build uses squashfs)')
            _p(f'    - Try: wsl --shutdown, then restart your terminal')
            raise SystemExit(1)
        dst.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(_tmp_img, dst)
    _p(f'  done: {img_rel}')
    with _meta_lock:
        meta = _read_meta()
        meta.setdefault('build', {})[img_rel] = {'deps_hash': current_dep_hash}
        _save_meta(meta)
    return True


def build_pyz():
    src = ROOT / 'main_html' / 'temp_dir' / 'hpcc_main.py'
    dst = GEN / 'hpcc_main.pyz'
    dst_py = GEN / 'hpcc_main.py'
    meta = _read_meta()
    current_hash = _sha256(src) if src.exists() else ''
    prev_hash = meta.get('build', {}).get('hpcc_main.pyz', {}).get('src_hash', '')
    if dst.exists() and current_hash and current_hash == prev_hash:
        _p(f'  up-to-date: hpcc_main.pyz')
        return
    _p('  building hpcc_main.pyz...')
    dst.parent.mkdir(parents=True, exist_ok=True)
    try:
        # Build a real ZIP archive compatible with Python 3.6-3.13+
        _build_compatible_zipapp(src, dst)
        print('  built hpcc_main.pyz')
    except Exception as exc:
        print(f'  zipapp build failed ({exc}), falling back to plain .py')
        shutil.copy2(src, dst_py)
        print('  copied hpcc_main.py (fallback)')
    meta.setdefault('build', {})['hpcc_main.pyz'] = {'src_hash': current_hash}
    _save_meta(meta)


def _build_compatible_zipapp(src: Path, dst: Path):
    import zipfile, io
    dst.parent.mkdir(parents=True, exist_ok=True)
    # Build a proper ZIP archive with __main__.py
    buf = io.BytesIO()
    with zipfile.ZipFile(buf, 'w', zipfile.ZIP_DEFLATED) as z:
        z.write(str(src), '__main__.py')
    zip_data = buf.getvalue()
    # Write shebang + ZIP data (compatible with all Python 3 versions)
    shebang = b'#!/usr/bin/env python3\n'
    dst.write_bytes(shebang + zip_data)


def generate():
    runtime = _check_runtime()
    if not runtime:
        _p('ERROR: apptainer/singularity is required')
        print('On Windows, run from WSL (Ubuntu) where apptainer is installed.')
        print('Or install: wsl --install -d Ubuntu')
        raise SystemExit(1)
    built_any = False
    errors = []
    with ThreadPoolExecutor(max_workers=2) as executor:
        f2i = {executor.submit(build_simg, img_rel): img_rel for img_rel in BUILD_ORDER}
        for future in as_completed(f2i):
            img_rel = f2i[future]
            try:
                if future.result():
                    built_any = True
            except BaseException:
                errors.append(img_rel)
    if errors:
        for e in errors:
            _p(f'  FAILED: {e}')
        raise SystemExit(1)
    if not built_any:
        print('  all simgs up-to-date')

    build_pyz()

    for s in SCRIPTS:
        src = ROOT / s
        if src.exists():
            shutil.copy2(src, GEN / s)
            print(f'  copied {s}')

    # Copy rResim_Gen7.sh from project root (parent directory)
    resim_sh_src = ROOT / 'rResim_Gen7.sh'
    if resim_sh_src.exists():
        shutil.copy2(resim_sh_src, GEN / 'rResim_Gen7.sh')
        print('  copied rResim_Gen7.sh')
    else:
        print('  WARNING: rResim_Gen7.sh not found at project root')

    # rResim_Gen7.sh shells out to `xxd -p -r` to decode its embedded hex
    # commands, but the main_html.simg base image (python:3.10-slim) does not
    # ship xxd. Rebuilding/redeploying the 1.2GB image just for one binary is
    # expensive, so drop in a tiny xxd-compatible shim (python3 is always
    # present) and app.py prepends its dir to PATH only for the resim run.
    xxd_shim_dir = GEN / 'bin'
    xxd_shim_dir.mkdir(parents=True, exist_ok=True)
    xxd_shim_path = xxd_shim_dir / 'xxd'
    # newline='\n' is required on Windows: write_text()'s default newline
    # translation turns \n into \r\n, which corrupts the shebang line
    # (`#!/usr/bin/env bash\r`) and makes the container's `env` fail with
    # "env: 'bash\r': No such file or directory".
    xxd_shim_path.write_text(
        '#!/usr/bin/env bash\n'
        '# Minimal drop-in for `xxd -p -r` (hex text -> raw bytes), since the\n'
        '# main_html.simg base image does not ship the real xxd binary.\n'
        'case "$1 $2" in\n'
        '  "-p -r"|"-r -p") exec python3 -c '
        '"import sys,binascii; sys.stdout.buffer.write(binascii.unhexlify(\'\'.join(sys.stdin.read().split())))"\n'
        '    ;;\n'
        '  *) echo "xxd shim: unsupported args: $*" >&2; exit 1 ;;\n'
        'esac\n',
        encoding='utf-8',
        newline='\n',
    )
    xxd_shim_path.chmod(0o755)
    print('  wrote bin/xxd (xxd -p -r shim)')

    for name in BUNDLE_DIRS:
        src = ROOT / name
        dst = GEN / 'bundle_src' / name
        if src.is_dir():
            if dst.exists():
                shutil.rmtree(dst)
            shutil.copytree(src, dst, ignore=shutil.ignore_patterns('__pycache__', '*.pyc', '.git'))
            print(f'  copied bundle_src/{name}/')

    if (ROOT / 'rag').is_dir():
        dst = GEN / 'rag'
        if dst.exists():
            shutil.rmtree(dst)
        shutil.copytree(ROOT / 'rag', dst, ignore=shutil.ignore_patterns('__pycache__', '*.pyc', 'model', 'tools', 'data'))
        print('  copied rag/ (app code only)')

    if (ROOT / 'resources.py').is_file():
        shutil.copy2(ROOT / 'resources.py', GEN / 'resources.py')

    if (ROOT / 'jira').is_dir():
        dst = GEN / 'jira'
        if dst.exists():
            shutil.rmtree(dst)
        shutil.copytree(ROOT / 'jira', dst, ignore=shutil.ignore_patterns('__pycache__', '*.pyc', '.git'))
        print('  copied jira/')

    _bundle_model_dir(ROOT / 'rag' / 'model', GEN / 'rag' / 'model', '*.gguf')

    for sub in ['db', 'logs', 'rag/vector_store']:
        (GEN / 'store' / sub).mkdir(parents=True, exist_ok=True)
    _p('  created store/')

    _write_launcher()
    _fix_shell_scripts()
    _ensure_rag_sh()
    _write_readme()
    _verify_critical_files()
    _p(f'Generate complete: {GEN}')


_CRITICAL_BUNDLE_FILES = [
    'bundle_src/Hyperlink_tool/code/html_online/static/viewer.html',
    'bundle_src/Hyperlink_tool/code/html_online/main.py',
    'bundle_src/main_html/app.py',
    'bundle_src/KPI',
    'run_hpcc.sh',
    'hpcc_main.pyz',
]

def _verify_critical_files() -> None:
    missing = []
    for rel in _CRITICAL_BUNDLE_FILES:
        if not (GEN / rel).exists():
            missing.append(rel)
    if missing:
        print('WARNING: critical files missing in output:')
        for path in missing:
            print(f'  MISSING: {path}')
        print('  Check that source files exist in the workspace before re-running.')
    else:
        print(f'  verified {len(_CRITICAL_BUNDLE_FILES)} critical files')


_MODEL_BINARY_EXT = {'.gguf', '.safetensors', '.bin', '.pt', '.pth'}

def _is_model_binary(path: Path) -> bool:
    return path.suffix in _MODEL_BINARY_EXT


def _bundle_model_dir(src: Path, dst: Path, pattern: str = '*'):
    if not src.exists():
        return
    key = str(dst.relative_to(GEN)) if dst.is_relative_to(GEN) else dst.name

    def _should_ignore(path: Path, base: Path) -> bool:
        for part in path.relative_to(base).parts:
            if part.startswith('.') or part in ('__pycache__',):
                return True
        return False

    if pattern == '*.gguf':
        for f in src.glob(pattern):
            if _should_ignore(f, src):
                continue
            rel = f.name
            target = dst / rel
            if target.exists():
                print(f'  model unchanged: rag/{key}/{rel}')
                continue
            dst.mkdir(parents=True, exist_ok=True)
            shutil.copy2(f, target)
            print(f'  copied model: rag/{key}/{rel}')
    else:
        files = sorted(src.rglob('*'))
        copied_any = False
        for f in files:
            if not f.is_file():
                continue
            if _should_ignore(f, src):
                continue
            if f.suffix in ('.pyc',):
                continue
            rel = f.relative_to(src).as_posix()
            target = dst / rel
            if target.exists():
                continue
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(f, target)
            print(f'  copied model: rag/{key}/{rel}')
            copied_any = True
        if not copied_any:
            print(f'  model unchanged: rag/{key}/')


def _dir_combined_hash(path: Path) -> str:
    h = hashlib.sha256()
    for f in sorted(path.rglob('*')):
        if f.is_file() and not f.name.startswith('.'):
            h.update(f.read_bytes())
    return h.hexdigest()


def _write_launcher():
    src = ROOT / 'main_hpcc.sh'
    if not src.exists():
        print('  WARNING: main_hpcc.sh not found')
        return
    content = src.read_text(encoding='utf-8')
    lines = content.splitlines()
    body = []
    for i, line in enumerate(lines):
        if i == 0 and line.strip().startswith('#!'):
            continue
        if line.strip().startswith('set -euo pipefail'):
            continue
        if line.strip().startswith('set -e'):
            continue
        if line.strip().startswith('SCRIPT_DIR=') and '"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"' in line:
            continue
        body.append(line)
    launcher = f'''#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${{BASH_SOURCE[0]}}")" && pwd)"
LOG_PATH="$SCRIPT_DIR/run_hpcc.log"

echo "[$(date -Iseconds 2>/dev/null || date)] Starting HPCC bundle" | tee -a "$LOG_PATH"

{chr(10).join(body)}
'''
    (GEN / 'run_hpcc.sh').write_text(launcher, encoding='utf-8')
    (GEN / 'run_hpcc.sh').chmod(0o755)
    _p('  wrote run_hpcc.sh')


def _fix_shell_scripts():
    count = 0
    candidates = list(GEN.rglob('*.sh'))
    bin_dir = GEN / 'bin'
    if bin_dir.is_dir():
        candidates.extend(f for f in bin_dir.iterdir() if f.is_file())
    for f in candidates:
        c = f.read_bytes()
        if b'\r\n' in c:
            f.write_bytes(c.replace(b'\r\n', b'\n'))
            count += 1
        st = f.stat()
        if not (st.st_mode & 0o111):
            f.chmod(st.st_mode | 0o111)
    if count:
        print(f'  fixed CRLF: {count} files')


def _ensure_rag_sh():
    rag_sh = GEN / 'rag' / 'run_rag.sh'
    if rag_sh.exists():
        rag_sh.chmod(0o755)
        return
    rag_sh.parent.mkdir(parents=True, exist_ok=True)
    rag_sh.write_text('#!/usr/bin/env bash\nset -euo pipefail\n# RAG auto-start placeholder\necho "RAG service not bundled"\nexit 0\n')
    rag_sh.chmod(0o755)
    _p('  created rag/run_rag.sh (placeholder)')


def _write_readme():
    text = '''# HPCC Runtime Console - Deployment

## Structure
```
deploy_root/
├── run_hpcc.sh            <- Launcher
├── hpcc_main.pyz          <- Broker (TCP 9100)
├── main_html.simg         <- Flask web app (port 5005, auto-increments if taken)
├── rag.simg               <- RAG service (port 5100)
├── kpi/
│   ├── can/can_kpi.simg
│   ├── udp/udp_kpi.simg
│   └── int_plot/intplot_kpi.simg
├── bundle_src/            <- Live source (bind-mounted)
├── store/                 <- Runtime data
├── resources.py           <- Resource allocation
├── jira/                  <- Jira integration
└── README_deploy.md
```

## Usage
1. `python generate_upload.py deploy` on machine with apptainer
2. Copy `generate_upload/` to cluster
3. `cd generate_upload && bash run_hpcc.sh`
4. Open `http://<cluster-ip>:5002/html`

## Resource Allocation
Edit `resources.py` to change partitions, accounts.
- Krakow: partition=plcyf-com, account=RNA-SDV-SRR7
- Southfield: partition=defq, account=radarcore

## RAG Service
- llama.cpp bundled inside rag.simg, GGUF model bind-mounted at runtime
- Port 5100, auto-starts with main_html
'''
    (GEN / 'README_deploy.md').write_text(text, encoding='utf-8')
    _p('  wrote README_deploy.md')


def _save_upload_hashes():
    meta = _read_meta()
    meta.setdefault('upload', {})
    for f in GEN.rglob('*'):
        if f.is_dir() or f.name.startswith('.'):
            continue
        rel = _rel_key(f.relative_to(GEN))
        if _is_model_binary(f):
            meta['upload'][rel] = '__name_only__'
        else:
            meta['upload'][rel] = _sha256(f)
    _save_meta(meta)


def _load_env():
    env_path = ROOT / '.env'
    if not env_path.exists():
        return {}
    env = {}
    for line in env_path.read_text().splitlines():
        line = line.strip()
        if not line or line.startswith('#'):
            continue
        if '=' in line:
            k, v = line.split('=', 1)
            env[k.strip()] = v.strip().strip('"').strip("'")
    return env


def _sftp_ensure_dir(sftp, path, known_dirs=None):
    normalized = str(PurePosixPath(path))
    if known_dirs is not None and normalized in known_dirs:
        return
    parts = normalized.strip('/').split('/')
    for i in range(1, len(parts) + 1):
        p = '/' + '/'.join(parts[:i])
        if known_dirs is not None and p in known_dirs:
            continue
        try:
            sftp.stat(p)
        except FileNotFoundError:
            sftp.mkdir(p)
        if known_dirs is not None:
            known_dirs.add(p)


def upload():
    env = _load_env()
    if not env:
        print('ERROR: .env file not found or empty. Create simg_zmq/.env with:')
        print('  netid=your_netid')
        print('  netid_password=your_password')
        print('  krakow_path=/path/...')
        print('  southfield_path=/path/...')
        raise SystemExit(1)

    netid = env.get('netid', '')
    password = env.get('netid_password', '')
    krakow_path = env.get('krakow_path', '')
    southfield_path = env.get('southfield_path', '')
    host = env.get('host', '') or env.get('HOST', '')
    port = int(env.get('port', '22'))
    timeout_s = int(env.get('sftp_timeout', '120'))

    krakow_host = env.get('krakow_host', '') or env.get('KRAKOW_HOST', '') or '10.214.45.45'
    southfield_host = env.get('southfield_host', '') or env.get('SOUTHFIELD_HOST', '') or '10.192.224.131'

    if not netid or not password or not (krakow_path or southfield_path):
        print('ERROR: .env must set netid, netid_password, and krakow_path and/or southfield_path')
        raise SystemExit(1)

    targets = []
    if krakow_path:
        targets.append(('krakow', krakow_host if southfield_path else (host or krakow_host), krakow_path))
    if southfield_path:
        targets.append(('southfield', southfield_host if krakow_path else (host or southfield_host), southfield_path))

    # Runtime data dirs that must NOT be overwritten (exist cluster-side with real data)
    _RUNTIME_EXCLUDE_DIRS = {'store/db', 'store/logs', 'store/rag/vector_store', 'store'}

    import paramiko
    meta = _read_meta()
    stored = {_rel_key(k): v for k, v in meta.get('upload', {}).items()}
    new_hashes = {}
    changed = 0
    skipped = 0
    excluded = 0

    files_to_upload = []
    eligible_files = []
    for f in sorted(GEN.rglob('*')):
        if f.is_dir() or f.name.startswith('.'):
            continue
        rel = _rel_key(f.relative_to(GEN))
        # Skip runtime data dirs (cluster has real data that must persist)
        if any(rel.startswith(ex + '/') or rel == ex for ex in _RUNTIME_EXCLUDE_DIRS):
            excluded += 1
            continue
        if f.suffix == '.gguf':
            # GGUF files are not uploaded automatically; handled by _gguf_deploy_check()
            excluded += 1
            continue
        if _is_model_binary(f):
            new_hashes[rel] = '__name_only__'
            eligible_files.append((rel, f, '__name_only__'))
            if rel in stored:
                skipped += 1
                continue
            files_to_upload.append((rel, f, '__name_only__'))
            continue
        sha = _sha256(f)
        new_hashes[rel] = sha
        eligible_files.append((rel, f, sha))
        if rel in stored and stored[rel] == sha:
            skipped += 1
            continue
        files_to_upload.append((rel, f, sha))

    if not files_to_upload and meta.get('upload_state') != 'remote':
        print('Upload metadata is not confirmed from a successful remote sync; forcing a full upload once.')
        files_to_upload = eligible_files
        skipped = 0

    total = len(files_to_upload)
    if total == 0:
        print(f'All files unchanged ({skipped} skipped, {excluded} runtime dirs excluded)')
        meta['upload'] = new_hashes
        _save_meta(meta)
        return

    _p(f'Uploading {total} changed files ({skipped} unchanged, {excluded} runtime dirs excluded)...')

    failures = []
    failures_lock = threading.Lock()
    changed_lock = threading.Lock()

    def _progress_callback(target_name, index, total_files, local_path, remote_path, size, started):
        last_logged_at = started
        last_logged_bytes = 0

        def _callback(sent, total_bytes):
            nonlocal last_logged_at, last_logged_bytes
            now = time.perf_counter()
            if sent == total_bytes or sent == 0:
                should_log = True
            else:
                should_log = (now - last_logged_at >= 10.0) or (sent - last_logged_bytes >= 32 * 1024 * 1024)
            if not should_log:
                return
            elapsed = max(now - started, 0.001)
            pct = (sent / total_bytes * 100.0) if total_bytes else 100.0
            rate_mib = (sent / elapsed) / (1024 * 1024)
            print(
                f'[{target_name}] [{index}/{total_files}] PROGRESS '
                f'local={local_path} remote={remote_path} '
                f'sent={sent}/{total_bytes or size}B pct={pct:.1f}% '
                f'elapsed={elapsed:.2f}s rate={rate_mib:.2f}MiB/s',
                flush=True,
            )
            last_logged_at = now
            last_logged_bytes = sent

        return _callback

    def _connect_target(target_name, target_host, remote_root):
        print(f'[{target_name}] Upload target: {netid}@{target_host}:{port}')
        print(f'[{target_name}] Remote root: {remote_root}')
        print(f'[{target_name}] Connecting via SFTP...', end=' ', flush=True)
        sock = socket.create_connection((target_host, port), timeout=timeout_s)
        transport = paramiko.Transport(sock)
        transport.connect(username=netid, password=password)
        transport.set_keepalive(30)
        sftp = paramiko.SFTPClient.from_transport(
            transport,
            window_size=64 * 1024 * 1024,
            max_packet_size=4 * 1024 * 1024,
        )
        sftp.get_channel().settimeout(timeout_s)
        ensured_dirs = {'/'}
        _sftp_ensure_dir(sftp, remote_root, ensured_dirs)
        print('connected')
        return transport, sftp, ensured_dirs

    def _upload_target(target_name, target_host, remote_root):
        nonlocal changed
        transport = None
        sftp = None
        ensured_dirs = None
        try:
            transport, sftp, ensured_dirs = _connect_target(target_name, target_host, remote_root)
            for index, (rel, local_path, sha) in enumerate(files_to_upload, start=1):
                remote_path = str(PurePosixPath(remote_root) / rel)
                remote_dir = str(PurePosixPath(remote_path).parent)
                size = local_path.stat().st_size
                started = time.perf_counter()
                stage = 'ensure_dir'
                print(
                    f'[{target_name}] [{index}/{total}] START '
                    f'local={local_path} remote={remote_path} size={size}B',
                    flush=True,
                )
                try:
                    _sftp_ensure_dir(sftp, remote_dir, ensured_dirs)
                    stage = 'put'
                    sftp.put(
                        str(local_path),
                        remote_path,
                        callback=_progress_callback(target_name, index, total, local_path, remote_path, size, started),
                        confirm=True,
                    )
                    if rel.endswith('.sh') or local_path.suffix.lower() == '.sh' or rel.startswith('bin/'):
                        stage = 'chmod'
                        sftp.chmod(remote_path, 0o755)
                    stage = 'verify'
                    remote_attr = sftp.stat(remote_path)
                    elapsed = time.perf_counter() - started
                    with changed_lock:
                        changed += 1
                    print(
                        f'[{target_name}] [{index}/{total}] DONE '
                        f'local={local_path} remote={remote_path} '
                        f'size={size}B remote_size={getattr(remote_attr, "st_size", "?")}B '
                        f'elapsed={elapsed:.2f}s',
                        flush=True,
                    )
                except Exception as exc:
                    elapsed = time.perf_counter() - started
                    msg = (
                        f'[{target_name}] [{index}/{total}] FAIL stage={stage} '
                        f'local={local_path} remote={remote_path} size={size}B '
                        f'elapsed={elapsed:.2f}s error={exc}'
                    )
                    print(msg, flush=True)
                    with failures_lock:
                        failures.append(msg)
        except Exception as exc:
            msg = f'[{target_name}] FAIL stage=connect remote_root={remote_root} error={exc}'
            print(msg, flush=True)
            with failures_lock:
                failures.append(msg)
        finally:
            if sftp is not None:
                sftp.close()
            if transport is not None:
                transport.close()

    target_threads = []
    for target_name, target_host, remote_root in targets:
        target_thread = threading.Thread(
            target=_upload_target,
            args=(target_name, target_host, remote_root),
            daemon=True,
        )
        target_thread.start()
        target_threads.append(target_thread)

    for target_thread in target_threads:
        target_thread.join()

    if failures:
        print(f'ERROR: {len(failures)} uploads failed:')
        for f in failures:
            print(f'  {f}')
    else:
        meta['upload'] = new_hashes
        meta['upload_state'] = 'remote'
        _save_meta(meta)

    _p(f'Upload complete: {changed} uploaded, {skipped} unchanged, {excluded} runtime dirs excluded')
    if failures:
        raise SystemExit(1)


def _gguf_deploy_check() -> bool:
    """Check GGUF file changes and print manual deploy instructions."""
    gguf_files = sorted(GEN.rglob('*.gguf'))
    if not gguf_files:
        print('  No GGUF files in bundle.')
        return False

    meta = _read_meta()
    changed = []

    for path in gguf_files:
        rel = _rel_key(path.relative_to(GEN))
        current_hash = _sha256(path)
        prev = meta.get('gguf', {}).get(rel, {}).get('sha256', '')
        if current_hash != prev:
            changed.append((rel, path, current_hash))

    for rel, _, h in changed:
        meta.setdefault('gguf', {})[rel] = {'sha256': h}
    for path in gguf_files:
        rel = _rel_key(path.relative_to(GEN))
        if rel not in {c[0] for c in changed}:
            meta.setdefault('gguf', {})[rel] = {'sha256': _sha256(path)}
    _save_meta(meta)

    env = _load_env()
    southfield_base = env.get('southfield_path', '')
    krakow_base = env.get('krakow_path', '')

    print()
    print(f'GGUF file changed: {"YES" if changed else "NO"}')
    print()

    for rel, path, _ in changed:
        print('Local GGUF file:')
        print(f'  {path}')
        print()
        print('Southfield destination:')
        sf_dest = str(PurePosixPath(southfield_base) / 'rag' / 'model' / rel) if southfield_base else 'N/A'
        print(f'  {sf_dest}')
        print()
        print('Krakow destination:')
        kr_dest = str(PurePosixPath(krakow_base) / 'rag' / 'model' / rel) if krakow_base else 'N/A'
        print(f'  {kr_dest}')
        print()

    print('Automatic upload is disabled.')
    print('Please copy the GGUF file manually to the above destination(s).')
    print()
    return bool(changed)


def main():
    mode = sys.argv[1] if len(sys.argv) > 1 else 'deploy'
    if mode == 'generate':
        print('=== Generating deployment folder ===')
        generate()
    elif mode == 'upload':
        print('=== Uploading to cluster ===')
        upload()
    elif mode == 'deploy':
        print('=== Deploy: generate + GGUF check + upload ===')
        if platform.system() == 'Windows':
            print('  Running generate in WSL (apptainer)...')
            _run_in_wsl(['generate'])
            print('  Checking GGUF files...')
            _gguf_deploy_check()
            print('  Running upload from Windows (paramiko)...')
            upload()
        else:
            generate()
            _gguf_deploy_check()
            upload()
    else:
        print(f'Usage: {sys.argv[0]} [generate|upload|deploy]')
        print('  default: deploy (build + assemble + upload)')
        sys.exit(1)


if __name__ == '__main__':
    main()
