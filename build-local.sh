#!/bin/bash

set -e

__usage="Usage: ./build-local.sh [-n] [-t target] flavor

-h,--help           print help and exit
-n,--dry-run        print the commands that would be executed and exit
-t,--target target  build target (options: jupyter, cell-build, cell-runtime,
                    cell-test, cell-all, all; default: all)
flavor              flavor name"

get_dockerfile() {
  flavor_dir="$1"
  dockerfile_base_name="$2"
  dockerfile="$flavor_dir/$dockerfile_base_name"
  [[ -f "$dockerfile" ]] || dockerfile="./docker/$dockerfile_base_name"
  echo "$dockerfile"
}

get_conda_env_file() {
  flavor_dir="$1"
  echo "$flavor_dir/environment.yaml"
}

get_image_name() {
  flavor="$1"
  image_base_name="$2"
  echo "naavre-fl-$flavor-$image_base_name:local"
}

get_docker_build_cmd() {
  flavor="$1"
  image_base_name="$2"
  flavor_dir="./flavors/$flavor/"
  dockerfile=$(get_dockerfile "$flavor_dir" "$image_base_name.Dockerfile")
  conda_env_file=$(get_conda_env_file "$flavor_dir")
  image_name=$(get_image_name "$flavor" "$image_base_name")
  cmd="docker build . -f $dockerfile --build-arg CONDA_ENV_FILE=$conda_env_file -t $image_name"
  echo "$cmd"
}

get_docker_build_cell_test_cmd() {
  flavor="$1"
  dockerfile="./docker/cell-test.Dockerfile"
  build_image=$(get_image_name "$flavor" "cell-build")
  runtime_image=$(get_image_name "$flavor" "cell-runtime")
  image_name=$(get_image_name "$flavor" "cell-test")
  cmd="docker build . -f $dockerfile --build-arg BUILD_IMAGE=$build_image --build-arg RUNTIME_IMAGE=$runtime_image -t $image_name"
  echo "$cmd"
}

main() {
  dry_run=0
  target="all"
  flavor=""

  POSITIONAL_ARGS=()
  while [[ $# -gt 0 ]]; do
    case $1 in
      -h|--help)
        echo "$__usage"
        exit 0
        ;;
      -n|--dry-run)
        dry_run=1
        shift
        ;;
      -t|--target)
        target="$2"
        shift
        shift
        ;;
      -*|--*)
        echo "Unknown option $1"
        exit 1
        ;;
      *)
        POSITIONAL_ARGS+=("$1") # save positional arg
        shift # past argument
        ;;
    esac
  done

  set -- "${POSITIONAL_ARGS[@]}" # restore positional parameters

  flavor="$1"

  if [[ -z "$flavor" ]]; then
    echo "No flavor selected"
    exit 1
  fi

  flavor_dir="./flavors/$flavor/"
  if [[ ! -d "$flavor_dir" ]]; then
    echo "Unknown flavor: $flavor"
    exit 1
  fi

  docker_build_cmds=()
  image_names=()
  case "$target" in
    "jupyter"|"all")
      docker_build_cmds+=("$(get_docker_build_cmd "$flavor" "jupyter")")
      image_names+=("$(get_image_name "$flavor" "jupyter")")
      ;;&
    "cell-build"|"cell-all"|"all")
      docker_build_cmds+=("$(get_docker_build_cmd "$flavor" "cell-build")")
      image_names+=("$(get_image_name "$flavor" "cell-build")")
      ;;&
    "cell-runtime"|"cell-all"|"all")
      docker_build_cmds+=("$(get_docker_build_cmd "$flavor" "cell-runtime")")
      image_names+=("$(get_image_name "$flavor" "cell-runtime")")
      ;;&
    "cell-test"|"cell-all"|"all")
      docker_build_cmds+=("$(get_docker_build_cell_test_cmd "$flavor")")
      image_names+=("$(get_image_name "$flavor" "cell-test")")
      ;;&
  esac

  if (( ${#docker_build_cmds[@]} == 0 )); then
    echo "Unknown target: $target"
    exit 1
  fi

  echo "Building images..."
  for cmd in "${docker_build_cmds[@]}"; do
    echo "$cmd"
    if [[ $dry_run -eq NO ]]; then
      $cmd
    fi
  done

  echo -e "\nBuilt images:"
  for image_name in "${image_names[@]}"; do
    echo "$image_name"
  done

}

main "$@"