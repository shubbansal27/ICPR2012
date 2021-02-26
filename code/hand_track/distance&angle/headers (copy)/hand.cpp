#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
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
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------

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

	//cvNot(thresh,thresh);															//??????
//	cvShowImage("thresh",thresh);

       //cvNot(thresh,thresh);															//??????
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

//---------------------------------------------------------------------------------------------------------------------------------------------------

void contourTrackBothHand(IplImage *thresh,IplImage *dst,CvPoint *pt,CvPoint *pt_2,float *lAngle, float *rAngle)
{
	int i,x,y;
        static CvPoint staticpt1,staticpt2;        
	CvMemStorage* storage = cvCreateMemStorage(0);
        CvSeq* contour = 0;
        CvSeq* contour1 = 0;
	cvThreshold( thresh, thresh, 170, 255, CV_THRESH_BINARY_INV );
   	int N  = cvFindContours( thresh, storage, &contour, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );
    
	contour1  =  contour;
        CvRect bndRect ;
           
        int width[N] , height[N];
        float ratio[N] ,ratio1[N],area[N];
	double area_blob[N] ;
        CvPoint spt[N];

        for(int n=0;n < N ; n++)
        {
           spt[n].x = 0;
           spt[n].y = 0;
        }
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
        CvPoint pt1[N],pt2[N];    
	float An[N]; 
        for( ; contour1 != 0; contour1 = contour1->h_next )
        {    
           // if( ratio[i] >= 1 && width[i] < 120 ) 
	    //if( ratio[i] >=1  &&  height[i] > 50 && area_blob[i] > 3000) 
	    if( height[i] > 30 && abs(area_blob[i]) > 2000) 
	    //if( height[i] > 30 && area[i] > 2000) 
            {
              CvScalar color = CV_RGB( 255, 255, 255 );
              cvDrawContours( dst, contour1, color, color, -1, CV_FILLED, 8 );
 //	      CvPoint pt1,pt2;
              bndRect = cvBoundingRect(contour1,0);
              pt1[k].x = bndRect.x;
              pt1[k].y = bndRect.y;
              pt2[k].x = bndRect.x + bndRect.width;
              pt2[k].y = bndRect.y + bndRect.height;
              cvRectangle(dst,pt1[k],pt2[k],CV_RGB(rand()&255,rand()&255,rand()&255),1,8,0);
	      x = (pt1[k].x + pt2[k].x)/2;
              y = (pt1[k].y + pt2[k].y)/2;
             An[k] = (float)(bndRect.height)/(bndRect.width) ;
             spt[k].x = x;
             spt[k].y = y;
              k = k + 1;	
            }
            else
            { 
               
                        CvScalar color = CV_RGB(0,0,0);
                	cvDrawContours( dst, contour1, color, color, -1, CV_FILLED, 8 );
            }
            i++;
      }

 //////////////////Now considering cases of when k =  1, k =2 , k =3//////////////////////////////////////////////////////////////////////////////

  int threshheight,threshwidth,threshstep,threshchannels;
  //uchar *data; 
 


  threshheight    = thresh->height;
  threshwidth     = thresh->width;
  //threshstep      = thresh->widthStep;
  //threshchannels  = thresh->nChannels;
  //data      = (uchar *)thresh->imageData;   
     // printf("k=%d\n",k);
    if(k ==2)
    {
	if(spt[0].x < spt[1].x)
	{        
		pt->x = spt[0].x;
        	pt->y = spt[0].y;
		*lAngle = An[0];
      		pt_2->x = spt[1].x;
        	pt_2->y = spt[1].y;
		*rAngle = An[1];
	}
	else if(spt[1].x < spt[0].x)
	{        
		pt->x = spt[1].x;
        	pt->y = spt[1].y;
		*lAngle = An[1];
      		pt_2->x = spt[0].x;
        	pt_2->y = spt[0].y;
		*rAngle = An[0];
	}
	
    }
     
   else if(k ==1)
   {
	if(pt2[0].x - pt1[0].x > 120)
        {
                 pt->x   =    ( ((pt1[0].x + pt2[0].x)/2) + pt1[0].x) / 2;
                 pt_2->x =    ((pt1[0].x + pt2[0].x)/2 + pt2[0].x)/2;
                  
                 pt->y = ((pt1[0].y + pt2[0].y))/2;
		 pt_2->x =  pt->y;
		 *lAngle = An[0];
		 *rAngle = An[0];	
        }
       
        else
        { 
          
            if( (pt1[0].x + pt2[0].x/2)  <= threshwidth/2)
            {
               pt->x = pt1[0].x + pt2[0].x/2;
               pt->y = pt1[0].y + pt2[0].y/2;
	       *lAngle = An[0];	
               
               pt_2->x = 0; 
               pt_2->y = 0;
	       *rAngle = 0;	
            }
           else
           {
                            pt_2->x = pt1[0].x + pt2[0].x/2;
                            pt_2->y = pt1[0].y + pt2[0].y/2;
			    *rAngle = An[0];	
                            pt->x = 0;
                            pt->y = 0;
			    *lAngle = 0;	
           }
        }
 
   }    
  
   else
   {
       pt->x = 0;
       pt->y = 0;
       *lAngle = 0;	
       pt_2->x = 0;
       pt_2->y = 0 ;
       *rAngle = 0;		
   }
       
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------



int skin_det(IplImage *src, IplImage *dst, double hue_lo, double hue_hi, double sat_lo, double sat_hi) {

  long i, j, Value, R, G, B, tmp;

  double Saturation, Hue, Cr, Cg, Cb; 

  uchar *ps = (uchar*)src->imageData; // use pointers to access frame data

  uchar *pd = (uchar*)dst->imageData;

  for ( i = 0 ; i < src->height ; i++ ) {

  for ( j = 0 ; j < src->width ; j++ ) {

      R = ps[i*src->widthStep + j*src->nChannels + 2]; // R

      G = ps[i*src->widthStep + j*src->nChannels + 1]; // G

      B = ps[i*src->widthStep + j*src->nChannels    ]; // B

      // find maximum VALUE

           if ( (R >= G) && (R >= B)) Value = R;

      else if ( (G >= R) && (G >= B)) Value = G;

      else if ( (B >= G) && (B >= R)) Value = B;

           if ( (R <= G) && (R <= B)) tmp = R;

      else if ( (G <= R) && (G <= B)) tmp = G;

      else if ( (B <= G) && (B <= R)) tmp = B;

 // find minimum SATURATION 

      if (Value == 0) Saturation = 0;

      else            Saturation = ((double)Value - (double)tmp)/(double)Value;

      // SATURATION

      if (Saturation == 0) {

Hue = -1;

 }

      else {

        Cr = ((double)Value-(double)R)/((double)Value-(double)tmp);

        Cg = ((double)Value-(double)G)/((double)Value-(double)tmp);

        Cb = ((double)Value-(double)B)/((double)Value-(double)tmp);

        if (R == Value) Hue = Cb - Cg;

        if (G == Value) Hue = 2 + Cr - Cb;

        if (B == Value) Hue = 4 + Cg - Cr;

        Hue *= 60;

        if (Hue < 0) Hue +=360;

      }

      // DETECT SKIN COLOR 

      if ( (Hue >= hue_lo) && (Hue <= hue_hi) && (Saturation >= sat_lo) && (Saturation <= sat_hi) ) {

        pd[i*src->widthStep + j*src->nChannels + 2] = (uchar)R; // R

        pd[i*src->widthStep + j*src->nChannels + 1] = (uchar)G; // G

        pd[i*src->widthStep + j*src->nChannels    ] = (uchar)B; // B

      }

      else { // force pixel to white if non-skin tone

        pd[i*src->widthStep + j*src->nChannels + 2] = 0xFF; // R

        pd[i*src->widthStep + j*src->nChannels + 1] = 0xFF; // G

        pd[i*src->widthStep + j*src->nChannels    ] = 0xFF; // B

      }

    }

  }

  return 0;

} 


//-------------------------------------------------------------------------------------------------------------------------------------------------------------


void superFastBlur(unsigned char *pix, int w, int h, int radius) {

   if (radius<1) return;

   int wm=w-1;

   int hm=h-1;

   int wh=w*h;

   int div=radius+radius+1;

   unsigned char *r=new unsigned char[wh];

   unsigned char *g=new unsigned char[wh];

   unsigned char *b=new unsigned char[wh];

   int rsum,gsum,bsum,x,y,i,p,p1,p2,yp,yi,yw;

   int *vMIN = new int[MAX(w,h)];

   int *vMAX = new int[MAX(w,h)];

   unsigned char *dv=new unsigned char[256*div];

   for (i=0;i<256*div;i++) dv[i]=(i/div);

   yw=yi=0;

   for (y=0;y<h;y++){

      rsum=gsum=bsum=0;

      for(i=-radius;i<=radius;i++){

         p = (yi + MIN(wm, MAX(i,0))) * 3;

         rsum += pix[p];

         gsum += pix[p+1];

         bsum += pix[p+2];

      }

      for (x=0;x<w;x++){

         r[yi]=dv[rsum];

         g[yi]=dv[gsum];

         b[yi]=dv[bsum];

         if(y==0){

            vMIN[x]=MIN(x+radius+1,wm);

            vMAX[x]=MAX(x-radius,0);

         } 

         p1 = (yw+vMIN[x])*3;

         p2 = (yw+vMAX[x])*3;

         rsum += pix[p1]      - pix[p2];

         gsum += pix[p1+1]   - pix[p2+1];

         bsum += pix[p1+2]   - pix[p2+2];

         yi++;

      }

      yw+=w;

   }

   for (x=0;x<w;x++){

      rsum=gsum=bsum=0;

      yp=-radius*w;

      for(i=-radius;i<=radius;i++){

         yi=MAX(0,yp)+x;

         rsum+=r[yi];

         gsum+=g[yi];

         bsum+=b[yi];

         yp+=w;

      }

      yi=x;

      for (y=0;y<h;y++){

         pix[yi*3]      = dv[rsum];

         pix[yi*3 + 1]   = dv[gsum];

         pix[yi*3 + 2]   = dv[bsum];

         if(x==0){

            vMIN[y]=MIN(y+radius+1,hm)*w;

            vMAX[y]=MAX(y-radius,0)*w;

         } 

         p1=x+vMIN[y];

         p2=x+vMAX[y];

         rsum+=r[p1]-r[p2];

         gsum+=g[p1]-g[p2];

         bsum+=b[p1]-b[p2];

         yi+=w;

      }

   }

   delete r;

   delete g;

   delete b;

   delete vMIN;

   delete vMAX;

   delete dv;

} 

//------------------------------------------------------------------------------------------------------------------------------------------------------------

void OpenCVsuperFastBlur(IplImage* src, IplImage* dst, int radius) {

   int i;

   unsigned char *pix;  

   uchar *ps = (uchar*)src->imageData; // use pointers to access source data

   uchar *pd = (uchar*)dst->imageData; // use pointers to access destination data

   pix = new unsigned char[src->width*src->height*3]; // allocate memory for pixel array … 3 chars for each pixel!

   for (i=0; i<(src->height*src->width*3); i++) pix[i] = (unsigned char) ps[i];

   superFastBlur(pix, src->width, src->height, radius);

   for (i=0; i<(src->height*src->width*3); i++) pd[i] = (char) pix[i];

   delete pix;

} 

//-----------------------------------------------------------------------------------------------------------------------------------------------

/*float calcAngle(CvPoint pt1, CvPoint pt)    //from top-left(0,0)     pt- object     pt1- hand
{ 
  float angle,m1,m2,m;  
  if(pt1.y == 0 && pt1.x ==0)
	return 0;
  else
  {	
  	m1 = (float)(pt.y-0)/(pt.x-0); 
  	m2 = (float)(pt1.y-0)/(pt1.x-0); 
  	m =  fabs((m2-m1)/(1+m2*m1));	 
  	angle = (180*(atan2((float)m,(float)1)))/3.14159265; 
  	return angle;
  }	

}*/

//---------------------------------------------------------------------------------------------------------------------------------------------

float calcAngleFromCentroid(CvPoint pt1,CvPoint pt,CvPoint centr)
{ 
  float angle,m1,m2,m;
  if(pt.y == 0 && pt.x ==0) 								//hand
  {
	if(pt1.y == 0 && pt1.x ==0)
	return 0;
	
	m1 = pt1.y-centr.y;
        m2 = pt1.x-centr.x;
	m = fabs((float)m1/m2);
	angle = (180*(atan2((float)m,(float)1)))/3.14159265;
	if(m1 < 0 && m2 > 0)
		angle = 180 - angle;
	else if(m2 > 0 && m1 > 0)
		angle = 180 + angle;
	else if(m1 > 0 && m2 < 0)
		angle = 360 - angle;
	return angle;
  }		
  else											//object
  {	
	float angle1,angle2;
	if(pt1.y == 0 && pt1.x ==0)
	return 0;

  	angle1 = calcAngleFromCentroid(pt,cvPoint(0,0),centr);
	angle2 = calcAngleFromCentroid(pt1,cvPoint(0,0),centr);
  	angle = fabs(angle1-angle2);
	if(angle > 180) angle = 360 - angle ; 
  	return angle;
  }
	
}
