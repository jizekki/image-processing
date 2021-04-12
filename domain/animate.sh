#!/bin/bash

EARTH_COLS=512;
STEP=16;

i=0;
while ((i < EARTH_COLS));
do 
    ./scroll $i 0 ../data/earth.ppm earth_scroll$i.ppm;
    ./sphere earth_scroll$i.ppm earth_sphere$i.ppm;
    i=$((i+STEP));
done

convert earth_sphere*.ppm earth.gif

rm -f earth_scroll*.ppm earth_sphere*.ppm