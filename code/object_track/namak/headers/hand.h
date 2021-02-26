#ifndef hand_header_included
#define hand_header_included

#include<cv.h>
#include<highgui.h>

int ycbcr_segment(IplImage *, IplImage *,int,int,int,int);
int hand_segment(IplImage *, IplImage *);
int bwareaopen(IplImage *, int);
void contourTrack(IplImage *,IplImage * ,CvPoint *);
void contourTrackBothHand(IplImage *,IplImage *,CvPoint *,CvPoint *,float *,float*);
int skin_det(IplImage *, IplImage *, double , double , double , double );
void superFastBlur(unsigned char *, int , int , int );
void OpenCVsuperFastBlur(IplImage* , IplImage* , int );
float calcAngle(CvPoint, CvPoint);
float calcAngleFromCentroid(CvPoint,CvPoint,CvPoint);
int ycbcr_segm(IplImage *, IplImage *,int,int,int,int,int,int);
int ycbcr_segment2(IplImage *, IplImage *,int,int,int,int,int,int);
#endif
