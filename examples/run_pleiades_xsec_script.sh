#!/bin/bash

cd ~/PYTHIA/pythia8310/examples/
make main1010

    sbatch \
            --partition=normal \
            --mail-type="ALL" \
            --mail-user="gaudu@uni-wuppertal.de" \
            --array=2-12 \
            --job-name="main1010_xsec"\
            -- ~/pleiades_xsec_script.sh 