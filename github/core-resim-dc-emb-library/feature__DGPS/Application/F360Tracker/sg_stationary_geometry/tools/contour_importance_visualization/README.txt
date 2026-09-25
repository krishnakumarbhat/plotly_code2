1. To create solution with project:
cmake -S . -B build

2. Build & run:

   Windows:
    Build and run solution "SG_StationaryGeometries\tools\contour_importance_visualization\build\SaveContourImportance.sln" in Visual Studio.
       
   Linux:
    to build and run the binary:
    
      $ cd build
      $ make
      $ ./save_contour_importance_to_csv
      $ cd ..
      $ ls
    
    Now you will find all the *csv files generated in current directory.
    
3. To visualize contour importance run visualize_importance.py:
   python visualize_importance.py

   Script requires Python libraries:
   - numpy,
   - pandas,
   - matplotlib