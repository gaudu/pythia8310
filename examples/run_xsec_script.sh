#!/bin/bash

cd ~/PYTHIA/pythia8310/examples/
make main1010

    sbatch \
            --partition=normal \
            --mail-type="ALL" \
            --mail-user="gaudu@uni-wuppertal.de" \
            --array=8-21 \
            --job-name="main1010_xsec"\
            -- ~/xsec_script.sh 