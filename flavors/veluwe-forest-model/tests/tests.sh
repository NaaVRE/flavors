#!/bin/bash
set -e
# activate environment in cell-test or in jupyter image
source /venv/bin/activate || eval "$(conda shell.bash activate veluwe-forest-model)"

target_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)
cd "$target_dir"

find "$dir" -maxdepth 1 -name "*.py" -print0 | xargs --null -I "{}" python "{}"
find "$dir" -maxdepth 1 -name "*.R" -print0 | xargs --null -I "{}" Rscript "{}"

cleanup() {
    echo "Cleaning up LANDIS output files..."
    rm -f Landis-log.txt
    rm -f Landis-climate-log.txt
    rm -rf output/
    rm -rf Output/
    rm -rf DFFS-output/
    rm -rf Metadata/
    rm -f *.tif
    rm -f *.csv
}

trap cleanup EXIT

dotnet "$LANDIS_CONSOLE" scenario.txt
