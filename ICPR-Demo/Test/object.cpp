#include "stdafx.h"
#include<highgui.h>
#include<cv.h>
#include "object.h"
//#include "hand.h"

//---------------------------------------------------------FryingPan Track------------------------------------------------------------------


int FPTrack(CvPoint pt_kadai,int kadai_W,int kadai_H,IplImage *img,IplImage *temp_kadai)
{
	int result = 0;
	CvRect rect = cvRect(pt_kadai.x-(kadai_W/2),pt_kadai.y-(kadai_H/2),kadai_W,kadai_H);
	cvSetImageROI(img,rect);  
	IplImage *img2 = cvCreateImage(cvGetSize(img), 8, 3);
	cvCopy(img,img2);
	cvResetImageROI(img);
	
	//cvShowImage("ROI-fryingPan",img2);
	IplImage *img3 = cvCreateImage(cvGetSize(img2), 8, 1);
	IplImage *kadaiGray = cvCreateImage(cvGetSize(temp_kadai), 8, 1);
	cvCvtColor(img2,img3,CV_BGR2GRAY);
	cvCvtColor(temp_kadai,kadaiGray,CV_BGR2GRAY);

    IplImage *diff = cvCreateImage(cvGetSize(img3), 8, 1);
	cvAbsDiff(img3,kadaiGray,diff);
	
	int sum = 0;
	for(int l=0;l<diff->imageSize;l++)
		sum += (uchar)diff->imageData[l];
	sum = sum / diff->imageSize;
 

	if(sum < 45)
	{
		/*for(int l=0;l<diff->imageSize;l++)
		{	
			if((uchar)img3->imageData[l] > 120) img3->imageData[l] = 0;
			else img3->imageData[l] = 255; 
		}
		CvRect eggRct = cvRect(10,10,img3->width-30,img3->height-28);
		cvSetImageROI(img3,eggRct);
		IplImage *eggReg = cvCreateImage(cvGetSize(img3),8,1);
		cvCopy(img3,eggReg);
		cvResetImageROI(img3);
		int blacks = 0 ;
		for(int l=0;l<eggReg->imageSize;l++)
		{	
			if((uchar)eggReg->imageData[l] == 0) blacks++;
		}
		//cvShowImage("akd",eggReg);
		cvReleaseImage(&eggReg);
		//printf("%d\n",blacks);
		
		if(blacks > 550) result = 1;
		else result = 0; */
		result = 0;
	}
	else result = 1;

	cvReleaseImage(&img2);
	cvReleaseImage(&img3);
	cvReleaseImage(&diff);
	cvReleaseImage(&kadaiGray);

	return result;
}

//------------------------------------------------------------------------------------------------------------------------

int PTrack(CvPoint pt_bhaguna,int bhaguna_W,int bhaguna_H,IplImage *img,IplImage *temp_bhaguna)
{
	int result;	
	CvRect rect = cvRect(pt_bhaguna.x-(bhaguna_W/2),pt_bhaguna.y-(bhaguna_H/2),bhaguna_W,bhaguna_H);
	cvSetImageROI(img,rect);  
	IplImage *img2 = cvCreateImage(cvGetSize(img), 8, 3);
	cvCopy(img,img2);
	cvResetImageROI(img);
	
	//cvShowImage("ROI-Pan",img2);
	IplImage *img3 = cvCreateImage(cvGetSize(img2), 8, 1);
	IplImage *bhagunaGray = cvCreateImage(cvGetSize(temp_bhaguna), 8, 1);
	cvCvtColor(img2,img3,CV_BGR2GRAY);
	cvCvtColor(temp_bhaguna,bhagunaGray,CV_BGR2GRAY);

        IplImage *diff = cvCreateImage(cvGetSize(img3), 8, 1);

        cvAbsDiff(img3,bhagunaGray,diff);
	
	int sum = 0;
	for(int l=0;l<diff->imageSize;l++)
		sum += (uchar)diff->imageData[l];
	sum = sum / diff->imageSize;
 

	if(sum < 45)
	{
		/*for(int l=0;l<diff->imageSize;l++)
		{	
			if((uchar)img3->imageData[l] > 120) img3->imageData[l] = 0;
			else img3->imageData[l] = 255; 
		}
		CvRect eggRct = cvRect(10,10,img3->width-30,img3->height-28);
		cvSetImageROI(img3,eggRct);
		IplImage *eggReg = cvCreateImage(cvGetSize(img3),8,1);
		cvCopy(img3,eggReg);
		cvResetImageROI(img3);
		int blacks = 0 ;
		for(int l=0;l<eggReg->imageSize;l++)
		{	
			if((uchar)eggReg->imageData[l] == 0) blacks++;
		}
		cvReleaseImage(&eggReg);
		//printf("%d\n",blacks);
		if(blacks > 620) result = 1;
		else result = 0;*/
		result = 0;
	}
	else result = 1;

	cvReleaseImage(&img2);
	cvReleaseImage(&img3);
	cvReleaseImage(&diff);
	cvReleaseImage(&bhagunaGray);

	return result;
}

//------------------------------------------------    Bottle track   -------------------------------------------------------------


int bottleTrack(CvPoint pt_bottle,int bottle_W,int bottle_H,IplImage *img,IplImage *temp_bottle)
{
	CvRect rect = cvRect(pt_bottle.x-(bottle_W/2),pt_bottle.y-(bottle_H/2),bottle_W,bottle_H);
	cvSetImageROI(img,rect);  
	IplImage *img2 = cvCreateImage(cvGetSize(img), 8, 3);
	cvCopy(img,img2);
	cvResetImageROI(img);
	
	cvShowImage("roi",img2);
	IplImage *img3 = cvCreateImage(cvGetSize(img2), 8, 1);
	cvCvtColor(img2,img3,CV_BGR2GRAY);
	
	int sum = 0;
	for(int l=0;l<img3->imageSize;l++)
		sum += (uchar)img3->imageData[l];
	sum = sum / img3->imageSize;
 
	//printf("%d\n",sum);
	//cvShowImage("dbsh",img3);
	cvReleaseImage(&img2);
	cvReleaseImage(&img3);

	if(sum < 115) 
		return 0;
	else 
		return 1;
}

//---------------------------------------------------------------------------------------------------------------------