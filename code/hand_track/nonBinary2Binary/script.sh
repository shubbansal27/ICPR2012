#!/bin/sh

sh compile.sh code.cpp

size=2

i=1

while [ $i -le 7 ]
do

echo "\nReading dataset-$i ...."

n=1;
while [ $n -le 5 ]
do

./a.out input/dataset"$i"/ftrs"$n".txt output/dataset$i/bin$n.txt $size;

echo "########  done $n #########\n"
n=$((n + 1));
done

i=$((i + 1));
done

echo "\nall done ....\n"
