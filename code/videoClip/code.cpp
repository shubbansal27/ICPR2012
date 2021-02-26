#include "./headers/conf.h"
#include "./headers/hand.h"
#include "./headers/cca.h"
#include <cv.h>
#include <highgui.h>
#include<stdio.h>
#include<string.h>
#include<math.h>

int main(int argc,char *argv[])
{
  
  printf("\nVideo playing now.....\n");
  char pathDepth[100],pathRGB[100],labellist[100],title[50];
  ConfigurePath(argv[1], pathRGB,pathDepth,labellist,title);							//path-configuration
  printf("%s\n%s\n%s\n",pathRGB,pathDepth,labellist);

  IplImage *img,*depth_img,*first_img,*change_img,*result_img,*dst,*final_result,*depthR; 
  int i,pos = 30,cnt=1,label,labelOut,j;
  int lz=77,hz=127,ly=133,hy=173,threshold=30;
  char image_name[120];
  char ima[30];
  char depth_image_name[120];
  char depth_ima[60];
  char action[50];	
  CvFont font;
  cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.5, 1.5, 0, 2, CV_AA);
  FILE *fp;
  FILE *depth_fp;
  FILE *outLb;	

  cvNamedWindow("controller", CV_WINDOW_AUTOSIZE); 
  cvMoveWindow("controller", 50, 50);
  cvCreateTrackbar( "Speed: ", "controller", &cnt, 50, NULL);						
  cvCreateTrackbar( "WaitKey: ","controller", &pos, 50, NULL);	
  
  outLb = fopen(argv[2],"r");
  fp=fopen("tmpR.txt","r");
  depth_fp=fopen("tmpD.txt","r");
  
  system("rm tmpR.txt");
  system("rm tmpD.txt");

  float dat[9];
  FILE *fp1,*fp2,*fp3,*fp4,*fp5,*fp6;
  fp1 = fopen(argv[3],"r"); //out 4 :bhaguna,bottle,kadai,none
  fp2 = fopen(argv[4],"r"); //stick 1
  fp3 = fopen(argv[5],"r"); //katori 1
  fp4 = fopen(argv[6],"r"); //knife 1
  fp5 = fopen(argv[7],"r"); //turner 1
  fp6 = fopen(argv[8],"r"); //namak 1
  
  IplImage *topPanel,*rightPanel,*reimg,*redepth1,*redepth;
  //topPanel = cvCreateImage(cvSize(240,100),8,3);
  //cvNot(topPanel,topPanel);		
  redepth = cvCreateImage(cvSize(240,150),8,3);	
  reimg = cvCreateImage(cvSize(490,410),8,3);
  redepth1 = cvCreateImage(cvSize(237,150),8,3);	
  rightPanel = cvCreateImage(cvSize(240,410),8,3);
  cvNot(rightPanel,rightPanel);	
  IplImage *container = cvCreateImage(cvSize(734,449),8,3);

  strcpy(depth_image_name,pathDepth);
  fscanf(depth_fp,"%s",depth_ima);
  strcat(depth_image_name,depth_ima); 
  first_img=cvLoadImage(depth_image_name,CV_LOAD_IMAGE_GRAYSCALE);

  CvPoint pt,rhpt;
  CvRect rectl,rectr;
  int hand1_count,hand2_count;

  //CvVideoWriter* writer  = cvCreateVideoWriter(argv[9], CV_FOURCC('M','J','P','G'), 28, cvGetSize(container), 1); 
  char key;
  int fc = 1;	
  for(i=2;;i++)
  {
    strcpy(image_name,pathRGB);
    fscanf(fp,"%s",ima);
    if(feof(fp))  break; 
   
    strcat(image_name,ima); 

    strcpy(depth_image_name,pathDepth);
    fscanf(depth_fp,"%s",depth_ima);
    strcat(depth_image_name,depth_ima);	
    
    fscanf(outLb,"%d",&labelOut);
	
    if(i%cnt==0)
    {
	img = cvLoadImage(image_name);
        depthR=cvLoadImage(depth_image_name);
        depth_img=cvLoadImage(depth_image_name,CV_LOAD_IMAGE_GRAYSCALE);
        change_img = cvCreateImage( cvGetSize( depth_img ), IPL_DEPTH_8U, 1);
        result_img = cvCreateImage( cvGetSize( depth_img ), IPL_DEPTH_8U, 1);	

	for(j=0;j<change_img->imageSize;j++)
	{

		if(abs(first_img->imageData[j] - depth_img->imageData[j]) > threshold)
			change_img->imageData[j] = 0;
		else
			change_img->imageData[j] = 255;

	}
        
	cvErode(change_img,result_img,NULL,3);													
	cvDilate(change_img,result_img,NULL,3);

	cvNot(result_img,result_img);										//depth-change detection

	dst = cvCreateImage( cvGetSize( img ), IPL_DEPTH_8U, 1);
        ycbcr_segment(img, dst,lz,hz,ly,hy);
	
	cvNot(dst,dst);												
	cvErode(dst,dst,NULL,3);
	cvDilate(dst,dst,NULL,3);

	int t;
	for(t=5*(dst->imageSize)/12;t<dst->imageSize;t++)								//intersection of both outputs
		dst->imageData[t] =  (uchar)(dst->imageData[t] & result_img->imageData[t]) ;
		
	
        cvNot(dst,dst);
	final_result = cvCreateImage( cvGetSize(dst), 8, 3 );


        contourTrackBothHand2(dst,final_result,&pt,&rhpt,&rectl,&rectr);
	
	if(pt.x > 0 && pt.y > 0) hand1_count = 1;
        else hand1_count = 0;
       
        if(rhpt.x > 0 && rhpt.y > 0 ) hand2_count = 1;
        else hand2_count = 0;

        if(hand1_count == 1 && rectl.width >0 && rectl.height >0)   //for hand-1         
        {
      		cvRectangle(img,cvPoint(rectl.x,rectl.y),cvPoint(rectl.x + rectl.width ,rectl.y + rectl.height),cvScalar(0,127,255),2,8,0);
    	}  
     
     	if(hand2_count == 1 && rectr.width >0 && rectr.height >0)  //for hand-2
     	{      
        	cvRectangle(img,cvPoint(rectr.x,rectr.y),cvPoint(rectr.x + rectr.width ,rectr.y + rectr.height),cvScalar(127,0,255),2,8,0);
        }

	cvResize(img,reimg,CV_INTER_LINEAR);
	cvResize(depthR,redepth,CV_INTER_LINEAR);
//-------------------------------------------------------------------  Extracting Object INFO   -------------------------------------------------- 

	fscanf(fp1,"%f %f %f %f",&dat[0],&dat[1],&dat[2],&dat[3]); //bhaguna,bottle,kadai,none
	fscanf(fp2,"%f",&dat[4]);  //stick 
	fscanf(fp3,"%f",&dat[5]);  //katori
	fscanf(fp4,"%f",&dat[6]);  //knife 
	fscanf(fp5,"%f",&dat[7]);  //turner
	fscanf(fp6,"%f",&dat[8]);  //namak

	//printf("%f %f %f %f %f %f %f %f %f\n",dat[0],dat[1],dat[2],dat[3],dat[4],dat[5],dat[6],dat[7],dat[8]);

	labelRightPanel("bhaguna",dat[0],rightPanel,25,87);
	labelRightPanel("bottle",dat[1],rightPanel,25,109);
	labelRightPanel("kadai",dat[2],rightPanel,25,131);
	labelRightPanel("chopstick",dat[4],rightPanel,25,153);
	labelRightPanel("katori",dat[5],rightPanel,25,175);
	labelRightPanel("knife",dat[6],rightPanel,25,197);
	labelRightPanel("turner",dat[7],rightPanel,25,219);
	labelRightPanel("namak",dat[8],rightPanel,25,241);
//--------------------------------------------------------------------- Container & Panels -------------------------------------------------------
	sprintf(action,"%d",labelOut);
	labelling_vid(action,rightPanel,font,51,45);

	CvRect rect; 
	/*rect = cvRect(400,0,topPanel->width,topPanel->height);		//topPanel
	cvSetImageROI(container,rect);
	cvCopy(topPanel,container);
	cvResetImageROI(container);*/

	cvLine(reimg,cvPoint(0,205),cvPoint(490,205),cvScalar(0,0,0),1,8,0);
	cvLine(reimg,cvPoint(245,0),cvPoint(245,410),cvScalar(0,0,0),1,8,0);
	cvLine(reimg,cvPoint(0,307),cvPoint(490,307),cvScalar(0,0,0),1,8,0);

	rect = cvRect(0,20,reimg->width,reimg->height);			//frame
	cvSetImageROI(container,rect);
	cvCopy(reimg,container);
	cvResetImageROI(container);

	CvRect rect1 = cvRect(3,0,redepth->width-3,redepth->height);
	cvSetImageROI(redepth,rect1);
	cvCopy(redepth,redepth1);
	cvResetImageROI(redepth);

	rect1 = cvRect(0,0,redepth1->width,1);
	cvSetImageROI(redepth1,rect1);
	cvZero(redepth1);
	cvResetImageROI(redepth1);

	rect = cvRect(1,260,redepth1->width,redepth1->height);
	cvSetImageROI(rightPanel,rect);
	cvCopy(redepth1,rightPanel);
	cvResetImageROI(rightPanel);	

	rect = cvRect(493,20,rightPanel->width,rightPanel->height);		//rightPanel
	cvSetImageROI(container,rect);
	cvCopy(rightPanel,container);
	cvResetImageROI(container);

         cvShowImage("container",container);
	 cvShowImage("controller",NULL);
	//cvWriteFrame(writer, container);	

	//for(int l=0;l<topPanel->imageSize;l++) topPanel->imageData[l] = 255;
	for(int l=0;l<rightPanel->imageSize;l++) rightPanel->imageData[l] = 255;	

//---------------------------------------------------------------------------------------------------------------------------------------------------

	key = cvWaitKey(pos);

	if(key == 's')
	{
		sprintf(action,"%s/%d.jpg",argv[9],fc);
		printf("%s\n",action);
		cvSaveImage(action,container);
		printf("frame saved !!\n");
		fc++;
	}
	cvReleaseImage(&img);
	cvReleaseImage(&depth_img);
	cvReleaseImage(&depthR);
        cvReleaseImage(&change_img);
	cvReleaseImage(&result_img);
	cvReleaseImage(&dst);
	cvReleaseImage(&final_result);
    }
	
  }
  //cvReleaseVideoWriter(&writer);
  //cvReleaseImage(&topPanel);
  cvReleaseImage(&rightPanel);
  cvReleaseImage(&container);	
  cvReleaseImage(&reimg);
  cvReleaseImage(&redepth);
  cvReleaseImage(&redepth1);		
  cvReleaseImage(&first_img);		
  fclose(fp);
  fclose(fp1);
  fclose(fp2);
  fclose(fp3);
  fclose(fp4);
  fclose(fp5);
  fclose(fp6);
  fclose(depth_fp);
  fclose(outLb);
return 0;

}//main ends


   
