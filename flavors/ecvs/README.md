# NaaVRE ECVs flavor

<!-- vscode-markdown-toc -->
- [NaaVRE ECVs flavor](#naavre-ecvs-flavor)
  - [Build \& run](#build--run)
    - [Build](#build)
    - [Run](#run)

<!-- vscode-markdown-toc-config
	numbering=false
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->

## <a name='Buildrun'></a>Build & run

### <a name='Build'></a>Build

```shell
docker build . -f flavors/ecvs/cell-runtime.Dockerfile --progress plain --build-arg CONDA_ENV_FILE=flavors/ecvs/environment.yaml -t naavre-fl-ecvs-runtime:local

docker build . -f flavors/ecvs/cell-build.Dockerfile --progress plain   --build-arg CONDA_ENV_FILE=flavors/ecvs/environment.yaml -t naavre-fl-ecvs-build:local

docker system prune -f
docker rmi naavre-fl-ecvs-jupyter:local
docker build . -f flavors/ecvs/jupyter.Dockerfile --progress plain      --build-arg CONDA_ENV_FILE=flavors/ecvs/environment.yaml -t naavre-fl-ecvs-jupyter:local
```

### <a name='Run'></a>Run

```shell
# dir_code = "/home/jovyan/Virtual Labs/Open Lab/Git public"
# dir_data = "/home/jovyan/Cloud Storage/naa-vre-user-data"

docker system prune -f
docker run -it -p 8888:8888 --name ecvs-jupyter --volume="//c/DockerShare/ECVs:/home/jovyan" naavre-fl-ecvs-jupyter:local

docker exec -it ecvs-jupyter bash
```

http://localhost:8888/lab?token=ac328bb70877f6839cd513cb01f56716ee86eada70c87fa0
