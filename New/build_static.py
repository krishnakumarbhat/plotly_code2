"""Build bundled static assets for the main_html Flask UI."""

from __future__ import annotations

import hashlib
import json
from pathlib import Path
from typing import Callable


ROOT = Path(__file__).resolve().parent
MAIN_HTML_ROOT = ROOT / 'main_html'
STATIC_ROOT = MAIN_HTML_ROOT / 'static'
DIST_ROOT = STATIC_ROOT / 'dist'


def _read_text(path: Path) -> str:
    return path.read_text(encoding='utf-8')


def _load_css_minifier() -> Callable[[str], str]:
    try:
        import rcssmin  # type: ignore
    except ImportError:
        return lambda text: text
    return rcssmin.cssmin


def _load_js_minifier() -> Callable[[str], str]:
    try:
        import rjsmin  # type: ignore
    except ImportError:
        return lambda text: text
    return rjsmin.jsmin


def _bundle_text(output_path: Path, sources: list[Path], minify: Callable[[str], str]) -> dict[str, int | str]:
    combined = '\n'.join(_read_text(source) for source in sources)
    bundled = minify(combined)
    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(bundled, encoding='utf-8')
    digest = hashlib.sha256(bundled.encode('utf-8')).hexdigest()[:12]
    return {
        'hash': digest,
        'bytes': output_path.stat().st_size,
        'sources': [str(source.relative_to(STATIC_ROOT)).replace('\\', '/') for source in sources],
    }


def main() -> None:
    css_minify = _load_css_minifier()
    js_minify = _load_js_minifier()

    manifest = {
        'css/dist/app.min.css': _bundle_text(
            DIST_ROOT / 'app.min.css',
            [STATIC_ROOT / 'css' / 'style.css'],
            css_minify,
        ),
        'js/dist/app.min.js': _bundle_text(
            DIST_ROOT / 'app.min.js',
            [
                STATIC_ROOT / 'js' / 'main.js',
                STATIC_ROOT / 'js' / 'file_browser.js',
            ],
            js_minify,
        ),
    }

    (DIST_ROOT / 'asset-manifest.json').write_text(
        json.dumps(manifest, indent=2, sort_keys=True),
        encoding='utf-8',
    )

    print('Built bundled assets:')
    for asset_name, metadata in manifest.items():
        print(f'  {asset_name} ({metadata["bytes"]} bytes, hash={metadata["hash"]})')


if __name__ == '__main__':
    main()