#ifndef algos_header_included
#define algos_header_included

int bottle_depth(IplImage *,IplImage *,CvRect,int);
int Pan_template2(IplImage *,IplImage *,CvRect,int,int,int,int,int,int,int);
int FP_template2(IplImage *,IplImage *,CvRect,int,int,int,int,int,int,int);
int FP_template(IplImage *,IplImage *,CvRect);
int pan_template(IplImage *,IplImage *,CvRect,int);
int  chopstickYellow_ycbcr( IplImage*,int, int,int,int,int,int,int);
int grater_ycbcr( IplImage*,int, int,int,int,int,int,int);
int knife_ycbcr( IplImage*,int, int,int,int,int,int,int);
int saltbox_ycbcr( IplImage*,int, int,int,int,int,int,int);
int Peeler_ycbcr( IplImage*,int, int,int,int,int,int,int);
int Bowl_ycbcr( IplImage*,int,int,int,int,int,int,int, CvRect *,CvRect,int);
int Bowl_ycbcr2( IplImage *, int,int,int,int,int,int,int); 
void track_ycbcr( IplImage*,int,int,int,int,int,int,int, CvRect *,CvRect);

#endif