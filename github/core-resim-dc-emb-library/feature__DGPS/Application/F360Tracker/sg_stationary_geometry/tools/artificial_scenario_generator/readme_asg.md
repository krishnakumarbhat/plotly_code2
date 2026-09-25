# Artificial Scenario Generator (ASG)
It is an extension of MATLAB Driving Scenario Designer by providing the following features:
- conversion to Vehicle Coordinate System \[VCS\]

# Condensed running guide
1. Prepare scenario using MATLAB Driving Scenario Designer
2. Convert scenario to MATLAB function and save it to some directory
> **WARNING:** due to MATLAB bug once function is generated go inside and for each sensor constructor add this "*'DetectionCoordinates', 'Sensor Spherical', ...*" 
3. Go to run_generator.m
    1. Update **Configuration** section
    2. Run script

# Tests
To run tests call **run_tests.m**

# Documentation
- [List of core signals](docs/list_of_core_signals.md)
- [Extended running guide - TBD]()
- [Scenario creation guide - TBD]()

# Version
Written in MATLAB R2021b