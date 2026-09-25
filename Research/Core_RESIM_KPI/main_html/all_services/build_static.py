import os
import re
import shutil
from pathlib import Path

ROOT = Path(__file__).resolve().parent
STATIC_SRC = ROOT / 'main_html' / 'static'
STATIC_DIST = STATIC_SRC / 'dist'


def _minify_js(text: str) -> str:
    text = re.sub(r'//.*', '', text)
    text = re.sub(r'/\*.*?\*/', '', text, flags=re.DOTALL)
    text = re.sub(r'\s+', ' ', text)
    text = re.sub(r'\s*([{}();,:.<>+\-*/%=!|&~^])\s*', r'\1', text)
    return text.strip()


def _minify_css(text: str) -> str:
    text = re.sub(r'/\*.*?\*/', '', text, flags=re.DOTALL)
    text = re.sub(r'\s+', ' ', text)
    text = re.sub(r'\s*([{}:;,>~+=])\s*', r'\1', text)
    text = text.replace(';}', '}')
    return text.strip()


def bundle_js() -> None:
    files = sorted(STATIC_SRC.rglob('*.js'))
    if not files:
        print('No JS files found')
        return
    combined = ''.join(f.read_text(encoding='utf-8') for f in files)
    minified = _minify_js(combined)
    STATIC_DIST.mkdir(parents=True, exist_ok=True)
    (STATIC_DIST / 'bundle.min.js').write_text(minified, encoding='utf-8')
    print(f'JS bundle: {len(files)} files -> {len(minified)} bytes')


def bundle_css() -> None:
    files = sorted(STATIC_SRC.rglob('*.css'))
    if not files:
        print('No CSS files found')
        return
    combined = ''.join(f.read_text(encoding='utf-8') for f in files)
    minified = _minify_css(combined)
    STATIC_DIST.mkdir(parents=True, exist_ok=True)
    (STATIC_DIST / 'style.min.css').write_text(minified, encoding='utf-8')
    print(f'CSS bundle: {len(files)} files -> {len(minified)} bytes')


def clean() -> None:
    if STATIC_DIST.exists():
        shutil.rmtree(STATIC_DIST)
        print('Cleaned dist/')


def main() -> None:
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('action', nargs='?', default='all', choices=['all', 'js', 'css', 'clean'])
    args = parser.parse_args()
    if args.action in ('all', 'clean'):
        clean()
    if args.action in ('all', 'js'):
        bundle_js()
    if args.action in ('all', 'css'):
        bundle_css()


if __name__ == '__main__':
    main()
