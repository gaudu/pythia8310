#!/bin/bash

cd ~/PYTHIA/pythia8310/examples/
make main1009

tune_folder_path="/beegfs/gaudu/APPRENTICE/bin/tune/"

if [ -d "$tune_folder_path" ]; then
    folders=$(find "$tune_folder_path" -mindepth 1 -maxdepth 1 -type d | wc -l)
    i=$((folders - 1))

    sbatch \
            --partition=normal \
            --mail-type="ALL" \
            --mail-user="gaudu@uni-wuppertal.de" \
            --array=0-$i \
            --job-name="main1009_tune"\
            -- ~/pleiades_apprentice_script.sh 
else
    echo "folder not found: $tune_folder_path"
fi