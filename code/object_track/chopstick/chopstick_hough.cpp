#include "./headers/conf.h"
#include "./headers/hand.h"
#include "./headers/cca.h"
#include <cv.h>
#include <highgui.h>
#include<stdio.h>
#include<string.h>
#include<math.h>

int houg(IplImage *img,int ll , int hl ,int lc,int hc)
{
    /* sets the Region of Interest
       Note that the rectangle area has to be __INSIDE__ the image */
IplImage *img1;
img1 = cvCreateImage(cvGetSize(img), img->depth, 1);
cvCvtColor(img, img1, CV_RGB2GRAY);
    cvSetImageROI(img1, cvRect(150, 80, 550, 260));
     
    /* create destination image
       Note that cvGetSize will return the width and the height of ROI */
    IplImage *src = cvCreateImage(cvGetSize(img1),
                                   img1->depth,
                                   img1->nChannels);
     
    /* copy subimage */
    cvCopy(img1, src, NULL);
     
    /* always reset the Region of Interest */
    cvResetImageROI(img1);

	// Change these thresholds for various images
	float th2 =((hl*3.14)/180);
	float th1 =((ll*3.14)/180);

    int use_status=0;

    IplImage* dst;
    IplImage* color_dst;
    IplImage* color_dst2;
    IplImage* color_probline;
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* lines = 0;
    int i;
    
    dst = cvCreateImage( cvGetSize(src), 8, 1 );
    color_dst = cvCreateImage( cvGetSize(src), 8, 3 );
    color_dst2 = cvCreateImage( cvGetSize(src), 8, 3 );
    color_probline = cvCreateImage(cvGetSize(src),8,3);    

    cvCanny( src, dst, lc,hc, 3 );
    cvCvtColor( dst, color_dst, CV_GRAY2BGR );
    cvCvtColor( dst, color_dst2, CV_GRAY2BGR );
    cvCvtColor(dst,color_probline,CV_GRAY2BGR);
    
 //       for( i = 0; i < lines->total; i++ )
  //  {
   //     CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
    //    cvLine( color_probline, line[0], line[1], CV_RGB(255,0,0), 3, CV_AA, 0 );
   // }
   // cvShowImage("lines of probability ",color_probline);
   // printf("1\n");
   // lines = cvHoughLines2(dst,storage,CV_HOUGH_PROBABILISTIC,1,CV_PI/180,100,50,10);	
   //  printf("2\n");
    lines = cvHoughLines2( dst, storage, CV_HOUGH_STANDARD, 1, CV_PI/180, 100, 0, 0 );  
    for( i = 0; i < MIN(lines->total,100); i++ )
    {
        float* line = (float*)cvGetSeqElem(lines,i);
        float rho = line[0];
        float theta = line[1];
	if(theta<th2 && theta>th1 )
	{
        CvPoint pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        cvLine( color_dst2, pt1, pt2, CV_RGB(255,0,0), 3, CV_AA, 0 );
	use_status =1;
	}
    }

  //  printf("3\n");
//   lines = cvHoughLines2( dst, storage, CV_HOUGH_STANDARD, 1, CV_PI/180, 100, 0, 0 );  
   // for( i = 0; i < lines->total; i++ )
 //   {
 //       CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
//        cvLine( color_dst, line[0], line[1], CV_RGB(255,0,0), 3, CV_AA, 0 );
 //   }
 /*   printf("4\n");
    cvNamedWindow( "Source", 1 );
    cvShowImage( "Source", src );

    cvNamedWindow( "Hough", 1 );
    cvShowImage( "Hough", color_dst );

 CvFont font;
 cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 1, CV_AA);
if(use_status==1)       
cvPutText(color_dst2, "In use", cvPoint(50,50), &font, cvScalar(255,0,0));
else
cvPutText(color_dst2, "NOT In use", cvPoint(50,50), &font, cvScalar(255,0,0));
    cvNamedWindow( "Hough2", 1 );
    cvShowImage( "Hough2", color_dst2 );*/

cvReleaseImage(&img1);
cvReleaseImage(&src);
cvReleaseImage(&dst);
cvReleaseImage(&color_dst);
cvReleaseImage(&color_dst2);
cvReleaseImage(&color_probline);
cvReleaseMemStorage(&storage);
return use_status;
}
//--------------------------------------------------------------------------- MAIN -----------------------------------------------------------------------

int main(int argc,char *argv[])
{
  
  char pathDepth[100],pathRGB[100],labellist[100],title[50];
  ConfigurePath(argv[1], pathRGB,pathDepth,labellist,title);					
  printf("%s\n%s\n%s\n",pathRGB,pathDepth,labellist);
  int hl = 50,ll = 10;
  IplImage *img;
 
  int i,pos = 25,cnt=1,j,k,z,sum,threshold=30,label;												
  int distance[9];
  int outcome ;
  int height_katori,width_katori;
  char image_name[120];
  char ima[30];
  char depth_image_name[120];
  char depth_ima[60];	
  char action[10];
  FILE *fp,*lp;
  FILE *depth_fp;
  FILE *fout = fopen(argv[2],"w");												
  CvFont font;
  cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 1, CV_AA);

 // lp = fopen(labellist,"r");
  fp=fopen("tmpR.txt","r");
  depth_fp=fopen("tmpD.txt","r");

  system("rm tmpR.txt");
  system("rm tmpD.txt");
  i=2; 
  int lz = 77,hz = 127,ly = 133,hy = 173 ,lx = 110,hx = 165,lc=50,hc=200;
  cvNamedWindow(title, CV_WINDOW_AUTOSIZE); 
  cvMoveWindow(title, 50, 50);
  cvCreateTrackbar( "Speed: ", title, &cnt, 50, NULL);						
  cvCreateTrackbar( "WaitKey: ", title, &pos, 50, NULL); 
  cvCreateTrackbar( "threshold: ", title, &threshold, 400, NULL);
  cvCreateTrackbar( "lower limit hough: ", title, &ll, 100, NULL);
  cvCreateTrackbar( "higher limit hough: ",title,&hl,100,NULL);
  cvCreateTrackbar( "lower limit canny: ", title, &lc, 300, NULL);
  cvCreateTrackbar( "higher limit canny : ",title,&hc,300,NULL);
  
  strcpy(depth_image_name,pathDepth);
  fscanf(depth_fp,"%s",depth_ima);
  strcat(depth_image_name,depth_ima); 
  for(i=2;;i++)
  {
    strcpy(image_name,pathRGB);
    fscanf(fp,"%s",ima);
    if(feof(fp))  break; 
   
    strcat(image_name,ima); 

    strcpy(depth_image_name,pathDepth);
    fscanf(depth_fp,"%s",depth_ima);
    strcat(depth_image_name,depth_ima);	
    
 //   fscanf(lp,"%d",&label);             
   // fscanf(lp,"%d",&label);
 if(cnt ==0)
  cnt = 1;
 if(i % cnt ==0)
 {
     img = cvLoadImage(image_name);
    // img_depth = cvLoadImage(depth_image_name);
     int status = houg(img,ll,hl,lc,hc);
    if (status==1)
		outcome = 1;
else 
		outcome = 0;

    fprintf(fout,"%d\n",outcome);
    //cvWaitKey(pos);
  
    cvReleaseImage(&img);
  //  cvReleaseImage(&img_depth);
  }
  }


  fclose(fp);
//  fclose(lp); 
  fclose(depth_fp);
 
return 0;

}//main 


//--------------------------------------------------------------------------------------------------------------------------------------------------------



