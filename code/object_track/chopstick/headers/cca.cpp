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
int ycbcr_segment(IplImage *img, IplImage *thresh,int lx,int hx,int lz,int hz,int ly,int hy)
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
	 // printf("\n %d %d %d",x,y,z);	
	  if(z>lz && z < hz && y > ly && y < hy && x > lx && x < hx )								//if(z>77 && z < 127 && y > 133 && y < 173)
		thresh->imageData[j*s/3+k] = 255;
	  else	
		thresh->imageData[j*s/3+k] = 0;	
	} 
     }   
	
	cvReleaseImage(&ycbcr);
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
  //cvMatchTemplate(img, tpl, res, CV_TM_SQDIFF_NORMED);
  //cvMatchTemplate(img, tpl, res, CV_TM_CCORR);
  //cvMatchTemplate(img, tpl, res, CV_TM_CCORR_NORMED);
  //cvMatchTemplate(img, tpl, res, CV_TM_CCOEFF);
  //cvMatchTemplate(img, tpl, res, CV_TM_CCOEFF_NORMED);

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


void centroidOfImageObjects(IplImage *img,CvPoint *pt_katori,CvPoint *pt_stick,CvPoint *pt_bhaguna,CvPoint *pt_kadai,CvPoint *pt_namak,CvPoint *pt_bottle,CvPoint *pt_egg,CvPoint *pt_ham,CvPoint *pt_knife)
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
  
  CvPoint pt_katori1,pt_stick1, pt_bhaguna1, pt_kadai1,pt_namak1,pt_bottle1,pt_egg1,pt_ham1,pt_knife1;
 
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


}
int distancePoints(CvPoint pt, CvPoint pt1)
{ 
  int distance;
 return distance = (int) sqrt((double)(pt.x - pt1.x)*(pt.x - pt1.x) + (pt.y - pt1.y)*(pt.y - pt1.y));
}

//----------------------------------------------------------------------------------------------------------------------------------------------


int template_matching1(IplImage *img,IplImage *tpl, CvPoint *pt)
{
    int thresh = 15;
    CvPoint   minloc, maxloc;
    double    minval, maxval;
    CvSize size = cvSize(img->width  - tpl->width  + 1,img->height - tpl->height + 1);
    IplImage* imgg = cvCreateImage(cvGetSize(img),8,1);
    cvCvtColor(img,imgg,CV_BGR2GRAY);
    IplImage* tplg = cvCreateImage(cvGetSize(tpl),8,1);
    IplImage* diff_tpl = cvCreateImage(cvGetSize(tplg),tplg->depth,tplg->nChannels);
    cvCvtColor(tpl,tplg,CV_BGR2GRAY);

 
         int i,j,k;
         int  height      = imgg->height;
         int  width       = imgg->width;
         int  step        = imgg->widthStep;
         int channels     = imgg->nChannels;
         uchar * data     = (uchar *)imgg->imageData;
         
         int height_tpl = tplg->height;
         int width_tpl = tplg->width;
         int step_tpl = tplg->widthStep;
         int channels_tpl = tplg->nChannels;
         uchar * data_tpl = (uchar *)tplg->imageData;
	
               
         int l,m;
	 i=0;
         j=0;
      
        int min = 10000;
        int iindex = 0;int jindex = 0 ; CvScalar sum;
        IplImage* img2;
        for(i = 0;i<(height-height_tpl+1);i++)
        {
          	for(j = 0;j<(width-width_tpl+1);j++)
          	{   
          	      CvRect rect  = cvRect(j,i,tplg->width,tplg->height);
          	      cvSetImageROI(imgg,rect);
          	      img2 = cvCreateImage(cvGetSize(imgg),imgg->depth,imgg->nChannels);
		      cvCopy(imgg,img2,NULL);
          	      cvResetImageROI(imgg);
          	      cvAbsDiff(tplg,img2,diff_tpl);
          	      sum =   cvAvg(diff_tpl,NULL);
                      cvReleaseImage(&img2);
							
          	  	if(sum.val[0] < min)
          	  	{
          	     		min  = sum.val[0];
          	     		iindex = i;
          	     		jindex = j;
          	      } //printf("%d\n",min);
          	}
         }   	
  
  cvReleaseImage(&imgg);
  cvReleaseImage(&tplg);	  
  cvReleaseImage(&img2);
  cvReleaseImage(&diff_tpl);
  printf("Minimum is %d\n",min);
  if(min < thresh)
  {	 	  
  	pt->x =  jindex + (tpl->width)/2;
  	pt->y =  iindex + (tpl->height)/2;
	return 1;	
  }
  	  	
}   

