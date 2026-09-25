# Core signals
The output information is splitted into three parts:
 - data 
   - scenario       - basic information about scenario like number of iteration, etc.
   - host           - host information
   - sensors        - sensors information
   - detections     - detections information
 - reference
   - segments       - array of stationary objects
   - objects        - array of movable objects
 - generator_info
   - type           - type of used generator ("Artificial Scenario Generator").

> **Note:** each of *data* element consists of two types. They are "runtime" (vary between iterations) and "setup" (constant between iterations).
  
# List of *data* signals
[MATLAB reference](https://www.mathworks.com/help/driving/ref/drivingscenario.actor.html)

## Host
### Setup
| Signal             | Unit | Description                                                 |
| ------------------ | ---- | ----------------------------------------------------------- |
| length             | m    | host length                                                 |
| width              | m    | host width                                                  |
| rear_overhang      | m    | distance between the rear axle and the rear of the vehicle  |
| rear_axle_distance | m    | distance between the rear axle and the front of the vehicle |

### Runtime
| Signal       | Unit  | Description                                      |
| ------------ | ----- | ------------------------------------------------ |
| speed        | m/s   | host speed                                       |
| position_wcs | m     | host position in WCS (center of rear axle)       |
| yaw_wcs      | rad   | host orientation in WCS                          |
| yaw_rate_wcs | rad/s | host yaw rate in WCS (rotation w.r.t. rear axle) |

## Sensor (RADAR)
[MATLAB reference](https://www.mathworks.com/help/fusion/ref/radarsensor-system-object.html?searchHighlight=sensor%20radar&s_tid=srchtitle_sensor%20radar_2)

### Setup
| Signal        | Unit | Description                       |
| ------------- | ---- | --------------------------------- |
| position_iso  | m    | position in ISO coordinate system |
| boresight_iso | rad  | orientation in ISO                |
| field_of_view | rad  | field of view                     |
| range         | m    | maximum range                     |

### Runtime
| Signal | Unit | Description |
| ------ | ---- | ----------- |


## Detection (RADAR)
### Setup
| Signal | Unit | Description |
| ------ | ---- | ----------- |

### Runtime
| Signal       | Unit | Description                       |
| ------------ | ---- | --------------------------------- |
| position_iso | m    | position in ISO coordinate system |
| position_vcs | m    | position in VCS                   |

> **WARNING:** z-position is set to 0 in current implementation. Elevation is not taken in calculations.

## Scenario
### Setup
| Signal         | Unit | Description          |
| -------------- | ---- | -------------------- |
| num_iterations | n/a  | number of iterations |

### Runtime
| Signal        | Unit | Description     |
| ------------- | ---- | --------------- |
| iteration_idx | n/a  | iteration index |
| timestamp     | s    | timestamp       |

# List of reference signals
## Segments (stationary obstacles)
| Signal       | Unit | Description                         |
| ------------ | ---- | ----------------------------------- |
| position_wcs | m    | position of a segment center in WCS |
| length       | m    | length                              |
| width        | m    | width                               |
| height       | m    | height                              |
| yaw_wcs      | rad  | orientation in WCS                  |

## Objects (moveable objects)
| Signal       | Unit | Description                         |
| ------------ | ---- | ----------------------------------- |
| position_wcs | m    | position of an object center in WCS |
| length       | m    | length                              |
| width        | m    | width                               |
| height       | m    | height                              |
| yaw_wcs      | rad  | orientation in WCS                  |
| velocity_wcs | m/s  | velocity vector in WCS              |


# Abbreviations
- **WCS** - Word Coordinate System
- **VCS** - Vehicle Coordinate System