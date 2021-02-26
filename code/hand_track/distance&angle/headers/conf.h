#ifndef conf_header_included
#define conf_header_included

#include <cv.h>
#include <highgui.h>

void ConfigurePath(char *, char *,char *,char *,char *);
void labelling_vid(char *,IplImage *,CvFont,int);
void hough(IplImage *);
void algo1(IplImage *);

#endif
