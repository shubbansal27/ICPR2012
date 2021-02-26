#ifndef algo_header_included
#define algo_header_included

#include <cv.h>
#include <highgui.h>

using namespace std;
using namespace cv;

void gist_vector(IplImage *,uchar *,int);

void RG_channel(IplImage *,IplImage *,uchar *,int *,int);

void BY_channel(IplImage *,IplImage *,uchar *,int *,int);

void I_channel(IplImage *,uchar *,int *,int);

IplImage *scalingImage(IplImage *,int);

void cs_diff_RGB(IplImage*,IplImage*,IplImage *,int,int);			

void cs_diff_I(IplImage*,IplImage*,int,int);				

void gabor_Result(IplImage *,uchar *,int *,int);

void avg_image(IplImage*,uchar *,int);

void gabor_filter(IplImage*,IplImage*,int);

PCA compressPCA(const Mat&,int,Mat&);

void buildTrainData(IplImage *, char *, char *);

void buildTestData(IplImage *, char *, char *);

#endif
