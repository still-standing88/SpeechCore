# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'SpeechCore'
copyright = '2024, Oussama Ben Gatrane'
author = 'Oussama Ben Gatrane'
import os
import sys

sys.path.insert(0, os.path.abspath('..'))

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = ["myst_parser",
    'sphinx.ext.autodoc',
    'breathe',
    'sphinx_rtd_theme',
    'sphinx.ext.viewcode',
    'sphinx_sitemap',
]

breathe_projects = { "SpeechCore": "../doxygen_output/xml/" }
breathe_default_project = "SpeechCore"


templates_path = ['_templates']
exclude_patterns = []

# MyST parser settings
myst_enable_extensions = [
    "colon_fence",
    "deflist",
]

source_suffix = {
    '.rst': 'restructuredtext',
    '.md': 'markdown',
}

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']
