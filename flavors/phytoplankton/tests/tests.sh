#!/bin/bash
set -e
# activate environment in cell-test or in jupyter image
source /venv/bin/activate || eval "$(conda shell.bash activate phytoplankton)"
dir="${0%/*}"
find "$dir" -name "*.R" -print0 | xargs --null -I "{}" Rscript "{}"
