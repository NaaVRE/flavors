# NaaVRE flavors

NaaVRE flavors are sets of Docker images tailor-made for a specific use-case.
They bring together NaaVRE Jupyter extensions and a Conda environment.
Each flavor consists of the three images:

- `naavre-fl-{myflavor}-jupyter`, the NaaVRE Jupyter Lab image. This is the image for the users Jupyter Lab instances, containing dependencies for notebook execution.
- `naavre-fl-{myflavor}-cell-runtime`, the base images for the `build` stage of NaaVRE cells. This image contains conda dependencies for containerized cells execution.
- `naavre-fl-{myflavor}-cell-build`, the base images for the `runtime` stage of NaaVRE cells. This image contains other dependencies (system, manually added, etc.) for containerized cells execution.

Each flavor corresponds to a directory `./flavors/{myflavor}`, with the following
structure:

```txt
{myflavor}
├── environment.yaml           # Conda environment with `name: {myflavor}`
├── flavor_config.yaml         # Build configuration
├── tests
│    └── tests.sh              # Test script run in naavre-{myflavor}-cell
├── [cell-build.Dockerfile]    # Optional override to docker/cell-build.Dockerfile
├── [cell-runtime.Dockerfile]  # Optional override to docker/cell-runtime.Dockerfile
└── [jupyter.Dockerfile]       # Optional override to docker/jupyter.Dockerfile
```


## Build and Run Containers Locally

### Jupyter Lab

Set the Dockerfile to the `naavre-jupyter.Dockerfile`:

```bash
dockerfile=./docker/jupyter.Dockerfile
```

Set the conda environment file to the flavor's `environment.yaml`. For example for building the vanilla flavor:

```bash
CONDA_ENV_FILE=./flavors/vanilla/environment.yaml
```

Run the build command:

```bash
docker build -t naavre-fl-vanilla-jupyter -f $dockerfile --build-arg CONDA_ENV_FILE=$CONDA_ENV_FILE .
```

To run the container:

```bash
docker run -it -p 8888:8888 --env-file ~/Downloads/notbooks/docker_VARS naavre-fl-vanilla-jupyter /bin/bash -c "source /venv/bin/activate && /tmp/init_script.sh && jupyter lab --debug --watch --NotebookApp.token='' --NotebookApp.ip='0.0.0.0' --NotebookApp.allow_origin='*' --collaborative"
```