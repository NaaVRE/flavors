# NaaVRE blue-carbon flavor

<!-- vscode-markdown-toc -->
- [NaaVRE blue-carbon flavor](#naavre-blue-carbon-flavor)
  - [Build \& run](#build--run)
    - [Build](#build)
    - [Run](#run)
    - [Run local](#run-local)

<!-- vscode-markdown-toc-config
	numbering=false
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->

## <a name='Buildrun'></a>Build & run

### <a name='Build'></a>Build

```shell
docker build . -f flavors/blue-carbon/cell-runtime.Dockerfile --progress plain --build-arg CONDA_ENV_FILE=flavors/blue-carbon/environment.yaml -t naavre-fl-blue-carbon-runtime:local

docker build . -f flavors/blue-carbon/cell-build.Dockerfile --progress plain   --build-arg CONDA_ENV_FILE=flavors/blue-carbon/environment.yaml -t naavre-fl-blue-carbon-build:local

docker build . -f flavors/blue-carbon/jupyter.Dockerfile --progress plain      --build-arg CONDA_ENV_FILE=flavors/blue-carbon/environment.yaml -t naavre-fl-blue-carbon-jupyter:local
```

### <a name='Run'></a>Run

```shell
# dir_code = "/home/jovyan/Virtual Labs/Open Lab/Git public"
# dir_data = "/home/jovyan/Cloud Storage/naa-vre-user-data"

docker system prune -f
docker rmi naavre-fl-blue-carbon-jupyter:local

docker build . -f flavors/blue-carbon/local.Dockerfile --progress plain --build-arg CONDA_ENV_FILE=flavors/blue-carbon/environment.yaml -t naavre-fl-blue-carbon-jupyter:local
```

### <a name='Runlocal'></a>Run local

```shell
docker system prune -f
docker run -it -p 8888:8888 -e JUPYTER_TOKEN="mytoken" --name blue-carbon-jupyter --volume="//c/DockerShare/MarcoBolo:/home/jovyan" naavre-fl-blue-carbon-jupyter:local

docker exec -it blue-carbon-jupyter bash
```

http://localhost:8888/lab?token=mytoken
