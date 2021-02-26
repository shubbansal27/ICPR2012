#ifndef hand_header_included
#define hand_header_included

#include<cv.h>
#include<highgui.h>

int ycbcr_seg(IplImage *, IplImage *,int,int,int,int);
int hand_segment(IplImage *, IplImage *);
int bwareaopen(IplImage *, int);
void contourTrack(IplImage *,IplImage * ,CvPoint *);
void contourTrackBothHand(IplImage *,IplImage *,CvPoint *,CvPoint *);
void contourTrack_katori(IplImage *,IplImage *);
#endif
