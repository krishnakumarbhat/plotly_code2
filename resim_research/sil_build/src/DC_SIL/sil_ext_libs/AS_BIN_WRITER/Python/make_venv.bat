REM Creates a virtual environment for Python, updates pip, and installs the package with its dependencies using pip

python -m venv venv
call venv\Scripts\activate
python -m easy_install -U pip
pip install -e .[dev]
