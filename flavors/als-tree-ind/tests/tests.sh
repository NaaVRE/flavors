#!/bin/bash
set -e
# activate environment in cell-test or in jupyter image
source /venv/bin/activate || eval "$(conda shell.bash activate als-tree-ind)"
dir="${0%/*}"
find "$dir" -name "*.py" -print0 | xargs --null -I "{}" python "{}"
find "$dir" -name "*.R" -print0 | xargs --null -I "{}" Rscript "{}"

if [ -x /bin/Workflow_ALS_Trees/clipping/release/bin/ClipLas ]; then echo "ClipLas executable" ; else exit 255;  fi
if [ -x /bin/Workflow_ALS_Trees/retile_by_count/release/bin/RetileByCount ]; then echo "RetileByCount executable" ; else exit 255;  fi
if [ -x /bin/Workflow_ALS_Trees/retile_by_size/release/bin/RetileBySize ]; then echo "RetileBySize executable" ; else exit 255;  fi
