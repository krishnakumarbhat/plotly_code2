import os
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent
BUILD_ORDER = ['main_html.simg', 'rag.simg', 'kpi/can/can_kpi.simg', 'kpi/udp/udp_kpi.simg', 'kpi/int_plot/intplot_kpi.simg']
BUNDLE_SRC = ['main_html', 'Hyperlink_tool', 'KPI', 'rag', 'jira']


def build_simg(def_file: Path, output: Path) -> None:
    output.parent.mkdir(parents=True, exist_ok=True)
    if output.exists():
        print(f'  exists: {output.relative_to(ROOT)}')
        return
    bin = shutil.which('apptainer') or shutil.which('singularity')
    if not bin:
        raise RuntimeError('apptainer/singularity not found')
    subprocess.run([bin, 'build', '--fakeroot', str(output), str(def_file)], check=True)


def build_pyz() -> None:
    src = ROOT / 'hpcc_main.py'
    dst = ROOT / 'simg_sh_hpcc' / 'hpcc_main.pyz'
    dst.parent.mkdir(parents=True, exist_ok=True)
    if dst.exists():
        dst.unlink()
    subprocess.run([sys.executable, '-m', 'zipapp', str(src), '--output', str(dst), '--python', '/usr/bin/env python3'], check=True)


def copy_bundle_src(target: Path) -> None:
    for name in BUNDLE_SRC:
        src = ROOT / name
        dst = target / 'bundle_src' / name
        if src.is_dir():
            shutil.copytree(src, dst, dirs_exist_ok=True, ignore=shutil.ignore_patterns('__pycache__', '*.pyc', '.git'))


def create_store_dirs(target: Path) -> None:
    for sub in ['db', 'logs', 'rag/vector_store']:
        (target / 'store' / sub).mkdir(parents=True, exist_ok=True)


def deploy_to(target: str) -> None:
    deploy_root = Path(target)
    print(f'Deploying to {deploy_root}')
    deploy_root.mkdir(parents=True, exist_ok=True)

    for img_rel in BUILD_ORDER:
        src = ROOT / 'simg_sh_hpcc' / img_rel
        dst = deploy_root / img_rel
        if src.exists() and not dst.exists():
            dst.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(src, dst)
            print(f'  copied {img_rel}')

    for f in ['run_hpcc.sh', 'hpcc_main.pyz', 'README_deploy.md']:
        src = ROOT / 'simg_sh_hpcc' / f
        dst = deploy_root / f
        if src.exists() and not dst.exists():
            shutil.copy2(src, dst)

    copy_bundle_src(deploy_root)
    create_store_dirs(deploy_root)
    print('Deploy complete')


def main():
    target = os.environ.get('DEPLOY_TARGET', '')
    mode = sys.argv[1] if len(sys.argv) > 1 else 'all'

    if mode == 'build':
        for img_rel in BUILD_ORDER:
            def_name = img_rel.replace('/', '_').replace('.simg', '')
            def_path = ROOT / f'Singularity_{def_name}.def'
            if not def_path.exists():
                def_path = ROOT / 'Singularity.def'
            build_simg(def_path, ROOT / 'simg_sh_hpcc' / img_rel)
    elif mode == 'pyz':
        build_pyz()
    elif mode == 'deploy' and target:
        deploy_to(target)
    elif mode == 'all':
        build_pyz()
        copy_bundle_src(ROOT / 'simg_sh_hpcc')
        create_store_dirs(ROOT / 'simg_sh_hpcc')
        print(f'Upload-ready at {ROOT / "simg_sh_hpcc"}')
        if target:
            deploy_to(target)
    else:
        print(f'Usage: {sys.argv[0]} [build|pyz|deploy|all]')
        print('  DEPLOY_TARGET=/path/cluster/path to deploy')


if __name__ == '__main__':
    main()
