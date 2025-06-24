# Biotope flavor

## Extra testing

With the (non-public) biotope repo:

```shell
# Build
docker build . -f flavors/biotope/naavre-cell-runtime.bioRad.Dockerfile --build-arg CONDA_ENV_FILE=flavors/biotope/environment.yaml -t tmp-cell-runtime
docker build . -f docker/naavre-cell-build.bioRad.Dockerfile --build-arg CONDA_ENV_FILE=flavors/biotope/environment.yaml -t tmp-cell-build
docker build . -f flavors/biotope/naavre-jupyter.bioRad.Dockerfile --build-arg NAAVRE_VERSION=v2.4.3 --build-arg CONDA_ENV_FILE=flavors/biotope/environment.yaml -t tmp-jupyter

# Run
docker run --rm -it -v /path/to/biotope/ZonalFilter/resources/:/mnt -v /path/to/biotope/ZonalFilter/:/code tmp-jupyter:latest /opt/conda/envs/biotope/bin/python3 /code/zonalFilter.py -s /mnt/zones -m /mnt/maps -srs /mnt/grid_srs_toWkt.txt -zf id -o /mnt
docker run --rm -it -v /path/to/biotope/ZonalFilter/resources/:/mnt -v /path/to/biotope/ZonalFilter/:/code tmp-cell-test:latest /venv/bin/python3 /code/zonalFilter.py -s /mnt/zones -m /mnt/maps -srs /mnt/grid_srs_toWkt.txt -zf id -o /mnt
```

Both run commands should return 0.
