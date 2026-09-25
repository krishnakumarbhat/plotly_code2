import os
import sys
import subprocess

# Source code dir relative to this file. Recursive search based on sphinx.ext.autosummary. Thus
# the root directory is enough to import.
sys.path.insert(0, os.path.abspath('../../'))
import python_src.ct_shared_resources as ct_sr

# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'CT_CalibrationTool'
copyright = '2023, SFL'
author = 'SFL'
release = ct_sr.cal_tool_version

# At first execute the calibration tool to receive the new format of generated files such that
# those can be appended to the documentation.
subprocess.run(['python', os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'ct_main.py'),
    os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'testing', 'example_files', 'Core', 'cool_feature_cal.xml')])

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
    'sphinx.ext.autodoc',     # Core library for html generation from docstrings
    'sphinx.ext.autosummary', # Create neat summary tables
    'sphinx.ext.napoleon',    # Support for google styled docstrings
    'myst_parser'             # Extension for mark down visualization
]

napoleon_google_docstring = True
napoleon_use_param = False
napoleon_use_ivar = True

source_suffix = ['.rst', '.md']

exec_code_source_folders = [f'..{os.path.sep}..{os.path.sep}']

# Add default
autodoc_default_options = {'members': True, 'private-members': True, 'special-members': True}

templates_path = ['_templates']
exclude_patterns = []

autosummary_generate = True  # Turn on sphinx.ext.autosummary

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'classic'
html_theme_path = ["."]
html_static_path = ['_static']
html_sidebars = {
    '**': [
        'globaltoc.html',
    ]
}

