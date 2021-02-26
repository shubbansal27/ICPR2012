#include<highgui.h>
#include<cv.h>
#include<stdio.h>
#include "object.h"
#include "hand.h"
#include "cca.h"

//---------------------------------------------------------------------- Bhaguna Track -----------------------------------------------------------------------


int bhagunaTrack(CvPoint pt_bhaguna,int bhaguna_W,int bhaguna_H,IplImage *img,IplImage *temp_bhaguna)
{
	int result;	
	CvRect rect = cvRect(pt_bhaguna.x-(bhaguna_W/2),pt_bhaguna.y-(bhaguna_H/2),bhaguna_W,bhaguna_H);
	cvSetImageROI(img,rect);  
	IplImage *img2 = cvCreateImage(cvGetSize(img), 8, 3);
	cvCopy(img,img2);
	cvResetImageROI(img);
	
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
		for(int l=0;l<diff->imageSize;l++)
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
		else result = 0;				
	}
	else result = 1;

	cvReleaseImage(&img2);
	cvReleaseImage(&img3);
	cvReleaseImage(&diff);
	cvReleaseImage(&bhagunaGray);

	return result;
}

//----------------------------------------------------------------------kadai Track------------------------------------------------------------------


int kadaiTrack(CvPoint pt_kadai,int kadai_W,int kadai_H,IplImage *img,IplImage *temp_kadai)
{
	int result = 0;	
	CvRect rect = cvRect(pt_kadai.x-(kadai_W/2),pt_kadai.y-(kadai_H/2),kadai_W,kadai_H);
	cvSetImageROI(img,rect);  
	IplImage *img2 = cvCreateImage(cvGetSize(img), 8, 3);
	cvCopy(img,img2);
	cvResetImageROI(img);
	
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
		for(int l=0;l<diff->imageSize;l++)
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
		else result = 0;				
	}
	else result = 1;

	cvReleaseImage(&img2);
	cvReleaseImage(&img3);
	cvReleaseImage(&diff);
	cvReleaseImage(&kadaiGray);

	return result;
}

//----------------------------------------------------------------------Bottle Track------------------------------------------------------------------


int bottleTrack(CvPoint pt_bottle,int bottle_W,int bottle_H,IplImage *img,IplImage *temp_bottle)
{
	CvRect rect = cvRect(pt_bottle.x-(bottle_W/2),pt_bottle.y-(bottle_H/2),bottle_W,bottle_H);
	cvSetImageROI(img,rect);  
	IplImage *img2 = cvCreateImage(cvGetSize(img), 8, 3);
	cvCopy(img,img2);
	cvResetImageROI(img);
	
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

//--------------------------------------------------------------- None Classify -------------------------------------------------

int noneClassify(IplImage *img,IplImage *first)
{	
	IplImage *img1 = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 1);
	cvCvtColor(img,img1,CV_BGR2GRAY);
	IplImage *change_imgF = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 1);
        cvAbsDiff(img1,first,change_imgF);
	uchar *dataF = (uchar *)change_imgF->imageData;
	
	int sum=0 ;
	for (int l=0;l<change_imgF->imageSize;l++)
	{
		 sum = sum + dataF[l] ;
	}
	sum = sum / change_imgF->imageSize ;

	cvReleaseImage(&change_imgF);
	cvReleaseImage(&img1);	

	if(sum < 20)
		return 1;	
	else
		return 0;
}


//------------------------------------------------------------ HAM -----------------------------------------------------------------------

int hamTrack(CvPoint pt_plate,int plate_W,int plate_H,IplImage *img_ham,IplImage *img)
{
	int result;
	CvRect rect = cvRect(pt_plate.x-(plate_W/2),pt_plate.y-(plate_H/2),plate_W,plate_H);
	cvSetImageROI(img,rect);  
	IplImage *img4 = cvCreateImage(cvGetSize(img), 8, 3);
	cvCopy(img,img4);
	cvResetImageROI(img);

	IplImage *thresh = cvCreateImage(cvGetSize(img4),8,1);
	ycbcr_segm(img4,thresh,115,255,151,159,128,255);
	//cvShowImage("th",thresh);
	
	int sum = 0;
	for(int l=0;l<thresh->imageSize;l++)
	{
		if((uchar)thresh->imageData[l] == 0) sum++;
	}	
 
	//printf("%d\n",sum);
	if(sum > 205) result = 1;	
	else result = 0;

	cvReleaseImage(&img4);
	cvReleaseImage(&thresh);
	return result;
	
}

//-----------------------------------------------------------------namak track ------------------------------------------------------------


int namakTrack(CvPoint pt_namak,int namak_W,int namak_H,IplImage *img,IplImage *temp_namak,int *fcnt , int *namakcount)
{
	int result;	
	CvRect rect = cvRect(pt_namak.x-(namak_W/2),pt_namak.y-(namak_H/2),namak_W,namak_H);
	cvSetImageROI(img,rect);  
	IplImage *img2 = cvCreateImage(cvGetSize(img), 8, 3);
	cvCopy(img,img2);
	cvResetImageROI(img);
	
	IplImage *img3 = cvCreateImage(cvGetSize(img2), 8, 1);
	IplImage *namakGray = cvCreateImage(cvGetSize(temp_namak), 8, 1);
	cvCvtColor(img2,img3,CV_BGR2GRAY);
	cvCvtColor(temp_namak,namakGray,CV_BGR2GRAY);
        
        IplImage *diff = cvCreateImage(cvGetSize(img3), 8, 1);
        uchar *data_img3 = (uchar*)img3->imageData;
        uchar *data_namakGray = (uchar *)namakGray->imageData;
        
        int sum = 0;
        for(int i = 0;i<img3->imageSize;i++)
        sum = sum + (data_namakGray[i]  - data_img3[i] );
      //  cvAbsDiff(img3,namakGray,diff);
	
//	int sum = 0;
//	for(int l=0;l<diff->imageSize;l++)
///		sum += (uchar)diff->imageData[l];
//	sum = sum ;
 
	//printf("sum is %d\n",sum); 

	if(sum > 90000 )	//check speed = 1 
	{	
		(*fcnt)++;
	} 
        
        if((*fcnt) > 25 && (*namakcount) < 280)
        {
           result = 1;
           (*namakcount)++;
        } 
	else result = 0;

	cvReleaseImage(&img2);
	cvReleaseImage(&img3);
	cvReleaseImage(&diff);
	cvReleaseImage(&namakGray);

	return result;
}

//---------------------------------------------------------------- egg track --------------------------------------------------------------------

int eggTrack(CvPoint pt_egg,int egg_W,int egg_H,IplImage *img,IplImage *temp_egg)
{
	int result;
	CvRect rect = cvRect(0,0,img->width,img->height-egg_H - 100);
	cvSetImageROI(img,rect);  
	IplImage *img2 = cvCreateImage(cvGetSize(img), 8, 3);
	cvCopy(img,img2);
	cvResetImageROI(img);

	IplImage *thresh = cvCreateImage(cvGetSize(img2),8,1);
	ycbcr_segm(img2,thresh,57,109,157,179,76,138);
	//cvShowImage("th",thresh);
	
	int sum = 0;
	for(int l=0;l<thresh->imageSize;l++)
	{
		if((uchar)thresh->imageData[l] == 0) sum++;
	}

	cvReleaseImage(&thresh);
	cvReleaseImage(&img2);
		
	if(sum > 150) return 1;
	else return 0;
	
}

//----------------------------------------------------------------------- Katori track ---------------------------------------------------------------------


int katoriTrack(CvPoint *oldpt,int *oldx,int *oldy,IplImage *img,IplImage *temp_katori)
{
	int lz = 100,hz = 120,ly = 145,hy = 155 ,lx = 108,hx = 165;
	IplImage *threshimage = cvCreateImage(cvGetSize(img),8,1);
    	ycbcr_segment2(img,threshimage,0,50,91,145,146,170);
    	cvErode(threshimage,threshimage);
	cvErode(threshimage,threshimage);
    	cvDilate(threshimage,threshimage);
	cvDilate(threshimage,threshimage);
    	cvNot(threshimage,threshimage);
  
    	contourTrack_katori(threshimage,threshimage);

    	CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
    	cvMoments(threshimage, moments, 1);
    	double moment10 = cvGetSpatialMoment(moments, 1, 0);
    	double moment01 = cvGetSpatialMoment(moments, 0, 1);
    	double area = cvGetCentralMoment(moments, 0, 0);
    	int x = moment10/area;
    	int y = moment01/area;

    	CvPoint pt1,pt2;
    	pt1.x = x - (temp_katori->width + 8)/2;
    	pt1.y = y - (temp_katori->height + 90) / 2;
       
    	pt2.x = x + (temp_katori->width + 8)/2;
    	pt2.y = y + (temp_katori->height + 90)/2;  
 
   	IplImage *img3 = cvCloneImage(img);
   	cvRectangle(img3,pt1,pt2,cvScalar(0,255,255),1,8,0);
 
    	if(pt1.x <0 || pt1.y <0 || pt1.x > img->width || pt1.y > img->height)
    		pt1 = *oldpt;
    	else
    		*oldpt = pt1;
     
    	CvRect rect = cvRect(pt1.x,pt1.y,temp_katori->width+8,temp_katori->height + 90);
    	cvSetImageROI(img,rect);
    	IplImage *img2rgb = cvCreateImage(cvGetSize(img),8,3);
    	IplImage *img2 = cvCreateImage(cvGetSize(img),8,1);
    	cvCopy(img,img2rgb);
    	cvCvtColor(img2rgb,img2,CV_RGB2GRAY);
    	cvResetImageROI(img);
   
    	ycbcr_segment2(img2rgb,img2,lx,hx,lz,hz,ly,hy);
    
    	int whites = 0; 
    	for(int l=0;l<img2->imageSize;l++)
    	{
		if((uchar)img2->imageData[l] == 255)
			whites++;	
     	}
     
     	
	if(x < 0 || y<0)
     	{
		x = *oldx;
		y = *oldy;	
     	}		
     	else
     	{
		*oldx = x;
		*oldy = y;
  	}	


	cvReleaseImage(&threshimage);
	cvReleaseImage(&img2);
	cvReleaseImage(&img2rgb);
	cvReleaseImage(&img3);
	delete moments;     

	if(whites > 230)
		return 1;	
  	else
		return 0 ;

}

//----------------------------------------------------------------- Hough line ---------------------------------------------------


int houg(IplImage * img,IplImage *img_out,int l,int h)
{
	float chktheta;
    /* sets the Region of Interest
       Note that the rectangle area has to be __INSIDE__ the image */
IplImage *img1;
img1 = cvCreateImage(cvGetSize(img), img->depth, 1);
 cvCvtColor(img, img1, CV_RGB2GRAY);
    //cvSetImageROI(img1, cvRect(150, 100, 550, 230));
    
    /* create destination image
       Note that cvGetSize will return the width and the height of ROI */
    IplImage *src = cvCreateImage(cvGetSize(img1),
                                   img1->depth,
                                   img1->nChannels);
    
    /* copy subimage */
    cvCopy(img1, src, NULL);
    
    /* always reset the Region of Interest */
    //cvResetImageROI(img1);

    // Change these thresholds for various images
    float th1 =((h*3.14)/180);
    float th2 =((l*3.14)/180);

int use_status=0;

    IplImage* dst;
    IplImage* color_dst;
    IplImage* color_dst2;
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* lines = 0;
    int i;
   
    dst = cvCreateImage( cvGetSize(src), 8, 1 );
    color_dst = cvCreateImage( cvGetSize(src), 8, 3 );
    color_dst2 = cvCreateImage( cvGetSize(src), 8, 3 );
   
    cvCanny( src, dst, 50, 200, 3 );
    cvCvtColor( dst, color_dst, CV_GRAY2BGR );
    cvCvtColor( dst, color_dst2, CV_GRAY2BGR );

   lines = cvHoughLines2( dst, storage, CV_HOUGH_STANDARD, 1, CV_PI/180, 70 );
   // lines = cvHoughLines2( dst,storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 100,0,0 );
    // lines = cvHoughLinesP( dst, lines,1, CV_PI/180, 100 );
    for( i = 0; i < MIN(lines->total,100); i++ )
    {
        float* line = (float*)cvGetSeqElem(lines,i);
        float rho = line[0];
        float theta = line[1];
    if(theta<th1 && theta>th2 )
    { 
        CvPoint pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        cvLine( color_dst2, pt1, pt2, CV_RGB(255,0,0), 3, CV_AA, 0 );
    use_status =1;
	chktheta = (theta*180)/3.14;
        //printf("in range theta is %f\n",chktheta);
    }
    else
      {
         chktheta = (theta*180)/3.14;
        //printf(" not in range theta is %f\n",chktheta);
      }

    }

    lines = cvHoughLines2( dst, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 100, 50, 10 );
    for( i = 0; i < lines->total; i++ )
    {
        CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
        cvLine( color_dst, line[0], line[1], CV_RGB(255,0,0), 3, CV_AA, 0 );
    }
   cvCopy(color_dst2,img_out,NULL);
    //cvNamedWindow( "Source", 1 );
    //cvShowImage( "Source", src );

  //  cvNamedWindow( "Hough", 1 );
  //  cvShowImage( "Hough", color_dst2 );

//CvFont font;
 //cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 1, CV_AA);
/*if(use_status==1)      
cvPutText(color_dst2, "In use", cvPoint(50,50), &font, cvScalar(255,0,0));
else
cvPutText(color_dst2, "NOT In use", cvPoint(50,50), &font, cvScalar(255,0,0));
    cvNamedWindow( "Hough2", 1 );
    cvShowImage( "Hough2", color_dst2 );

*/
cvReleaseImage(&img1);
cvReleaseImage(&src);
cvReleaseImage(&dst);
cvReleaseImage(&color_dst);
cvReleaseImage(&color_dst2);
cvReleaseMemStorage(&storage);
return use_status;
}

//------------------------------------------------------------- knife track -----------------------------------------------------------

int knifeTrack(CvPoint pt_plate,int plate_W,int plate_H,IplImage *img_knife,IplImage *img,int l,int h)
{
	int result;
	CvRect rect = cvRect(pt_plate.x-(plate_W/2)+30,pt_plate.y-(plate_H/2)-100,plate_W,plate_H+100); 
	cvSetImageROI(img,rect);  
	IplImage *img5 = cvCreateImage(cvGetSize(img), 8, 3);
	cvCopy(img,img5);
	cvResetImageROI(img);
	IplImage *dst = cvCreateImage(cvGetSize(img5),8,1);
	ycbcr_segment(img5, dst,77,127,133,173);
	cvShowImage("ycbcr",dst);

	//rect = cvRect(pt_plate.x-(plate_W/2)-50,pt_plate.y-(plate_H/2)-50,plate_W+100,plate_H+100);
	/*cvSetImageROI(img,rect);  
	IplImage *img4 = cvCreateImage(cvGetSize(img), 8, 3);
	cvCopy(img,img4);
	cvResetImageROI(img);
	IplImage* img_out = cvCreateImage(cvGetSize(img4),8,3);
	result = houg(img4,img_out,l,h);		
	
	cvShowImage("img_out",img_out);*/
	
	//cvReleaseImage(&img4);
	cvReleaseImage(&img5);
	//cvReleaseImage(&img_out);
	return result;
	
}

