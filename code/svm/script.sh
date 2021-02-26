#!/bin/sh

z=$(zenity --entry --text "which test-DATA ??" --entry-text "test1");
if [ "$z" = "test1" ] 
then
	pathTest="learning-classify/svm/test/test1.txt"
	pathOrig="learning-classify/svm/original_label/test1.txt"

elif [ "$z" = "test2" ]
then
 	pathTest="learning-classify/svm/test/test2.txt"
	pathOrig="learning-classify/svm/original_label/test2.txt"

elif [ "$z" = "test3" ]
then
 	pathTest="learning-classify/svm/test/test3.txt"
	pathOrig="learning-classify/svm/original_label/test3.txt"

elif [ "$z" = "test4" ]
then
 	pathTest="learning-classify/svm/test/test4.txt"
	pathOrig="learning-classify/svm/original_label/test4.txt"

elif [ "$z" = "test5" ]
then
 	pathTest="learning-classify/svm/test/test5.txt"
	pathOrig="learning-classify/svm/original_label/test5.txt"	
fi


#-----------------------------------------------------------------------------------------------------------------------------------------------------

cluster=$(zenity --entry --text "Cluster size" --entry-text "50");


#training

./learning-classify/svm/svm_multiclass_learn -c 5000 learning-classify/svm/train/train.txt learning-classify/svm/model



#classification

./learning-classify/svm/svm_multiclass_classify $pathTest learning-classify/svm/model learning-classify/svm/output.txt



#first_word

./learning-classify/svm/firstword learning-classify/svm/output.txt learning-classify/svm/outputLabel.txt
rm learning-classify/svm/output.txt
rm learning-classify/svm/model



#expand output label n times


./learning-classify/svm/expandOut learning-classify/svm/outputLabel.txt learning-classify/svm/output.dat $cluster
rm learning-classify/svm/outputLabel.txt



#accuracy
clear;
echo "path of test data: $pathTest\n"
echo "path of original labels: $pathOrig\n"
./learning-classify/svm/accuracy $pathOrig learning-classify/svm/output.dat > learning-classify/svm/result.txt 










