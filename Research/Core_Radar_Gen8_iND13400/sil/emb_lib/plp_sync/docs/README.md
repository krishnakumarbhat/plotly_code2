# PLP Synchronization Module

## Overview
The PLP Synchronization Module is responsible for synchronizing data from ViGEM files with the framework timestamps. This module ensures that the correct data is available to the tracker at the right time, and that the output data is properly formatted and synchronized.

## Architecture
The PLP Sync module consists of the following components:

- **plp_iface**: Public interface for the PLP Sync module
- **plp_driver**: Implementation of the PLP synchronization logic
- **mdf_read**: Library for reading MDF files (used by PLP Sync)

The module follows a clean separation of concerns:

1. **sil_wrapper** calls the public API in **plp_iface**
2. **plp_iface** delegates to **plp_driver** for implementation
3. **plp_driver** uses **mdf_read** to access MDF file data

## Flow Diagrams
Flow diagrams are available in the `diagrams` directory:

- `plp_flow.mermaid`: Overall flow of the PLP synchronization process
- `component_diagram.mermaid`: Component relationships
- `sequence_diagram.mermaid`: Sequence of operations

## Dependencies
- C++11 or higher
- MDF Library (`mdf_lib`)
