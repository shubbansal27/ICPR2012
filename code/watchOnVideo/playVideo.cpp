#include "./headers/conf.h"
#include <cv.h>
#include <highgui.h>
#include<stdio.h>
#include<string.h>

int main(int argc,char *argv[])
{
  
  printf("\nVideo playing now.....\n");
  char pathDepth[100],pathRGB[100],labellist[100],title[50];
  ConfigurePath(argv[1], pathRGB,pathDepth,labellist,title);							//path-configuration
  printf("%s\n%s\n%s\n",pathRGB,pathDepth,labellist);

  IplImage *img,*depth_img; 
  int i,pos = 30,cnt=1,label,labelOut;
  char image_name[120];
  char ima[30];
  char depth_image_name[120];
  char depth_ima[60];
  char action[25];	
  CvFont font;
  cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, CV_AA);
  FILE *fp,*lp;
  FILE *depth_fp;
  FILE *outLb;	
  
  outLb = fopen(argv[2],"r");
  lp = fopen(labellist,"r");
  fp=fopen("tmpR.txt","r");
  depth_fp=fopen("tmpD.txt","r");
  
  system("rm tmpR.txt");
  system("rm tmpD.txt");
  
 cvNamedWindow(title, CV_WINDOW_AUTOSIZE); 
 cvMoveWindow(title, 50, 50);
 cvCreateTrackbar( "Speed: ", title, &cnt, 50, NULL);						
 cvCreateTrackbar( "WaitKey: ", title, &pos, 50, NULL);
 
  for(i=2;;i++)
  {
    strcpy(image_name,pathRGB);
    fscanf(fp,"%s",ima);
    if(feof(fp))  break; 
   
    strcat(image_name,ima); 

    strcpy(depth_image_name,pathDepth);
    fscanf(depth_fp,"%s",depth_ima);
    strcat(depth_image_name,depth_ima);	
    
    fscanf(lp,"%d",&label);             
    fscanf(lp,"%d",&label);

    fscanf(outLb,"%d",&labelOut);
	
    if(i%cnt==0)
    {
	img = cvLoadImage(image_name);
        //depth_img=cvLoadImage(depth_image_name,CV_LOAD_IMAGE_GRAYSCALE);
        
	sprintf(action,"%d",label);
	labelling_vid(action,img,font,100);
	sprintf(action,"%d",labelOut);
	labelling_vid(action,img,font,500);
   	cvShowImage(title,img);
	//cvShowImage("Depth",depth_img);
	cvWaitKey(pos);

	cvReleaseImage(&img);
        //cvReleaseImage(&depth_img);
        
    }
	
  }
  cvDestroyWindow(title);
  fclose(fp);
  fclose(lp); 
  fclose(depth_fp);
  fclose(outLb);
return 0;

}//main ends


   
