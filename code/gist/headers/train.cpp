#include "conf.h"
#include "algo.h"
#include "train.h"
#include <cv.h>
#include <highgui.h>
#include<stdio.h>
#include<string.h>

using namespace std;
using namespace cv;														//pca

int train(char *inFile,int scale,int comp,int mod,char *outPath)
{
  
  char pathDepth[100],pathRGB[100],labellist[100],title[50];
  ConfigurePath(inFile, pathRGB,pathDepth,labellist,title);									//path-configuration
  printf("%s\n%s\n%s\n",pathRGB,pathDepth,labellist);

  IplImage* img,*res,*imgN; 
  int counter = 0,cntt=0,set=0; 	 
  int i,pos = 28,j,ftrs,lpt;												
  char image_name[120];
  char ima[30],buffer[40];
  FILE *fp;											
  //FILE *lp_in;
  //FILE *lp_out;
  FILE *tm;			

  fp = fopen("tmpR.txt","r");
  set = 0;
  while(fscanf(fp,"%s",ima) == 1) set++;		
  fclose(fp);	

  fp=fopen("tmpR.txt","r");
  //lp_in=fopen(labellist,"r");
  //lp_out=fopen("class.txt","w");  	
  system("rm tmpR.txt");

  imgN = cvCreateImage(cvSize(352,240),8,3); 
  ftrs = 34 * scale * scale;
  res = cvCreateImage(cvSize(ftrs,(int)(set/mod)),8,1);							//544 feature + 400 images
  uchar t[ftrs];

   for(i=1;i<=set;i++)
   {
    strcpy(image_name,pathRGB);
    fscanf(fp,"%s",ima);
    //fscanf(lp_in,"%d",&lpt);
    //fscanf(lp_in,"%d",&lpt);
    		
    if(feof(fp)) { printf("end"); break;} 
	
    strcat(image_name,ima); 
    
    if(!(i % mod))
    {
	cntt++;
        img=cvLoadImage(image_name);	
	cvResize(img,imgN,CV_INTER_LINEAR); 

	cvSetImageROI(imgN, cvRect(5,90,310,235));
	IplImage *img2 = cvCreateImage(cvGetSize(imgN),imgN->depth,imgN->nChannels);
	cvCopy(imgN, img2, NULL);
	cvResetImageROI(imgN);
      
	//fprintf(lp_out,"%d\n",lpt);
 	gist_vector(img2,&t[0],4);						//gist feature extraction

	for(j=0;j<ftrs;j++)
	{
		res->imageData[counter]= (uchar)(*(t+j));
		counter++;	
	}

     }

    }//loop	
	
	Mat mat(res);
	Mat compressed;	
	compressPCA(mat,comp,compressed);
	IplImage pca = compressed;
	//fclose(lp_in);
	//fclose(lp_out);
	
	buildTrainData(&pca,NULL,outPath);					//train-DATA
	//system("rm class.txt");

	printf("\nTotal frames = %d",cntt);
	cvReleaseImage(&res);
	cvReleaseImage(&img);
	cvReleaseImage(&imgN);
	fclose(fp);
  	return 0;

}//main ends

