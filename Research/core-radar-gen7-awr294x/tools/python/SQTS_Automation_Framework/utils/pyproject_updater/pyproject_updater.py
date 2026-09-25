"""
This module contains a command-line application to update pyproject.toml, either by
specifying specific entries to update, or by providing another toml file.
"""
from pathlib import Path
import json
import argparse
import tomlkit
import re

from toml_merger import merge_toml_item, update_toml_entry

THIS_DIR = Path(__file__).parent
SQTS_DIR = THIS_DIR.parent.parent


def get_cli_arguments() -> argparse.Namespace:
    """
    Function to parse arguments passed to the script.

    Returns:
        A namespace whose attributes are the parsed contents
        of the passed arguments.
    """
    cli_args_parser = argparse.ArgumentParser(
        prog='Updater for pyproject.toml',
        description='Update pyproject.toml.'
    )

    cli_args_parser.add_argument(
        '--pyproject',
        metavar='PYPROJECT_TOML',
        type=str,
        action='store',
        default=SQTS_DIR / 'pyproject.toml',
        help='The path of the pyproject.toml file to update.'
    )

    cli_args_parser.add_argument(
        '--merge_strategy',
        metavar='MERGE_STRATEGY',
        type=str,
        action='store',
        default=THIS_DIR / 'merge_strategy.json',
        help='The path to the json describing the merge strategy in case a source toml is provided.'
    )

    source_commands = cli_args_parser.add_mutually_exclusive_group(required=True)

    source_commands.add_argument(
        '--source_toml',
        metavar='SOURCE_TOML',
        type=str,
        action='store',
        default=None,
        help="toml file to use for updating pyproject.toml"
    )

    source_commands.add_argument(
        '-e',
        '--entry',
        metavar=('TOML_ENTRY', 'ENTRY_VALUE'),
        type=str,
        nargs=2,
        default=[],
        action='append',
        help='Entry to update in pyproject.toml.'
    )

    return cli_args_parser.parse_args()


def main(args: argparse.Namespace) -> None:
    """
    Main function to update pyproject.toml.

    Args:
        args: A namespace containing the required fields to update pyproject.toml.
    """
    # Verify that the pyproject.toml file to update exists
    base_toml = Path(args.pyproject)
    if not base_toml.exists():
        raise FileNotFoundError(f'Specified pyproject file does not exist: {base_toml}.')

    # Load contents of pyproject.toml file to update
    with base_toml.open('rt', encoding='utf-8') as fp:
        base = tomlkit.load(fp)

    if args.source_toml is not None:  # Updating by using another toml file
        # Verify that the source toml file exists
        source_toml = Path(args.source_toml)
        if not source_toml.exists():
            raise FileNotFoundError(f'Specified source toml does not exist: {source_toml}.')

        # Verify that the merge strategy file to use exists
        merge_rules_json = Path(args.merge_strategy)
        if not merge_rules_json.exists():
            raise FileNotFoundError(f'Specified merge strategy json file does not exist: {merge_rules_json}.')

        # Load contents of source toml
        with source_toml.open('rt', encoding='utf-8') as fp:
            source = tomlkit.load(fp)

        # Load merge strategy
        with merge_rules_json.open('r') as json_file:
            merge_rules = json.load(json_file)

        # Merge contents
        merged = merge_toml_item(base, source, merge_rules)

        # Write contents into pyproject.toml
        with base_toml.open('wt', encoding='utf-8') as temp:
            tomlkit.dump(merged, temp)

        # Remove extra newlines
        with base_toml.open('r') as base:
            base_data = base.read()
        with base_toml.open('w') as base:
            base.write(re.sub('\n\n\n+', '\n\n', base_data))

    elif args.entry:  # Updating by specifying the entries and their values
        for entry_name, entry_value in args.entry:
            update_toml_entry(base, entry_name, entry_value)

        # Write contents into pyproject.toml
        with base_toml.open('wt', encoding='utf-8') as temp:
            tomlkit.dump(base, temp)

        # Remove extra newlines
        with base_toml.open('r') as base:
            base_data = base.read()
        with base_toml.open('w') as base:
            base.write(re.sub('\n\n\n+', '\n\n', base_data))


if __name__ == '__main__':
    args = get_cli_arguments()
    main(args)
