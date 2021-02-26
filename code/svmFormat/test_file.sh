#!/bin/sh

rm results/test1.txt
rm results/test2.txt
rm results/test3.txt
rm results/test4.txt
rm results/test5.txt
clear


i=6
while [ $i -le 6 ]
do

echo "\nReading dataset-$i ...."

str="boild-egg-$i.txt;ham-egg-$i.txt;kinshi-egg-$i.txt;omelette-$i.txt;scramble-egg-$i.txt"
labels=$(echo $str | tr ";" "\n")

n=1;
for item in $labels
do

./test_file test-dataset/dataset"$i"/"$n"_left_dist.txt test-dataset/dataset"$i"/"$n"_right_dist.txt test-dataset/dataset"$i"/"$n"_left_angle.txt test-dataset/dataset"$i"/"$n"_right_angle.txt test-dataset/dataset"$i"/"$n"_hand_angle.txt test-dataset/dataset"$i"/out"$n".txt test-dataset/dataset"$i"/stick"$n".txt test-dataset/dataset"$i"/katori"$n".txt test-dataset/dataset"$i"/knife"$n".txt test-dataset/dataset"$i"/turner"$n".txt test-dataset/dataset"$i"/namak"$n".txt test-dataset/dataset"$i"/ftrs"$n".txt test-dataset/label"$i"/$item results/test$n.txt test-dataset/dataset"$i"/breaking"$n".txt;

echo "########  done $n #########\n"
n=$((n + 1));
done

i=$((i + 1));
done

echo "\nall test.txt successfully created ....\n"
