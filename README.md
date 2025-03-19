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

### Build

Use the helper script:

```console
$ ./build-local.sh -h
Usage: ./build-local.sh [-n] [-t target] flavor

-h,--help           print help and exit
-n,--dry-run        print the commands that would be executed and exit
-t,--target target  build target (options: jupyter, cell-build, cell-runtime,
                    cell-test, cell-all, all; default: all)
flavor              flavor name
```

Example: all images for the vanilla flavor:

```console
$ ./build-local.sh vanilla -t all
Building images...
docker build . -f ./docker/jupyter.Dockerfile --build-arg CONDA_ENV_FILE=./flavors/vanilla//environment.yaml -t naavre-fl-vanilla-jupyter:local
[+] Building 0.3s (11/11) FINISHED                                                             docker:default
 => ...
docker build . -f ./docker/cell-build.Dockerfile --build-arg CONDA_ENV_FILE=./flavors/vanilla//environment.yaml -t naavre-fl-vanilla-cell-build:local
[+] Building 0.3s (9/9) FINISHED                                                               docker:default
 => ...
docker build . -f ./docker/cell-runtime.Dockerfile --build-arg CONDA_ENV_FILE=./flavors/vanilla//environment.yaml -t naavre-fl-vanilla-cell-runtime:local
[+] Building 0.7s (5/5) FINISHED                                                               docker:default
 => ...
docker build . -f ./docker/cell-test.Dockerfile --build-arg BUILD_IMAGE=naavre-fl-vanilla-cell-build:local --build-arg RUNTIME_IMAGE=naavre-fl-vanilla-cell-runtime:local -t naavre-fl-vanilla-cell-test:local
[+] Building 0.3s (11/11) FINISHED                                                             docker:default
 => ...

Built images:
naavre-fl-vanilla-jupyter:local
naavre-fl-vanilla-cell-build:local
naavre-fl-vanilla-cell-runtime:local
naavre-fl-vanilla-cell-test:local
```

Example: jupyter image for the vanilla flavor:

```console
$ ./build-local.sh vanilla -t jupyter
Building images...
docker build . -f ./docker/jupyter.Dockerfile --build-arg CONDA_ENV_FILE=./flavors/vanilla//environment.yaml -t naavre-fl-vanilla-jupyter:local
[+] Building 0.3s (11/11) FINISHED                                                             docker:default
 => ...

Built images:
naavre-fl-vanilla-jupyter:local
```

Example: all cell images for the vanilla flavor:

```console
o ./build-local.sh vanilla -t cell-all
Building images...
docker build . -f ./docker/cell-build.Dockerfile --build-arg CONDA_ENV_FILE=./flavors/vanilla//environment.yaml -t naavre-fl-vanilla-cell-build:local
[+] Building 0.3s (9/9) FINISHED                                                               docker:default
 => ...
docker build . -f ./docker/cell-runtime.Dockerfile --build-arg CONDA_ENV_FILE=./flavors/vanilla//environment.yaml -t naavre-fl-vanilla-cell-runtime:local
[+] Building 1.0s (5/5) FINISHED                                                               docker:default
 => ...
docker build . -f ./docker/cell-test.Dockerfile --build-arg BUILD_IMAGE=naavre-fl-vanilla-cell-build:local --build-arg RUNTIME_IMAGE=naavre-fl-vanilla-cell-runtime:local -t naavre-fl-vanilla-cell-test:local
[+] Building 0.3s (11/11) FINISHED                                                             docker:default
 => ...

Built images:
naavre-fl-vanilla-cell-build:local
naavre-fl-vanilla-cell-runtime:local
naavre-fl-vanilla-cell-test:local
```

_Note: the `cell-test` target requires `cell-build` and `cell-runtime`. While the helper script allows to build only `cell-test`, it will fail if its dependencies haven’t previously been built. Use `cell-all` to build all three targets._

### Run

#### Jupyter Lab

Example for the vanilla flavor:

```shell
docker run -it -p 8888:8888 naavre-fl-vanilla-jupyter:dev
```

#### Cell tests

Example for the vanilla flavor:

```shell
docker run -v ./flavors/vanilla/tests/:/tests/ naavre-fl-vanilla-cell-test:dev /bin/bash /tests/tests.sh
```
