import pandas as pd
import re
import matplotlib as mpl
import matplotlib.pyplot as plt
from pathlib import Path


def get_resim_params(csv_path):
    resim_params = re.findall(r"(\d*)_(\-?\d*\.\d*)_(\-?\d*\.\d*)", str(csv_path))
    host_speed = resim_params[0][0]
    curvature = resim_params[0][1]
    dist_rear_axle_to_vcs = resim_params[0][2]
    return host_speed, curvature, dist_rear_axle_to_vcs



if __name__ == "__main__":


    #set path for csv_dir & output_dir where plots will be saved
    csv_dir = r""
    output_dir = Path(r"C:\Users\wjdl3n\Repositories\SG_StationaryGeometries\tools\contour_importance_visualization")

    csv_files = Path(csv_dir).glob("*.csv")

    mpl.rcParams.update({'font.size': 5})

    for csv_path in csv_files:
        host_speed, curvature, dist_rear_axle_to_vcs = get_resim_params(csv_path)
        df = pd.read_csv(csv_path, header=None)
        df = df.rename(columns={0:"contour_id", 1:"priority",2:"x",3:"y"})
        plt.scatter(x=df['y'], y=df['x'], c=df['priority'], cmap='jet',s=1)
        plt.colorbar(label='importance')
        plt.grid()
        plt.title(f"Contour importance,\nhost_speed={host_speed},\ncurvature={curvature},\ndist_rear_axle_to_vcs={dist_rear_axle_to_vcs}")
        plt.axis('equal')
        figname = csv_path.with_suffix('.png')
        figname = output_dir / figname.name
        plt.savefig(figname, dpi=400)
        plt.clf()