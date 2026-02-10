# NaaVRE aneris-dna flavor

<!-- vscode-markdown-toc -->
* [Build & run](#Buildrun)
	* [Build](#Build)
	* [Run](#Run)

<!-- vscode-markdown-toc-config
	numbering=false
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->

## <a name='Buildrun'></a>Build & run

### <a name='Build'></a>Build

```shell
docker build . -f flavors/aneris-dna/cell-runtime.Dockerfile --progress plain --build-arg CONDA_ENV_FILE=flavors/aneris-dna/environment.yaml -t naavre-fl-aneris-dna-runtime:local

docker build . -f flavors/aneris-dna/cell-build.Dockerfile --progress plain   --build-arg CONDA_ENV_FILE=flavors/aneris-dna/environment.yaml -t naavre-fl-aneris-dna-build:local

docker system prune -f
docker rmi naavre-fl-aneris-dna-jupyter:local
docker build . -f flavors/aneris-dna/jupyter.Dockerfile --progress plain      --build-arg CONDA_ENV_FILE=flavors/aneris-dna/environment.yaml -t naavre-fl-aneris-dna-jupyter:local
```

### <a name='Run'></a>Run

```shell
# dir_code = "/home/jovyan/Virtual Labs/Open Lab/Git public"
# dir_data = "/home/jovyan/Cloud Storage/naa-vre-user-data"

docker system prune -f
docker run -it -p 8888:8888 --name aneris-dna-jupyter --volume="//c/DockerShare/ANERIS_DNA:/home/jovyan" naavre-fl-aneris-dna-jupyter:local

docker exec -it aneris-dna-jupyter bash
```

http://localhost:8888/lab?token=8cc65e14c4f8522d32665950d6cbde0bb36f263309650cd7
