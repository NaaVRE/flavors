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

    point_cloud = load(output_path, attributes = ["x","y","z", "classification", "number_of_returns", "return_number"])

    print(f"Loaded  {len(point_cloud)} points from {output_path}")


# https://geotiles.citg.tudelft.nl/

# AHN4
url = "https://scruffy.lab.uvalight.net:9000/naa-vre-user-data/shiyifang529%40gmail.com/vl-laserfarm/AHN5/AHN5_small.laz?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=7MP8OH1CWJEZPRD71QSD%2F20251029%2Fnl-uvalight%2Fs3%2Faws4_request&X-Amz-Date=20251029T130311Z&X-Amz-Expires=43200&X-Amz-Security-Token=eyJhbGciOiJIUzUxMiIsInR5cCI6IkpXVCJ9.eyJhY2Nlc3NLZXkiOiI3TVA4T0gxQ1dKRVpQUkQ3MVFTRCIsImV4cCI6MTc2MTc4NjEwNywicGFyZW50Ijoic3Bpcm9zIn0.uxJi-kORKMwHFxE_BBunKQ3oyV7W3vpR14_CDRfMGY0stPtRzCOb4eNw0NQsUdLLwd2fatyViA9lhQrfNhGLVA&X-Amz-SignedHeaders=host&versionId=a2563029-4760-499e-8f11-cc1ac14a041e&X-Amz-Signature=f340f2803c5f3ebe21ec6c2439699c9afabfae7f0713f5c8db47dd4603c27e60"
output_path = "C_20BN2.LAZ"

download_file_and_load(url, output_path)

# AHN5
url = "https://basisdata.nl/hwh-ahn/AHN5/01_LAZ/2023_C_20BN2.LAZ"
output_path = "2023_C_20BN2.LAZ"


download_file_and_load(url, output_path)