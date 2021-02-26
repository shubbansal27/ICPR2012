sh compile.sh handtrack.cpp
clear

echo "dataset10 \n"
mkdir test-dataset
mkdir test-dataset/video1

dataset1="test-dataset/video1/"
conf1="test_conf/Video1/"

echo "\n\nreading dataset-1"
./a.out $conf1/code1.conf $dataset1/1_left_angle.txt $dataset1/1_right_angle.txt $dataset1/1_hand_angle.txt $dataset1/1_left_dist.txt $dataset1/1_right_dist.txt
./a.out $conf1/code2.conf $dataset1/2_left_angle.txt $dataset1/2_right_angle.txt $dataset1/2_hand_angle.txt $dataset1/2_left_dist.txt $dataset1/2_right_dist.txt
./a.out $conf1/code3.conf $dataset1/3_left_angle.txt $dataset1/3_right_angle.txt $dataset1/3_hand_angle.txt $dataset1/3_left_dist.txt $dataset1/3_right_dist.txt
./a.out $conf1/code4.conf $dataset1/4_left_angle.txt $dataset1/4_right_angle.txt $dataset1/4_hand_angle.txt $dataset1/4_left_dist.txt $dataset1/4_right_dist.txt
./a.out $conf1/code5.conf $dataset1/5_left_angle.txt $dataset1/5_right_angle.txt $dataset1/5_hand_angle.txt $dataset1/5_left_dist.txt $dataset1/5_right_dist.txt

#---------------------------------------------------------------------------------------------------------------------------------------------------------------

echo "dataset11 \n"
mkdir test-dataset/video2

dataset1="test-dataset/video2/"
conf1="test_conf/Video2/"

echo "\n\nreading dataset-1"
./a.out $conf1/code1.conf $dataset1/1_left_angle.txt $dataset1/1_right_angle.txt $dataset1/1_hand_angle.txt $dataset1/1_left_dist.txt $dataset1/1_right_dist.txt
./a.out $conf1/code2.conf $dataset1/2_left_angle.txt $dataset1/2_right_angle.txt $dataset1/2_hand_angle.txt $dataset1/2_left_dist.txt $dataset1/2_right_dist.txt
./a.out $conf1/code3.conf $dataset1/3_left_angle.txt $dataset1/3_right_angle.txt $dataset1/3_hand_angle.txt $dataset1/3_left_dist.txt $dataset1/3_right_dist.txt
./a.out $conf1/code4.conf $dataset1/4_left_angle.txt $dataset1/4_right_angle.txt $dataset1/4_hand_angle.txt $dataset1/4_left_dist.txt $dataset1/4_right_dist.txt
./a.out $conf1/code5.conf $dataset1/5_left_angle.txt $dataset1/5_right_angle.txt $dataset1/5_hand_angle.txt $dataset1/5_left_dist.txt $dataset1/5_right_dist.txt




