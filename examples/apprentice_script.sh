#!/bin/bash

cd /home/PYTHIA/pythia8310/examples/
make main1009

tune_folder_path="/home/gaudu/pyapprentice-1.1.0/bin/tune/"

if [ -d "$tune_folder_path" ]; then
    folders=$(find "$tune_folder_path" -mindepth 1 -maxdepth 1 -type d | wc -l)
else
    echo "folder not found: $tune_folder_path"
fi

for ((i=0; i<$folders; i++)); do
    formatted_i=$(printf "%02d" $i)
    template_path=$tune_folder_path"$formatted_i/template.dat"
    
    if [ -e "$template_path" ]; then
        mapfile -t lines < "$template_path"        
    else
        echo "File not found: $template_path"
    fi
    
    for ((j=0; j<${#lines[@]}; j++)); do
        lines[j]=${lines[j]// /}
    done
	    
    ./main1009 158 $formatted_i ${lines[0]} ${lines[1]} ${lines[2]} ${lines[3]} ${lines[4]} && ./main1009 350 $formatted_i ${lines[0]} ${lines[1]} ${lines[2]} ${lines[3]} ${lines[4]}

    cat main1009_tune_158_"$formatted_i"_10000.yoda main1009_tune_350_"$formatted_i"_10000.yoda > main1009_tune_"$formatted_i"_10000.yoda
    cp main1009_tune_"$formatted_i"_10000.yoda $tune_folder_path/$formatted_i/main1009_tune_"$formatted_i"_10000.yoda
done