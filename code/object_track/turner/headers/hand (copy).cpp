#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include "hand.h"

IplImage *hand_segment(IplImage *img)
{
    IplImage *hsv, *hue, *sat, *val ,*imgg , *merge ,*thresh ,*erode_thresh,*dilate_thresh ,*bwAreaOpenBW;
    int key = 0, depth;
    CvSize size;
    uchar *data;
    int i,j,k;
    float m ;

 
   
 
        /* Create a hsv image with 3 channels and hue, sat e val with 1 channel.
           All with the same size */
     
       size = cvGetSize(img);
       depth = img->depth;
       hue = cvCreateImage(size, IPL_DEPTH_8U, 1);
       sat = cvCreateImage(size, IPL_DEPTH_8U, 1);
       val = cvCreateImage(size, IPL_DEPTH_8U, 1);
       hsv = cvCreateImage(size, IPL_DEPTH_8U, 3);
       imgg = cvCreateImage(size, IPL_DEPTH_8U, 3);
       merge = cvCreateImage(size, IPL_DEPTH_8U, 3);
       thresh = cvCreateImage(size, IPL_DEPTH_8U, 1);    
       erode_thresh =cvCreateImage(size, IPL_DEPTH_8U, 1);         
       dilate_thresh =cvCreateImage(size, IPL_DEPTH_8U, 1);    
       bwAreaOpenBW = cvCreateImage(size, IPL_DEPTH_8U, 3);    
	
      /* cvNamedWindow("original", CV_WINDOW_AUTOSIZE);
       cvNamedWindow("hsv", CV_WINDOW_AUTOSIZE);
       cvNamedWindow("hue", CV_WINDOW_AUTOSIZE);
       cvNamedWindow("merge", CV_WINDOW_AUTOSIZE);
       cvNamedWindow("saturation", CV_WINDOW_AUTOSIZE);
       cvNamedWindow("value", CV_WINDOW_AUTOSIZE);
       cvNamedWindow("Duplicate", CV_WINDOW_AUTOSIZE);

 	//cvErode(img,erode_thresh,NULL,1);
//	cvErode(erode_thresh,erode_thresh,NULL,1);
//	cvErode(erode_thresh,erode_thresh,NULL,1);
//	cvShowImage("ErodeThresh", erode_thresh ); 
 //      Convert from Red-Green-Blue to Hue-Saturation-Value */
        cvCvtColor( img, hsv, CV_RGB2HSV );
 
    /* Split hue, saturation and value of hsv on them */
        cvSplit(hsv, hue, sat, val, 0);
        
 //       cvShowImage("original", img);
 //       cvShowImage("hsv",hsv);
 //       cvShowImage("hue",hue);
        
	int height1 = hsv->height;
	int width1  = hsv->width;
	int step1   = hsv->widthStep;///sizeof(uchar);
	int channels1 = hsv->nChannels;
	uchar* data1 ;
	data1 = (uchar *)hsv->imageData;

//	for(i=0;i<height1;i++)  for(j=0;j<width1;j++)  for(k=0;k<channels1;k++)
  //  	data1[i*step1+j*channels1+k]=0;

	int height2 = hue->height;
	int width2  = hue->width;
	int step2   = hue->widthStep;
	int channels2 = hue->nChannels;
	uchar* data2 = (uchar *)hue->imageData;
        
//	for(i=0;i<height2;i++) for(j=0;j<width2;j++) for(k=0;k<channels2;k++)
  //  	data1[i*step1+j*channels1+k]=data2[i*step2+j*channels2+k] ;

	
        /* Create windows, display them, wait for a key */
//	cvShowImage("saturation", sat);
 //       cvShowImage("value", val);   

	int height3 = sat->height;
	int width3  = sat->width;
	int step3   = sat->widthStep;///sizeof(uchar);
	int channels3 = sat->nChannels;
	uchar* data3 ;
	data3 = (uchar *)sat->imageData;
	//cvSetZero(sat);
	for(i=0;i<height3;i++)  for(j=0;j<width3;j++)  for(k=0;k<channels3;k++)
  	data3[i*step3+j*channels3+k]=255 ;

	int height4 = val->height;
	int width4  = val->width;
	int step4   = val->widthStep;
	int channels4 = val->nChannels;
	uchar* data4 = (uchar *)val->imageData;
//	cvSetZero(val);
	for(i=0;i<height4;i++) for(j=0;j<width4;j++) for(k=0;k<channels4;k++)
    	data4[i*step4+j*channels4+k]=255 ;
        
    	cvMerge(hue, sat, val , NULL ,  merge);
//	cvShowImage("merge",merge);
        
	cvCvtColor( merge, imgg , CV_HSV2RGB);

        int height5 = imgg->height;
	int width5  = imgg->width;
	int step5   = imgg->widthStep;
	int channels5 = imgg->nChannels;
	uchar* data5 = (uchar *)imgg->imageData;
	
//	for(i=0;i<height5;i++){ for(j=0;j<width5;j++) {for(k=0;k<channels5;k++)
  //         m =	(float)data5[i*step5+j*channels5+k] ;
    //       printf("%f\n",m); 
      //     }}
        
  //      cvShowImage("Duplicate", imgg ); 

	int height_imgg = imgg->height;
	int width_imgg  = imgg->width;
	int step_imgg   = imgg->widthStep;///sizeof(uchar);
	int channels_imgg = imgg->nChannels;
	uchar* data_imgg ;
	data_imgg = (uchar *)imgg->imageData;
	
      //  for(i=0;i<height_imgg;i++){ for(j=0;j<width_imgg;j++) {for(k=0;k<channels_imgg;k++)
       //    m =	(float)data_imgg[i*step_imgg+j*channels_imgg+k] ;
        //   printf("%f\n",m); 
         //  }}

	int height_thresh = thresh->height;
        int width_thresh  = thresh->width;
        int step_thresh   = thresh->widthStep;///sizeof(uchar);
	int channels_thresh = thresh->nChannels;
	uchar* data_thresh ;
	data_thresh = (uchar *)thresh->imageData;

      //printf("Hi1\n");
	for(i=1;i<=height_imgg;i++)
           { 
            for(j=1;j<=width_imgg;j++) 
            { 
             for(k=1;k<=channels_imgg;k++)
             {
              if ( k == 1 )
          	{ 
                 if ((imgg->imageData + imgg->widthStep*i)[j] < 255)
                 {
                  (thresh->imageData + thresh->widthStep*i)[j]=255 ;
        	 }
                 else
                  (thresh->imageData + thresh->widthStep*i)[j]=0 ;
	 	}
 //     printf("Hi2\n");
	      if ( k == 2 )
	       {
		if ((imgg->imageData + imgg->widthStep*i)[j] > 9.7665  && (imgg->imageData + imgg->widthStep*i)[j] < 140.25 )
		{
         	 (thresh->imageData + thresh->widthStep*i)[j]=255 ;
  		}
		else
         	 (thresh->imageData + thresh->widthStep*i)[j]=0 ;
	       }
   ///   printf("Hi3\n");
 	      if ( k == 3 )
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
     

	//cvShowImage("Thresh", thresh ); 
	     
	cvErode(thresh,erode_thresh,NULL,1);
	cvErode(erode_thresh,erode_thresh,NULL,1);
	cvErode(erode_thresh,erode_thresh,NULL,1); 
//	cvErode(erode_thresh,erode_thresh,NULL,1); 
//	cvErode(erode_thresh,erode_thresh,NULL,1); 
//	cvShowImage("erosion2", erode_thresh );
     
	cvDilate(erode_thresh,dilate_thresh,NULL,1);
	cvDilate(dilate_thresh,dilate_thresh,NULL,1);
	cvDilate(dilate_thresh,dilate_thresh,NULL,1);
//	cvShowImage("Erosion+Dilation",dilate_thresh);
        cvNot(dilate_thresh,dilate_thresh);
	//cvShowImage("DT_original", dilate_thresh);
       
        int no_of_contours = bwareaopen(dilate_thresh, 2000) ;
//	cvShowImage("bwAreaOpenBW", dilate_thresh);
	//printf("\nNo. of contours remaining = %d\n", no_of_contours );
	//cvShowImage("T", thresh);
	//cvShowImage("DT", dilate_thresh);


//////////////////////////////////////////////////////////////////// Gupta Contours Start

	IplImage* dst = cvCreateImage( cvGetSize(dilate_thresh), 8, 3 );
        CvMemStorage* storage = cvCreateMemStorage(0);
        CvSeq* contour = 0;
        CvSeq* contour1 = 0;

        cvThreshold( dilate_thresh, dilate_thresh, 170, 255, CV_THRESH_BINARY_INV );
        //cvNamedWindow( "Source", 1 );
        //cvShowImage( "Source", dilate_thresh );
   
        int N    =    cvFindContours( dilate_thresh, storage, &contour, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );
        contour1  =  contour;
        CvRect bndRect ;
        //  CvPoint width;   
        int width[N] , height[N];
        float ratio[N] ,ratio1[N],area[N];
	double area_blob[N] ;
         i = 0;
        for( ; contour != 0; contour = contour->h_next )
        {
         //Ge t a bounding rectangle around the moving object.
		bndRect = cvBoundingRect(contour, 0);
	 	width[i] = bndRect.width;
                height[i] = bndRect.height;
                ratio[i] = (height[i] / width[i]);
                ratio1[i] = ratio[i];
		area[i]= ( height[i] * width[i] ) ;
		area_blob[i] = cvContourArea(contour,CV_WHOLE_SEQ);
		//printf("width=%d\n ", width[i]);
              	//printf("height=%d ; Ratio=%f ; Area= %f ; Area_Blob= %f \n",height[i],ratio[i],area[i],area_blob[i]);
                i++;
        }
        
/*        int elements = sizeof(ratio1) / sizeof(ratio1[0]); 
        std::sort(ratio1, ratio1 + elements);*/

        cvZero( dst );
        i = 0;
        for( ; contour1 != 0; contour1 = contour1->h_next )
        {    // if(ratio[i] >= ratio1[N-2] )
             if( ratio[i] >= 1 && width[i] < 120 ) 
            {
              CvScalar color = CV_RGB( 255, 255, 255 );
              // replace CV_FILLED with 1 to see the outlines //
              cvDrawContours( dst, contour1, color, color, -1, CV_FILLED, 8 );
 CvPoint pt1,pt2;
              bndRect = cvBoundingRect(contour1,0);
              pt1.x = bndRect.x;
              pt1.y = bndRect.y;
              pt2.x = bndRect.x + bndRect.width;
              pt2.y = bndRect.y + bndRect.height;
              cvRectangle(dst,pt1,pt2,CV_RGB(rand()&255,rand()&255,rand()&255),1,8,0);
		
            }
            else
            {
                            CvScalar color = CV_RGB(0,0,0);
                         // replace CV_FILLED with 1 to see the outlines //
                         cvDrawContours( dst, contour1, color, color, -1, CV_FILLED, 8 );

            }
            i++;
        } 

        //cvShowImage("destination",dst);
	//cvSaveImage("dushy.jpg",dst);
    
////////////////////////////////////////////////////////////////////// Gupta Contours End


        //cvWaitKey(0);
 
       /* Free memory and get out */
       /*cvDestroyWindow("original");
       cvDestroyWindow("hue");
       cvDestroyWindow("hsv");
       cvDestroyWindow("merge");
       cvDestroyWindow("saturation");
       cvDestroyWindow("value");
       cvDestroyWindow("Duplicate");
 	*/

       cvReleaseImage(&hsv);
       cvReleaseImage(&hue);
       cvReleaseImage(&sat);
       cvReleaseImage(&val);
       cvReleaseImage(&imgg);

	return dst;
	
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

