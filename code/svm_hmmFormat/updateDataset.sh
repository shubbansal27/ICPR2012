clear
rm train-dataset/dataset1/*
rm train-dataset/dataset2/*
rm train-dataset/dataset3/*
rm train-dataset/dataset4/*
rm train-dataset/dataset5/*
rm train-dataset/dataset6/*
rm train-dataset/dataset7/* 

rm test-dataset/dataset1/*
rm test-dataset/dataset2/*
rm test-dataset/dataset3/*
rm test-dataset/dataset4/*
rm test-dataset/dataset5/*
rm test-dataset/dataset6/*
rm test-dataset/dataset7/*

clear
echo "old data removed.....\n" 
echo "updating train-dataset........\n"
echo "dataset-1..."
cp ../object_track/chopstick/dataset1/* train-dataset/dataset1/
cp ../object_track/katori/dataset1/* train-dataset/dataset1/
cp ../object_track/knife/dataset1/* train-dataset/dataset1/
cp ../object_track/namak/dataset1/* train-dataset/dataset1/
cp ../object_track/turner/dataset1/* train-dataset/dataset1/
cp ../object_track/some_objects/dataset1/* train-dataset/dataset1/
cp ../object_track/breaking/dataset1/* train-dataset/dataset1/
cp ../hand_track/newFeatures/dataset1/* train-dataset/dataset1/
cp ../hand_track/distance\&angle/train-dataset/dataset1/* train-dataset/dataset1/
echo "updated\n"

echo "dataset-2..."
cp ../object_track/chopstick/dataset2/* train-dataset/dataset2/
cp ../object_track/katori/dataset2/* train-dataset/dataset2/
cp ../object_track/knife/dataset2/* train-dataset/dataset2/
cp ../object_track/namak/dataset2/* train-dataset/dataset2/
cp ../object_track/turner/dataset2/* train-dataset/dataset2/
cp ../object_track/some_objects/dataset2/* train-dataset/dataset2/
cp ../object_track/breaking/dataset2/* train-dataset/dataset2/
cp ../hand_track/newFeatures/dataset2/* train-dataset/dataset2/
cp ../hand_track/distance\&angle/train-dataset/dataset2/* train-dataset/dataset2/
echo "updated\n"

echo "dataset-3..."
cp ../object_track/chopstick/dataset3/* train-dataset/dataset3/
cp ../object_track/katori/dataset3/* train-dataset/dataset3/
cp ../object_track/knife/dataset3/* train-dataset/dataset3/
cp ../object_track/namak/dataset3/* train-dataset/dataset3/
cp ../object_track/turner/dataset3/* train-dataset/dataset3/
cp ../object_track/some_objects/dataset3/* train-dataset/dataset3/
cp ../object_track/breaking/dataset3/* train-dataset/dataset3/
cp ../hand_track/newFeatures/dataset3/* train-dataset/dataset3/
cp ../hand_track/distance\&angle/train-dataset/dataset3/* train-dataset/dataset3/
echo "updated\n"

echo "dataset-4..."
cp ../object_track/chopstick/dataset4/* train-dataset/dataset4/
cp ../object_track/katori/dataset4/* train-dataset/dataset4/
cp ../object_track/knife/dataset4/* train-dataset/dataset4/
cp ../object_track/namak/dataset4/* train-dataset/dataset4/
cp ../object_track/turner/dataset4/* train-dataset/dataset4/
cp ../object_track/some_objects/dataset4/* train-dataset/dataset4/
cp ../object_track/breaking/dataset4/* train-dataset/dataset4/
cp ../hand_track/newFeatures/dataset4/* train-dataset/dataset4/
cp ../hand_track/distance\&angle/train-dataset/dataset4/* train-dataset/dataset4/
echo "updated\n"

echo "dataset-5..."
cp ../object_track/chopstick/dataset5/* train-dataset/dataset5/
cp ../object_track/katori/dataset5/* train-dataset/dataset5/
cp ../object_track/knife/dataset5/* train-dataset/dataset5/
cp ../object_track/namak/dataset5/* train-dataset/dataset5/
cp ../object_track/turner/dataset5/* train-dataset/dataset5/
cp ../object_track/some_objects/dataset5/* train-dataset/dataset5/
cp ../object_track/breaking/dataset5/* train-dataset/dataset5/
cp ../hand_track/newFeatures/dataset5/* train-dataset/dataset5/
cp ../hand_track/distance\&angle/train-dataset/dataset5/* train-dataset/dataset5/
echo "updated\n"

echo "dataset-6..."
cp ../object_track/chopstick/dataset6/* train-dataset/dataset6/
cp ../object_track/katori/dataset6/* train-dataset/dataset6/
cp ../object_track/knife/dataset6/* train-dataset/dataset6/
cp ../object_track/namak/dataset6/* train-dataset/dataset6/
cp ../object_track/turner/dataset6/* train-dataset/dataset6/
cp ../object_track/some_objects/dataset6/* train-dataset/dataset6/
cp ../object_track/breaking/dataset6/* train-dataset/dataset6/
cp ../hand_track/newFeatures/dataset6/* train-dataset/dataset6/
cp ../hand_track/distance\&angle/test-dataset/video1/* train-dataset/dataset6/
echo "updated\n"

echo "dataset-7..."
cp ../object_track/chopstick/dataset7/* train-dataset/dataset7/
cp ../object_track/katori/dataset7/* train-dataset/dataset7/
cp ../object_track/knife/dataset7/* train-dataset/dataset7/
cp ../object_track/namak/dataset7/* train-dataset/dataset7/
cp ../object_track/turner/dataset7/* train-dataset/dataset7/
cp ../object_track/some_objects/dataset7/* train-dataset/dataset7/
cp ../object_track/breaking/dataset7/* train-dataset/dataset7/
cp ../hand_track/newFeatures/dataset7/* train-dataset/dataset7/
cp ../hand_track/distance\&angle/test-dataset/video2/* train-dataset/dataset7/
echo "updated\n"

#-------------------------------------------------------------------------------------------------------------------------------------------

echo "\nupdating test-dataset........\n"
echo "dataset-1..."
cp ../object_track/chopstick/dataset1/* test-dataset/dataset1/
cp ../object_track/katori/dataset1/* test-dataset/dataset1/
cp ../object_track/knife/dataset1/* test-dataset/dataset1/
cp ../object_track/namak/dataset1/* test-dataset/dataset1/
cp ../object_track/turner/dataset1/* test-dataset/dataset1/
cp ../object_track/some_objects/dataset1/* test-dataset/dataset1/
cp ../object_track/breaking/dataset1/* test-dataset/dataset1/
cp ../hand_track/newFeatures/dataset1/* test-dataset/dataset1/
cp ../hand_track/distance\&angle/train-dataset/dataset1/* test-dataset/dataset1/
echo "updated\n"

echo "dataset-2..."
cp ../object_track/chopstick/dataset2/* test-dataset/dataset2/
cp ../object_track/katori/dataset2/* test-dataset/dataset2/
cp ../object_track/knife/dataset2/* test-dataset/dataset2/
cp ../object_track/namak/dataset2/* test-dataset/dataset2/
cp ../object_track/turner/dataset2/* test-dataset/dataset2/
cp ../object_track/some_objects/dataset2/* test-dataset/dataset2/
cp ../object_track/breaking/dataset2/* test-dataset/dataset2/
cp ../hand_track/newFeatures/dataset2/* test-dataset/dataset2/
cp ../hand_track/distance\&angle/train-dataset/dataset2/* test-dataset/dataset2/
echo "updated\n"

echo "dataset-3..."
cp ../object_track/chopstick/dataset3/* test-dataset/dataset3/
cp ../object_track/katori/dataset3/* test-dataset/dataset3/
cp ../object_track/knife/dataset3/* test-dataset/dataset3/
cp ../object_track/namak/dataset3/* test-dataset/dataset3/
cp ../object_track/turner/dataset3/* test-dataset/dataset3/
cp ../object_track/some_objects/dataset3/* test-dataset/dataset3/
cp ../object_track/breaking/dataset3/* test-dataset/dataset3/
cp ../hand_track/newFeatures/dataset3/* test-dataset/dataset3/
cp ../hand_track/distance\&angle/train-dataset/dataset3/* test-dataset/dataset3/
echo "updated\n"

echo "dataset-4..."
cp ../object_track/chopstick/dataset4/* test-dataset/dataset4/
cp ../object_track/katori/dataset4/* test-dataset/dataset4/
cp ../object_track/knife/dataset4/* test-dataset/dataset4/
cp ../object_track/namak/dataset4/* test-dataset/dataset4/
cp ../object_track/turner/dataset4/* test-dataset/dataset4/
cp ../object_track/some_objects/dataset4/* test-dataset/dataset4/
cp ../object_track/breaking/dataset4/* test-dataset/dataset4/
cp ../hand_track/newFeatures/dataset4/* test-dataset/dataset4/
cp ../hand_track/distance\&angle/train-dataset/dataset4/* test-dataset/dataset4/
echo "updated\n"

echo "dataset-5..."
cp ../object_track/chopstick/dataset5/* test-dataset/dataset5/
cp ../object_track/katori/dataset5/* test-dataset/dataset5/
cp ../object_track/knife/dataset5/* test-dataset/dataset5/
cp ../object_track/namak/dataset5/* test-dataset/dataset5/
cp ../object_track/turner/dataset5/* test-dataset/dataset5/
cp ../object_track/some_objects/dataset5/* test-dataset/dataset5/
cp ../object_track/breaking/dataset5/* test-dataset/dataset5/
cp ../hand_track/newFeatures/dataset5/* test-dataset/dataset5/
cp ../hand_track/distance\&angle/train-dataset/dataset5/* test-dataset/dataset5/
echo "updated\n"

echo "dataset-6..."
cp ../object_track/chopstick/dataset6/* test-dataset/dataset6/
cp ../object_track/katori/dataset6/* test-dataset/dataset6/
cp ../object_track/knife/dataset6/* test-dataset/dataset6/
cp ../object_track/namak/dataset6/* test-dataset/dataset6/
cp ../object_track/turner/dataset6/* test-dataset/dataset6/
cp ../object_track/some_objects/dataset6/* test-dataset/dataset6/
cp ../object_track/breaking/dataset6/* test-dataset/dataset6
cp ../hand_track/newFeatures/dataset6/* test-dataset/dataset6/
cp ../hand_track/distance\&angle/test-dataset/video1/* test-dataset/dataset6/
echo "updated\n"

echo "dataset-7..."
cp ../object_track/chopstick/dataset7/* test-dataset/dataset7/
cp ../object_track/katori/dataset7/* test-dataset/dataset7/
cp ../object_track/knife/dataset7/* test-dataset/dataset7/
cp ../object_track/namak/dataset7/* test-dataset/dataset7/
cp ../object_track/turner/dataset7/* test-dataset/dataset7/
cp ../object_track/some_objects/dataset7/* test-dataset/dataset7/
cp ../object_track/breaking/dataset7/* test-dataset/dataset7/
cp ../hand_track/newFeatures/dataset7/* test-dataset/dataset7/
cp ../hand_track/distance\&angle/test-dataset/video2/* test-dataset/dataset7/
echo "updated\n"



