#include "./headers/conf.h"
#include "./headers/hand.h"
#include "./headers/cca.h"
#include <cv.h>
#include <highgui.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
int distancePoints(CvPoint ,CvPoint );
int main(int argc,char *argv[])
{
  
  char pathDepth[100],pathRGB[100],labellist[100],title[50];
  ConfigurePath(argv[1], pathRGB,pathDepth,labellist,title);					//path-configuration
  printf("%s\n%s\n%s\n",pathRGB,pathDepth,labellist);

  IplImage *img,*result,*first_img,*change_img, *dst,*depth_img,*result_img,*img_tempmatch; 
  int height,width,step,channels;
  uchar *data;
  int i,pos = 0,cnt=1,j,k,z,sum,threshold=30,label;
  CvPoint pt_katori,pt_stick, pt_bhaguna,   pt_kadai,pt_namak,pt_bottle,pt_egg,pt_ham,pt_knife;												
  int distance[9];
  char image_name[120];
  char ima[30];
  char depth_image_name[120];
  char depth_ima[60];	
  
  FILE *fp,*lp;
  FILE *depth_fp;
  FILE *plot_fp,*dist_fp;												
  

  lp = fopen(labellist,"r");
  fp=fopen("tmpR.txt","r");
  depth_fp=fopen("tmpD.txt","r");
  plot_fp = fopen("graph4.txt","w");
  dist_fp = fopen("graph_distance4.txt","w");
  system("rm tmpR.txt");
  system("rm tmpD.txt");
  i=2; 
  cvNamedWindow(title, CV_WINDOW_AUTOSIZE); 
  cvMoveWindow(title, 50, 50);
  cvCreateTrackbar( "Speed: ", title, &cnt, 50, NULL);						
  cvCreateTrackbar( "WaitKey: ", title, &pos, 50, NULL); 
  cvCreateTrackbar( "threshold: ", title, &threshold, 400, NULL);
  //cvCreateTrackbar( "Frame_Number ", title, &i, 7000, NULL);

  strcpy(depth_image_name,pathDepth);
  fscanf(depth_fp,"%s",depth_ima);
  strcat(depth_image_name,depth_ima); 
  first_img=cvLoadImage(depth_image_name,CV_LOAD_IMAGE_GRAYSCALE);	
  CvPoint pt;
   
  
  for(i=2;;i++)
  {
    strcpy(image_name,pathRGB);
    fscanf(fp,"%s",ima);
    if(feof(fp))  break; 
   
    strcat(image_name,ima); 

    strcpy(depth_image_name,pathDepth);
    fscanf(depth_fp,"%s",depth_ima);
    strcat(depth_image_name,depth_ima);	
    
    fscanf(lp,"%d",&label);             //For printing labels on graph
    fscanf(lp,"%d",&label);
    if(i==2)
    {
       img_tempmatch = cvLoadImage(image_name);
       centroidOfImageObjects(img_tempmatch,&pt_katori,&pt_stick, &pt_bhaguna, &pt_kadai,&pt_namak,&pt_bottle,&pt_egg,&pt_ham,&pt_knife);
      // cvShowImage("template_matched",img_tempmatch);
     //  cvWaitKey(0);
    }
     

    if(i%cnt==0)
    {

        img = cvLoadImage(image_name);
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

	cvErode(result_img,result_img,NULL,3);
	cvDilate(change_img,result_img,NULL,3);
//	cvErode(result_img,result_img,NULL,3);

	
	cvNot(result_img,result_img);										//depth-change detection

	
        dst = cvCreateImage( cvGetSize( img ), IPL_DEPTH_8U, 1);
        hand_segment(img,dst);			
	cvNot(dst,dst);												//HSV segmentation
	cvErode(dst,dst,NULL,3);
	cvDilate(dst,dst,NULL,3);
//	cvShowImage("Change_image",result_img);
//	cvShowImage("hand_segmented",dst);
	int t;
	//for(t=0;t<(dst->imageSize)/2;t++)										//intersection of both outputs
	//	dst->imageData[t] =  (uchar)(dst->imageData[t] & result_img->imageData[t]) ;
	for(t=5*(dst->imageSize)/12;t<dst->imageSize;t++)										//intersection of both outputs
		dst->imageData[t] =  (uchar)(dst->imageData[t] & result_img->imageData[t]) ;
		

  //      cvShowImage("Multiplied_Video",dst);


//	cvSaveImage("Seg.jpg",dst);
        cvNot(dst,dst);
	IplImage *final_result = cvCreateImage( cvGetSize(dst), 8, 3 );
        contourTrack(dst,final_result,&pt);
          height    = depth_img->height;
          width     = depth_img->width;
          step      = depth_img->widthStep;
          channels  = depth_img->nChannels;
          data      = (uchar *)depth_img->imageData;

         z = data[(pt.x) *step + pt.y + 1];
        if(label == -1000)
        label = 0;
        fprintf(plot_fp,"%d %d %d %d \n",pt.x,pt.y ,z ,label);

        // now storing distance between objects and hand in file

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
      fprintf(dist_fp,"\n");
//	cvShowImage(title,img);
//	cvShowImage("contour",final_result);

        cvReleaseImage(&img);
        cvReleaseImage(&dst);
        cvReleaseImage(&final_result);	
        cvReleaseImage(&change_img);
        cvReleaseImage(&result_img);
        cvReleaseImage(&depth_img);
        
//        cvWaitKey(pos);											 			
      
     }
	
  }

  cvReleaseImage(&first_img);	  
  cvReleaseImage(&img_tempmatch);
  cvDestroyWindow(title);
  fclose(fp);
  fclose(lp); 
  fclose(depth_fp);
  fclose(plot_fp);
  fclose(dist_fp);
return 0;

}//main ends

int distancePoints(CvPoint pt, CvPoint pt1)
{ 
  int distance;
 return distance = (int) sqrt((double)(pt.x - pt1.x)*(pt.x - pt1.x) + (pt.y - pt1.y)*(pt.y - pt1.y));
}
   
