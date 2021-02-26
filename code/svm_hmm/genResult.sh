rm result.txt
echo "compiling soures....\n\n"
g++ accuracy.cpp -o accuracy

clear
echo "Training model....\n\n"

#./svm_hmm_learn -c 5 -e 0.5 train/train.txt model #default

#./svm_hmm_learn -c $1 -e $2 --t $3 --e $4 train/train.txt model

#./svm_hmm_learn -c $1 -e $2 --t $3 train/train.txt model


echo "Classification starting....\n\n"

./svm_hmm_classify test/test1.txt model out1.txt
./svm_hmm_classify test/test2.txt model out2.txt
./svm_hmm_classify test/test3.txt model out3.txt
./svm_hmm_classify test/test4.txt model out4.txt
./svm_hmm_classify test/test5.txt model out5.txt


echo "building all results...\n\n"
##dataset5
#./accuracy original_label/dataset5/test1.txt out1.txt video1 >> result.txt 
#./accuracy original_label/dataset5/test2.txt out2.txt video2 >> result.txt 
#./accuracy original_label/dataset5/test3.txt out3.txt video3 >> result.txt 
#./accuracy original_label/dataset5/test4.txt out4.txt video4 >> result.txt 
#./accuracy original_label/dataset5/test5.txt out5.txt video5 >> result.txt

##dataset6
#./accuracy original_label/dataset6/video1.txt out1.txt video1 >> result.txt 
#./accuracy original_label/dataset6/video2.txt out2.txt video2 >> result.txt 
#./accuracy original_label/dataset6/video3.txt out3.txt video3 >> result.txt 
#./accuracy original_label/dataset6/video4.txt out4.txt video4 >> result.txt 
#./accuracy original_label/dataset6/video5.txt out5.txt video5 >> result.txt

##dataset7
./accuracy original_label/dataset7/video1.txt out1.txt video1 >> result.txt 
./accuracy original_label/dataset7/video2.txt out2.txt video2 >> result.txt 
./accuracy original_label/dataset7/video3.txt out3.txt video3 >> result.txt 
./accuracy original_label/dataset7/video4.txt out4.txt video4 >> result.txt 
./accuracy original_label/dataset7/video5.txt out5.txt video5 >> result.txt
 
gedit result.txt &

