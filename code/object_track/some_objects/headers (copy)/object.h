#ifndef object_header_included
#define object_header_included 

int bhagunaTrack(CvPoint,int,int,IplImage *,IplImage *);
int kadaiTrack(CvPoint,int,int,IplImage *,IplImage *);
int bottleTrack(CvPoint,int,int,IplImage *,IplImage *);
int noneClassify(IplImage *,IplImage *);
int hamTrack(CvPoint,int,int,IplImage *,IplImage *);
int namakTrack(CvPoint,int,int,IplImage *,IplImage *,int *);
int eggTrack(CvPoint,int,int,IplImage *,IplImage *);
int katoriTrack(CvPoint *,int *,int *,IplImage *,IplImage *);

#endif

