"""
Helper functions for formatting values.

Convert string to int, int to hex string, and any value to string.
"""

def to_hex_string(value):
    """Convert an integer to a hex string with 0x prefix."""
    return "0x%X" % value

def to_string(value):
    """Convert a value to a string."""
    return str(value)

def to_int(value):
    """Convert a string to an integer."""
    return int(value, 0)
