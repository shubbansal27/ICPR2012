#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include "hand.h"

int ycbcr_seg(IplImage *img, IplImage *thresh,int lz,int hz,int ly,int hy)
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
		thresh->imageData[j*s/3+k] = 255;
	  else	
		thresh->imageData[j*s/3+k] = 0;	
	} 
     }   
	
	cvReleaseImage(&ycbcr);
}

//----------------------------------------------------------------------------------------

int hand_segment(IplImage *img, IplImage *thresh)
{
    IplImage *hsv, *hue, *sat, *val ,*imgg , *merge;
    int key = 0;
    CvSize size;
    int i,j,k;
   

    size = cvGetSize(img);
    hue = cvCreateImage(size, IPL_DEPTH_8U, 1);
    sat = cvCreateImage(size, IPL_DEPTH_8U, 1);
    val = cvCreateImage(size, IPL_DEPTH_8U, 1);
    hsv = cvCreateImage(size, IPL_DEPTH_8U, 3);
    imgg = cvCreateImage(size, IPL_DEPTH_8U, 3);
    merge = cvCreateImage(size, IPL_DEPTH_8U, 3);
          
    cvCvtColor( img, hsv, CV_RGB2HSV );
    cvSplit(hsv, hue, sat, val, 0);
        
    for(i=0;i<sat->imageSize;i++)  sat->imageData[i] = 255;  
    for(i=0;i<val->imageSize;i++)  val->imageData[i] = 255;
        
    cvMerge(hue, sat, val , NULL ,  merge);
    cvCvtColor( merge, imgg , CV_HSV2RGB);

        int step_imgg   = imgg->widthStep;
	int channels_imgg = imgg->nChannels;
	uchar* data_imgg ;
	data_imgg = (uchar *)imgg->imageData;
	
      	for(i=0;i<imgg->height;i++)
        { 
        	for(j=0;j<imgg->width;j++) 
        	{ 
          		for(k=0;k<channels_imgg;k++)
           		{
              			if ( k == 0 )
          			{ 
                 			if (data_imgg[i*step_imgg+j*channels_imgg+k]  < 255)
                 			{
                  				(thresh->imageData + thresh->widthStep*i)[j]=255 ;
        	 			}
                 			else
                  				(thresh->imageData + thresh->widthStep*i)[j]=0 ;
	 	   		 }
 
	      			if ( k == 1 )
	       			{
					if (data_imgg[i*step_imgg+j*channels_imgg+k] > 9.7665  && data_imgg[i*step_imgg+j*channels_imgg+k] < 140.25 )
					{
         	 				(thresh->imageData + thresh->widthStep*i)[j]=255 ;
  					}
					else
         	 				(thresh->imageData + thresh->widthStep*i)[j]=0 ;
	       			}
   
 	      			if ( k == 2 )
               			{
                			if (data_imgg[i*step_imgg+j*channels_imgg+k] <=140.25 )
					{
         	 				(thresh->imageData + thresh->widthStep*i)[j]=255 ;
					}
					else
         	 				(thresh->imageData + thresh->widthStep*i)[j]=0 ;
	       			}
             		}
            	}
           }
	
//      cvDilate(thresh,thresh,NULL,1);
//	cvDilate(thresh,thresh,NULL,1);
//	cvDilate(thresh,thresh,NULL,1);


//      cvErode(thresh,thresh,NULL,1);
//      cvErode(thresh,thresh,NULL,1);
//      cvErode(thresh,thresh,NULL,1);


	
	cvNot(thresh,thresh);
//	cvShowImage("thresh",thresh);

       cvNot(thresh,thresh);
       cvReleaseImage(&hsv);  
       cvReleaseImage(&hue);
       cvReleaseImage(&sat);  
       cvReleaseImage(&val);
       cvReleaseImage(&imgg);
       cvReleaseImage(&merge);

       return 1;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------

void contourTrack(IplImage *thresh,IplImage *dst,CvPoint *pt)
{
	int i , x,y;        
	CvMemStorage* storage = cvCreateMemStorage(0);
        CvSeq* contour = 0;
        CvSeq* contour1 = 0;
	cvThreshold( thresh, thresh, 170, 255, CV_THRESH_BINARY_INV );
   	int N    =    cvFindContours( thresh, storage, &contour, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );
       
        contour1  =  contour;
        CvRect bndRect ;
           
        int width[N] , height[N];
        float ratio[N] ,ratio1[N],area[N];
	double area_blob[N] ;

        i = 0;
        for( ; contour != 0; contour = contour->h_next )
        {
         	bndRect = cvBoundingRect(contour, 0);
	 	width[i] = bndRect.width;
                height[i] = bndRect.height;
                ratio[i] = (height[i] / width[i]);
                ratio1[i] = ratio[i];
		area[i]= ( height[i] * width[i] ) ;
		area_blob[i] = cvContourArea(contour,CV_WHOLE_SEQ);
		i++;
        }
        
	cvZero( dst );
        i = 0;int k = 0;
        for( ; contour1 != 0; contour1 = contour1->h_next )
        {    
           // if( ratio[i] >= 1 && width[i] < 120 ) 
	    //if( ratio[i] >=1  &&  height[i] > 50 && area_blob[i] > 3000) 
	    if( height[i] > 30 && abs(area_blob[i]) > 2000) 
	    //if( height[i] > 30 && area[i] > 2000) 
            {
              CvScalar color = CV_RGB( 255, 255, 255 );
              cvDrawContours( dst, contour1, color, color, -1, CV_FILLED, 8 );
 	      CvPoint pt1,pt2;
              bndRect = cvBoundingRect(contour1,0);
              pt1.x = bndRect.x;
              pt1.y = bndRect.y;
              pt2.x = bndRect.x + bndRect.width;
              pt2.y = bndRect.y + bndRect.height;
              cvRectangle(dst,pt1,pt2,CV_RGB(rand()&255,rand()&255,rand()&255),1,8,0);
	      x = (pt1.x + pt2.x)/2;
              y = (pt1.y + pt2.y)/2;
              pt->x =  (k * pt->x + x)/(k+1);
              pt->y =  (k * pt->y + y)/(k+1);
              k = k + 1;	
            }
            else
            { 
               
                        CvScalar color = CV_RGB(0,0,0);
                	cvDrawContours( dst, contour1, color, color, -1, CV_FILLED, 8 );
            }
            i++;
        } 
       if(k==0)
       {
           pt->x = 0;
           pt->y = 0;
       }
}


//------------------------------------------------------------------------------------------------------------------------------------------------------------------

int bwareaopen(IplImage *image, int size1)
{
/* OpenCV equivalent of Matlab's bwareaopen.image must be 8 bits, 1 channel, black and white(objects) with values 0 and 255 respectively */
CvMemStorage *storage;
CvSeq *contour = NULL;
CvScalar white, black;
IplImage *input = NULL; // cvFindContours changes the input
double area;
int foundCountours = 0;

black = CV_RGB( 0, 0, 0 );
white = CV_RGB( 255, 255, 255 );

if(image == NULL || size1 == 0)
return(foundCountours);

input = cvCloneImage(image);

storage = cvCreateMemStorage(0); // pl.Ensure you will have enough room here.


cvFindContours(input, storage, &contour, sizeof (CvContour),
CV_RETR_LIST,
CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));

while(contour)
{
area = cvContourArea(contour, CV_WHOLE_SEQ );
if( -size1 <= area && area <= 0)
{ // removes white dots
cvDrawContours( image, contour, black, black, -1, CV_FILLED, 8 );
}
else
{
if( 0 < area && area <= size1) // fills in black holes
cvDrawContours( image, contour, white, white, -1, CV_FILLED, 8 );
}
contour = contour->h_next;
}

cvReleaseMemStorage( &storage ); // desallocate CvSeq as well.
cvReleaseImage(&input);

return(foundCountours);
}



void contourTrackBothHand(IplImage *thresh,IplImage *dst,CvPoint *pt,CvPoint *pt_2)
{
	int i , x,y;        
	CvMemStorage* storage = cvCreateMemStorage(0);
        CvSeq* contour = 0;
        CvSeq* contour1 = 0;
	cvThreshold( thresh, thresh, 170, 255, CV_THRESH_BINARY_INV );
   	int N    =    cvFindContours( thresh, storage, &contour, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );
       
        contour1  =  contour;
        CvRect bndRect ;
           
        int width[N] , height[N];
        float ratio[N] ,ratio1[N],area[N];
	double area_blob[N] ;

        i = 0;
        for( ; contour != 0; contour = contour->h_next )
        {
         	bndRect = cvBoundingRect(contour, 0);
	 	width[i] = bndRect.width;
                height[i] = bndRect.height;
                ratio[i] = (height[i] / width[i]);
                ratio1[i] = ratio[i];
		area[i]= ( height[i] * width[i] ) ;
		area_blob[i] = cvContourArea(contour,CV_WHOLE_SEQ);
		i++;
        }
        
	cvZero( dst );
        i = 0;int k = 0;
        for( ; contour1 != 0; contour1 = contour1->h_next )
        {    
           // if( ratio[i] >= 1 && width[i] < 120 ) 
	    //if( ratio[i] >=1  &&  height[i] > 50 && area_blob[i] > 3000) 
	    if( height[i] > 30 && abs(area_blob[i]) > 2000) 
	    //if( height[i] > 30 && area[i] > 2000) 
            {
              CvScalar color = CV_RGB( 255, 255, 255 );
              cvDrawContours( dst, contour1, color, color, -1, CV_FILLED, 8 );
 	      CvPoint pt1,pt2;
              bndRect = cvBoundingRect(contour1,0);
              pt1.x = bndRect.x;
              pt1.y = bndRect.y;
              pt2.x = bndRect.x + bndRect.width;
              pt2.y = bndRect.y + bndRect.height;
              cvRectangle(dst,pt1,pt2,CV_RGB(rand()&255,rand()&255,rand()&255),1,8,0);
	      x = (pt1.x + pt2.x)/2;
              y = (pt1.y + pt2.y)/2;
              pt->x =  (k * pt->x + x)/(k+1);
              pt->y =  (k * pt->y + y)/(k+1);
              k = k + 1;	
            }
            else
            { 
               
                        CvScalar color = CV_RGB(0,0,0);
                	cvDrawContours( dst, contour1, color, color, -1, CV_FILLED, 8 );
            }
            i++;
        } 
       if(k==0)
       {
           pt->x = 0;
           pt->y = 0;
       }
}

void contourTrack_katori(IplImage *thresh,IplImage *dst)
{       CvPoint pt;
	int i , x,y;        
	CvMemStorage* storage = cvCreateMemStorage(0);
        CvSeq* contour = 0;
        CvSeq* contour1 = 0;
	cvThreshold( thresh, thresh, 170, 255, CV_THRESH_BINARY_INV );
   	int N    =    cvFindContours( thresh, storage, &contour, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );
       
        contour1  =  contour;
        CvRect bndRect ;
           
        int width[N] , height[N];
        float ratio[N] ,ratio1[N],area[N];
	double area_blob[N] ;

        i = 0;
        for( ; contour != 0; contour = contour->h_next )
        {
         	bndRect = cvBoundingRect(contour, 0);
	 	width[i] = bndRect.width;
                height[i] = bndRect.height;
                ratio[i] = (height[i] / width[i]);
                ratio1[i] = ratio[i];
		area[i]= ( height[i] * width[i] ) ;
		area_blob[i] = cvContourArea(contour,CV_WHOLE_SEQ);
		i++;
        }
        
	cvZero( dst );
        i = 0;int k = 0;
        for( ; contour1 != 0; contour1 = contour1->h_next )
        {    
           
	    if(abs(area_blob[i]) > 90) 
	
            {
              CvScalar color = CV_RGB( 255, 255, 255 );
              cvDrawContours( dst, contour1, color, color, -1, CV_FILLED, 8 );
 	      CvPoint pt1,pt2;
              bndRect = cvBoundingRect(contour1,0);
              pt1.x = bndRect.x;
              pt1.y = bndRect.y;
              pt2.x = bndRect.x + bndRect.width;
              pt2.y = bndRect.y + bndRect.height;
              cvRectangle(dst,pt1,pt2,CV_RGB(rand()&255,rand()&255,rand()&255),1,8,0);
	      x = (pt1.x + pt2.x)/2;
              y = (pt1.y + pt2.y)/2;
              pt.x =  (k * pt.x + x)/(k+1);
              pt.y =  (k * pt.y + y)/(k+1);
              k = k + 1;	
            }
            else
            { 
               
                        CvScalar color = CV_RGB(0,0,0);
                	cvDrawContours( dst, contour1, color, color, -1, CV_FILLED, 8 );
            }
            i++;
        } 
       if(k==0)
       {
           pt.x = 0;
           pt.y = 0;
       }
}

