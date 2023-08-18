#!/bin/bash

ffmpeg -i iMovie.mp4 -filter:v "crop=910:682:0:38,scale=320:240" -c:a copy -an -y crop.mp4
ffmpeg -i crop.mp4 -vf "fps=12" -q:v 6  -y loop.mjpg