#include "stdafx.h"
#include<opencv/cv.h>
#include<opencv/highgui.h>
#include "hand.h"

int ycbcr_segment(IplImage *img, IplImage *thresh,int lz,int hz,int ly,int hy)
{
	IplImage *ycbcr = cvCreateImage(cvGetSize(img),8,3);
	int s   = img->widthStep;
	int ch = img->nChannels;

	cvCvtColor(img,ycbcr,CV_BGR2YCrCb);

     int x,y,z,j,k;
		
     for(j=0;j<img->height;j++)
     {
	for(k=0;k<img->width;k++)
	{
	  x = (uchar)ycbcr->imageData[j*s+k*ch+0];  	
          y = (uchar)ycbcr->imageData[j*s+k*ch+1]; 	
	  z= (uchar)ycbcr->imageData[j*s+k*ch+2];  	
	  //printf("\n %d %d %d",x,y,z);	
	  if(z>lz && z < hz && y > ly && y < hy)								//if(z>77 && z < 127 && y > 133 && y < 173)
		thresh->imageData[j*s/3+k] = 0;
	  else	
		thresh->imageData[j*s/3+k] = 255;	
	} 
     }   
	
	cvReleaseImage(&ycbcr);
	return 0;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
