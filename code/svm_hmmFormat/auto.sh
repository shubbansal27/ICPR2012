sh compile.sh testRescan.cpp 
sh compile.sh trainRescan.cpp
sh train_file.sh
sh test_file.sh

sh norm.sh 28


rm ../svm_hmm/train/*
rm ../svm_hmm/test/*
cp results/normalised/svm/train.txt ../svm_hmm/train/
cp results/normalised/svm/test1.txt ../svm_hmm/test/  
cp results/normalised/svm/test2.txt ../svm_hmm/test/  
cp results/normalised/svm/test3.txt ../svm_hmm/test/  
cp results/normalised/svm/test4.txt ../svm_hmm/test/  
cp results/normalised/svm/test5.txt ../svm_hmm/test/
cd ../svm_hmm

C=10
e=2
t=2
#ee=1
sh genResult.sh $C $e $t #$ee
sh genScore.sh

f=12
mkdir result/result$f
cp result.txt result/result$f/result.txt
mv score1.txt result/result$f/score1.txt
mv score2.txt result/result$f/score2.txt
mv score3.txt result/result$f/score3.txt
mv score4.txt result/result$f/score4.txt
mv score5.txt result/result$f/score5.txt

mv out1.txt result/result$f/out1.txt
mv out2.txt result/result$f/out2.txt
mv out3.txt result/result$f/out3.txt
mv out4.txt result/result$f/out4.txt
mv out5.txt result/result$f/out5.txt

cd ../svm_hmmFormat 
