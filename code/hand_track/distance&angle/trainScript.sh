sh compile.sh handtrack.cpp

#mkdir train-dataset
#mkdir train-dataset/dataset1
#mkdir train-dataset/dataset2
#mkdir train-dataset/dataset3
#mkdir train-dataset/dataset4
#mkdir train-dataset/dataset5

dataset1="train-dataset/dataset1"
dataset2="train-dataset/dataset2"
dataset3="train-dataset/dataset3"
dataset4="train-dataset/dataset4"
dataset5="train-dataset/dataset5"

conf1="train_conf/conf1"
conf2="train_conf/conf2"
conf3="train_conf/conf3"
conf4="train_conf/conf4"
conf5="train_conf/conf5"

echo "\n\nreading dataset-1"
#./a.out $conf1/code1.conf $dataset1/1_left_angle.txt $dataset1/1_right_angle.txt $dataset1/1_hand_angle.txt $dataset1/1_left_dist.txt $dataset1/1_right_dist.txt
#./a.out $conf1/code2.conf $dataset1/2_left_angle.txt $dataset1/2_right_angle.txt $dataset1/2_hand_angle.txt $dataset1/2_left_dist.txt $dataset1/2_right_dist.txt
./a.out $conf1/code3.conf $dataset1/3_left_angle.txt $dataset1/3_right_angle.txt $dataset1/3_hand_angle.txt $dataset1/3_left_dist.txt $dataset1/3_right_dist.txt
#./a.out $conf1/code4.conf $dataset1/4_left_angle.txt $dataset1/4_right_angle.txt $dataset1/4_hand_angle.txt $dataset1/4_left_dist.txt $dataset1/4_right_dist.txt
./a.out $conf1/code5.conf $dataset1/5_left_angle.txt $dataset1/5_right_angle.txt $dataset1/5_hand_angle.txt $dataset1/5_left_dist.txt $dataset1/5_right_dist.txt

#echo "\n\nreading dataset-2"
#./a.out $conf2/code1.conf $dataset2/1_left_angle.txt $dataset2/1_right_angle.txt $dataset2/1_hand_angle.txt $dataset2/1_left_dist.txt $dataset2/1_right_dist.txt
#./a.out $conf2/code2.conf $dataset2/2_left_angle.txt $dataset2/2_right_angle.txt $dataset2/2_hand_angle.txt $dataset2/2_left_dist.txt $dataset2/2_right_dist.txt
#./a.out $conf2/code3.conf $dataset2/3_left_angle.txt $dataset2/3_right_angle.txt $dataset2/3_hand_angle.txt $dataset2/3_left_dist.txt $dataset2/3_right_dist.txt
#./a.out $conf2/code4.conf $dataset2/4_left_angle.txt $dataset2/4_right_angle.txt $dataset2/4_hand_angle.txt $dataset2/4_left_dist.txt $dataset2/4_right_dist.txt
#./a.out $conf2/code5.conf $dataset2/5_left_angle.txt $dataset2/5_right_angle.txt $dataset2/5_hand_angle.txt $dataset2/5_left_dist.txt $dataset2/5_right_dist.txt

#echo "\n\nreading dataset-3"
#./a.out $conf3/code1.conf $dataset3/1_left_angle.txt $dataset3/1_right_angle.txt $dataset3/1_hand_angle.txt $dataset3/1_left_dist.txt $dataset3/1_right_dist.txt
#./a.out $conf3/code2.conf $dataset3/2_left_angle.txt $dataset3/2_right_angle.txt $dataset3/2_hand_angle.txt $dataset3/2_left_dist.txt $dataset3/2_right_dist.txt
#./a.out $conf3/code3.conf $dataset3/3_left_angle.txt $dataset3/3_right_angle.txt $dataset3/3_hand_angle.txt $dataset3/3_left_dist.txt $dataset3/3_right_dist.txt
#./a.out $conf3/code4.conf $dataset3/4_left_angle.txt $dataset3/4_right_angle.txt $dataset3/4_hand_angle.txt $dataset3/4_left_dist.txt $dataset3/4_right_dist.txt
#./a.out $conf3/code5.conf $dataset3/5_left_angle.txt $dataset3/5_right_angle.txt $dataset3/5_hand_angle.txt $dataset3/5_left_dist.txt $dataset3/5_right_dist.txt

#echo "\n\nreading dataset-4"
#./a.out $conf4/code1.conf $dataset4/1_left_angle.txt $dataset4/1_right_angle.txt $dataset4/1_hand_angle.txt $dataset4/1_left_dist.txt $dataset4/1_right_dist.txt
#./a.out $conf4/code2.conf $dataset4/2_left_angle.txt $dataset4/2_right_angle.txt $dataset4/2_hand_angle.txt $dataset4/2_left_dist.txt $dataset4/2_right_dist.txt
#./a.out $conf4/code3.conf $dataset4/3_left_angle.txt $dataset4/3_right_angle.txt $dataset4/3_hand_angle.txt $dataset4/3_left_dist.txt $dataset4/3_right_dist.txt
#./a.out $conf4/code4.conf $dataset4/4_left_angle.txt $dataset4/4_right_angle.txt $dataset4/4_hand_angle.txt $dataset4/4_left_dist.txt $dataset4/4_right_dist.txt
#./a.out $conf4/code5.conf $dataset4/5_left_angle.txt $dataset4/5_right_angle.txt $dataset4/5_hand_angle.txt $dataset4/5_left_dist.txt $dataset4/5_right_dist.txt

#echo "\n\nreading dataset-5"
#./a.out $conf5/code1.conf $dataset5/1_left_angle.txt $dataset5/1_right_angle.txt $dataset5/1_hand_angle.txt $dataset5/1_left_dist.txt $dataset5/1_right_dist.txt
#./a.out $conf5/code2.conf $dataset5/2_left_angle.txt $dataset5/2_right_angle.txt $dataset5/2_hand_angle.txt $dataset5/2_left_dist.txt $dataset5/2_right_dist.txt
#./a.out $conf5/code3.conf $dataset5/3_left_angle.txt $dataset5/3_right_angle.txt $dataset5/3_hand_angle.txt $dataset5/3_left_dist.txt $dataset5/3_right_dist.txt
#./a.out $conf5/code4.conf $dataset5/4_left_angle.txt $dataset5/4_right_angle.txt $dataset5/4_hand_angle.txt $dataset5/4_left_dist.txt $dataset5/4_right_dist.txt
#./a.out $conf5/code5.conf $dataset5/5_left_angle.txt $dataset5/5_right_angle.txt $dataset5/5_hand_angle.txt $dataset5/5_left_dist.txt $dataset5/5_right_dist.txt





