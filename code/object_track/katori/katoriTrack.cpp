#include "./headers/conf.h"
#include "./headers/hand.h"
#include "./headers/cca.h"
#include <cv.h>
#include <highgui.h>
#include<stdio.h>
#include<string.h>
#include<math.h>

int distancePoints(CvPoint ,CvPoint );
//CvPoint track(IplImage *,IplImage *,CvPoint,int);

//--------------------------------------------------------------------------- MAIN -----------------------------------------------------------------------

int main(int argc,char *argv[])
{
  
  char pathDepth[100],pathRGB[100],labellist[100],title[50];
  ConfigurePath(argv[1], pathRGB,pathDepth,labellist,title);					
  printf("%s\n%s\n%s\n",pathRGB,pathDepth,labellist);

  IplImage *img,*img_tempmatch ;
  IplImage *threshimage , *img2rgb,*img2,*img_depth;
  int height,width,step,channels;
  uchar *data;
  int i,pos = 25,cnt=1,j,k,z,sum,threshold=30,label;
  CvPoint pt_katori,pt_stick, pt_bhaguna,            pt_kadai,pt_namak,pt_bottle,pt_egg,pt_ham,pt_knife;												
  int distance[9];
  int height_katori,width_katori;
  char image_name[120];
  char ima[30];
  char depth_image_name[120];
  char depth_ima[60];	
  char action[10];
  FILE *fp,*lp;
  FILE *depth_fp;												
  CvFont font;
  cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 1, CV_AA);
  FILE *katori_fp;
  //lp = fopen(labellist,"r");
  katori_fp= fopen(argv[2],"w");
  fp=fopen("tmpR.txt","r");
  depth_fp=fopen("tmpD.txt","r");

  system("rm tmpR.txt");
  system("rm tmpD.txt");
  i=2; 
  int lz = 100,hz = 120,ly = 145,hy = 155 ,lx = 108,hx = 165;
  /*cvNamedWindow(title, CV_WINDOW_AUTOSIZE); 
  cvMoveWindow(title, 50, 50);
  cvCreateTrackbar( "Speed: ", title, &cnt, 50, NULL);						
  cvCreateTrackbar( "WaitKey: ", title, &pos, 50, NULL); 
  cvCreateTrackbar( "threshold: ", title, &threshold, 400, NULL);
  cvCreateTrackbar( "lower cb: ", title, &lz, 200, NULL);
  cvCreateTrackbar( "higher cb: ",title,&hz,200,NULL);
  cvCreateTrackbar( "lower cr ",title,&ly,200,NULL);
  cvCreateTrackbar( "higher cr ",title,&hy,200,NULL);
  cvCreateTrackbar( "lower y" , title,&lx,200,NULL);
  cvCreateTrackbar( "higher y",title,&hx,200,NULL);*/
  
  strcpy(depth_image_name,pathDepth);
  fscanf(depth_fp,"%s",depth_ima);
  strcat(depth_image_name,depth_ima); 
  CvPoint pt,oldpt;
  CvRect rect;
  IplImage* temp_katori = cvLoadImage("crop_katori.jpg");
  IplImage* temp_stick = cvLoadImage("crop_stick.jpg");
  IplImage* temp_bhaguna = cvLoadImage("crop_bhaguna.jpg");
  IplImage* temp_kadai = cvLoadImage("crop_kadai.jpg");
  IplImage* temp_namak = cvLoadImage("crop_namak.jpg");
  IplImage* temp_bottle = cvLoadImage("crop_bottle.jpg");
  IplImage* temp_egg = cvLoadImage("crop_egg.jpg");
  IplImage* temp_ham = cvLoadImage("crop_ham.jpg");
  IplImage* temp_knife = cvLoadImage("crop_knife.jpg");
  int mask = 0;
  int old_x,old_y; 
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
 if(cnt ==0)
  cnt = 1;
 if(i % cnt ==0)
 {
    img = cvLoadImage(image_name);
    //IplImage* img = cvLoadImage("crop_katori.jpg");
    threshimage = cvCreateImage(cvGetSize(img),8,1);
    //ycbcr_segment(img,threshimage,25,35,117,125,140,155);   
    ycbcr_segment(img,threshimage,0,50,91,145,146,170);
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
 //   if(x > ((temp_katori->width + 6)/2) && (y > (temp_katori->height + 48) / 2 ))	
   IplImage *img3 = cvCloneImage(img);
   cvRectangle(img3,pt1,pt2,cvScalar(0,255,255),1,8,0);
 
    if(pt1.x <0 || pt1.y <0 || pt1.x > img->width || pt1.y > img->height)
    {
       //pt1.x = 0;
       //pt1.y = 0;
         pt1 = oldpt;
    }
    else
    {
       oldpt = pt1;
     }
    CvRect rect = cvRect(pt1.x,pt1.y,temp_katori->width+8,temp_katori->height + 90);
    cvSetImageROI(img,rect);
    img2rgb = cvCreateImage(cvGetSize(img),8,3);
    img2 = cvCreateImage(cvGetSize(img),8,1);
    cvCopy(img,img2rgb);
    cvCvtColor(img2rgb,img2,CV_RGB2GRAY);
    cvResetImageROI(img);
    //ycbcr_segment(img2rgb,img2,100,165,77,127,133,173);
    ycbcr_segment(img2rgb,img2,lx,hx,lz,hz,ly,hy);
    //cvShowImage("egg segmentation rgb image",img2rgb);
   // cvShowImage("template",img2rgb);
   // ycbcr_seg(img2rgb,img2,77,127,133,173);
   // cvThreshold(img2,img2,0,255,CV_THRESH_OTSU);
    //cvShowImage("threshhold image",img2);	
    int whites = 0; 
    for(int l=0;l<img2->imageSize;l++)
    {
	if((uchar)img2->imageData[l] == 255)
		whites++;		
		
    }
	int katori=1 , f_katori ;
//         printf("%d\n",whites);
         if(whites > 230)
	{
	f_katori = katori;			
	}
  //        printf("1\n");
         else
	{
	f_katori = katori - 1 ;
//	printf("%d\n",f_katori);
	}
          //printf("%d\n",f_katori);
	

      fprintf(katori_fp,"%d\n ",f_katori);

   // img_depth = cvLoadImage(depth_image_name);
  //  int height_depth    = img_depth->height;
  //  int width_depth     = img_depth->width;
  //  int step_depth      = img_depth->widthStep;
  //  int channels_depth  = img_depth->nChannels;
 //   uchar* data_depth      = (uchar *)img_depth->imageData;
     
     if(x < 0 || y<0)
     {
	x = old_x;
	y = old_y;	
     }		
     else
     {
	old_x = x;
	old_y = y;
     }	
     
     		
    //if(whites < 2800) printf("in use....\n");
   //else printf("not in use....\n");
    		
    //sprintf(action,"%d",label);	
    //labelling_vid(action,img,font);
   
    //cvShowImage("title",img3);
    //cvShowImage("thresh",threshimage);
    
     //cvWaitKey(pos);
  
    cvReleaseImage(&img);
  
   cvReleaseImage(&threshimage);

    cvReleaseImage(&img2);
    cvReleaseImage(&img3);
    cvReleaseImage(&img2rgb);
  }
  }

 cvReleaseImage(&temp_katori);
 cvReleaseImage(&temp_stick);
 cvReleaseImage(&temp_bhaguna);
 cvReleaseImage(&temp_kadai);
 cvReleaseImage(&temp_namak);
 cvReleaseImage(&temp_bottle);
 cvReleaseImage(&temp_ham);
 cvReleaseImage(&temp_egg);
 cvReleaseImage(&temp_knife);

 
  //fclose(lp); 
  fclose(depth_fp);
  fclose(katori_fp);
return 0;

}//main 


//--------------------------------------------------------------------------------------------------------------------------------------------------------



