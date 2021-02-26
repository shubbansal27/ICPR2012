sh compile.sh obj_track.cpp

mkdir dataset1
mkdir dataset2
mkdir dataset3
mkdir dataset4
mkdir dataset5
mkdir dataset6
mkdir dataset7

echo "\n\nreading dataset-1"
./a.out conf1/code1.conf dataset1/out1.txt 
./a.out conf1/code2.conf dataset1/out2.txt 
./a.out conf1/code3.conf dataset1/out3.txt 
./a.out conf1/code4.conf dataset1/out4.txt 
./a.out conf1/code5.conf dataset1/out5.txt 

echo "\n\nreading dataset-2"
./a.out conf2/code1.conf dataset2/out1.txt 
./a.out conf2/code2.conf dataset2/out2.txt 
./a.out conf2/code3.conf dataset2/out3.txt 
./a.out conf2/code4.conf dataset2/out4.txt 
./a.out conf2/code5.conf dataset2/out5.txt 


echo "\n\nreading dataset-3"
./a.out conf3/code1.conf dataset3/out1.txt 
./a.out conf3/code2.conf dataset3/out2.txt 
./a.out conf3/code3.conf dataset3/out3.txt 
./a.out conf3/code4.conf dataset3/out4.txt 
./a.out conf3/code5.conf dataset3/out5.txt 


echo "\n\nreading dataset-4"
./a.out conf4/code1.conf dataset4/out1.txt 
./a.out conf4/code2.conf dataset4/out2.txt 
./a.out conf4/code3.conf dataset4/out3.txt 
./a.out conf4/code4.conf dataset4/out4.txt 
./a.out conf4/code5.conf dataset4/out5.txt 


echo "\n\nreading dataset-5"
./a.out conf5/code1.conf dataset5/out1.txt 
./a.out conf5/code2.conf dataset5/out2.txt 
./a.out conf5/code3.conf dataset5/out3.txt 
./a.out conf5/code4.conf dataset5/out4.txt 
./a.out conf5/code5.conf dataset5/out5.txt 

echo "\n\nreading dataset-6"
./a.out conf6/code1.conf dataset6/out1.txt 
./a.out conf6/code2.conf dataset6/out2.txt 
./a.out conf6/code3.conf dataset6/out3.txt 
./a.out conf6/code4.conf dataset6/out4.txt 
./a.out conf6/code5.conf dataset6/out5.txt 

echo "\n\nreading dataset-7"
./a.out conf7/code1.conf dataset7/out1.txt 
./a.out conf7/code2.conf dataset7/out2.txt 
./a.out conf7/code3.conf dataset7/out3.txt 
./a.out conf7/code4.conf dataset7/out4.txt 
./a.out conf7/code5.conf dataset7/out5.txt 




