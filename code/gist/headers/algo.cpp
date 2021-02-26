#include <cv.h>
#include <highgui.h>
#include<stdio.h>
#include<iostream>
#include<string.h>
#include<math.h>
#include "algo.h"

using namespace std;
using namespace cv;

void gist_vector(IplImage *img,uchar *vect,int scale)					//vect : pointer to uchar array
{
	IplImage *imgR,*imgG,*imgB,*imgY,*imgI,*imgGray;
	int i,j,k,height,width,step,channels;
        uchar *data;
        
	int counter = 0;        

        height    = img->height;
        width     = img->width;
        step      = img->widthStep;
        channels  = img->nChannels;

	data  = (uchar *)img->imageData;
	imgR = cvCreateImage(cvGetSize(img),8,1);
	imgG = cvCreateImage(cvGetSize(img),8,1);
	imgB = cvCreateImage(cvGetSize(img),8,1); 
	imgY = cvCreateImage(cvGetSize(img),8,1); 
	imgI = cvCreateImage(cvGetSize(img),8,1);
	imgGray = cvCreateImage(cvGetSize(img),8,1);        

	cvCvtColor(img,imgGray,CV_RGB2GRAY);	
	
	for(i=0;i<height;i++) for(j=0;j<width;j++) for(k=0;k<channels;k++) 
   	  imgB->imageData[i*step/3+j]=data[i*step+j*channels+0];					//blue

	for(i=0;i<height;i++) for(j=0;j<width;j++) 
   	  imgG->imageData[i*step/3+j]=data[i*step+j*channels+1];					//green

	for(i=0;i<height;i++) for(j=0;j<width;j++) 
   	  imgR->imageData[i*step/3+j]=data[i*step+j*channels+2];					//red


	for(i=0;i<height;i++) {for(j=0;j<width;j++){							//Yellow and Intensity
	   imgY->imageData[i*step/3+j] = (uchar)imgR->imageData[i*step/3+j]+(uchar)imgG->imageData[i*step/3+j] - 2*((uchar)imgB->imageData[i*step/3+j] + abs((uchar)imgR->imageData[i*step/3+j] - (uchar)imgG->imageData[i*step/3+j]));

	   imgI->imageData[i*step/3+j] = ((uchar)imgR->imageData[i*step/3+j]+(uchar)imgG->imageData[i*step/3+j]+(uchar)imgB->imageData[i*step/3+j])/3 ;	
	
	}}

	gabor_Result(imgGray,vect,&counter,scale);			//16*16 = 256			//scale for averaging : 4  
	
	RG_channel(imgR,imgG,vect,&counter,scale);			//6*16 = 96
	
	BY_channel(imgB,imgY,vect,&counter,scale);			//6*16 = 96

	I_channel(imgI,vect,&counter,scale);				//6*16 = 96			/* Total = 544  features */


	cvReleaseImage(&imgR);
	cvReleaseImage(&imgG);
	cvReleaseImage(&imgB);	
	cvReleaseImage(&imgY);
	cvReleaseImage(&imgI);
	cvReleaseImage(&imgGray);


}//func ends


//----------------------------------------------------------------------------------------------------------------------------
															// 2-16   2-32  4-32  4-64  8-64   8-128

void RG_channel(IplImage *imgR,IplImage *imgG,uchar *vect,int *counter,int scale)
{
	IplImage *tt;	
	int i;
	int cent[]={2,2,4,4,8,8,0};
	int sr[]={16,32,32,64,64,128,0};
	int c=2,s=16,k;	
	
	uchar *avg_vector = (uchar*)malloc(sizeof(uchar)*scale*scale);

	for(i=1;i<=6;i++)
	{
		tt = scalingImage(imgG,c);
		cs_diff_RGB(imgR,imgG,tt,c,s);
		avg_image(tt,avg_vector,scale);
			
		for(k=0;k<16;k++) { *(vect + *counter) = *(avg_vector+k);  (*counter)++; }
		
		c = cent[i];
		s = sr[i];
		cvReleaseImage(&tt);
	}
	
}

//--------------------------------------------------------------------------------------------------------------------------
	
void BY_channel(IplImage *imgB,IplImage *imgY,uchar *vect,int *counter,int scale)
{
	IplImage *tt;	
	int i;
	int cent[]={2,2,4,4,8,8,0};
	int sr[]={16,32,32,64,64,128,0};
	int c=2,s=16,k;	

	uchar *avg_vector = (uchar*)malloc(sizeof(uchar)*scale*scale);
	
	for(i=1;i<=6;i++)
	{
		tt = scalingImage(imgY,c);
		cs_diff_RGB(imgB,imgY,tt,c,s);
		avg_image(tt,avg_vector,scale);
			
		for(k=0;k<16;k++) { *(vect + *counter) = *(avg_vector+k);  (*counter)++; }
		
		c = cent[i];
		s = sr[i];
		cvReleaseImage(&tt);
	}	
}

//------------------------------------------------------------------------------------------------------------------------------
	
void I_channel(IplImage *imgI,uchar *vect,int *counter,int scale)
{
	
	IplImage *tt;	
	int i;
	int cent[]={2,2,4,4,8,8,0};
	int sr[]={16,32,32,64,64,128,0};
	int c=2,s=16,k;	
	
	uchar *avg_vector = (uchar*)malloc(sizeof(uchar)*scale*scale);

	for(i=1;i<=6;i++)
	{
		tt = scalingImage(imgI,c);
		cs_diff_I(imgI,tt,c,s);
		avg_image(tt,avg_vector,scale);
			
		for(k=0;k<16;k++) { *(vect + *counter) = *(avg_vector+k);  (*counter)++; }
		
		c = cent[i];
		s = sr[i];
		cvReleaseImage(&tt);
	}	

}

//-----------------------------------------------------------------------------------------------------------------------------

IplImage *scalingImage(IplImage *src,int scale)				//dest :  NULL pointer
{
	IplImage *dest = cvCreateImage(cvSize(src->width/scale,src->height/scale),8,1);
	cvResize( src, dest, CV_INTER_LINEAR );
	return dest;
}
	
//----------------------------------------------------------------------------------------------------------------------------------

											
void cs_diff_RGB(IplImage* src1,IplImage* src2,IplImage *dest,int c,int s)			// 2-16   2-32  4-32  4-64  8-64   8-128
{												//dest : NULL pointer
													
	IplImage* t1,*t3,*t4;
	t1 = scalingImage(src1,c);
	cvAbsDiff(t1,dest,dest);

	t3 = scalingImage(src1,s);
	t4 = scalingImage(src2,s);
	cvAbsDiff(t3,t4,t4);
	
	cvResize( t4,t1, CV_INTER_LINEAR );
	cvAbsDiff(dest,t1,dest);
	
	cvReleaseImage(&t1);
	cvReleaseImage(&t3);
	cvReleaseImage(&t4);
}

//-----------------------------------------------------------------------------------------------------------------------------------------

void cs_diff_I(IplImage* src1,IplImage *dest,int c,int s)				// 2-16   2-32  4-32  4-64  8-64   8-128
{											
	IplImage* t1,*t2;
	t1 = scalingImage(src1,c);
	t2 = scalingImage(src1,s);
	
	cvResize(t2,dest, CV_INTER_LINEAR );
	cvAbsDiff(t1,dest,dest);
	
	cvReleaseImage(&t1);
	cvReleaseImage(&t2);
}

//-----------------------------------------------------------------------------------------------------------------------------------

void gabor_Result(IplImage *src,uchar *vect,int *counter,int scle)
{
		
	IplImage *im,*im2;
	int i,j,k,scale=2,angle=0;
	
	uchar *avg_vector = (uchar*)malloc(sizeof(uchar)*scle*scle);

	for(i=0;i<4;i++)
	{
		im = scalingImage(src,scale);
		im2 = cvCreateImage(cvGetSize(im),IPL_DEPTH_32F,1);

		for(j=0;j<4;j++)
		{
			gabor_filter(im,im2,angle);
			
			avg_image(im2,avg_vector,scle);
			
			for(k=0;k<16;k++) { *(vect + *counter) = *(avg_vector+k);  (*counter)++; }
			
			angle += 45;
		}
		scale *= 2;
		cvReleaseImage(&im2);			
		cvReleaseImage(&im);
	}
	

}

//--------------------------------------------------------------------------------------------------------------------------------


void avg_image(IplImage *img,uchar *avg_vector,int scale)	
{
	int i,j,k,l,count=0;
	float tmp=0;
	int x_incr = (int)(img->width)/scale;
	int y_incr = (int)(img->height)/scale;
	int pixels  = x_incr*y_incr;
	int stepI = img->widthStep;

	for(i=0;i<y_incr*scale;i=i+y_incr)
	{
		for(j=0;j<x_incr*scale;j=j+x_incr)	
		{
			tmp = 0;	
			for(k=0;k<y_incr;k++)
				for(l=0;l<x_incr;l++)	
					tmp += (float)(img->imageData[(i+k)*stepI+(l+j)])/pixels;
					
				*(avg_vector+count) = (uchar)tmp;
				count++;
			}	
	}
	
}//function ends


//---------------------------------------------------------------------------------------------------------------------------------------


void gabor_filter(IplImage *img,IplImage *dest_mag,int angle)
{
  IplImage* src = 0;
  IplImage* src_f = 0;
  IplImage* image = 0;
  IplImage* dest = 0;
  IplImage* kernelimg=0;
  IplImage* big_kernelimg=0;
  CvMat * kernel=0;
  int kernel_size =21;
  int pos_var = 50;
  int pos_w = 5;
  int pos_phase = angle;
  int pos_psi = 90;
  
  
   if (kernel_size%2==0)
      kernel_size++;
    kernel = cvCreateMat(kernel_size,kernel_size,CV_32FC1);
    kernelimg = cvCreateImage(cvSize(kernel_size,kernel_size),IPL_DEPTH_32F,1);
    big_kernelimg = cvCreateImage(cvSize(kernel_size*20,kernel_size*20),IPL_DEPTH_32F,1);

    src = cvCloneImage(img);
    src_f = cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_32F,1);

    cvConvertScale(src,src_f,1.0/255,0);
    dest = cvCloneImage(src_f);
    cvCopy(src_f,dest_mag);

    int x,y;
    float kernel_val;
    float var = (float)pos_var/10;
    float w = (float)pos_w/10;
    float phase = (float) pos_phase*CV_PI/180;
    float psi = CV_PI*pos_psi/180;

    cvZero(kernel);
    for (x = -kernel_size/2;x<=kernel_size/2; x++) {
      for (y = -kernel_size/2;y<=kernel_size/2; y++) {
        kernel_val = exp( -((x*x)+(y*y))/(2*var))*cos( w*x*cos(phase)+w*y*sin(phase)+psi);
        cvSet2D(kernel,y+kernel_size/2,x+kernel_size/2,cvScalar(kernel_val));
        cvSet2D(kernelimg,y+kernel_size/2,x+kernel_size/2,cvScalar(kernel_val/2+0.5));
      }
    }

    cvFilter2D(src_f, dest,kernel,cvPoint(-1,-1));
    cvResize(kernelimg,big_kernelimg);
    cvPow(dest,dest_mag,2);
  
    cvReleaseImage(&src);
    cvReleaseImage(&src_f);
   
    cvReleaseImage(&dest);
    cvReleaseMat(&kernel);
    cvReleaseImage(&kernelimg);
    cvReleaseImage(&big_kernelimg);
}

//-------------------------------------------------------------------------------------------------------------------


PCA compressPCA(const Mat& pcaset, int maxComponents,Mat& compressed)
{
	PCA pca(pcaset, Mat(), CV_PCA_DATA_AS_ROW, maxComponents);
        
	compressed.create(pcaset.rows, maxComponents, pcaset.type());
     
      	for( int i = 0; i < pcaset.rows; i++ )
        {
            Mat vec = pcaset.row(i), coeffs = compressed.row(i);
	    pca.project(vec, coeffs);
        }
     
	return pca;	
}


//-----------------------------------------------------------------------------------------------------------------------


void buildTrainData(IplImage *im, char *label, char *out)				//training data for multiclass SVM
{ 
	int h,w,i,j,target,step;	
	//FILE *in = fopen(label,"r");
	FILE *output = fopen(out,"w");   //correction:  "a" --> "w"
	uchar * data = (uchar*)im->imageData;	

	h = im->height;
	w = im->width;
	step = im->widthStep;

	for(i=0;i<h;i++)
	{
		//fscanf(in,"%d",&target);
		//fprintf(output,"%d  ",target);		
		for(j=0;j<w;j++)
		{
		   fprintf(output,"%f ",(float)data[i*step+j]);				
		}
		fprintf(output,"\n");
	}
	fclose(output);
}

//---------------------------------------------------------------------------------------------------------------------------------------

void buildTestData(IplImage *im, char *label, char *out)				//test data for multiclass SVM
{ 
	int h,w,i,j,target,step;	
	FILE *output = fopen(out,"w");
	uchar * data = (uchar*)im->imageData;	

	h = im->height;
	w = im->width;
	step = im->widthStep;

	if(label != NULL)
	{
		FILE *in = fopen(label,"r");
		for(i=0;i<h;i++)
		{
			fscanf(in,"%d",&target);
			fprintf(output,"%d  ",target);		
			for(j=0;j<w;j++)
			{
		   	fprintf(output,"%d:%f ",j+1,(float)data[i*step+j]);				
			}
			fprintf(output,"\n");
		}
		fclose(in);
	}
	else
	{
		for(i=0;i<h;i++)
		{
			fprintf(output,"%d  ",1000);		
			for(j=0;j<w;j++)
			{
		   	fprintf(output,"%d:%f ",j+1,(float)data[i*step+j]);				
			}
			fprintf(output,"\n");
		}		

	}

	fclose(output);
}


