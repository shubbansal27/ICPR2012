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
  
  printf("\nYCbCr segmentation\n");
  char pathDepth[100],pathRGB[100],labellist[100],title[50];
  ConfigurePath(argv[1], pathRGB,pathDepth,labellist,title);					//path-configuration
  printf("%s\n%s\n%s\n",pathRGB,pathDepth,labellist);

  IplImage *img,*depth_img,*img_tempmatch; 

  int height,width,step,channels;
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
  FILE *outFile = fopen(argv[2],"w");	
  
  //lp = fopen(labellist,"r");
  fp=fopen("tmpR.txt","r");
  depth_fp=fopen("tmpD.txt","r");
  system("rm tmpR.txt");
  system("rm tmpD.txt");
  
  int l = 44,h=54; 
  cvNamedWindow(title, CV_WINDOW_AUTOSIZE); 
  cvMoveWindow(title, 50, 50);
  cvCreateTrackbar( "Speed: ", title, &cnt, 50, NULL);						
  cvCreateTrackbar( "WaitKey: ", title, &pos, 50, NULL);
  cvCreateTrackbar( "l: ", title, &l, 180, NULL);
  cvCreateTrackbar( "h: ", title, &h, 180, NULL);	
  
	

  CvPoint pt,rhpt;
  CvPoint pt_katori,pt_stick, pt_bhaguna,   pt_kadai,pt_namak,pt_bottle,pt_egg,pt_ham,pt_knife,pt_plate;  
  IplImage* temp_katori = cvLoadImage("crop_katori.jpg");
  IplImage* temp_stick = cvLoadImage("crop_stick.jpg");
  IplImage* temp_bhaguna = cvLoadImage("crop_bhaguna.jpg");
  IplImage* temp_kadai = cvLoadImage("crop_kadai.jpg");
  IplImage* temp_namak = cvLoadImage("crop_namak.jpg");
  IplImage* temp_bottle = cvLoadImage("crop_bottle.jpg");
  IplImage* temp_egg = cvLoadImage("crop_egg.jpg");
  IplImage* temp_ham = cvLoadImage("crop_ham.jpg");
  IplImage* temp_knife = cvLoadImage("crop_knife.jpg");
  IplImage* temp_plate = cvLoadImage("crop_plate.jpg");	

  int bhaguna_W = temp_bhaguna->width;
  int bhaguna_H = temp_bhaguna->height;
  int kadai_W = temp_kadai->width;
  int kadai_H = temp_kadai->height;
  int bottle_W = temp_bottle->width;
  int bottle_H = temp_bottle->height;
  int plate_W = temp_plate->width;
  int plate_H = temp_plate->height;
  int namak_W = temp_namak->width;
  int namak_H = temp_namak->height;
  int egg_W = temp_egg->width;
  int egg_H = temp_egg->height;					
  int bottl = 0,katori; 	 
  int res,bhag,nn,ham,namak,fcnt = 0,egg,knife;
  CvPoint oldpt;
  int oldx,oldy;		  

  strcpy(depth_image_name,pathDepth);
  fscanf(depth_fp,"%s",depth_ima);
  strcat(depth_image_name,depth_ima); 
  IplImage *firstFrame = cvLoadImage(depth_image_name,CV_LOAD_IMAGE_GRAYSCALE);

  for(i=2;;i++)
  {
    //printf("\n%d",i);	  
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
           centroidOfImageObjects(img_tempmatch,&pt_katori,&pt_stick, &pt_bhaguna, &pt_kadai,&pt_namak,&pt_bottle,&pt_egg,&pt_ham,&pt_knife,&pt_plate);
           //cvShowImage("template_matched",img_tempmatch);
           //cvWaitKey(0);
    }
    if(cnt == 0)  cnt = 1;

    if(i%cnt==0)
    {

        img = cvLoadImage(image_name);
	depth_img = cvLoadImage(depth_image_name); 
        IplImage *depthGray = cvCreateImage(cvGetSize(depth_img),8,1);
	
	bhag = bhagunaTrack(pt_bhaguna,bhaguna_W,bhaguna_H,img,temp_bhaguna);		
	bottl = bottleTrack(pt_bottle,bottle_W,bottle_H,depth_img,temp_bottle);	
	res = kadaiTrack(pt_kadai,kadai_W,kadai_H,img,temp_kadai);
	nn = noneClassify(depth_img,firstFrame);	
	//ham = hamTrack(pt_plate,plate_W,plate_H,temp_ham,img);
	//namak = namakTrack(pt_namak,namak_W,namak_H,img,temp_namak,&fcnt);				//check speed must be 1
	//egg = eggTrack(pt_egg,egg_W,egg_H,img,temp_egg);
	//katori = katoriTrack(&oldpt,&oldx,&oldy,img,temp_katori);	
	//knife = knifeTrack(pt_plate,plate_W,plate_H,temp_knife,img,depth_img,l,h,firstFrame);
	
	
	//printf("%d\n",knife);

	fprintf(outFile,"%d %d %d %d\n",bhag,bottl,res,nn);
	//printf("%d %d %d %d\n",bhag,bottl,res,nn);
	
	//if(knife == 1) printf("in use...\n");
	//else printf("not in use...\n");

	//cvShowImage("img",img);
	//cvShowImage("depth",depth_img);
	//cvShowImage(title,NULL);
	
	//cvWaitKey(pos);	
	cvReleaseImage(&img);
	cvReleaseImage(&depth_img);
	cvReleaseImage(&depthGray);
    }
	
  }
  cvReleaseImage(&temp_katori);
  cvReleaseImage(&temp_stick);
  cvReleaseImage(&temp_bhaguna);
  cvReleaseImage(&temp_kadai);
  cvReleaseImage(&temp_namak);
  cvReleaseImage(&temp_plate);	
  cvReleaseImage(&temp_bottle);
  cvReleaseImage(&temp_egg);
  cvReleaseImage(&temp_ham);
  cvReleaseImage(&temp_knife);
  cvReleaseImage(&firstFrame);
 cvDestroyWindow(title);
  fclose(fp);
  //fclose(lp); 
  fclose(outFile);
return 0;

}//main ends


//------------------------------------------------------------------------------------------------------------------------------------------------
   
