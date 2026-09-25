import glob
import os
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

folder_path = "./"

# Get a list of all XLSM files in the directory
pattern_files = glob.glob('*.xlsm')
#print(pattern_files)

fields_to_plot = ['antenna_gain_dB', 'elevation_gain_dB']
pdf_path = folder_path + "\\" + 'Antenna_gain_pattern.pdf'
with PdfPages(pdf_path) as pdf:
# Add the headline to the PDF
    plt.figure(figsize=(8, 5))
    plt.text(0.5, 0.5, 'Antenna Gain Pattern', fontsize=25, ha='center', va='center')
    plt.axis('off')
    pdf.savefig()
    plt.close()
    # Iterate through each file
    for file in pattern_files:
        file_name = os.path.splitext(os.path.basename(file))[0]
        # Load the XLSM file into a pandas DataFrame
        df = pd.read_excel(file, sheet_name='AntennaPattern')
        # Generate the scatter plot using the DataFrame
        df.plot(x=0, y =fields_to_plot, kind = 'line', title = file_name)
        plt.grid()
        pdf.savefig()
        plt.close()