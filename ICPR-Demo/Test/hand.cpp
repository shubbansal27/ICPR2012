#include "stdafx.h"
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
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
	return 0;
}

//--------------------------------------------------------------------------------------------------------------------

int ycbcr_segment3(IplImage *img, IplImage *thresh,int lx,int hx,int lz,int hz,int ly,int hy)
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
	 
	  if(z>lz && z < hz && y > ly && y < hy && x > lx && x < hx )
		thresh->imageData[j*s/3+k] = 0;
	  else	
		thresh->imageData[j*s/3+k] = 255;	
	} 
     }   
	
	cvReleaseImage(&ycbcr);
	return 1;
}

//--------------------------------------------------------------------------------------------------------------------

int ycbcr_segment2(IplImage *img, IplImage *thresh,int lx,int hx,int lz,int hz,int ly,int hy)
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
	 
	  if(z>lz && z < hz && y > ly && y < hy && x > lx && x < hx )
		thresh->imageData[j*s/3+k] = 255;
	  else	
		thresh->imageData[j*s/3+k] = 0;	
	} 
     }   
	
	cvReleaseImage(&ycbcr);
	return 1;
}

//-----------------------------------------------------------------------------------------------------------------------------------------


int ycbcr_segmentAll(IplImage *img, IplImage *thresh,CvPoint *katori,CvPoint *turner,CvPoint *chopstick,CvPoint *namak)
{
	IplImage *ycbcr = cvCreateImage(cvGetSize(img),8,3);
	int s   = img->widthStep;
	int ch = img->nChannels;

	cvCvtColor(img,ycbcr,CV_BGR2YCrCb);
	
     int x,y,z,j,k;
	 int Xkatori=0,Xturner=0,Xchopstick=0,Xnamak=0,Ykatori=0,Yturner=0,Ychopstick=0,Ynamak=0,cntkatori=0,cntturner=0,cntchopstick=0,cntnamak=0;
		
     for(j=0;j<img->height;j++)
     {
	for(k=0;k<img->width;k++)
	{
	  x = (uchar)ycbcr->imageData[j*s+k*ch+0];
      y = (uchar)ycbcr->imageData[j*s+k*ch+1];
	  z= (uchar)ycbcr->imageData[j*s+k*ch+2];
	 
	  if(z>89 && z < 113 && y > 176 && y < 242 && x > 0 && x < 105)
	  {
		  thresh->imageData[j*s/3+k] = 255;
		  Xkatori+=j;
		  Ykatori+=k;
		  cntkatori++;
	  }
	  else if(z>51 && z < 111 && y > 118 && y < 168 && x > 159 && x < 231)
	  {
		  thresh->imageData[j*s/3+k] = 255;
		  Xturner+=j;
		  Yturner+=k;
		  cntturner++;
	  }
	  else if(z>39 && z < 115 && y > 62 && y < 131 && x > 46 && x < 219) 
	  {
		  thresh->imageData[j*s/3+k] = 255;
		  Xchopstick+=j;
		  Ychopstick+=k;
		  cntchopstick++;
	  }
	  else if(z>131 && z < 182 && y > 140 && y < 159 && x > 124 && x < 152)
	  {
		thresh->imageData[j*s/3+k] = 255;
		Xnamak+=j;
		Ynamak+=k;
		cntnamak++;
	  }
	  else	
		thresh->imageData[j*s/3+k] = 0;	
	} 
     }   
	
	katori->x = Xkatori/cntkatori;
	katori->y = Ykatori/cntkatori;

	turner->x = Xturner/cntturner;
	turner->y = Yturner/cntturner;

	chopstick->x = Xchopstick/cntchopstick;
	chopstick->y = Ychopstick/cntchopstick;

	namak->x = Xnamak/cntnamak;
	namak->y = Ynamak/cntnamak;

	cvReleaseImage(&ycbcr);
	return 1;
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------

void contourTrack_katori(IplImage *thresh,IplImage *dst)
{       
		CvPoint pt;
		pt.x = 0;
		pt.y = 0;
		int i , x,y;        
		CvMemStorage* storage = cvCreateMemStorage(0);
        CvSeq* contour = 0;
        CvSeq* contour1 = 0;
		cvThreshold( thresh, thresh, 170, 255, CV_THRESH_BINARY_INV );
   		int N    =    cvFindContours( thresh, storage, &contour, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );
       
        contour1  =  contour;
        CvRect bndRect ;
        
		int *width = (int*)malloc(sizeof(int)*N);
		int *height = (int*)malloc(sizeof(int)*N);
        float *ratio = (float*)malloc(sizeof(float)*N);
		float *ratio1 = (float*)malloc(sizeof(float)*N);
		float *area = (float*)malloc(sizeof(float)*N);
		double *area_blob = (double*)malloc(sizeof(double)*N);

		i = 0;
        for( ; contour != 0; contour = contour->h_next )
        {
         	bndRect = cvBoundingRect(contour, 0);
	 		*(width+i) = bndRect.width;
			*(height+i) = bndRect.height;
            *(ratio+i) = ((*(height+i)) / (*(width+i)));
            *(ratio1+i) = *(ratio+i);
			*(area+i)= ( (*(height+i)) * (*(width+i)) ) ;
			*(area_blob+i) = cvContourArea(contour,CV_WHOLE_SEQ);
			i++;
        }
        
		cvZero( dst );
        i = 0;int k = 0;
        for( ; contour1 != 0; contour1 = contour1->h_next )
        {    
           
	    if(abs((*(area_blob+i))) > 90) 
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

//----------------------------------------------------------------------------------------------


//template matching
void template_matching(IplImage *img,IplImage *tpl, CvPoint *pt)
{
  
  IplImage  *res;
  CvPoint   minloc, maxloc;
  double    minval, maxval;
  CvSize size = cvSize(
    img->width  - tpl->width  + 1,
    img->height - tpl->height + 1
  );

  /* create new image for template matching computation */
  res = cvCreateImage(size, IPL_DEPTH_32F, 1);

  /* choose template matching method to be used */
  cvMatchTemplate(img, tpl, res, CV_TM_SQDIFF);
  /*cvMatchTemplate(img, tpl, res, CV_TM_SQDIFF_NORMED);
  cvMatchTemplate(img, tpl, res, CV_TM_CCORR);
  cvMatchTemplate(img, tpl, res, CV_TM_CCORR_NORMED);
  cvMatchTemplate(img, tpl, res, CV_TM_CCOEFF);
  cvMatchTemplate(img, tpl, res, CV_TM_CCOEFF_NORMED);*/

  cvMinMaxLoc(res, &minval, &maxval, &minloc, &maxloc, 0);

  /* Draw White Rectangle*/
  cvRectangle(img,
         cvPoint(minloc.x, minloc.y),
         cvPoint(minloc.x + tpl->width, minloc.y + tpl->height),
         CV_RGB(rand()&255, rand()&255, rand()&255), 3, 0, 0);  
  pt->x = minloc.x + (tpl->width)/2;
  pt->y = minloc.y + (tpl->height)/2;
  
}

//-----------------------------------------------------------------------------------------------------------

void template_matchingAll(IplImage *img,CvPoint *pan,CvPoint *fp,CvPoint *bowl,CvPoint *chopstick,CvPoint *bottle,CvPoint *knife,CvPoint *turner,CvPoint *saltbox,CvPoint *potato,CvPoint *peeler)
{
	IplImage *Pan = cvLoadImage("templates/Pan.jpg");
	IplImage *Fp = cvLoadImage("templates/FP.jpg");
	IplImage *Bottle = cvLoadImage("templates/Bottle.jpg");
	//IplImage *Bowl = cvLoadImage("templates/Bowl.jpg");
	//IplImage *Chopstick = cvLoadImage("templates/Chopstick.jpg");
	//IplImage *Knife = cvLoadImage("templates/Knife.jpg");
	//IplImage *Potato = cvLoadImage("templates/Potato.jpg");
	//IplImage *Saltbox = cvLoadImage("templates/Saltbox.jpg");
	//IplImage *Turner = cvLoadImage("templates/Turner.jpg");
	//IplImage *Peeler = cvLoadImage("templates/Peeler.jpg");

	/*cvShowImage("Pan",Pan);
	cvShowImage("Fp",Fp);
	cvShowImage("Bowl",Bowl);
	cvShowImage("Chopstick",Chopstick);
	cvShowImage("Knife",Knife);
	cvShowImage("Potato",Potato);
	cvShowImage("Saltbox",Saltbox);
	cvShowImage("Turner",Turner);*/
	
	template_matching(img,Pan,pan);
	template_matching(img,Fp,fp);
	template_matching(img,Bottle,bottle);
	//template_matching(img,Bowl,bowl);
	//template_matching(img,Chopstick,chopstick);
	//template_matching(img,Knife,knife);
	//template_matching(img,Potato,potato);
	//template_matching(img,Saltbox,saltbox);
	//template_matching(img,Turner,turner);
	//template_matching(img,Peeler,peeler);

	cvReleaseImage(&Pan);
	//cvReleaseImage(&Bowl);
	cvReleaseImage(&Fp);
	cvReleaseImage(&Bottle);
	
	
}

//------------------------------------------------------------------------------------------------------------------


/*
int bwareaopen(IplImage *image, int size1)
{
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
*/

//---------------------------------------------------------------------------------------------------------------------------------------------------

void contourTrackBothHand(IplImage *thresh,IplImage *dst,CvRect *rect1,CvRect *rect2,float *lAngle, float *rAngle)
{
	int i,x,y;
	*lAngle = 0;
	*rAngle = 0;
	static CvPoint staticpt1,staticpt2;        
	CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* contour = 0;
    CvSeq* contour1 = 0;
	cvThreshold( thresh, thresh, 170, 255, CV_THRESH_BINARY);
   	int N  = cvFindContours( thresh, storage, &contour, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );
    
	contour1  =  contour;
    CvRect bndRect ;
           
	int *width = (int*)malloc(sizeof(int) * N); 
	int *height = (int*)malloc(sizeof(int) * N); 
	float *ratio = (float*)malloc(sizeof(float) * N); 
	float *ratio1 = (float*)malloc(sizeof(float) * N); 
	float *area = (float*)malloc(sizeof(float) * N); 
	double *area_blob = (double*)malloc(sizeof(double) * N); 
	CvPoint *spt = (CvPoint*)malloc(sizeof(CvPoint) * N); 
	
		for(int n=0;n < N ; n++)
        {
           (*(spt + n)).x = 0;
           (*(spt + n)).y = 0;
        }
        i = 0;
        for( ; contour != 0; contour = contour->h_next )
        {
         	    

                bndRect = cvBoundingRect(contour, 0);
	 	*(width + i) = bndRect.width;
                *(height + i) = bndRect.height;
                *(ratio + i) = ((*(ratio + i)) / (*(width + i)));
                *(ratio1 + i) = *(ratio + i);
		*(area + i)= ( *(height + i) * (*(width + i) )) ;
		*(area_blob + i) = cvContourArea(contour,CV_WHOLE_SEQ);
		i++;
        }
        
	cvZero( dst );
        i = 0;int k = 0;
		CvPoint *pt1 = (CvPoint*)malloc(sizeof(CvPoint) * N);
		CvPoint *pt2 = (CvPoint*)malloc(sizeof(CvPoint) * N);
		float *An = (float*)malloc(sizeof(float) * N);

        for( ; contour1 != 0; contour1 = contour1->h_next )
        {    

	    if( *(height + i) > 30 && abs(*(area_blob + i)) > 4000) 
	    //if( height[i] > 30 && area[i] > 2000) 
            {
              CvScalar color = CV_RGB( 255, 255, 255 );
              cvDrawContours( dst, contour1, color, color, -1, CV_FILLED, 8 );
 //	      CvPoint pt1,pt2;
              bndRect = cvBoundingRect(contour1,0);
              (*(pt1 + k)).x = bndRect.x;
              (*(pt1 + k)).y = bndRect.y;
              (*(pt2 + k)).x = bndRect.x + bndRect.width;
              (*(pt2 + k)).y = bndRect.y + bndRect.height;
              cvRectangle(dst,(*(pt1 + k)),(*(pt2 + k)),CV_RGB(rand()&255,rand()&255,rand()&255),1,8,0);
	      x = ((*(pt1 + k)).x + (*(pt2 + k)).x)/2;
              y = ((*(pt1 + k)).y + (*(pt2 + k)).y)/2;
             *(An + k) = (float)(bndRect.height)/(bndRect.width) ;
             (*(spt + k)).x = x;
             (*(spt + k)).y = y;
              k = k + 1;	
            }
            else
            { 
               
                        CvScalar color = CV_RGB(0,0,0);
                	cvDrawContours( dst, contour1, color, color, -1, CV_FILLED, 8 );
            }
            i++;
		}

		if(k ==0)
		{
			(*rect1) = cvRect(0,0,0,0);
			(*rect2) = cvRect(0,0,0,0);
		}
		else if(k == 1)
		{
			(*rect1) = cvRect((*(pt1)).x ,(*(pt1)).y ,(*(pt2)).x - (*(pt1)).x , (*(pt2)).y - (*(pt1)).y);
		//	(*rect2) = cvRect((*(pt1 + 1)).x ,(*(pt1 + 1)).y ,(*(pt2 + 1)).x - (*(pt1+ 1)).x , (*(pt2 + 1)).y - (*(pt1+ 1)).y);
			(*rect2) = cvRect(0,0,0,0);
		}
		else if(k == 2)
		{
			(*rect1) = cvRect((*(pt1)).x ,(*(pt1)).y ,(*(pt2)).x - (*(pt1)).x , (*(pt2)).y - (*(pt1)).y);
			(*rect2) = cvRect((*(pt1 + 1)).x ,(*(pt1 + 1)).y ,(*(pt2 + 1)).x - (*(pt1+ 1)).x , (*(pt2 + 1)).y - (*(pt1+ 1)).y);
		}
		else
		{
			(*rect1) = cvRect((*(pt1)).x ,(*(pt1)).y ,(*(pt2)).x - (*(pt1)).x , (*(pt2)).y - (*(pt1)).y);
			(*rect2) = cvRect((*(pt1 + 1)).x ,(*(pt1 + 1)).y ,(*(pt2 + 1)).x - (*(pt1+ 1)).x , (*(pt2 + 1)).y - (*(pt1+ 1)).y);
		}
 //////////////////Now considering cases of when k =  1, k =2 , k =3//////////////////////////////////////////////////////////////////////////////

 /* int threshheight,threshwidth,threshstep,threshchannels;
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
   */
       
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

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

/*float calcAngleFromCentroid(CvPoint pt1,CvPoint pt,CvPoint centr)
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
	
}*/

//-----------------------------------------------------------------------------------------------------------------

void OverlayImage(IplImage* src, IplImage* overlay, CvPoint location, CvScalar S, CvScalar D)
    {
     int x,y,i;
 
      for(x=0;x < overlay->width -10;x++)
        {
            if(x+location.x>=src->width) continue;
            for(y=0;y < overlay->height -10;y++)
            {
                if(y+location.y>=src->height) continue;
                CvScalar source = cvGet2D(src, y+location.y, x+location.x);
                CvScalar over = cvGet2D(overlay, y, x);
                CvScalar merged;
                for(i=0;i<4;i++)
                merged.val[i] = (S.val[i]*source.val[i]+D.val[i]*over.val[i]);
                cvSet2D(src, y+location.y, x+location.x, merged);
            }
        }
    }