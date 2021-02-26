#ifndef skindetection_header_included
#define skindetection_header_included

#include <cv.h>
#include <highgui.h>




int skin_det(IplImage *, IplImage *, double , double , double , double );
void superFastBlur(unsigned char *, int , int , int );
void OpenCVsuperFastBlur(IplImage* , IplImage* , int );









#endif
