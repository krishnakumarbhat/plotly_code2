"""
This module contains utilities to merge toml files.
"""

from tomlkit import items
from tomlkit.toml_document import TOMLDocument
from typing import TypeVar

T = TypeVar('T')


def merge_toml_item(base_item: T, source_item: T, merge_rules: dict) -> T:
    """
    Function to merge two toml items.

    Args:
        base_item: The base item to modify. This could be mutated depending on the merge rules.
        source_item: The source item to modify.  This could be mutated depending on the merge rules.

    Returns:
        The merged item.
    """
    type_base_item = type(base_item)
    type_source_item = type(source_item)

    if type_base_item != type_source_item:
        raise ValueError(f'Incompatible types:\nBase item has type {type_base_item} - {base_item}\nSource item has type {type_source_item} - {source_item}.')

    current_merge_rule = merge_rules.get('merge_strategy', 'ignore_source')

    match current_merge_rule:
        case 'ignore_source':
            return base_item

        case 'ignore_base':
            return source_item

        case 'prefer_source':
            match base_item:
                case items.AoT(_) | items.Array(_) | items.String(_) | items.Float(_) | items.Integer(_) | bool(_):
                    return source_item

                case items.Table(_) | TOMLDocument(_):
                    entries_base = set(base_item)
                    entries_source = set(source_item)

                    for entry_name in entries_base - entries_source:
                        source_item.add(entry_name, base_item[entry_name])

                    for entry_name in entries_base & entries_source:
                        source_item[entry_name] = merge_toml_item(
                            base_item[entry_name],
                            source_item[entry_name],
                            merge_rules.get('items', {}).get(entry_name, {'merge_strategy': current_merge_rule})
                        )

                    return source_item

                case _:
                    raise ValueError(f'Unknown type for merging: {type_base_item} - {base_item}.')

        case 'prefer_base':
            match base_item:
                case items.AoT(_) | items.Array(_) | items.String(_) | items.Float(_) | items.Integer(_) | bool(_):
                    return base_item

                case items.Table(_) | TOMLDocument(_):
                    entries_base = set(base_item)
                    entries_source = set(source_item)

                    for entry_name in entries_source - entries_base:
                        base_item.add(entry_name, source_item[entry_name])

                    for entry_name in entries_base & entries_source:
                        base_item[entry_name] = merge_toml_item(
                            base_item[entry_name],
                            source_item[entry_name],
                            merge_rules.get('items', {}).get(entry_name, {'merge_strategy': current_merge_rule})
                        )
                    return base_item

                case _:
                    raise ValueError(f'Unknown type for merging: {type_base_item} - {base_item}.')

        case _:
            raise ValueError(f'Unknown merge strategy: {current_merge_rule}.')


def get_toml_entry_path_parts(entry_name: str) -> list[str | int]:
    """
    Function to split a toml entry name into its individual parts, e.g.
    "Trace32.connection[0].port" -> ["Trace32", "connection", 0, "port"]

    Args:
        entry_name: The toml entry name to split.

    Returns:
        A list with the split parts of the entry name.
    """
    entry_parts = []
    rest_of_name = entry_name

    while rest_of_name:
        if rest_of_name.startswith("'"):
            next_single_quote_index = rest_of_name.find("'", 1)
            if next_single_quote_index == -1:
                raise NameError(f'Invalid toml entry name, missing closing single-quote: {entry_name}.')
            entry_parts.append(rest_of_name[1:next_single_quote_index])
            rest_of_name = rest_of_name[next_single_quote_index + 1:]

        elif rest_of_name.startswith('['):
            closing_bracket_index = rest_of_name.find(']')
            if closing_bracket_index == -1:
                raise NameError(f'Invalid toml entry name, missing closing bracket: {entry_name}.')
            index = rest_of_name[1:closing_bracket_index]
            rest_of_name = rest_of_name[closing_bracket_index + 1:]
            try:
                entry_parts.append(int(index))
            except ValueError:
                raise NameError(f'Invalid toml entry name, could not convert index {index} to int: {entry_name}.') from None

        elif rest_of_name.startswith('.'):
            rest_of_name = rest_of_name[1:]

        elif rest_of_name[0].isalpha():
            for (index, character) in enumerate(rest_of_name):
                if character in [".", "'", "["]:
                    entry_parts.append(rest_of_name[:index])
                    rest_of_name = rest_of_name[index:]
                    break
            else:
                entry_parts.append(rest_of_name)
                rest_of_name = ''

        else:
            raise NameError(f'Invalid toml entry name: {entry_name}.')

    return entry_parts


def update_toml_entry(toml_document: TOMLDocument, entry_name: str, entry_value: str) -> None:
    """
    Function to update a particular entry in a TOML document.

    Args:
        toml_document: The TOML document to update.
        entry_name: The entry to update.
        entry_value: The value to update the entry with.

    NOTE:
        The entry must already have a value in the TOML document.
    """
    entry_name_parts = get_toml_entry_path_parts(entry_name)
    current_container = toml_document

    for name_part in entry_name_parts[:-1]:
        current_container = current_container[name_part]

    match current_container[entry_name_parts[-1]]:
        case items.String(_):
            pass

        case items.Float(_):
            entry_value = float(entry_value)

        case items.Integer(_):
            entry_value = int(entry_value)

        case bool(_) | items.Bool(_):
            entry_value = {'true': True, 'false': False}[entry_value.lower()]

    current_container[entry_name_parts[-1]] = entry_value
