#!/bin/bash
set -e
# activate environment in cell-test or in jupyter image
source /venv/bin/activate || eval "$(conda shell.bash activate biotisan-euromarec)"
dir="${0%/*}"
cd "$dir"
find "$dir" -name "*.py" -print0 | xargs --null -I "{}" python "{}"
find "$dir" -name "*.R" -print0 | xargs --null -I "{}" Rscript "{}"
rm "metadata_Example.csv"
rm "data_Example.csv"
