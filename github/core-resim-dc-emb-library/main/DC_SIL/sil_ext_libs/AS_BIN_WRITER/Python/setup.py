import setuptools


with open("README.md", "r") as fh:
    long_description = fh.read()

# For more information check https://packaging.python.org/guides/distributing-packages-using-setuptools/#setup-args
setuptools.setup(
    name="pybin",
    version="0.0.1",
    description="Package for reading bin files",
    long_description=long_description,
    long_description_content_type="text/markdown",
    author="Aptiv Radar Tracker Team",
    author_email="somebody@aptiv.com",
    classifiers=[
        # How mature is this project? Common values are
        #   3 - Alpha
        #   4 - Beta
        #   5 - Production/Stable
        "Development Status :: 4 - Beta",
        # Indicate who your project is intended for
        "Intended Audience :: Developers",
        # Specify the Python versions you support here. In particular, ensure
        # that you indicate whether you support Python 2, Python 3 or both.
        "Programming Language :: Python :: 3.7.1",
    ],
    packages=setuptools.find_packages(),
    python_requires=">=3.7.1",
    install_requires=["numpy"],
    extras_require={
        "dev": [
            "flake8",
            "black",
            "pep8-naming",
            "scipy",
        ],
    },
)
