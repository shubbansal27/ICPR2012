sh compile.sh testRescan.cpp 
sh compile.sh trainRescan.cpp

sh train_file.sh
sh test_file.sh
sh norm.sh 10
rm ../svm/train/*
rm ../svm/test/*
cp results/normalised/svm/train.txt ../svm/train/
cp results/normalised/svm/test1.txt ../svm/test/  
cp results/normalised/svm/test2.txt ../svm/test/  
cp results/normalised/svm/test3.txt ../svm/test/  
cp results/normalised/svm/test4.txt ../svm/test/  
cp results/normalised/svm/test5.txt ../svm/test/
cd ../svm

C=5
sh genResult.sh $C
cp result.txt result/result$C.txt
cd ../svmFormat 
