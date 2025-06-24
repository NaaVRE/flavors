#!/bin/bash
set -e
source /venv/bin/activate
dir="${0%/*}"
find "$dir" -name "*.py" -print0 | xargs --null -I "{}" python "{}"
find "$dir" -name "*.R" -print0 | xargs --null -I "{}" Rscript "{}"
