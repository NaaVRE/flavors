#!/bin/bash
set -e
# activate environment in cell-test or in jupyter image
source /venv/bin/activate || eval "$(conda shell.bash activate jupyter-quality)"
dir="${0%/*}"
find "$dir" -name "*.py" -print0 | xargs --null -I "{}" python "{}"
