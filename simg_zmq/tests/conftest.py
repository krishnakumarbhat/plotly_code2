"""Make simg_zmq test imports resolve without PYTHONPATH hacks.

The tests import top-level modules (`generate_upload`, `main_html.*`,
`hpcc_main`) that live inside the simg_zmq package directory. Insert the
relevant roots on sys.path so `pytest simg_zmq/tests` works out of the box.
"""

from pathlib import Path
import sys

_ROOT = Path(__file__).resolve().parent.parent
for _sub in ("", "main_html", "main_html/temp_dir"):
    _p = str(_ROOT / _sub)
    if _p not in sys.path:
        sys.path.insert(0, _p)