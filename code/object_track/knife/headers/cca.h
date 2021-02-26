#ifndef cca_header_included
#define cca_header_included 

int openCCA(IplImage *);
void doCCALabelling( IplImage * , int * , int *);
void recurseAndResolve( int , bool * , int,  int   );
void getFrequency( IplImage *, int *, int *, int);
void removeBlob(IplImage*,int,int*);
void overlay(IplImage*,IplImage*);
void template_matching(IplImage *,IplImage *,CvPoint *);
void segment(IplImage *,int);
void centroidOfImageObjects(IplImage *,CvPoint *,CvPoint *,CvPoint *,CvPoint *,CvPoint *,CvPoint *,CvPoint *,CvPoint *,CvPoint *,CvPoint *);
int distancePoints(CvPoint ,CvPoint );

#endif

