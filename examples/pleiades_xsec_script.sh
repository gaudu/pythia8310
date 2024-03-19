#!/bin/bash

cd ~/PYTHIA/pythia8310/examples/

idA=(2212 -2212 2112 -2112 111 211 -211 311 321 -321 130 310 3122 3212 3222 3112 3322)
idB=(2212 1000060120 1000070140 1000080160 1000180400)
lab_energy=$SLURM_ARRAY_TASK_ID # in GeV

for id1 in "${idA[@]}"; do

    for id2 in "${idB[@]}"; do

        ./main1010 $id1 $id2 $lab_energy

    done
done