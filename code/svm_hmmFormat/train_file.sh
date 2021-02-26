#!/bin/sh
rm results/train.txt
clear

i=1
qid=1
while [ $i -le 5 ]
do

echo "\nReading dataset-$i ...."

str="boild-egg-$i.txt;ham-egg-$i.txt;kinshi-egg-$i.txt;omelette-$i.txt;scramble-egg-$i.txt"
labels=$(echo $str | tr ";" "\n")

n=1;
for item in $labels
do

./train_file train-dataset/dataset"$i"/"$n"_left_dist.txt train-dataset/dataset"$i"/"$n"_right_dist.txt train-dataset/dataset"$i"/"$n"_left_angle.txt train-dataset/dataset"$i"/"$n"_right_angle.txt train-dataset/dataset"$i"/"$n"_hand_angle.txt train-dataset/dataset"$i"/out"$n".txt train-dataset/dataset"$i"/stick"$n".txt train-dataset/dataset"$i"/katori"$n".txt train-dataset/dataset"$i"/knife"$n".txt train-dataset/dataset"$i"/turner"$n".txt train-dataset/dataset"$i"/namak"$n".txt train-dataset/dataset"$i"/ftrs"$n".txt train-dataset/label"$i"/$item results/train.txt train-dataset/dataset"$i"/breaking"$n".txt $qid train-dataset/dataset"$i"/bin"$n".txt;

echo "########  done $n #########\n"
n=$((n + 1));
qid=$((qid + 1));
done

i=$((i + 1));
done

echo "\ntrain.txt successfully created ....\n"
