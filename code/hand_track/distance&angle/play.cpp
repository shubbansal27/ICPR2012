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
  
  printf("\nYCbCr segmentation\n");
  char pathDepth[100],pathRGB[100],labellist[100],title[50];
  ConfigurePath(argv[1], pathRGB,pathDepth,labellist,title);							//path-configuration
  printf("%s\n%s\n%s\n",pathRGB,pathDepth,labellist);

  IplImage *img,*result,*first_img,*change_img, *dst,*depth_img,*result_img,*img_tempmatch  ,*final_result ; 

  int height,width,step,channels;
  uchar *data;
  int i,pos = 0,cnt=1,j,k,z,sum,threshold=30,label;
  CvPoint pt_katori,pt_stick, pt_bhaguna, pt_kadai,pt_namak,pt_bottle,pt_egg,pt_ham,pt_knife;
  CvPoint centr;
  CvPoint origin;
  origin.x = 0;
  origin.y = 0;	 													
  float angle[9];
  int distance[9];
  char image_name[120];
  char ima[30];
  char depth_image_name[120];
  char depth_ima[60];
  char action[25];	
  CvFont font;
  cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, CV_AA);
  int lz=77,hz=127,ly=133,hy=173;
			
  
  FILE *fp,*lp;
  FILE *depth_fp;
  FILE *angle_fp,*angle_fp_righthand,*centroidAngle;												
  FILE *dist_fp,*dist_fp_righthand;	  

  //lp = fopen(labellist,"r");
  fp=fopen("tmpR.txt","r");
  depth_fp=fopen("tmpD.txt","r");
  //angle_fp = fopen(argv[2],"w");
  //angle_fp_righthand = fopen(argv[3],"w");
  //centroidAngle = fopen(argv[4],"w");	
  //dist_fp = fopen(argv[5],"w");
  //dist_fp_righthand = fopen(argv[6],"w");
  system("rm tmpR.txt");
  system("rm tmpD.txt");
  
 cvNamedWindow(title, CV_WINDOW_AUTOSIZE); 
 cvMoveWindow(title, 50, 50);
 cvCreateTrackbar( "Speed: ", title, &cnt, 50, NULL);						
 cvCreateTrackbar( "WaitKey: ", title, &pos, 50, NULL);
 
  strcpy(depth_image_name,pathDepth);
  fscanf(depth_fp,"%s",depth_ima);
  strcat(depth_image_name,depth_ima); 
  first_img=cvLoadImage(depth_image_name,CV_LOAD_IMAGE_GRAYSCALE);	
  CvPoint pt,rhpt;
   
  for(i=2;;i++)
  {
    strcpy(image_name,pathRGB);
    fscanf(fp,"%s",ima);
    if(feof(fp))  break; 
   
    strcat(image_name,ima); 

    strcpy(depth_image_name,pathDepth);
    fscanf(depth_fp,"%s",depth_ima);
    strcat(depth_image_name,depth_ima);	
    
    //fscanf(lp,"%d",&label);             
    //fscanf(lp,"%d",&label);
	
    if(i==2)
    {
       img_tempmatch = cvLoadImage(image_name);
       centroidOfImageObjects(img_tempmatch,&pt_katori,&pt_stick, &pt_bhaguna, &pt_kadai,&pt_namak,&pt_bottle,&pt_egg,&pt_ham,&pt_knife);
       centr.x = img_tempmatch->width/2;
       centr.y = img_tempmatch->height/2;
    }
     
    if(i%cnt==0)
    {
	img = cvLoadImage(image_name);
        depth_img=cvLoadImage(depth_image_name,CV_LOAD_IMAGE_GRAYSCALE);
        change_img = cvCreateImage( cvGetSize( depth_img ), IPL_DEPTH_8U, 1);
        result_img = cvCreateImage( cvGetSize( depth_img ), IPL_DEPTH_8U, 1);

    
/* ------------------------------------------------------------- Hand segmentation (YCbCr )------------------------------------------------------------*/
   
	//cvShowImage("first2",first_img);
	//cvShowImage("depth2",depth_img);
	for(j=0;j<change_img->imageSize;j++)
	{

		if(abs(first_img->imageData[j] - depth_img->imageData[j]) > 30)
			change_img->imageData[j] = 0;
		else
			change_img->imageData[j] = 255;

	}

	//cvShowImage("chnage2",change_img);									//depth-change detection

	cvErode(change_img,result_img,NULL,3);													
	cvDilate(change_img,result_img,NULL,3);

	cvNot(result_img,result_img);
		
	

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
	float lAngle,rAngle;
	contourTrackBothHand(dst,final_result,&pt,&rhpt,&lAngle,&rAngle);


/* ------------------------------------------------------------------ Angle based Files ------------------------------------------------------------*/
	/*

	

	//centroid to hands angle
											                //printf("%f\n",calcAngleFromCentroid(rhpt,origin,centr)); 
	fprintf(centroidAngle,"%f ",calcAngleFromCentroid(pt,origin,centr)); 
	fprintf(centroidAngle,"%f\n",calcAngleFromCentroid(rhpt,origin,centr)); 
        
       // for Left hand storing
       angle[0] = calcAngleFromCentroid(pt,pt_katori,centr); 								//printf("l-katori %f\n",angle[0]);
       angle[1] = calcAngleFromCentroid(pt,pt_kadai,centr);
       angle[2] = calcAngleFromCentroid(pt,pt_bhaguna,centr);
       angle[3] = calcAngleFromCentroid(pt,pt_stick,centr);
       angle[4] = calcAngleFromCentroid(pt,pt_knife,centr);
       angle[5] = calcAngleFromCentroid(pt,pt_namak,centr);
       angle[6] = calcAngleFromCentroid(pt,pt_bottle,centr);
       angle[7] = calcAngleFromCentroid(pt,pt_egg,centr);
       angle[8] = calcAngleFromCentroid(pt,pt_ham,centr);
       
      for(int h = 0;h<9;h++)
      { 
         fprintf(angle_fp,"%f ",angle[h]); 
      }
	fprintf(angle_fp,"\n");
// For right hand storing
       angle[0] = calcAngleFromCentroid(rhpt,pt_katori,centr); 							//printf("r-katori %f\n",angle[0]);
       angle[1] = calcAngleFromCentroid(rhpt,pt_kadai,centr);
       angle[2] = calcAngleFromCentroid(rhpt,pt_bhaguna,centr);
       angle[3] = calcAngleFromCentroid(rhpt,pt_stick,centr);
       angle[4] = calcAngleFromCentroid(rhpt,pt_knife,centr);
       angle[5] = calcAngleFromCentroid(rhpt,pt_namak,centr);
       angle[6] = calcAngleFromCentroid(rhpt,pt_bottle,centr);
       angle[7] = calcAngleFromCentroid(rhpt,pt_egg,centr);
       angle[8] = calcAngleFromCentroid(rhpt,pt_ham,centr);
       
      for(int h = 0;h<9;h++)
      { 
         fprintf(angle_fp_righthand,"%f ",angle[h]); 
      }
      	
      fprintf(angle_fp_righthand,"\n");  

   */
/* ------------------------------------------------------------------ Distance based Files ------------------------------------------------------------*/
     /*

	step      = depth_img->widthStep;
        data      = (uchar *)depth_img->imageData;
	int z_lefthand = data[(pt.x) *step + pt.y + 1];
        int z_righthand = data[(rhpt.x)*step + rhpt.y + 1];


	
  	// for Left hand storing
       distance[0] = distancePoints(pt,pt_katori);
       distance[1] = distancePoints(pt,pt_kadai);
       distance[2] = distancePoints(pt,pt_bhaguna);
       distance[3] = distancePoints(pt,pt_stick);
       distance[4] = distancePoints(pt,pt_knife);
       distance[5] = distancePoints(pt,pt_namak);
       distance[6] = distancePoints(pt,pt_bottle);
       distance[7] = distancePoints(pt,pt_egg);
       distance[8] = distancePoints(pt,pt_ham);
       
      for(int h = 0;h<9;h++)
      { 
         fprintf(dist_fp,"%d ",distance[h]); 
      }
	fprintf(dist_fp,"%f ",lAngle);
	fprintf(dist_fp,"%d ",z_lefthand);
        fprintf(dist_fp,"\n");

// For right hand storing
       distance[0] = distancePoints(rhpt,pt_katori);
       distance[1] = distancePoints(rhpt,pt_kadai);
       distance[2] = distancePoints(rhpt,pt_bhaguna);
       distance[3] = distancePoints(rhpt,pt_stick);
       distance[4] = distancePoints(rhpt,pt_knife);
       distance[5] = distancePoints(rhpt,pt_namak);
       distance[6] = distancePoints(rhpt,pt_bottle);
       distance[7] = distancePoints(rhpt,pt_egg);
       distance[8] = distancePoints(rhpt,pt_ham);
       
      for(int h = 0;h<9;h++)
      { 
         fprintf(dist_fp_righthand,"%d ",distance[h]); 
      }
      fprintf(dist_fp_righthand,"%f ",rAngle);
      fprintf(dist_fp_righthand,"%d ",z_righthand);	
      fprintf(dist_fp_righthand,"\n");  	

    */
/* ------------------------------------------------------------------ Display Image ------------------------------------------------------------*/
/*	
	if(pt.x > 0 || pt.y >0) 
	{
		sprintf(action,"left");
		cvPutText(img, action, pt , &font, cvScalar(255,0,0));
	}
  	if(rhpt.x > 0 || rhpt.y >0) 
	{
		sprintf(action,"right");
		cvPutText(img, action, rhpt , &font, cvScalar(255,0,0));
	}		
*/
	cvShowImage(title,img);
	//cvShowImage("first2",first_img);	
	cvShowImage("result",final_result);
	//cvShowImage("Depth",depth_img);
	cvWaitKey(pos);



/* ------------------------------------------------------------------ Releasing Memory ------------------------------------------------------------*/
        cvReleaseImage(&img);
        cvReleaseImage(&dst);
        cvReleaseImage(&final_result);	
        cvReleaseImage(&change_img);
        cvReleaseImage(&result_img);
        cvReleaseImage(&depth_img);
        
	}
	
  }
  cvReleaseImage(&first_img);	  
  cvReleaseImage(&img_tempmatch);
  cvDestroyWindow(title);
  fclose(fp);
  //fclose(lp); 
  fclose(depth_fp);
  //fclose(dist_fp);
  //fclose(dist_fp_righthand);
  //fclose(angle_fp);
  //fclose(angle_fp_righthand);	
  //fclose(centroidAngle);
return 0;

}//main ends


   
