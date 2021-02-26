#include <stdlib.h>
#include <stdio.h>
#include <math.h>     
#include <cv.h>
#include <highgui.h>
#include "cca.h"

void getFrequency( IplImage *img, int * labelVec, int * freqCountAry, int connectedComponentNum )
{
	int widthStep = img->widthStep;
	int width = img->width;
	int height = img->height;

	int baseInd = -widthStep;
	
	for( int i=0 ; i<height ; i++ )
	{
		baseInd += widthStep;
		for( int j=0 ; j<width ; j++ )
		{
			freqCountAry[  labelVec[ baseInd + j ]  ]++;
		}
	}
}

void recurseAndResolve( int currI, bool * tmpLabelAry, int collisionVec[][500],  int size )
{
	tmpLabelAry[currI] = true;
	for( int i=1 ; i<size ; i++ )
	{
		if( collisionVec[currI][i] == 1 && currI < i)
		{
			tmpLabelAry[ i ] = true;
                        recurseAndResolve( i, tmpLabelAry, collisionVec, size );
		}
	}
	
}



//It is assumed, 
//	white (255) = back-ground
//	black (0)   = fore-gound
//	there is no color other than 0 and 255
//	labelVec is of name size as that of binImg->imageSize
void doCCALabelling( IplImage * binImg, int * labelVec, int * totalConnectedComponentNum )
{
   //Pass one...
   int width = binImg->width;
   int height = binImg->height;
   int widthStep = binImg->widthStep;

   //init labelVec to zero
   for( int i=0 ; i<binImg->imageSize ; i++ ) labelVec[i] = 0;

   int currentLabel=1;

   int baseInd = 0; //start frm row 1 nd not row 0
   for( int h=1 ; h<height-1 ; h++ )
   {
	baseInd += widthStep;
	for( int w=1 ; w<width-1 ; w++ )
	{
		if( binImg->imageData[ baseInd+w ] == (uchar)0 ) //we r interested only in fore-gound ie black
		{
 			//iterate in 3x3 window around this pixel, if any of the label is found, assign that label to (h,w),
			//else apply a new label
			int ibaseInd;
			ibaseInd = baseInd - widthStep - widthStep;
			bool flag=false;
			flag = false;
			for( int ih=-1 ; ih<=1 ; ih++ )
			{
				ibaseInd+= widthStep;
				for( int iw=-1 ; iw<=1 ; iw++ )
				{
					if( flag==false && labelVec[ ibaseInd + w + iw ]  != 0 ) //some label already exist in the neighboorhood,
					{
						labelVec[ baseInd + w ] = labelVec[ ibaseInd + w + iw ] ;
						flag=true;
					}
				}
			}
			if( flag == false )
			{
				labelVec[ baseInd + w ] = currentLabel;
				currentLabel++;
			}
		}
	}
   }
   //printf( "currentLabel : %d\n", currentLabel );
   //End of Pass-1

   
   //Start Pass-2
   int collisionVec[500][500]; 
   for( int il=0 ; il<currentLabel ; il++ ) for( int jl=0 ; jl<currentLabel ; jl++ )  collisionVec[il][jl]=0;
   //iterate thru the labelVec, and then thru a 3x3 window, if 2 different labels are found in that window, 
   //register that these two labels are of connected component in collisionVec[][].
   baseInd = 0; //start frm row 1 nd not row 0

   int thisPositionLabel;
   for( int h=1 ; h<height-1 ; h++ )
   {
	   baseInd += widthStep;
	   for( int w=1 ; w<width-1 ; w++ )
	   {
  		   thisPositionLabel = labelVec[ baseInd + w ];
		   int ibaseInd = baseInd - widthStep - widthStep;
		   for( int ih=-1 ; ih<=1 ; ih++ )
		   {
			   ibaseInd+= widthStep;
			   for( int iw=-1 ; iw<=1 ; iw++ )
			   {
				   if( labelVec[ ibaseInd + w + iw ]  != thisPositionLabel   &&   labelVec[ ibaseInd + w + iw ] != 0 ) //some label already exist in the neighboorhood,
					   {
						collisionVec[ thisPositionLabel ][  labelVec[ ibaseInd + w + iw ] ]  = 1;
						//collisionVec[  labelVec[ ibaseInd + w + iw ] ][ thisPositionLabel ]  = 1;
					   }
			   }
		   }
	   }

   }
   // End of Pass-2

	/////////////??PRINTING///////////////
	printf( "       " );
	for( int i=1 ; i<currentLabel ; i++ )  printf( "%2d ", i );
		printf( "\n" );
	for( int i=1 ; i<currentLabel ; i++ )
	{
		printf( "%2d  :: ", i );
		for( int j=1 ; j<currentLabel ; j++ )
		{
			printf( "%2d ", collisionVec[i][j] );
		}
		printf( "\n" );
	}
		printf( "\n" );
	///////////// END PRINTING///////////////
 	

   // Start of Pass-3
   int newCurrentLabel=1;
   int * newLabels = new int[currentLabel]; 
   bool * tmpLabelAry = new bool[currentLabel]; 
   for( int i=0 ; i<currentLabel; i++ ) newLabels[i]=0;

   for( int i=1 ; i<currentLabel ; i++ )
   {
	   for( int ii=0 ; ii<currentLabel; ii++ ) tmpLabelAry[ii]=false;
	   recurseAndResolve( i, tmpLabelAry, collisionVec, currentLabel );
          

	   if( newLabels[i] == 0 )  //if the newLabel has not been assigned
	   {
		//assign the newCurrentLabel to all those which are true in tmpLabelAry[]
 		for( int ii=1 ; ii<currentLabel ; ii++ )
		{
			if( tmpLabelAry[ii] == true )
				newLabels[ ii ] = newCurrentLabel;
		}
		newCurrentLabel++;
	   }
   }	 

  

  // Re-label labelVec
   baseInd = 0; //start frm row 1 nd not row 0

   for( int h=1 ; h<height-1 ; h++ )
   {
	   baseInd += widthStep;
	   for( int w=1 ; w<width-1 ; w++ )
	   {
  		   labelVec[ baseInd + w ] = newLabels[   labelVec[ baseInd + w ] ] ;
	   }
   }
   *totalConnectedComponentNum = newCurrentLabel - 1;

   delete [] newLabels;
   delete [] tmpLabelAry;
  
}


void removeBlob(IplImage* im,int l,int *labelvec)
{
    int i;
    int size = im->imageSize;
    uchar *data;
    data = (uchar *)im->imageData;
    for(i = 0;i<size;i++)
    {
        if( labelvec[i] == l)
        {
          data[i] = 255;
        }
    }
}

int openCCA(IplImage *img)
{
 
  int height,width,step,channels;
  uchar *data;
  int i,j,k;

  // get the image data
  height    = img->height;
  width     = img->width;
  step      = img->widthStep;
  channels  = img->nChannels;
  data      = (uchar *)img->imageData;
  printf("Processing a %dx%d image with %d channels\n",height,width,channels); 

   //CCA labelling
  int * labelVec = new int[ img->imageSize ];
  int connectedComponentNum=0;
  doCCALabelling( img, labelVec, &connectedComponentNum );
  printf( "Total Conected Component Num = %d\n", connectedComponentNum );
  
  int * freqCountAry = new int[ connectedComponentNum+1 ];
  for( int i=0 ; i<=connectedComponentNum ; i++ )  freqCountAry[i] = 0;
  getFrequency( img, labelVec, freqCountAry, connectedComponentNum );
  

  for( int i=1; i<=connectedComponentNum ; i++ )
  {
	printf( "%d :: %d\n", i, freqCountAry[ i ] );
  }
  


  //delete [] labelVec;
  //delete [] freqCountAry;
  return connectedComponentNum;
}


void overlay(IplImage *img ,IplImage *img1)
{ 
  int height,width,step,channels,i,j,k;
  uchar *data,*data1;
        height = img->height;
        width  = img->width;
        step   = img->widthStep;
        channels = img->nChannels;
        data =   (uchar *)img->imageData;
        data1 = (uchar *)img1->imageData;
       for(i=0;i<height;i++) for(j=0;j<width;j++) for(k=0;k<channels;k++)
       { 
         if(data1[i*step + j*channels + k] == 255)
         {
               data1[i*step +j*channels +k] =  data[i*step + j*channels + k];
         }
       }
}

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

void segment(IplImage *gray2,int threshold)
{
         int i,j,k;
         int  height_gray2      = gray2->height;
         int  width_gray2       = gray2->width;
         int  step_gray2        = gray2->widthStep;
         int channels_gray2     = gray2->nChannels;
         uchar * data_gray2     = (uchar *)gray2->imageData;
         
         
         for(i=0;i<height_gray2;i++) {for(j=0;j<width_gray2;j++){for(k=0;k<channels_gray2;k++)
         { 
          
           if(data_gray2[i*step_gray2+j*channels_gray2+k] >=  threshold)
           {
              data_gray2[i*step_gray2 + j*channels_gray2 + k] = 255;
           }
           else
           {
             data_gray2[i*step_gray2 + j*channels_gray2 + k] = 0;
           }
         }}}
}


void centroidOfImageObjects(IplImage *img,CvPoint *pt_katori,CvPoint *pt_stick,CvPoint *pt_bhaguna,CvPoint *pt_kadai,CvPoint *pt_namak,CvPoint *pt_bottle,CvPoint *pt_egg,CvPoint *pt_ham,CvPoint *pt_knife,CvPoint *pt_plate)
{
  //IplImage* img = cvLoadImage(argv[1]);
  IplImage* temp_katori = cvLoadImage("crop_katori.jpg");
  IplImage* temp_stick = cvLoadImage("crop_stick.jpg");
  IplImage* temp_bhaguna = cvLoadImage("crop_bhaguna.jpg");
  IplImage* temp_kadai = cvLoadImage("crop_kadai.jpg");
  IplImage* temp_namak = cvLoadImage("crop_namak.jpg");
  IplImage* temp_bottle = cvLoadImage("crop_bottle.jpg");
  IplImage* temp_egg = cvLoadImage("crop_egg.jpg");
  IplImage* temp_ham = cvLoadImage("crop_ham.jpg");
  IplImage* temp_knife =cvLoadImage("crop_knife.jpg");
  IplImage* temp_plate =cvLoadImage("crop_plate.jpg");
  
  CvPoint pt_katori1,pt_stick1, pt_bhaguna1, pt_kadai1,pt_namak1,pt_bottle1,pt_egg1,pt_ham1,pt_knife1,pt_plate1;
 
  template_matching(img,temp_katori,&pt_katori1);
  pt_katori->x = pt_katori1.x;pt_katori->y = pt_katori1.y;
  //printf("centre of katori is %d and %d \n",pt_katori.x , pt_katori.y);  
 
  template_matching(img,temp_stick,&pt_stick1);
  //printf("centroid of stick is %d and %d \n",pt_stick.x,pt_stick.y); 
 pt_stick->x = pt_stick1.x;
 pt_stick->y = pt_stick1.y;
  template_matching(img,temp_bhaguna,&pt_bhaguna1);
 // printf("centroid of bhaguna is %d and %d\n",pt_bhaguna.x,pt_bhaguna.y);  
  pt_bhaguna->x = pt_bhaguna1.x;
  pt_bhaguna->y = pt_bhaguna1.y;
  template_matching(img,temp_kadai,&pt_kadai1);
 // printf("centroid of kadai is %d and %d\n",pt_kadai.x,pt_kadai.y);
  pt_kadai->x = pt_kadai1.x;
  pt_kadai->y = pt_kadai1.y;

  template_matching(img,temp_namak,&pt_namak1);
  //printf("centroid of namak is %d and %d\n is",pt_namak.x,pt_namak.y);
   pt_namak->x = pt_namak1.x;
   pt_namak->y = pt_namak1.y;

  template_matching(img,temp_bottle,&pt_bottle1);
  //printf("centroid of bottle is %d and %d \n ",pt_bottle.x,pt_bottle.y);
  pt_bottle->x = pt_bottle1.x;
  pt_bottle->y = pt_bottle1.y;
  template_matching(img,temp_egg,&pt_egg1); 
  //printf("centroid of egg is %d and %d \n", pt_egg.x,pt_egg.y);
  pt_egg->x = pt_egg1.x;
  pt_egg->y = pt_egg1.y;

  template_matching(img,temp_ham,&pt_ham1);
 // printf("centroid of ham is %d and %d \n", pt_egg.x ,pt_egg.y);
  pt_ham->x = pt_ham1.x;
  pt_ham->y = pt_ham1.y;
  template_matching(img,temp_knife,&pt_knife1); 
 // printf("centroid of knife is %d and %d \n",pt_knife.x,pt_knife.y);
  pt_knife->x = pt_knife1.x;
  pt_knife->y = pt_knife1.y;

  template_matching(img,temp_plate,&pt_plate1); 
 // printf("centroid of plate is %d and %d \n",pt_plate.x,pt_plate.y);
  pt_plate->x = pt_plate1.x;
  pt_plate->y = pt_plate1.y;

}

int distancePoints(CvPoint pt, CvPoint pt1)
{ 
  int distance;
 return distance = (int) sqrt((double)(pt.x - pt1.x)*(pt.x - pt1.x) + (pt.y - pt1.y)*(pt.y - pt1.y));
}


//--------------------------------------------------------------------------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------------------------------------------------------------------

void contourTrackBothHand2(IplImage *thresh,IplImage *dst,CvPoint *pt,CvPoint *pt_2,CvRect *rectl ,CvRect *rectr)
{
	int i , x,y;
        static CvPoint staticpt1,staticpt2;        
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
             // pt->x =  (k * pt->x + x)/(k+1);
             // pt->y =  (k * pt->y + y)/(k+1);
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
  //   printf("value of k is %d \n",k);
   //  printf("just check %d %d %d %d\n",pt1[0].x,pt1[0].y,pt2[0].x - pt1[0].x,pt2[0].y - pt1[0].y);
     if(k ==1)
     {  
      *rectl = cvRect(pt1[0].x,pt1[0].y,pt2[0].x - pt1[0].x,pt2[0].y - pt1[0].y);
      *rectr = cvRect(0,0,0,0);
    //  printf("hi bansal\n");
      }  
     if(k ==2)
     {  
      *rectl = cvRect(pt1[0].x,pt1[0].y,pt2[0].x - pt1[0].x,pt2[0].y - pt1[0].y);
      *rectr = cvRect(pt1[1].x,pt1[1].y,pt2[1].x - pt1[1].x,pt2[1].y - pt1[1].y);
     } 
      if(k == 0 || k == 3 || k==4)
     {  
      *rectl = cvRect(0,0,0,0);
      *rectr = cvRect(0,0,0,0);
     } 
    
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
      		pt_2->x = spt[1].x;
        	pt_2->y = spt[1].y;
	}
	else if(spt[1].x < spt[0].x)
	{        
		pt->x = spt[1].x;
        	pt->y = spt[1].y;
      		pt_2->x = spt[0].x;
        	pt_2->y = spt[0].y;
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
        }
       
        else
        { 
          
            if( (pt1[0].x + pt2[0].x/2)  <= threshwidth/2)
           {
               pt->x = pt1[0].x + pt2[0].x/2;
               pt->y = pt1[0].y + pt2[0].y/2;
               
               pt_2->x = 0; 
               pt_2->y = 0;
          }
          
          else
          {
                            pt_2->x = pt1[0].x + pt2[0].x/2;
                            pt_2->y = pt1[0].y + pt2[0].y/2;
                            pt->x = 0;
                            pt->y = 0;
          }
        }
 
   }    
  
   else
   {
       pt->x = 0;
       pt->y = 0;
       pt_2->x = 0;
       pt_2->y = 0 ;
   }
       
}

