"""
This module defines a helper class for logging to stderr and stdout.
"""
import sys


class Logger:
    """Utility class for logging helpers."""

    def __init__(self, stdout=sys.stdout, stderr=sys.stderr, verbose=False):
        """Default log output to sys.stdout/sys.stderr with verbose mode is disabled."""
        self.stdout = sys.stdout
        self.stderr = sys.stderr
        self.__flag_verbose = verbose

    def stdprint(self, *objects, sep=" ", end="\n", flush=False):
        """Emit a normal log output line to self.stdout."""
        print(*objects, sep=sep, end=end, flush=flush, file=self.stdout)

    def verbose(self, *objects, sep=" ", end="\n", flush=False):
        """Emit a log output line to self.stderr only when verbose mode is enabled."""
        if self.__flag_verbose:
            print(*objects, sep=sep, end=end, flush=flush, file=self.stderr)

    def error(self, *objects, sep=" ", end="\n", flush=False):
        """Emit an error output line to self.stderr regardless of verbose mode flag."""
        print(*objects, sep=sep, end=end, flush=flush, file=self.stderr)

    def set_verbose(self, verbose):
        """Setter function for the verbose mode flag."""
        self.__flag_verbose = verbose
