#include "./headers/conf.h"
#include "./headers/hand.h"
#include "./headers/cca.h"
#include <cv.h>
#include <highgui.h>
#include<stdio.h>
#include<string.h>
#include<math.h>


void hand_pose(IplImage * src, float * angle) {


   IplImage* im_bw = cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
   cvThreshold(src, im_bw, 150, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

    CvSeq* lines = 0;
    //printf("Height =%d, Width =%d\n", im_bw->height, im_bw->width);

    CvPoint ini_pt_arm;
    CvPoint final_pt_arm;
    CvPoint ini_pt_wrist;
    CvPoint final_pt_wrist;
    CvPoint ini_pt;
    CvPoint final_pt;

    for(int i=0;i<im_bw->height;i++)
    {
      long sumX = 0;
      long sumY = 0;
      int count = 0;
      for(int j=0; j<im_bw->width; j++)
      {
        if((uchar)(im_bw->imageData[i*(im_bw->widthStep) +j]) == 255)
        {
          count++;
          sumX = sumX + j;
        }
      }
      sumY = i;
      //printf("\n");
      if(count>0)
      {
        sumX = sumX/count;
        CvPoint P;
        P.x = (int)sumX;
        P.y = (int)sumY;
        CvScalar RGB_color;
        CvScalar RGB_color2;
        if(i == (int) (0.70*(im_bw->height)))
        {
          RGB_color.val[0]=0;
          RGB_color.val[1]=255;
          RGB_color.val[2]=0;
          cvCircle(src, P, 1, RGB_color, 5, 8, 0);
          ini_pt_wrist = P;
          final_pt_arm = P;
        }
        else
        {
          RGB_color.val[0]=0;
          RGB_color.val[1]=0;
          RGB_color.val[2]=255;
          cvCircle(src, P, 1, RGB_color, 1, 8, 0);
        }
      
        if(i==0)
        {
          ini_pt = P;
          ini_pt_arm = P;
        }
        else
        {
          final_pt = P;
          final_pt_wrist = P;
        } 
      }

    }

    angle[0] = (atan2((final_pt.y - ini_pt.y),(final_pt.x - ini_pt.x)))*(180/3.14) + 180;
    angle[1] = (atan2((final_pt_arm.y - ini_pt_arm.y),(final_pt_arm.x - ini_pt_arm.x)))*(180/3.14) + 180;
    angle[2] = (atan2((final_pt_wrist.y - ini_pt_wrist.y),(final_pt_wrist.x - ini_pt_wrist.x)))*(180/3.14) + 180;

    /*printf("Hand Angle\n");
    printf("%d, %d \n%d, %d\nAngle= %f \n", final_pt.y, final_pt.x, ini_pt.y, ini_pt.x, angle[0]);
    printf("Hand Angle Arm\n");
    printf("%d, %d \n%d, %d\nAngle= %f \n", final_pt_arm.y, final_pt_arm.x, ini_pt_arm.y, ini_pt_arm.x, angle[1]);
    printf("Hand Angle Wrist\n");
    printf("%d, %d \n%d, %d\nAngle= %f \n", final_pt_wrist.y, final_pt_wrist.x, ini_pt_wrist.y, ini_pt_wrist.x, angle[2]);*/

    //cvNamedWindow( "Source", 1 );
    //cvShowImage( "Source", src );

    //cvNamedWindow( "Binary", 1 );
    //cvShowImage( "Binary", im_bw );

cvReleaseImage(&im_bw);


}




int main(int argc,char *argv[])
{
  float leftAngle[3]={0,0,0};	  
  float rightAngle[3]={0,0,0};
  int blkCnt = 0;
  int blkTh = 4500; 
  int outcome;
  printf("\nYCbCr segmentation\n");
  char pathDepth[100],pathRGB[100],labellist[100],title[50];
  ConfigurePath(argv[1], pathRGB,pathDepth,labellist,title);					//path-configuration
  printf("%s\n%s\n%s\n",pathRGB,pathDepth,labellist);

  IplImage *img,*result,*first_img,*change_img, *dst,*depth_img,*result_img,*img_tempmatch  ,*final_result, *hand1,*hand2 ,*img1 ,*final_result_binary,*imgKadai,* temp_kadai  ;

  int height,width,step,channels;
  uchar *data;
  int i,pos = 0,cnt=1,j,k,z,sum,threshold=30,label;
  CvPoint pt_katori,pt_stick, pt_bhaguna, pt_kadai,pt_namak,pt_bottle,pt_egg,pt_ham,pt_knife,pt_plate;												
  int distance[9];
  char image_name[120];
  char ima[30];
  char depth_image_name[120];
  char depth_ima[60];	

  double t ,ms, tms=0; 
  int    fr = 0;       
  int    line_width=1; 
  double hue_lo=0.0;   
  double hue_hi=50.0;
  double sat_lo=0.23;
  double sat_hi=0.68; 
  CvRect rect1,rect2;
  int hand1_count = 0,hand2_count = 0;
  int lz=77,hz=127,ly=133,hy=173;
  FILE *fp,*lp;
  FILE *depth_fp;
  FILE *fout = fopen(argv[2],"w");
											
  CvFont font;
  cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, CV_AA);
  char action[25];

 // lp = fopen(labellist,"r");
  fp=fopen("tmpR.txt","r");
  depth_fp=fopen("tmpD.txt","r");
 
  system("rm tmpR.txt");
  system("rm tmpD.txt");
  
 int ll1 = 225,ll2=215,ll3=220,rl1=230,rl2=230,rl3=220,lh1=310,lh2=320,lh3=315,rh1=305,rh2=315,rh3=305;
// int ll1 = 245,ll2=250,ll3=235,rl1=250,rl2=255,rl3=235,lh1=310,lh2=320,lh3=315,rh1=305,rh2=310,rh3=295;
 cvNamedWindow(title, CV_WINDOW_AUTOSIZE); 
 cvMoveWindow(title, 50, 50);
 //cvNamedWindow("title1", CV_WINDOW_AUTOSIZE); 
 //cvMoveWindow("title1", 50, 50); 
 cvCreateTrackbar( "Speed: ", title, &cnt, 50, NULL);						
 cvCreateTrackbar( "WaitKey: ", title, &pos, 50, NULL); 
 /*cvCreateTrackbar( "ll1: ", title, &ll1, 360, NULL);
 cvCreateTrackbar( "lh1: ", title, &lh1, 360, NULL);

 cvCreateTrackbar( "ll2: ", title, &ll2, 360, NULL);
 cvCreateTrackbar( "lh2: ", title, &lh2, 360, NULL);

 cvCreateTrackbar( "ll3: ", title, &ll3, 360, NULL);
  cvCreateTrackbar( "lh3: ", title, &lh3, 360, NULL);

 cvCreateTrackbar( "rl1: ", "title1", &rl1, 360, NULL);
 cvCreateTrackbar( "rh1: ", "title1", &rh1, 360, NULL);

 cvCreateTrackbar( "rl2: ", "title1", &rl2, 360, NULL);
 cvCreateTrackbar( "rh2: ", "title1", &rh2, 360, NULL);

 cvCreateTrackbar( "rl3: ", "title1", &rl3, 360, NULL);
 cvCreateTrackbar( "rh3: ", "title1", &rh3, 360, NULL);*/
	
 
 
  strcpy(depth_image_name,pathDepth);
  fscanf(depth_fp,"%s",depth_ima);
  strcat(depth_image_name,depth_ima); 
  first_img=cvLoadImage(depth_image_name,CV_LOAD_IMAGE_GRAYSCALE);	
  CvPoint pt,rhpt;
  CvRect rectl,rectr; 

  temp_kadai = cvLoadImage("crop_kadai.jpg");
  IplImage *temp_turner = cvLoadImage("crop_stick.jpg");	
  int kadai_W = temp_kadai->width;
  int kadai_H = temp_kadai->height;
  CvPoint pt_turner;
  int turnerW,turnerH;
  IplImage *region;
  for(i=2;;i++)
  {
    hand1_count  = 0;
    hand2_count  = 0;
    //printf("\n%d",i);	  
    strcpy(image_name,pathRGB);
    fscanf(fp,"%s",ima);
    if(feof(fp))  break; 
   
    strcat(image_name,ima); 

    strcpy(depth_image_name,pathDepth);
    fscanf(depth_fp,"%s",depth_ima);
    strcat(depth_image_name,depth_ima);	
    
  //  fscanf(lp,"%d",&label);             //For printing labels on graph
    //fscanf(lp,"%d",&label);
	
    if(i==2)
    {
        img_tempmatch = cvLoadImage(image_name);
     	CvRect rec = cvRect(530,180,120,200);
	cvSetImageROI(img_tempmatch,rec);  
	region = cvCreateImage(cvGetSize(img_tempmatch), 8, 3);
        cvCopy(img_tempmatch,region);
	cvResetImageROI(img_tempmatch);
	centroidOfImageObjects(img_tempmatch,&pt_katori,&pt_stick, &pt_bhaguna, &pt_kadai,&pt_namak,&pt_bottle,&pt_egg,&pt_ham,&pt_knife,&pt_plate);
     }
     
    if(cnt == 0) 
    cnt = 1;
    if(i%cnt==0)
    {
     
       // printf("1\n");   
        img = cvLoadImage(image_name);
        depth_img=cvLoadImage(depth_image_name,CV_LOAD_IMAGE_GRAYSCALE);
        change_img = cvCreateImage( cvGetSize( depth_img ), IPL_DEPTH_8U, 1);
        result_img = cvCreateImage( cvGetSize( depth_img ), IPL_DEPTH_8U, 1);
//        printf("2\n");

	CvRect rect = cvRect(pt_kadai.x-(kadai_W/2),pt_kadai.y-(kadai_H/2),kadai_W,kadai_H);
	cvSetImageROI(img,rect);  
	imgKadai = cvCreateImage(cvGetSize(img), 8, 3);
        cvCopy(img,imgKadai);
	cvResetImageROI(img);
        IplImage *kadaiGray = cvCreateImage(cvGetSize(imgKadai),8,1);
	cvCvtColor(imgKadai,kadaiGray,CV_BGR2GRAY);
	
	int blks = 0;
	for(int l =0;l<kadaiGray->imageSize;l++)
	{
		if((uchar)kadaiGray->imageData[l] > 120){ blks++; kadaiGray->imageData[l] = 255; } 
		else kadaiGray->imageData[l] = 0;

	}

	rect = cvRect(pt_kadai.x-(kadai_W/2)+30,pt_kadai.y-(kadai_H/2),kadai_W-20,kadai_H);
	cvSetImageROI(depth_img,rect); 
	//cvShowImage("depth_kadai",depth_img); 
	IplImage *kadaiG = cvCreateImage(cvGetSize(depth_img), 8, 1);
        cvCopy(depth_img,kadaiG);
	cvResetImageROI(depth_img);
        
	int whites = 0;
	for(int l =0;l<kadaiG->imageSize;l++)
	{
		if((uchar)kadaiG->imageData[l] < 10){ whites++; kadaiG->imageData[l] = 255; } 
		else kadaiG->imageData[l] = 0;

	}
        //cvShowImage("kadaiG",kadaiG);
	cvReleaseImage(&kadaiG);

	rect = cvRect(530,180,120,200);
	cvSetImageROI(img,rect);
	IplImage *newR = cvCreateImage(cvGetSize(img),8,3);  
	cvCopy(img,newR);
	cvResetImageROI(img);
	IplImage *image1 = cvCreateImage(cvGetSize(region),8,1);
	IplImage *image2 = cvCreateImage(cvGetSize(newR),8,1);
	cvCvtColor(region,image1,CV_BGR2GRAY);
	cvCvtColor(newR,image2,CV_BGR2GRAY);

	IplImage *diff_image = cvCreateImage(cvGetSize(image1),8,1);
	cvAbsDiff(image1,image2,diff_image);	
	
	int chng = 0;
	for(int l=0 ; l < diff_image->imageSize;l++) chng += (uchar)diff_image->imageData[l];
	chng = (int)chng/diff_image->imageSize ;

	//printf("%d\n",chng);

	//cvShowImage("newR",newR);	
	//cvShowImage("region",region);
        cvReleaseImage(&newR);
        cvReleaseImage(&image1);
	cvReleaseImage(&image2);
	

	//cvShowImage("kadai",kadaiGray);
	//printf("%d\n",blks);
	cvReleaseImage(&kadaiGray);
	cvReleaseImage(&imgKadai);
	//cvReleaseImage(&platbinary);
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
       
        final_result_binary = cvCreateImage(cvGetSize(final_result),8,1);
        cvCvtColor(final_result,final_result_binary,CV_RGB2GRAY);
         //cvShowImage("hand segmented image",final_result_binary);
  //     printf("3\n");
       if(pt.x > 0 && pt.y > 0)
	 hand1_count = 1;
       else
         hand1_count = 0;
    //   printf("4\n");
         
      if(rhpt.x > 0 && rhpt.y > 0 )
          hand2_count = 1;
      else
        hand2_count = 0;
        
     // printf("5\n");
      if(hand1_count == 1 && rectl.width >0 && rectl.height >0)               //////////////For Hand 1///////////////if it exist///////////////////////
     {
      
       cvSetImageROI(final_result_binary,rectl);
       hand1 = cvCreateImage(cvGetSize(final_result_binary),
                                   final_result_binary->depth,
                                   final_result_binary->nChannels);
       cvCopy(final_result_binary,hand1,NULL);
       cvResetImageROI(final_result_binary);
    float angle[3] = {0,0,0};
    hand_pose(hand1,angle);
    /*printf("Hand Angle");
    printf("= %f \n", angle[0]);
    printf("Hand Angle Arm");
    printf("= %f \n", angle[1]);
    printf("Hand Angle Wrist");
    printf("= %f \n", angle[2]);*/

       //cvShowImage("hand1",hand1);
     leftAngle[0] = angle[0];
     leftAngle[1] = angle[1];
     leftAngle[2] = angle[2]; 			

     }  
     /*else
     {
	 leftAngle[0] = 0;
     	 leftAngle[1] = 0;
     	 leftAngle[2] = 0; 	
     }*/		
   //  printf("6\n");
     if(hand2_count == 1 && rectr.width >0 && rectr.height >0) /////////////////////////////////For showing hand 2 if it exist///////////////////////////
     {      
       //  printf("12\n");
        cvSetImageROI(final_result_binary,rectr);
        hand2 = cvCreateImage(cvGetSize(final_result_binary),
                                   final_result_binary->depth,
                                   final_result_binary->nChannels);
         // printf("13\n");
        cvCopy(final_result_binary,hand2,NULL);
       // printf("14\n");
        cvResetImageROI(final_result_binary);
        //printf("15\n");
    float angle[3] = {0,0,0};
    hand_pose(hand2,angle);
    /*printf("Hand Angle");
    printf("= %f \n", angle[0]);
    printf("Hand Angle Arm");
    printf("= %f \n", angle[1]);
    printf("Hand Angle Wrist");
    printf("= %f \n", angle[2]);*/

        //cvShowImage("hand2",hand2); 
     rightAngle[0] = angle[0];
     rightAngle[1] = angle[1];
     rightAngle[2] = angle[2]; 	

     }
     /*else
     {
	rightAngle[0] = 0;
     	rightAngle[1] = 0;
     	rightAngle[2] = 0; 	
     }*/			
     //printf("7\n");
      

	//printf("%d\n",blkCnt);
       //printf("left: %f %f %f\n",leftAngle[0],leftAngle[1],leftAngle[2]);
       //printf("right: %f %f %f\n",rightAngle[0],rightAngle[1],rightAngle[2]);	

	if(leftAngle[0] > ll1 && leftAngle[0] < lh1 && leftAngle[1] > ll2 && leftAngle[1] < lh2 && leftAngle[2] > ll3 && leftAngle[2] < lh3 && rightAngle[0] > rl1 && rightAngle[0] < rh1 && rightAngle[1] > rl2 && rightAngle[1] < rh2 && rightAngle[2] > rl3 && rightAngle[2] < rh3 && blks > blkTh && chng > 30 && whites > 400)
	{
 		//sprintf(action,"turner in use..");
		outcome = 1;
		blkCnt++;
	} 
	else 
	{
		//sprintf(action,"turner not in use..");
		outcome = 0;
	}
	//cvPutText(img, action, cvPoint(300,100), &font, cvScalar(255,0,0));

	if(blkCnt > 50) blkTh = 1500;
	//sprintf(action,"%d",whites);
	//cvPutText(img, action, cvPoint(100,100), &font, cvScalar(255,0,0));
        
	//sprintf(action,"%d",label);
	//labelling_vid(action,img,font);
	//cvShowImage("Main image",img);
	//cvShowImage("depth",depth_img);
        //cvWaitKey(pos);

	fprintf(fout,"%d\n",outcome);

        cvReleaseImage(&img);
        cvReleaseImage(&dst);
        cvReleaseImage(&final_result);	
        cvReleaseImage(&change_img);
        cvReleaseImage(&result_img);
        cvReleaseImage(&depth_img); 
        cvReleaseImage(&final_result_binary);
         //printf("9\n");
         if(hand1_count == 1 && rectl.width >0 && rectl.height >0)
         {
          cvReleaseImage(&hand1);
         }
         //printf("10\n");
        if(hand2_count == 1 && rectr.width >0 && rectr.height >0)
        {
          cvReleaseImage(&hand2);
        }
     }
  }
  //printf("11\n");
  cvReleaseImage(&first_img);	  
  cvReleaseImage(&img_tempmatch);
  cvReleaseImage(&temp_kadai);
  cvReleaseImage(&temp_turner);	
  cvReleaseImage(&region);			
  //cvReleaseImage(&imgPlat); 
  cvDestroyWindow(title);
  fclose(fp);
 // fclose(lp); 
  fclose(depth_fp);
  fclose(fout);	
  cvDestroyWindow("title1");
return 0;

}//main ends


   
