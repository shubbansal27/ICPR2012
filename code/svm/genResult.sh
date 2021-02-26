rm result.txt
echo "compiling soures....\n\n"
g++ firstWord.cpp -o firstword
g++ accuracy.cpp -o accuracy

clear
echo "Training model....\n\n"
#./svm_multiclass_learn -c 5000 train/train.txt model
./svm_multiclass_learn -c $1 train/train.txt model
#./svm_multiclass_learn -t 2 -c 2 train/train.txt model
#./svm_multiclass_learn -c $1 -t 2  train/train.txt model
clear
echo "Classification starting....\n\n"
./svm_multiclass_classify test/test1.txt model output.txt
./firstword output.txt out1.txt

./svm_multiclass_classify test/test2.txt model output.txt
./firstword output.txt out2.txt

./svm_multiclass_classify test/test3.txt model output.txt
./firstword output.txt out3.txt

./svm_multiclass_classify test/test4.txt model output.txt
./firstword output.txt out4.txt

./svm_multiclass_classify test/test5.txt model output.txt
./firstword output.txt out5.txt

clear
echo "building all results...\n\n"
#./accuracy original_label/dataset5/test1.txt out1.txt video1 >> result.txt 
#./accuracy original_label/dataset5/test2.txt out2.txt video2 >> result.txt 
#./accuracy original_label/dataset5/test3.txt out3.txt video3 >> result.txt 
#./accuracy original_label/dataset5/test4.txt out4.txt video3 >> result.txt 
#./accuracy original_label/dataset5/test5.txt out5.txt video4 >> result.txt

./accuracy original_label/dataset6/video1.txt out1.txt video1 >> result.txt 
./accuracy original_label/dataset6/video2.txt out2.txt video2 >> result.txt 
./accuracy original_label/dataset6/video3.txt out3.txt video3 >> result.txt 
./accuracy original_label/dataset6/video4.txt out4.txt video3 >> result.txt 
./accuracy original_label/dataset6/video5.txt out5.txt video4 >> result.txt

#./accuracy original_label/dataset7/video1.txt out1.txt video1 >> result.txt 
#./accuracy original_label/dataset7/video2.txt out2.txt video2 >> result.txt 
#./accuracy original_label/dataset7/video3.txt out3.txt video3 >> result.txt 
#./accuracy original_label/dataset7/video4.txt out4.txt video3 >> result.txt 
#./accuracy original_label/dataset7/video5.txt out5.txt video4 >> result.txt

clear 
rm output.txt

gedit result.txt &

