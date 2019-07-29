#!/sur/bin/env bash

## Parameter testing!
VIDEO_DIR=$1
if [[ ! -d $VIDEO_DIR ]]; then
    echo "Als zweiter Parameter muss ein Ordner mit Videos angegeben werden!"
    exit 1
fi

## Values gatheres from the existing test videos!
WIDTH=640
HEIGHT=392

## Delete last "/" if exists!
VIDEO_DIR=${VIDEO_DIR%*/}

for file in $VIDEO_DIR/*; do
    
    ## Test if file is video!
    ENDING=${file##*.}
    ENDING=$(echo "$ENDING" | tr '[:upper:]' '[:lower:]')
    if [[ ! "$ENDING" =~ ^(mkv|avi|mp4|m4p|mpg|mpeg)$ ]]; then
        continue;
    fi

    echo "Processing Video: $file ..."

    ## Get dimensions of the video frames
    VALUES=($(echo "$(ffprobe -v error -select_streams v:0 -show_entries stream=width,height -of csv=s=x:p=0 $file)" | tr "x" "\n"))
    if [[ ${VALUES[0]} -ne WIDTH || ${VALUES[1]} -ne HEIGHT ]]; then
        echo "$file ist nicht $WIDTH x $HEIGHT px breit! (Sondern: ${VALUES[0]}x${VALUES[1]})"
        continue
    fi

    FILENAME=($(echo "${file##*/}" | tr "." "\n"))
    NEU="c.$FILENAME.$ENDING"

    ## Create new video from existing one
    ffmpeg -y -i $file -vf "crop=510:350:65:0" -c:v libx264 -crf 5 -c:a copy $VIDEO_DIR/$NEU &>/dev/null
    if [[ $? -ne 0 ]]; then
        echo "Croppen des Videos $file hat nicht funktioniert!"
        exit 1
    fi

    echo "Processing Video done!"
done