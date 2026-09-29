# Interactive Plot Visualization Tool

An advanced tool for visualizing and analyzing HDF5 sensor data with interactive Plotly charts.

## Table of Contents
- [Overview](#overview)
- [Project Architecture](#project-architecture)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Packaging into Executables](#packaging-into-executables)
- [Directory Structure](#directory-structure)

## Overview

This tool processes HDF5 sensor data files using configuration from XML and JSON files to generate interactive HTML visualizations. It supports various analysis visualizations including scatter plots, histograms, box plots, and statistical comparisons.

![System Overview](docs/system_overview.png)

## Project Architecture

The application follows a layered architecture pattern for separation of concerns:

```
                          ┌─────────────────────┐
                          │     Main Module     │
                          └──────────┬──────────┘
                                     │
                     ┌───────────────┴─
                     ▼                           
        ┌──────────────────────┐         
        │ Config Layer (XML/JSON)│    
        └──────────┬──────────┘        
                   │                         
                   ▼                              
        ┌──────────────────────┐                  
        │ Persistence Layer    │                  
        │ (HDF Parser)         │                  
        └──────────┬──────────┘                  
                   │                              
                   ▼                              
        ┌──────────────────────┐                  
        │ Data Storage         │────────────-----──┐     
        └──────────┬──────────┘                    ▼   
                   │                          ---------------   
                   ▼                        |    KPI Module   |    
        ┌──────────────────────┐              ----------------
        │ Business Layer       │◄─────────────────┘
        │ (Data Processing)    │
        └──────────┬──────────┘
                   │
                   ▼
        ┌──────────────────────┐
        │ Presentation Layer   │
        │ (Plotly/HTML)        │
        └──────────────────────┘
```

## Features

- **Multi-file Processing**: Process multiple HDF5 files in one run
- **Interactive Visualizations**: Powered by Plotly.js
- **Statistical Analysis**: Mean, standard deviation, box plots, and histograms
- **Data Comparison**: Input vs. output data comparison
- **Sensor-specific Analysis**: Support for different sensor types
- **HTML Report Generation**: Interactive HTML reports with tabbed navigation
- **Performance Optimized**: Multiprocessing for faster data processing
- **Histogram Statistics**: Interactive mean and standard deviation visualization

## Installation

### Prerequisites
- Python 3.9+
- Required packages (see requirements.txt)

### Setup
1. Clone the repository
2. Install dependencies:
```bash
pip install -r requirements.txt
```

## Usage

Run the tool with the following command:

```bash
python main.py <config_file> <input_plot_json_file> [output_directory]
```

### Parameters:
- `config_file`: XML configuration file path
- `input_plot_json_file`: JSON file defining input/output mapping
- `output_directory`: (Optional) Directory for HTML reports (default: "html")

### Example:
```bash
python main.py config.xml plot_config.json output_reports
```

## Packaging into Executables

### Windows (.exe)
```bash
pyinstaller --onefile --hidden-import=lxml --hidden-import=h5py --hidden-import=plotly --add-data "InteractivePlot/a_config_layer:./a_config_layer" --add-data "InteractivePlot/b_persistence_layer:./b_persistence_layer" --add-data "InteractivePlot/c_data_storage:./c_data_storage" --add-data "InteractivePlot/d_business_layer:./d_business_layer" --add-data "InteractivePlot/e_presentation_layer:./e_presentation_layer" --add-data "KPI/a_business_layer:./KPI/a_business_layer" --add-data "KPI/b_presentation_layer:./KPI/b_presentation_layer" main.py
```

### Linux (executable)
```bash
pyinstaller --onefile --hidden-import=lxml --hidden-import=h5py --hidden-import=plotly --add-data "InteractivePlot/a_config_layer:./a_config_layer" --add-data "InteractivePlot/b_persistence_layer:./b_persistence_layer" --add-data "InteractivePlot/c_data_storage:./c_data_storage" --add-data "InteractivePlot/d_business_layer:./d_business_layer" --add-data "InteractivePlot/e_presentation_layer:./e_presentation_layer" --add-data "KPI/a_business_layer:./KPI/a_business_layer" --add-data "KPI/b_presentation_layer:./KPI/b_presentation_layer" main.py
```

## Directory Structure

- **InteractivePlot/**: Main application package
  - **a_config_layer/**: Configuration parsing (XML, JSON)
  - **b_persistence_layer/**: HDF5 file processing
  - **c_data_storage/**: Data models and storage
  - **d_business_layer/**: Data processing and calculations
  - **e_presentation_layer/**: Visualization and HTML generation
- **KPI/**: Key Performance Indicators module
  - **a_business_layer/**: KPI calculation logic
  - **b_presentation_layer/**: KPI visualization
- **tests/**: Unit tests
- **docs/**: Documentation and diagrams


*Interactive Plot Generation Project
This project processes configuration and JSON files to produce interactive plots. The main workflow involves parsing an XML configuration file, reading JSON mappings, processing HDF5 files, and outputting HTML files with Plotly visualizations.
<!-- ---------------------------------------------------------------------------------------------------------------- -->
*Project Structure
├── ConfigInteractivePlots.xml                # XML configuration file
├── InputsInteractivePlot.json                # JSON configuration for all sensors
├── InputsPerSensorInteractivePlot.json       # JSON configuration for per-sensor processing
├── InteractivePlot                           # Main package containing project modules
├── KPI                                       # Additional KPI calculations
│   └── detection_matching_kpi.py
└── tests                                     # Unit tests for the project

<!-- ---------------------------------------------------------------------------------------------------------------- -->
*Prerequisites
Python 3.6 or above
Required Python packages (see below)
Install the required packages using pip. For example, if all necessary packages are listed in a requirements file:

pip install -r requirements.txt

<!-- ---------------------------------------------------------------------------------------------------------------- -->
*How to Run
You can run the project from the command line by specifying the XML configuration file and the JSON input file. For example:

python main.py ConfigInteractivePlots.xml InputsInteractivePlot.json

This command will:
Parse the XML configuration with an instance of XmlConfigParser.
Determine the HDF5 file type.
Use JSONParserFactory to obtain the appropriate JSON parser to generate an input/output map.
Process the HDF5 files using the HdfProcessorFactory.


<!-- ---------------------------------------------------------------------------------------------------------------- -->
*Testing
Unit tests are provided in the tests directory. To run all the tests, use your preferred Python testing framework (e.g., pytest). For example:

pytest tests/
Make sure to install pytest if you haven't already:

pip install pytest


<!-- ---------------------------------------------------------------------------------------------------------------- -->

Development Tools

Visual Studio Code is configured using files in .vscode.
Diagrams and documentation images can be found in the Readme folder for an overview of the project architecture.
Additional Information

The project uses a layered approach to separate configuration parsing (a_config_layer), persistence (b_persistence_layer), business logic (c_business_layer), and presentation (d_presentation_layer).


The main entry point of the application is main.py.
