#include <cv.h>
#include <highgui.h>
#include<stdio.h>
#include<string.h>
#include "conf.h"

void ConfigurePath(char *fname, char *pathRGB,char *pathDepth,char *labelList,char *title)
{
	char str[100] ;					//reading code.conf
	FILE *p = fopen(fname,"r");
	if(p == NULL)
	{
		printf("\nError: opening file\n");
		
	}	
	else	
	{
		fgets(str,100,p);	
		str[strlen(str)-1] = '\0';
		strcpy(title,str); 
		printf("Title = %s\n",title);

		fgets(str,100,p);	
		str[strlen(str)-1] = '\0';
		strcpy(pathRGB,str);
		printf("pathRGB = %s\n",pathRGB);
 
		fgets(str,100,p);	
		str[strlen(str)-1] = '\0';
		strcpy(pathDepth,str);
		printf("pathDepth = %s\n",pathDepth);

		fgets(str,100,p);	
		str[strlen(str)-1] = '\0';		
		strcpy(labelList,str);
		printf("labellist = %s\n",labelList);
		
		strcpy(str,"ls ");					//generate tmpR.txt
		strcat(str,pathRGB);
		strcat(str," > ./tmpR.txt");	
		system(str); 
		

		strcpy(str,"ls ");					//generate tmpD.txt
		strcat(str,pathDepth);
		strcat(str," > ./tmpD.txt");
		system(str);
		
	}
	fclose(p);

}

//--------------------------------------------------------------------------------------------------------------------------------

void labelling_vid(char *lb2,IplImage *img,CvFont font,int x,int y)
{
       
       char action[40];
      
       if (!strcmp(lb2,"-1000"))   strcpy(action,"None");
       else if (!strcmp(lb2,"1000"))   strcpy(action,"None"); 
        else if (!strcmp(lb2,"9"))   strcpy(action,"None"); 	
       else if  (!strcmp(lb2,"1")) strcpy(action,"breaking");
       else if  (!strcmp(lb2,"2")) strcpy(action,"mixing"); 
       else if  (!strcmp(lb2,"3")) strcpy(action,"baking");
       else if  (!strcmp(lb2,"4")) strcpy(action,"turning");
       else if  (!strcmp(lb2,"5")) strcpy(action,"cutting");
       else if  (!strcmp(lb2,"6")) strcpy(action,"boiling");
       else if  (!strcmp(lb2,"7")) strcpy(action,"seasoning");
       else if  (!strcmp(lb2,"8")) strcpy(action,"peeling");
       else  strcpy(action,"Unknown");
		
       cvPutText(img, action, cvPoint(x,y), &font, cvScalar(0,0,0));

}

//-------------------------------------------------------------------------------------------------------------------------------------

void labelRightPanel(char *str,float chk,IplImage *img,int x,int y)
{
       
       char action[40];
       CvFont font;
       cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, .5, .5, 0, 2, CV_AA);

	if(strcmp(str,"bhaguna") == 0) strcpy(action,"Pan: ");
        else if(strcmp(str,"bottle") == 0) strcpy(action,"Bottle: ");  
	else if(strcmp(str,"kadai") == 0) strcpy(action,"Frying Pan: ");
	else if(strcmp(str,"chopstick") == 0) strcpy(action,"Chopstick: ");
	else if(strcmp(str,"katori") == 0) strcpy(action,"Bowl: ");
	else if(strcmp(str,"knife") == 0) strcpy(action,"Knife: ");
	else if(strcmp(str,"turner") == 0) strcpy(action,"Turner: ");
	else if(strcmp(str,"namak") == 0) strcpy(action,"Salt-box: ");            

        cvPutText(img, action, cvPoint(x,y), &font, cvScalar(255,0,0));

	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, .5, .5, 0, 1, CV_AA);

	if (chk == 1)
	{
	   cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, .5, .5, 0, 2, CV_AA);	
	   strcpy(action,"in use");
	   cvPutText(img, action, cvPoint(x+110,y), &font, cvScalar(0,255,0));	
	}
        else 
	{	
		strcpy(action,"not in use");
		cvPutText(img, action, cvPoint(x+110,y), &font, cvScalar(0,0,255));
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------

void hough(IplImage *img)
{
    
        IplImage* gray = cvCreateImage( cvGetSize(img), 8, 1 );
        CvMemStorage* storage = cvCreateMemStorage(0);
        cvCvtColor( img, gray, CV_BGR2GRAY );
        cvSmooth( gray, gray, CV_GAUSSIAN, 9, 9 ); // smooth it, otherwise a lot of false circles may be detected
        CvSeq* circles = cvHoughCircles( gray, storage, CV_HOUGH_GRADIENT, 2, gray->height/4, 200, 100 );
        int i;
        for( i = 0; i < circles->total; i++ )
        {
             float* p = (float*)cvGetSeqElem( circles, i );
             cvCircle( img, cvPoint(cvRound(p[0]),cvRound(p[1])), 3, CV_RGB(0,255,0), -1, 8, 0 );
             cvCircle( img, cvPoint(cvRound(p[0]),cvRound(p[1])), cvRound(p[2]), CV_RGB(255,0,0), 3, 8, 0 );
        }
        cvReleaseImage(&gray);
        cvReleaseMemStorage(&storage); 
    
}


//----------------------------------------------------------------------------------------------------------------------------------------

void algo1(IplImage *img)
{

    IplConvKernel* element = 0;
    const int element_shape = CV_SHAPE_RECT;
    int i,j,k;	
    int height,width,step,channels;
    uchar *data;	

     height    = img->height;
     width     = img->width;
     step      = img->widthStep;
     channels  = img->nChannels;
     data      = (uchar *)img->imageData;

     for(i=0;i<height;i++) for(j=0;j<width;j++) for(k=0;k<channels;k++)
     data[i*step+j*channels+k]=255-data[i*step+j*channels+k];

     for(i=0;i<height;i++) { for(j=0;j<width;j++){ for(k=0;k<channels;k++){
    	if(data[i*step+j*channels+k] > 195) data[i*step+j*channels+k] = 255;
	else data[i*step+j*channels+k]=0;
        }}}

       element = cvCreateStructuringElementEx( 3, 3, 1, 1, element_shape, 0 );
		
       cvErode(img,img,element,2);   		
       cvDilate(img,img,element,1);	

       cvReleaseStructuringElement(&element);

}


//-------------------------------------------------------------------------------------------------------------------------------------------


