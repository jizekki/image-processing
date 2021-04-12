#!/bin/bash

make
./filter-zoom 1 box ../data/cameraman.ppm box1.ppm
./filter-zoom 1 bell ../data/cameraman.ppm bell1.ppm
./filter-zoom 1 tent ../data/cameraman.ppm tent1.ppm
./filter-zoom 1 mitch ../data/cameraman.ppm mitch1.ppm
./filter-zoom 10 box ../data/cameraman.ppm box10.ppm
./filter-zoom 10 bell ../data/cameraman.ppm bell10.ppm
./filter-zoom 10 tent ../data/cameraman.ppm tent10.ppm
./filter-zoom 10 mitch ../data/cameraman.ppm mitch10.ppm

