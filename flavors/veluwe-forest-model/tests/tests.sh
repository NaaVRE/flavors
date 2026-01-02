#!/bin/bash
set -e
# activate environment in cell-test or in jupyter image
source /venv/bin/activate || eval "$(conda shell.bash activate veluwe-forest-model)"

target_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)
cd "$target_dir"

find "$dir" -maxdepth 1 -name "*.py" -print0 | xargs --null -I "{}" python "{}"
find "$dir" -maxdepth 1 -name "*.R" -print0 | xargs --null -I "{}" Rscript "{}"

mkdir -m 777 output/ Output/ DFFS-output/ Metadata/

cleanup() {
    echo "Cleaning up LANDIS output files..."
    if [ "$CI" != "true" ]; then
        rm -f Landis-log.txt Landis-climate-log.txt *.tif *.csv
        rm -rf output/ Output/ DFFS-output/ Metadata/
    fi
}

trap cleanup EXIT

dotnet "$LANDIS_CONSOLE" scenario.txt
