"""This file contains the initialization routine for the testing package. Here the python package manager is modified,
such that the python source folder is appended to the package path."""
import os
import sys

# Import source folder in python package manager for testing.
sys.path.append(os.path.join(os.path.dirname(__file__), "..", ".."))
