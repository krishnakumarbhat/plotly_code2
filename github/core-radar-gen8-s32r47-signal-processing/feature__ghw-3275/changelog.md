# CHANGELOG

The pattern for releases is MAJOR.MINOR.REVISION.
See https://semver.org/ for more information.

1. MAJOR version when you make incompatible API changes
1. MINOR version when you add functionality in a backward compatible manner
1. PATCH version when you make backward compatible bug fixes

By updating this changelog with changes made, a set of release notes will be generated when the version.yaml file is updated.
Versions can be released and the subsequent JFrog artifacts created when a new changeset is merged to dev or release/*.

## Version 0.0
### Revision 0
- GHW-2604 Toolchains and Support for BBE Sim and EVB Diag Tool tests
   - Initial version of SPBB providing only Sim and EVB Diag Tool test support
   - Based on Gen7v2 with various build updates to match Gen8
   - Profiling and Timing Helpers available for integration

## Version 0.1
### Revision 0
- GHW-2866 Create baseline profiling tests using evb_diag_tool
- GHW-2867 Port existing SPT profiling to diagnostic tool implementation
- GHW-2887 Create parameterized evb_diag_tool_test for SPT DDR testing
   - Rearranged the evb_diag_tool subdirectories, into base, testtypes, examples
   - Added the concept of "testtype" to the evb_diag_tool
   - Implemented "profile_spt" testtype and example for simple SPT profiling
   - Implemented "profile" testtype and example for multicore parameterized profiling
