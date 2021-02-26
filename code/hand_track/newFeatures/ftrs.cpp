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
//cvReleaseImage(&src);


}




int main(int argc,char *argv[])
{
  float leftAngle[3]={0,0,0};	  
  float rightAngle[3]={0,0,0};
  float vl=0,vr=0,dirl=0,dirr=0;	
  CvPoint lxy,rxy,old_lxy,old_rxy;
  lxy.x = 0;
  lxy.y = 0;
  rxy.x = 0;
  rxy.y = 0; 
  
  old_lxy.x = 0;
  old_lxy.y = 0;
  old_rxy.x = 0;
  old_rxy.y = 0; 

  printf("\nYCbCr segmentation\n");
  char pathDepth[100],pathRGB[100],labellist[100],title[50];
  ConfigurePath(argv[1], pathRGB,pathDepth,labellist,title);					//path-configuration
  printf("%s\n%s\n%s\n",pathRGB,pathDepth,labellist);

  IplImage *img,*result,*first_img,*change_img, *dst,*depth_img,*result_img,*img_tempmatch  ,*final_result, *hand1,*hand2 ,*img1 ,*final_result_binary  ;

  int height,width,step,channels;
  uchar *data;
  int i,pos = 30,cnt=1,j,k,z,sum,threshold=30,label;
  //CvPoint pt_katori,pt_stick, pt_bhaguna, pt_kadai,pt_namak,pt_bottle,pt_egg,pt_ham,pt_knife,pt_plate;												
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

  //lp = fopen(labellist,"r");
  fp=fopen("tmpR.txt","r");
  depth_fp=fopen("tmpD.txt","r");
 
  system("rm tmpR.txt");
  system("rm tmpD.txt");
  
 cvNamedWindow(title, CV_WINDOW_AUTOSIZE); 
 cvMoveWindow(title, 50, 50);
 cvCreateTrackbar( "Speed: ", title, &cnt, 50, NULL);						
 cvCreateTrackbar( "WaitKey: ", title, &pos, 50, NULL); 
 /*cvCreateTrackbar( "lz: ", title, &th1, 255, NULL);
 cvCreateTrackbar( "hz: ", title, &th2, 255, NULL);
 cvCreateTrackbar( "ly: ", title, &th3, 255, NULL);
 cvCreateTrackbar( "hy: ", title, &th4, 255, NULL); 
 */
  strcpy(depth_image_name,pathDepth);
  fscanf(depth_fp,"%s",depth_ima);
  strcat(depth_image_name,depth_ima); 
  first_img=cvLoadImage(depth_image_name,CV_LOAD_IMAGE_GRAYSCALE);	
  CvPoint pt,rhpt;
  CvRect rectl,rectr; 
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
    
    //fscanf(lp,"%d",&label);             //For printing labels on graph
    //fscanf(lp,"%d",&label);
	
    /*if(i==2)
    {
       img_tempmatch = cvLoadImage(image_name);
       //centroidOfImageObjects(img_tempmatch,&pt_katori,&pt_stick, &pt_bhaguna, &pt_kadai,&pt_namak,&pt_bottle,&pt_egg,&pt_ham,&pt_knife,&pt_plate);
      //cvShowImage("template_matched",img_tempmatch);
      //cvWaitKey(0);
    }*/
     
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

	int no_white =0;
    	int t;
    	for(t=0;t<dst->imageSize;t++)
    	{
        	if (t>=5*(dst->imageSize)/12)                               
          		dst->imageData[t] =  (uchar)(dst->imageData[t] & result_img->imageData[t]) ;
       
        	if (t<=2*(dst->imageSize)/12)
          		dst->imageData[t] =  (uchar)(0) ;

        	if((uchar)(dst->imageData[t])>0)
            		no_white ++;
    	}
    	//printf("w = %d \n",no_white);
    	int no_threshold = 3000;
    	if(no_white>no_threshold)
    	{
    		ycbcr_segment(img, dst,lz+5,hz+5,ly+5,hy+5);
   		//printf("change\n");
    		cvNot(dst,dst);                                               
    		cvErode(dst,dst,NULL,3);
    		cvDilate(dst,dst,NULL,3);
    		int t;
    		for(t=0;t<dst->imageSize;t++)
    		{
        		if (t>=5*(dst->imageSize)/12)                               
          		dst->imageData[t] =  (uchar)(dst->imageData[t] & result_img->imageData[t]) ;
       
        		if (t<=2*(dst->imageSize)/12)
          		dst->imageData[t] =  (uchar)(0) ;

    		}
    	}
		
	
        cvNot(dst,dst);
	final_result = cvCreateImage( cvGetSize(dst), 8, 3 );


         contourTrackBothHand2(dst,final_result,&pt,&rhpt,&rectl,&rectr);
         

        final_result_binary = cvCreateImage(cvGetSize(final_result),8,1);
        cvCvtColor(final_result,final_result_binary,CV_RGB2GRAY);
         //cvShowImage("hand segmented image",final_result_binary);
  //     printf("3\n");
       if(pt.x > 0 && pt.y > 0)
       {
	 hand1_count = 1;
	 lxy.x = pt.x;
	 lxy.y = pt.y;
       }
       else
         hand1_count = 0;
    //   printf("4\n");
         
      if(rhpt.x > 0 && rhpt.y > 0 )
      {	
          hand2_count = 1;
	  rxy.x = rhpt.x;
          rxy.y = rhpt.y;
      }
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
    leftAngle[0] = angle[0];
    leftAngle[1] = angle[1];
    leftAngle[2] = angle[2]; 

       //cvShowImage("hand1",hand1);
     }  
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
    rightAngle[0] = angle[0];
     rightAngle[1] = angle[1];
     rightAngle[2] = angle[2];
     }
     //printf("7\n");

	
	int step      = depth_img->widthStep;
        data      = (uchar *)depth_img->imageData;
	int z_l = data[(lxy.x) *step + lxy.y + 1];
        int z_r = data[(rxy.x)*step + rxy.y + 1];


	if(hand1_count == 1  && rectl.width >0 && rectl.height >0)
	{
		float vx = (float)(lxy.x-old_lxy.x);
		float vy = (float)(lxy.y-old_lxy.y);
	
		vl = sqrt(vx * vx + vy *vy) ;
 		
		if(vy ==0 && vx == 0)
			dirl = 0;
		else
			dirl =  (atan2 ((float)vy,(float)vx))*(180/3.14);
	}

	if(hand2_count == 1 && rectr.width >0 && rectr.height >0)
	{
		float vx = (float)(rxy.x-old_rxy.x);
		float vy = (float)(rxy.y-old_rxy.y);
	
		vr = sqrt(vx * vx + vy *vy) ;
 		
		if(vy ==0 && vx == 0)
			dirr = 0;
		else
			dirr =  (atan2 ((float)vy,(float)vx))*(180/3.14);
	}


	old_lxy.x = lxy.x;
	old_lxy.y = lxy.y;
	old_rxy.x = rxy.x;
	old_rxy.y = rxy.y;

	//printf("left: %f %f %f\n",leftAngle[0],leftAngle[1],leftAngle[2]);
        //printf("right: %f %f %f\n",rightAngle[0],rightAngle[1],rightAngle[2]);	
	 //printf("left XY: %d %d\n",lxy.x,lxy.y);
         //printf("right XY: %d %d\n",rxy.x,rxy.y);
	 //printf("left z: %d\n",z_l);
         //printf("right z: %d\n",z_r);
	 //printf("left : v = %f  %f\n",vl,dirl);
 	 //printf("right : v = %f  %f\n",vr,dirr);
	
	 fprintf(fout,"%f %f %f ",leftAngle[0],leftAngle[1],leftAngle[2]); //left angle
         fprintf(fout,"%f %f %f ",rightAngle[0],rightAngle[1],rightAngle[2]);	//right angle
	 fprintf(fout,"%d %d ",lxy.x,lxy.y); //left x ,y
         fprintf(fout,"%d %d ",rxy.x,rxy.y); // right x,y
	 fprintf(fout,"%d ",z_l); //left z
         fprintf(fout,"%d ",z_r);  //right z
	 fprintf(fout,"%f %f ",vl,dirl);  //left velocity magnitude and angle
 	 fprintf(fout,"%f %f\n",vr,dirr); //right velocity magnitude and angle

     
      //cvShowImage("Main image",img);
      //cvWaitKey(pos);
//printf("8\n");
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
  cvDestroyWindow(title);
  fclose(fp);
  //fclose(lp); 
  //fclose(fout);	
  fclose(depth_fp);
  cvDestroyWindow(title);
return 0;

}//main ends


   
