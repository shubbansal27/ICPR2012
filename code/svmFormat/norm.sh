#!/bin/sh

echo "normalising results.......\n"
total_feature=$1

./norm results/train.txt results/normalised/hmm/train.txt results/normalised/svm/train.txt $total_feature
echo "train.txt normalised..\n"
#cp results/normalised/svm/train.txt learning-classify/svm/train/train.txt



./norm results/test1.txt results/normalised/hmm/test1.txt results/normalised/svm/test1.txt $total_feature
echo "test1.txt normalised..\n"

./norm results/test2.txt results/normalised/hmm/test2.txt results/normalised/svm/test2.txt $total_feature
echo "test2.txt normalised..\n"

./norm results/test3.txt results/normalised/hmm/test3.txt results/normalised/svm/test3.txt $total_feature
echo "test3.txt normalised..\n"

./norm results/test4.txt results/normalised/hmm/test4.txt results/normalised/svm/test4.txt $total_feature
echo "test4.txt normalised..\n"

./norm results/test5.txt results/normalised/hmm/test5.txt results/normalised/svm/test5.txt $total_feature
echo "test5.txt normalised..\n"

#cp results/normalised/test1.txt learning-classify/svm/test/test1.txt
#cp results/normalised/test2.txt learning-classify/svm/test/test2.txt
#cp results/normalised/test3.txt learning-classify/svm/test/test3.txt
#cp results/normalised/test4.txt learning-classify/svm/test/test4.txt
#cp results/normalised/test5.txt learning-classify/svm/test/test5.txt





