"""
Check if the Coverity intermediate directory contains translation units.
"""

from cov_cli.cov_config import Config
from cov_cli.cov_util import (
    CONFIG_FILE,
    VERSION,
    URL,
    add_dir_flag,
    die,
    find_file_recursive,
    parse_toml_config,
)
from cov_cli.cov_cli import exit_on_empty_idir
import argparse

import os


def main():
    """Check if the Coverity intermediate directory contains translation units."""
    # try to find config file first.
    toml_path = find_file_recursive(CONFIG_FILE)
    if not toml_path:
        path1 = os.path.join("coverity", CONFIG_FILE)
        if os.path.exists(path1):
            toml_path = path1
        elif os.path.exists(CONFIG_FILE):
            toml_path = CONFIG_FILE
        else:
            die(
                f"""

Can not find {CONFIG_FILE}.
Tool version: {VERSION}

Create {CONFIG_FILE} in one of the following locations:
    - <{os.getcwd()}>/{CONFIG_FILE} or all of its parents.
    - <ROOT>/{CONFIG_FILE}

See {URL} for examples."""
            )

    content = ""
    try:
        with open(toml_path) as r:
            content = r.read()
    except Exception as e:
        die(
            f"""
        Can not read TOML config: {toml_path}
        Reason: {e}
        """
        )

    tomldata = parse_toml_config(content)
    config = Config(tomldata, toml_path)

    parser = argparse.ArgumentParser()

    parser.add_argument(
        "--no-fail-on-empty-idir",
        dest="fail_on_empty_idir",
        default=True,
        action="store_false",
        required=False,
        help="If set, most commands will fail with exit code 1 in case of empty idir",
    )

    add_dir_flag(parser)

    args, unknown = parser.parse_known_args()
    args.remaining = unknown

    config.update_from_args(args)

    exit_on_empty_idir(config, args)


if __name__ == "__main__":
    main()
