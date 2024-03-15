#!/bin/bash

cd ~/PYTHIA/pythia8310/examples/

idA=(2212 -2212 2112 -2112 111 211 -211 311 321 -321 130 310 3122 3212 3222 3112 3322)
idB=(2212 1000060120 1000070140 1000080160 1000180400)
lab_energy="1e$SLURM_ARRAY_TASK_ID" # in eV

for idA in "${idA[@]}"; do

    for idB in "${idB[@]}"; do

        ./main1010 $idA $idB $lab_energy

    done
done