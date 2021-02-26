#ifndef hand_header_included
#define hand_header_included

int ycbcr_segment(IplImage *, IplImage *,int,int,int,int);
int ycbcr_segment2(IplImage *,IplImage *,int,int,int,int,int,int);
int ycbcr_segment3(IplImage*,IplImage*,int,int,int,int,int,int);
int ycbcr_segmentAll(IplImage *, IplImage *,CvPoint *,CvPoint *,CvPoint *,CvPoint *);
void contourTrack_katori(IplImage *,IplImage *);
void template_matching(IplImage *img,IplImage *tpl, CvPoint *pt);
void template_matchingAll(IplImage *img,CvPoint *,CvPoint*,CvPoint *,CvPoint *,CvPoint *,CvPoint*,CvPoint *,CvPoint*,CvPoint *,CvPoint *);

//int bwareaopen(IplImage *, int);
void contourTrackBothHand(IplImage *,IplImage *,CvRect *,CvRect *,float *, float *);
//float calcAngle(CvPoint, CvPoint);
//float calcAngleFromCentroid(CvPoint,CvPoint,CvPoint);
void OverlayImage(IplImage*, IplImage*, CvPoint, CvScalar, CvScalar);
#endif
