from laserchicken.utils import create_point_cloud, add_to_point_cloud, get_point
create_point_cloud([], [], [])


import fnmatch
from dask.distributed import LocalCluster, SSHCluster 
from laserfarm import Retiler, DataProcessing, GeotiffWriter, MacroPipeline
from laserfarm.remote_utils import get_wdclient, get_info_remote, list_remote
from laserfarm.data_processing import DataProcessing
from laserchicken.compute_neighbors import compute_neighborhoods


from laserchicken import load
import requests

def download_file_and_load(url, output_path):
    response = requests.get(url, stream=True)
    response.raise_for_status()
    with open(output_path, "wb") as f:
        for chunk in response.iter_content(chunk_size=8192):
            f.write(chunk)
    print(f"Downloaded to {output_path}")

    point_cloud = load(output_path)

    print(f"Loaded  {len(point_cloud)} points from {output_path}")


# https://geotiles.citg.tudelft.nl/

# AHN4
url = "https://basisdata.nl/hwh-ahn/ahn4/01_LAZ/C_20BN2.LAZ"
output_path = "C_20BN2.LAZ"

download_file_and_load(url, output_path)

url = "https://basisdata.nl/hwh-ahn/AHN5/01_LAZ/2023_C_20BN2.LAZ"
output_path = "2023_C_20BN2.LAZ"


download_file_and_load(url, output_path)