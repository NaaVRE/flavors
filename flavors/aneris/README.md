# NaaVRE aneris flavor

<!-- vscode-markdown-toc -->
- [NaaVRE aneris flavor](#naavre-aneris-flavor)
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
docker build . -f flavors/aneris/cell-runtime.Dockerfile --progress plain --build-arg CONDA_ENV_FILE=flavors/aneris/environment.yaml -t naavre-fl-aneris-runtime:local

docker build . -f flavors/aneris/cell-build.Dockerfile --progress plain   --build-arg CONDA_ENV_FILE=flavors/aneris/environment.yaml -t naavre-fl-aneris-build:local

docker build . -f flavors/aneris/jupyter.Dockerfile --progress plain      --build-arg CONDA_ENV_FILE=flavors/aneris/environment.yaml -t naavre-fl-aneris-jupyter:local
```

### <a name='Run'></a>Run

```shell
# dir_code = "/home/jovyan/Virtual Labs/Open Lab/Git public"
# dir_data = "/home/jovyan/Cloud Storage/naa-vre-user-data"

docker system prune -f
docker rmi naavre-fl-aneris-jupyter:local
docker build . -f flavors/aneris/local.Dockerfile --progress plain --build-arg CONDA_ENV_FILE=flavors/aneris/environment.yaml -t naavre-fl-aneris-jupyter:local

docker system prune -f
docker run -it -p 8888:8888 -e JUPYTER_TOKEN="mytoken" --name aneris-jupyter --volume="//c/DockerShare/ANERIS:/home/jovyan" naavre-fl-aneris-jupyter:local

docker exec -it aneris-jupyter bash
```

http://localhost:8888/lab?token=mytoken
