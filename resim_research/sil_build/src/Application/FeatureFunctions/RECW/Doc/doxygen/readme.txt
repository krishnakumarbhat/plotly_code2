To generate HTML documentation with doxygen in the SFL_SideFeatureLogic repository two steps are needed:

1. Run the python script "Jenkins/Tools/doxygen/create_doxy_file_for_feature.py" with the desired feature and customer
   as command line argument. This will create all necessary files in RECW/Doc/doxygen.

2. Call doxygen on the configuration file RECW/Doc/doxygen/html.doxy. All files in the subfolder "pages" will be
   included in the HTML documenation. Images will be included if they are referenced in any document in "pages" and
   are located in the subfolder "images".
