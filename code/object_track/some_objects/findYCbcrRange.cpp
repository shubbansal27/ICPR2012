#include "./headers/conf.h"
#include "./headers/hand.h"
#include "./headers/cca.h"
#include "./headers/object.h"
#include <cv.h>
#include <highgui.h>
#include<stdio.h>
#include<string.h>
#include<math.h>


//--------------------------------------------------------------------  MAIN  --------------------------------------------------------------------------


int main(int argc,char *argv[])
{
  
  /*printf("\nYCbCr segmentation\n");
  char pathDepth[100],pathRGB[100],labellist[100],title[50];
  ConfigurePath(argv[1], pathRGB,pathDepth,labellist,title);					//path-configuration
  printf("%s\n%s\n%s\n",pathRGB,pathDepth,labellist);
*/
  IplImage *img,*depth_img,*img_tempmatch; 

  /*int height,width,step,channels;
  uchar *data;
  int i,pos = 0,cnt=1,j,k,z,sum,threshold=30,label;
  char image_name[120];
  char ima[30];
  char depth_image_name[120];
  char depth_ima[60];	
  CvFont font;
  cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, CV_AA);
  char action[25];
  
  FILE *fp,*lp;
  FILE *depth_fp;
  //FILE *outFile = fopen(argv[2],"w");	
  
  lp = fopen(labellist,"r");
  fp=fopen("tmpR.txt","r");
  depth_fp=fopen("tmpD.txt","r");
  system("rm tmpR.txt");
  system("rm tmpD.txt");*/
  int lz=100,hz=130,ly=140,hy=160,lx =1,hx=255;
  cvNamedWindow("title", CV_WINDOW_AUTOSIZE); 
  cvMoveWindow("title", 50, 50);
  cvCreateTrackbar( "lx: ", "title", &lx, 255, NULL);
  cvCreateTrackbar( "hx: ", "title", &hx, 255, NULL);	
  cvCreateTrackbar( "lz: ", "title", &lz, 255, NULL);						
  cvCreateTrackbar( "hz: ", "title", &hz, 255, NULL);
  cvCreateTrackbar( "ly: ", "title", &ly, 255, NULL);
  cvCreateTrackbar( "hy: ", "title", &hy, 255, NULL);	
/*
  CvPoint pt,rhpt;
  CvPoint pt_katori,pt_stick, pt_bhaguna,   pt_kadai,pt_namak,pt_bottle,pt_egg,pt_ham,pt_knife;  
  IplImage* temp_katori = cvLoadImage("crop_katori.jpg");
  IplImage* temp_stick = cvLoadImage("crop_stick.jpg");
  IplImage* temp_bhaguna = cvLoadImage("crop_bhaguna.jpg");
  IplImage* temp_kadai = cvLoadImage("crop_kadai.jpg");
  IplImage* temp_namak = cvLoadImage("crop_namak.jpg");
  IplImage* temp_bottle = cvLoadImage("crop_bottle.jpg");
  IplImage* temp_egg = cvLoadImage("crop_egg.jpg");
  IplImage* temp_ham = cvLoadImage("crop_ham.jpg");
  IplImage* temp_knife = cvLoadImage("crop_knife.jpg");

  int bhaguna_W = temp_bhaguna->width;
  int bhaguna_H = temp_bhaguna->height;
  int kadai_W = temp_kadai->width;
  int kadai_H = temp_kadai->height;
  int bottle_W = temp_bottle->width;
  int bottle_H = temp_bottle->height;
  int ham_W = temp_ham->width;
  int ham_H = temp_ham->height;			
  int bottl = 0; 	 
  int res,bhag,nn,ham;
  
  strcpy(depth_image_name,pathDepth);
  fscanf(depth_fp,"%s",depth_ima);
  strcat(depth_image_name,depth_ima); 
  IplImage *firstFrame = cvLoadImage(depth_image_name,CV_LOAD_IMAGE_GRAYSCALE);
*/ 
   int i;	
  for(i=2;;i++)
  {
    //printf("\n%d",i);	  
    /*strcpy(image_name,pathRGB);
    fscanf(fp,"%s",ima);
    if(feof(fp))  break; 
    strcat(image_name,ima); 
    
    strcpy(depth_image_name,pathDepth);
    fscanf(depth_fp,"%s",depth_ima);	
    strcat(depth_image_name,depth_ima);

    fscanf(lp,"%d",&label);
    fscanf(lp,"%d",&label);
	
    if(i==2)
    {
 	   img_tempmatch = cvLoadImage(image_name);
           centroidOfImageObjects(img_tempmatch,&pt_katori,&pt_stick, &pt_bhaguna, &pt_kadai,&pt_namak,&pt_bottle,&pt_egg,&pt_ham,&pt_knife);
           //cvShowImage("template_matched",img_tempmatch);
           //cvWaitKey(0);
    }*/

    if(1)
    {

        img = cvLoadImage(argv[1]);
	//depth_img = cvLoadImage(depth_image_name); 
        
	IplImage *thresh = cvCreateImage(cvGetSize(img),8,1);
	ycbcr_segm(img,thresh,lz,hz,ly,hy,lx,hx);
	//ycbcr_segment(img,thresh,100,130,140,160);
	//cvShowImage("ham",temp_ham);	
	cvShowImage("th",thresh);

	
 	cvShowImage("title",img);
	//cvShowImage("depth",depth_img);
	//cvShowImage(title,NULL);
	cvWaitKey(0);	
	cvReleaseImage(&img);
	cvReleaseImage(&thresh);
    }
     
	
  }
  /*cvReleaseImage(&temp_katori);
  cvReleaseImage(&temp_stick);
  cvReleaseImage(&temp_bhaguna);
  cvReleaseImage(&temp_kadai);
  cvReleaseImage(&temp_namak);
  cvReleaseImage(&temp_bottle);
  cvReleaseImage(&temp_egg);
  cvReleaseImage(&temp_ham);
  cvReleaseImage(&temp_knife);
  cvReleaseImage(&firstFrame);		  
  cvDestroyWindow(title);
  fclose(fp);
  fclose(lp);*/ 
  //fclose(outFile);
return 0;

}//main ends


//------------------------------------------------------------------------------------------------------------------------------------------------
   
