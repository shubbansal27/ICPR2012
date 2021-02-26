#!/bin/sh

if [ "$1" = "trainWithClst.cpp" ] 
then
	output="train_file"
	
elif [ "$1" = "trainWithoutClst.cpp" ]
then
 	output="train_file"

elif [ "$1" = "pca.cpp" ]
then
 	output="pca"

elif [ "$1" = "testWithClst.cpp" ]
then
 	output="test_file"

elif [ "$1" = "testWithoutClst.cpp" ]
then
 	output="test_file"

elif [ "$1" = "norm.cpp" ]
then
 	output="norm"

elif [ "$1" = "overlap.cpp" ]
then
 	output="overlap"

elif [ "$1" = "clustering.cpp" ]
then
 	output="clustering"

elif [ "$1" = "normByCol.cpp" ]
then
 	output="norm"

elif [ "$1" = "trainRescan.cpp" ]
then
 	output="train_file"

elif [ "$1" = "testRescan.cpp" ]
then
 	output="test_file"

else
	output="a.out"
fi

g++ headers/timeseries.cpp $1 -o $output -I /usr/include/opencv -L /usr/lib  -lm -lcv -lhighgui -lcvaux


