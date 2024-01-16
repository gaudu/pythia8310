#!/bin/bash

cd ~/PYTHIA/pythia8310/examples/

tune_folder_path="/beegfs/gaudu/APPRENTICE/bin/tune/tune_results/"
declare -A folder_names
folder_names[0]="tnc"
folder_names[1]="ngc"
folder_names[3]="lbfgsb"
folder_names[4]="trust"

if [ -d "$tune_folder_path" ]; then
    formatted_i=$(printf "%01d" $SLURM_ARRAY_TASK_ID)

    for formatted_i in "${!folder_names[@]}"; do
        folder_name="${folder_names[$formatted_i]}"
        template_path=$tune_folder_path"$folder_name/template.dat"
    
        if [ -e "$template_path" ]; then
            mapfile -t lines < "$template_path" 
            
            for ((j=0; j<${#lines[@]}; j++)); do
                lines[j]=${lines[j]// /}
            done 

            ./main1009 158 $folder_name ${lines[0]} ${lines[1]} ${lines[2]} ${lines[3]} ${lines[4]} && ./main1009 350 $folder_name ${lines[0]} ${lines[1]} ${lines[2]} ${lines[3]} ${lines[4]}
        else
            echo "File not found: $template_path"
        fi
        
        cat main1009_tune_158_"$folder_name"_1M.yoda main1009_tune_350_"$folder_name"_1M.yoda > main1009_tune_"$folder_name"_1M.yoda
        cp main1009_tune_"$folder_name"_1M.yoda $tune_folder_path/$folder_name/main1009_tune_"$folder_name"_1M.yoda
    done
else
    echo "folder not found: $tune_folder_path"
fi