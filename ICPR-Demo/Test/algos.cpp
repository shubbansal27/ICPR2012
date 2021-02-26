#include "stdafx.h"
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include"algos.h"
#include "hand.h"


int bottle_depth(IplImage *RGB,IplImage *DEP,CvRect rBottle,int th)
{
	cvSetImageROI(RGB,rBottle); 
	IplImage *rgb = cvCreateImage(cvGetSize(RGB),8,3);
	cvCopy(RGB,rgb);
	cvResetImageROI(RGB);

	CvRect rBot = cvRect(rBottle.x - 40,rBottle.y - 40,rBottle.width+80,rBottle.height+80);
	cvSetImageROI(DEP,rBot);
	IplImage *depth = cvCreateImage(cvGetSize(DEP),8,3);
	IplImage *depthGray = cvCreateImage(cvGetSize(depth),8,1);
	cvCopy(DEP,depth);
	cvCvtColor(depth,depthGray,CV_BGR2GRAY);
	cvResetImageROI(DEP);

	int sum = 0;
	for(int i=0;i<depthGray->imageSize;i++)
	{
		if(depthGray->imageData[i] > 120) sum++; 
	}

	//printf("sum = %d\n",sum);
	//cvShowImage("rgb",rgb);
	//cvShowImage("depthGray",depthGray);

	cvReleaseImage(&rgb);
	cvReleaseImage(&depth);
	cvReleaseImage(&depthGray);
	
	if(sum > th)
		return 0;
	else
		return 1;
}

//--------------------------------------------------------------------------------------------------------------

int Pan_template2(IplImage *RGB,IplImage *temp,CvRect rPan,int lx,int hx,int lz,int hz,int ly,int hy,int th)
{
	cvSetImageROI(RGB,rPan);
	IplImage *rgb = cvCreateImage(cvGetSize(RGB),8,3);
	cvCopy(RGB,rgb);
	IplImage *ycbcr = cvCreateImage(cvGetSize(rgb),8,1);
	
	ycbcr_segment2(rgb,ycbcr,lx,hx,lz,hz,ly,hy);

	int sum = 0;
	for(int i =0;i<ycbcr->imageSize;i++)
	{
		if(ycbcr->imageData[i] == 0) sum++;
	}

	//printf("sum = %d\n",sum);
	//cvShowImage("rgb",rgb);
	//cvShowImage("ycbcr",ycbcr);
	cvResetImageROI(RGB);
	cvReleaseImage(&ycbcr);
	cvReleaseImage(&rgb);
	
	if(sum > th)
		return 1;
	else 
		return 0;
}

//--------------------------------------------------------------------------------------------------------------


int pan_template(IplImage *RGB,IplImage *temp,CvRect rPan,int th)
{
	cvSetImageROI(RGB,rPan);
	IplImage *RGBGray = cvCreateImage(cvGetSize(RGB),8,1);
	cvCvtColor(RGB,RGBGray,CV_BGR2GRAY);
	cvResetImageROI(RGB);
	
	IplImage *tmpGray = cvCreateImage(cvGetSize(temp),8,1);
	cvCopy(temp,tmpGray);

	cvEqualizeHist(RGBGray,RGBGray);
	cvEqualizeHist(tmpGray,tmpGray);

	IplImage *change_img = cvCreateImage(cvGetSize(temp),8,1);
	cvAbsDiff(RGBGray,tmpGray,change_img);
	CvScalar sum = cvSum(change_img); //Pan
	int avg = (int)(sum.val[0] / change_img->imageSize);

	//printf("%d\n",avg);
	//cvShowImage("RGBGray",RGBGray);
	//cvShowImage("tmpGray",tmpGray);
	//cvShowImage("change_img",change_img);

	cvReleaseImage(&tmpGray);
	cvReleaseImage(&RGBGray);
	cvReleaseImage(&change_img);

	if(avg > th)
		return 1;
	else
		return 0;

}



int FP_template2(IplImage *RGB,IplImage *temp,CvRect rPan,int lx,int hx,int lz,int hz,int ly,int hy,int th)
{
	cvSetImageROI(RGB,rPan);
	IplImage *rgb = cvCreateImage(cvGetSize(RGB),8,3);
	cvCopy(RGB,rgb);
	IplImage *ycbcr = cvCreateImage(cvGetSize(rgb),8,1);
	
	ycbcr_segment2(rgb,ycbcr,lx,hx,lz,hz,ly,hy);

	int sum = 0;
	for(int i =0;i<ycbcr->imageSize;i++)
	{
		if(ycbcr->imageData[i] == 0) sum++;
	}

	//printf("sum = %d\n",sum);
	//cvShowImage("rgb",rgb);
	//cvShowImage("ycbcr",ycbcr);
	cvResetImageROI(RGB);
	cvReleaseImage(&ycbcr);
	cvReleaseImage(&rgb);
	
	if(sum > th)
		return 1;
	else 
		return 0;
}


//-----------------------------------------------------------------------------------------------------

int FP_template(IplImage *RGB,IplImage *temp,CvRect rPan)
{
	cvSetImageROI(RGB,rPan);
	IplImage *RGBGray = cvCreateImage(cvGetSize(RGB),8,1);
	cvCvtColor(RGB,RGBGray,CV_BGR2GRAY);
	cvResetImageROI(RGB);
	
	IplImage *tmpGray = cvCreateImage(cvGetSize(temp),8,1);
	cvCopy(temp,tmpGray);

	cvEqualizeHist(RGBGray,RGBGray);
	cvEqualizeHist(tmpGray,tmpGray);

	IplImage *change_img = cvCreateImage(cvGetSize(temp),8,1);
	cvAbsDiff(RGBGray,tmpGray,change_img);
	CvScalar sum = cvSum(change_img); //Pan
	//int avg = (int)sum.val[0];
	int avg = (int)(sum.val[0] / change_img->imageSize);

	int tt = 0;
	for(int i=0;i<RGBGray->imageSize;i++)
	{
		if(RGBGray->imageData[i] > 120) tt++; 
	}

	//printf("avg = %d   tt = %d\n",avg,tt);
	//cvShowImage("RGBGray",RGBGray);
	//cvShowImage("tmpGray",tmpGray);
	//cvShowImage("change_img",change_img);

	cvReleaseImage(&tmpGray);
	cvReleaseImage(&RGBGray);
	cvReleaseImage(&change_img);

	if(avg > 15 )
		return 1;
	else
		return 0;

		
}

//----------------------------------------------------------------------------------------------------------------------------


int grater_ycbcr( IplImage * templ, int lx,int hx,int lz,int hz,int ly,int hy,int thresh) 
{
	IplImage *ycbcr = cvCreateImage(cvGetSize(templ),8,1);
	ycbcr_segment2(templ,ycbcr,lx,hx,lz,hz,ly,hy);
	cvErode(ycbcr,ycbcr);
	cvDilate(ycbcr,ycbcr);
	//cvDilate(ycbcr,ycbcr);
	//cvShowImage("ycbcr",ycbcr);
	CvScalar tmp = cvSum(ycbcr); 
	int sum = tmp.val[0] ;

	//printf("%d\n",sum);
	if(sum > thresh)
		return 0;
	else 
		return 1;

	cvReleaseImage(&ycbcr);
}

//-----------------------------------------------------------------------------------------------------

int  chopstickYellow_ycbcr( IplImage * templ, int lx,int hx,int lz,int hz,int ly,int hy,int thresh) 
{
	IplImage *ycbcr = cvCreateImage(cvGetSize(templ),8,1);
	ycbcr_segment2(templ,ycbcr,lx,hx,lz,hz,ly,hy);
	cvErode(ycbcr,ycbcr);
	cvDilate(ycbcr,ycbcr);
	//cvShowImage("ycbcr",ycbcr);
	CvScalar tmp = cvSum(ycbcr); 
	int sum = tmp.val[0] ;

	//printf("%d\n",sum);
	if(sum > thresh)
		return 0;
	else 
		return 1;

	cvReleaseImage(&ycbcr);
}

//---------------------------------------------------------------------------------------------------------------

int  knife_ycbcr( IplImage * templ, int lx,int hx,int lz,int hz,int ly,int hy,int thresh) 
{
	IplImage *ycbcr = cvCreateImage(cvGetSize(templ),8,1);
	ycbcr_segment2(templ,ycbcr,lx,hx,lz,hz,ly,hy);
	cvErode(ycbcr,ycbcr);
	cvDilate(ycbcr,ycbcr);
	//cvShowImage("ycbcr",ycbcr);
	CvScalar tmp = cvSum(ycbcr); 
	int sum = tmp.val[0] ;

	//printf("%d\n",sum);
	if(sum > thresh)
		return 0;
	else 
		return 1;

	cvReleaseImage(&ycbcr);
}

//-----------------------------------------------------------------------------------------------------
int saltbox_ycbcr( IplImage * templ, int lx,int hx,int lz,int hz,int ly,int hy,int thresh) 
{
	IplImage *ycbcr = cvCreateImage(cvGetSize(templ),8,1);
	ycbcr_segment2(templ,ycbcr,lx,hx,lz,hz,ly,hy);
	cvErode(ycbcr,ycbcr);
	cvDilate(ycbcr,ycbcr);
	CvScalar tmp = cvSum(ycbcr); 
	int sum = tmp.val[0] ;

	//cvShowImage("ycbcr",ycbcr);
	//printf("%d\n",sum);
	if(sum > thresh)
		return 0;
	else 
		return 1;

	cvReleaseImage(&ycbcr);
}

//------------------------------------------------------------------------------------------

int  Peeler_ycbcr( IplImage * templ, int lx,int hx,int lz,int hz,int ly,int hy,int thresh) 
{
	IplImage *ycbcr = cvCreateImage(cvGetSize(templ),8,1);
	ycbcr_segment2(templ,ycbcr,lx,hx,lz,hz,ly,hy);
	cvErode(ycbcr,ycbcr);
	cvDilate(ycbcr,ycbcr);
	cvDilate(ycbcr,ycbcr);
	//cvShowImage("ycbcr",ycbcr);
	CvScalar tmp = cvSum(ycbcr); 
	int sum = tmp.val[0] ;

	//printf("%d\n",sum);
	if(sum > thresh)
		return 0;
	else 
		return 1;

	cvReleaseImage(&ycbcr);
}


//-------------------------------------------------------------------------------------------------------


int Bowl_ycbcr( IplImage * templ,int K, int lx,int hx,int lz,int hz,int ly,int hy, CvRect *newLoc,CvRect orig_bowl,int threshold) 
{
	//printf("newLoc:  %d %d %d %d\n",newLoc->x,newLoc->y,newLoc->width,newLoc->height);

	CvRect loc = cvRect(newLoc->x,newLoc->y,newLoc->width,newLoc->height);
	//printf("loc:  %d %d %d %d\n",loc.x-K,loc.y-K,loc.width+2*K,loc.height+2*K);
	
	CvRect tmp = cvRect(loc.x-K,loc.y-K,loc.width+2*K,loc.height+2*K);
	//CvRect tmp = cvRect(149,231,62,58);
	//CvRect tmp = cvRect(149,231,100,100);
	//printf("loc:  %d %d %d %d\n",tmp.x,tmp.y,tmp.width,tmp.height);
	
	//printf("entering1..\n");
	if(tmp.x > 0 && tmp.y  > 0 && tmp.x < templ->width && tmp.y < templ->height)
	{
	cvSetImageROI(templ,tmp);
	
	IplImage *thresh = cvCreateImage(cvSize(tmp.width,tmp.height),8,3);
	
	cvCopy(templ,thresh);
	cvResetImageROI(templ);
	//printf("entering2..\n");
	IplImage *ycbcr = cvCreateImage(cvGetSize(thresh),8,1);
	ycbcr_segment2(thresh,ycbcr,lx,hx,lz,hz,ly,hy);
	CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
    cvMoments(ycbcr, moments, 1);
	double moment10 = cvGetSpatialMoment(moments, 1, 0);
	double moment01 = cvGetSpatialMoment(moments, 0, 1);
    double area = cvGetCentralMoment(moments, 0, 0);
	
	//newLoc->x = loc.x+(int)(moment10/area-(loc.width/2));
	//newLoc->y = loc.y+(int)(moment01/area-(loc.height/2));

	int diffX = (int)(moment10/area-(tmp.width/2));
	int diffY = (int)(moment01/area-(tmp.height/2));
	newLoc->x = tmp.x+diffX;
	newLoc->y = tmp.y+diffY;
	newLoc->width = loc.width;
	newLoc->height = loc.height;

	//printf("x,y : %d %d\n",(int)(moment10/area-(tmp.width/2)),(int)(moment01/area-(tmp.height/2)));
	//printf("diff:  %f %f\n",moment10,moment01);
	//printf("newLoc:  %d %d %d %d\n",newLoc->x,newLoc->y,newLoc->width,newLoc->height);
	
	int jsum=0;
	for(int j=0;j<ycbcr->imageSize;j++)
	{
		if(ycbcr->imageData[j] != 0) jsum++;
	}

	//printf("%d\n",jsum);
	//cvShowImage("thresh",thresh);
	//cvShowImage("ycbcr",ycbcr);
	cvReleaseImage(&thresh);
	cvReleaseImage(&ycbcr);
	delete moments;
	
	if(moment10 == 0 || moment10 == 0)	
		*newLoc = orig_bowl;
	
	if(jsum > threshold)
		return 0;
	else
		return 1;
	//printf("leaving..\n");
	}
	else
	{
		*newLoc = orig_bowl;
		return 1;
	}

}


//-------------------------------------------------------------------------------------------------------------------


void track_ycbcr( IplImage * templ,int K, int lx,int hx,int lz,int hz,int ly,int hy, CvRect *newLoc,CvRect orig_bowl) 
{
	//printf("newLoc:  %d %d %d %d\n",newLoc->x,newLoc->y,newLoc->width,newLoc->height);

	CvRect loc = cvRect(newLoc->x,newLoc->y,newLoc->width,newLoc->height);
	//printf("loc:  %d %d %d %d\n",loc.x-K,loc.y-K,loc.width+2*K,loc.height+2*K);
	
	CvRect tmp = cvRect(loc.x-K,loc.y-K,loc.width+2*K,loc.height+2*K);
	//CvRect tmp = cvRect(149,231,62,58);
	//CvRect tmp = cvRect(149,231,100,100);
	//printf("loc:  %d %d %d %d\n",tmp.x,tmp.y,tmp.width,tmp.height);
	
	//printf("entering1..\n");
	if(tmp.x > 0 && tmp.y  > 0 && tmp.x < templ->width && tmp.y < templ->height)
	{
	cvSetImageROI(templ,tmp);
	
	IplImage *thresh = cvCreateImage(cvSize(tmp.width,tmp.height),8,3);
	
	cvCopy(templ,thresh);
	cvResetImageROI(templ);
	//printf("entering2..\n");
	IplImage *ycbcr = cvCreateImage(cvGetSize(thresh),8,1);
	ycbcr_segment2(thresh,ycbcr,lx,hx,lz,hz,ly,hy);
	CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
    cvMoments(ycbcr, moments, 1);
	double moment10 = cvGetSpatialMoment(moments, 1, 0);
	double moment01 = cvGetSpatialMoment(moments, 0, 1);
    double area = cvGetCentralMoment(moments, 0, 0);
	
	//newLoc->x = loc.x+(int)(moment10/area-(loc.width/2));
	//newLoc->y = loc.y+(int)(moment01/area-(loc.height/2));

	int diffX = (int)(moment10/area-(tmp.width/2));
	int diffY = (int)(moment01/area-(tmp.height/2));
	newLoc->x = tmp.x+diffX;
	newLoc->y = tmp.y+diffY;
	newLoc->width = loc.width;
	newLoc->height = loc.height;

	//printf("x,y : %d %d\n",(int)(moment10/area-(tmp.width/2)),(int)(moment01/area-(tmp.height/2)));
	//printf("diff:  %f %f\n",moment10,moment01);
	//printf("newLoc:  %d %d %d %d\n",newLoc->x,newLoc->y,newLoc->width,newLoc->height);
	
	//cvShowImage("thresh",thresh);
	//cvShowImage("ycbcr",ycbcr);
	cvReleaseImage(&thresh);
	cvReleaseImage(&ycbcr);
	delete moments;
	
	if(moment10 == 0 || moment10 == 0)	
		*newLoc = orig_bowl;
	
	//printf("leaving..\n");
	}
	else
		*newLoc = orig_bowl;

}

//------------------------------------------------------------------------------------------------------------------

int Bowl_ycbcr2( IplImage * templ, int lx,int hx,int lz,int hz,int ly,int hy,int threshold) 
{
	IplImage *ycbcr = cvCreateImage(cvGetSize(templ),8,1);
	ycbcr_segment2(templ,ycbcr,lx,hx,lz,hz,ly,hy);
	
	int jsum=0;
	for(int j=0;j<ycbcr->imageSize;j++)
	{
		if(ycbcr->imageData[j] != 0) jsum++;
	}

	cvReleaseImage(&ycbcr);

	if(jsum > threshold)
		return 0;
	else
		return 1;
	//printf("leaving..\n");
	

}