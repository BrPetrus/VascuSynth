#!/bin/bash

split() {
    # Load all image folders
    folders=( $(ls -d image* | grep -v "imageNames.txt") )

    # Shuffle the array
    shuffled_folders=($(echo "${folders[@]}" | tr ' ' '\n' | shuf))

    # Split the data
    total=${#shuffled_folders[@]}
    train_size=$((total * 80 / 100))
    train_set=("${shuffled_folders[@]:0:train_size}")
    test_set=("${shuffled_folders[@]:train_size}")

    echo "Train Set: ${train_set[@]}"
    echo "Test Set: ${test_set[@]}"

    # Move the data
    if ! [[ -d dataset ]]; then mkdir dataset; fi
    if ! [[ -d "dataset/training" ]]; then mkdir "dataset/training"; fi
    if ! [[ -d "dataset/test" ]]; then mkdir "dataset/test"; fi
    mv "${train_set[@]}" "dataset/training"
    mv "${test_set[@]}" "dataset/test"
}

generate() {
    #param_files=( $(ls -d p*.txt | grep -v "paramFiles.txt") )
    echo ${param_files[@]}
    return 1
}

split

