#include "stdafx.h"
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "disp.h"


void colorgraph(IplImage *img,int action ,int coloumn)
{
	CvScalar color = cvScalar(0,0,0);
	 if(action == 0)
		  color = cvScalar(255,0,0);
	  if(action == 1)
		  color = cvScalar(0,255,0);
	  if(action == 2)
		  color = cvScalar(0,0,255);
	  if(action == 3)
		  color = cvScalar(255,255,0);
	  if(action == 4)
		  color = cvScalar(0,255,255);
	  if(action == 5)
		  color = cvScalar(255,0,255);
	  if(action == 6)
		  color = cvScalar(50,125,200);
	  if(action == 7)
		  color = cvScalar(125,200,50);
	  if(action == 8)
		  color = cvScalar(0,0,0);
	  if(action == 9)
		  color = cvScalar(255,255,255);
	  int height = img->height;
	  int width  = img->width;

	  int loc = coloumn % width; 
	  if(loc == 0)
	  { 
		   cvZero(img);
	       cvAddS(img,cvScalar(255,255,255),img,0);
	  }


	  uchar *data = (uchar*)img->imageData;
	  int channels = img->nChannels;
	  int step = img->widthStep;
	  
	  for(int i = 10 ;i <=49 ;i++)
	  {

		  data[i*step + (loc - 1)*channels + 0] = color.val[0];
		  data[i*step + (loc - 1)*channels + 1] = color.val[1];
		  data[i*step + (loc - 1)*channels + 2] = color.val[2];
	  }

}

//---------------------------------------------------------------------------------------------------------------------

int get_action_class(char *str)
{
	if(strcmpi(str,"Seasoning") == 0) return 0;
    else if(strcmpi(str,"Putting Oil") == 0) return 1;
	else if(strcmpi(str,"Grating") == 0) return 2;
	else if(strcmpi(str,"Cutting") == 0) return 3;
	else if(strcmpi(str,"Peeling") == 0) return 4;
	else if(strcmpi(str,"Boiling") == 0) return 5;
	else if(strcmpi(str,"Mixing") == 0) return 6;
	else if(strcmpi(str,"Baking") == 0) return 7;
	else if(strcmpi(str,"None") == 0) return 8;
  
}

int recipe_chklst_init(IplImage *img, IplImage *checkbox, char *filename, int *checklist)
{
	int initial_y = 90;
	int initial_x = 80;
	int x = 15;
	int y = 75;
	int count=0;
	CvFont font;
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, .5, .5, 0, 1, CV_AA);
	cvPutText(img, "RECIPE CHECKLIST", cvPoint(45,20), &font, cvScalar(0,0,0));
	
	FILE *fp;
	fp = fopen(filename,"r");
	if(fp == NULL)
	{
		printf("Unable to read file");
		return 0;
	}
	char str[100];
	fgets(str, 100, fp);
	str[strlen(str)-1]='\0';
	puts(str);
	cvPutText(img, str, cvPoint(60,50), &font, cvScalar(0,0,255));
	while(!feof(fp))
	{
		fgets(str, 100, fp);
		if(feof(fp))
			break;
		str[strlen(str)-1]='\0';
		puts(str);
				
		checklist[count]=get_action_class(str);
				
		cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, .5, .5, 0, 1, CV_AA);
		cvPutText(img, str, cvPoint(initial_x,initial_y), &font, cvScalar(255,0,0));

		// Putting chkboxes
		cvSetImageROI(img, cvRect(x, y, 30, 30));
		cvCopy(checkbox, img, NULL);
		
		cvResetImageROI(img);
		y = y + 50;
		initial_y = initial_y + 50;
		count++;
	}
	return count;
}

void recipe_checklist(IplImage *img, IplImage *checkbox_ok, IplImage  *checkbox, int *count_actions, int *checklist, int count)
{

	int initial_y = 90;
	int initial_x = 80;
	int x = 15;
	int ini_y = 75;
	int y = 75;
	int index =0;
	for(int i=0; i<count; i++ )
	{
		//condition for changing status to finished task
		if(count_actions[checklist[i]]>20)
		{
			//printf("Changing status .. !! \n");
			// Putting chkboxes
			for(int k=0;k<count;k++)
				if(checklist[i]==checklist[k])
				{
					index = i;
					break;
				}

			y = ini_y + index*50;
			cvSetImageROI(img, cvRect(x, y, 30, 30));
			cvCopy(checkbox_ok, img, NULL);
			cvResetImageROI(img);
		}	
	}

}


//--------------------------------------------------------------------------------------------


void priorties_objects(int *arr)
{
	if(*(arr+1) == 1)//saltbox
	{
		*(arr+1) = 0; *(arr+3) = 0; *(arr+4) = 0; *(arr+5) = 0; *(arr+6) = 0; *(arr+7) = 0;
	}
	else if(*(arr+4) == 1)//grater
	{
		*(arr+0) = 0; *(arr+2) = 0; *(arr+3) = 0; *(arr+4) = 0; *(arr+5) = 0; *(arr+6) = 0; *(arr+7) = 0; *(arr+8) = 0;
	}
	else if(*(arr+2) == 1)//chopstick
	{
		*(arr+0) = 0; *(arr+1) = 0; *(arr+2) = 0; *(arr+3) = 0; *(arr+4) = 0; *(arr+5) = 0; *(arr+6) = 0; *(arr+7) = 0;*(arr+8) = 0;
	}
	else if(*(arr+3) == 1)//knife
	{
		*(arr+0) = 0; *(arr+1) = 0; *(arr+2) = 0; *(arr+3) = 0; *(arr+4) = 0; *(arr+5) = 0; *(arr+6) = 0; *(arr+7) = 0;*(arr+8) = 0;
	}
	else if(*(arr+0) == 1)//bowl
	{
		*(arr+0) = 0; *(arr+1) = 0; *(arr+2) = 0; *(arr+3) = 0; *(arr+4) = 0; *(arr+5) = 0; *(arr+6) = 0; *(arr+7) = 0;*(arr+8) = 0;
	}
	else if(*(arr+5) == 1)//pan
	{
		*(arr+0) = 0; *(arr+1) = 0; *(arr+2) = 0; *(arr+3) = 0; *(arr+4) = 0; *(arr+5) = 0; *(arr+6) = 0; *(arr+7) = 0;*(arr+8) = 0;
	}
	else if(*(arr+6) == 1)//frying-pan
	{
		*(arr+0) = 0; *(arr+1) = 0; *(arr+2) = 0; *(arr+3) = 0; *(arr+4) = 0; *(arr+5) = 0; *(arr+6) = 0; *(arr+7) = 0;*(arr+8) = 0;
	}
	else if(*(arr+7) == 1)//bottle
	{
		*(arr+0) = 0; *(arr+1) = 0; *(arr+2) = 0; *(arr+3) = 0; *(arr+4) = 0; *(arr+5) = 0; *(arr+6) = 0; *(arr+7) = 0;*(arr+8) = 0;
	}
	else if(*(arr+8) == 1)//peeling
	{
		*(arr+0) = 0; *(arr+1) = 0; *(arr+2) = 0; *(arr+3) = 0; *(arr+4) = 0; *(arr+5) = 0; *(arr+6) = 0; *(arr+7) = 0;*(arr+8) = 0;
	}

}

//-----------------------------------------------------------------------------------

int classify_action(IplImage *img,int *arr)
{
	CvFont font;
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1, 1, 0, 1, CV_AA);

	for(int i=0;i<img->imageSize;i++)
		img->imageData[i] = 255;

	int matrix[20][9]={ 
		{0,1	,0,	0,	0,	0,	0,	0,	0}, //1 seasoning
		{0	,1	,0	,0	,0	,0	,1	,0,	0}, //2 seasoning
		{1	,1,	0,	0,	0,	0,	0,	0,	0}, //3 seasoning
		{0,	0,	0,	1,	0,	0,	0,	0,	0},//4 cutting
		{0,	0,	1,	0,	0,	0,	0,	0,	0},//5 mixing
		{1,	0,	1,	0,	0,	0,	0,	0,	0},//6 mixing
		{0,	0,	1,	0,	0,	1,	0,	0,	0},//7 mixing
		{0,	0,	1,	0,	0,	0,	1,	0,	0},//8 mixing
		{0,	0,	0,	0,	1,	0,	0,	0,	0},//9 grating
		{1,	0,	0,	0,	1,	0,	0,	0,	0},//10 grating
		{0,	0,	0,	0,	1,	0,	1,	0,	0},//11 grating
		{1,	0,	0,	0,	1,	0,	0,	0,	0},//12 grating
		{1,	0,	0,	0,	0,	0,	0,	0,	0},//13 peeling
		{0,	0,	0,	0,	0,	0,	0,	0,	1},//14 peeling
		{1,	0,	0,	0,	0,	0,	0,	0,	1},//15 peeling
		{0,	0,	0,	0,	0,	0,	0,	1,	0},//16 putting-oil
		{0,	0,	0,	0,	0,	0,	1,	1,	0},//17 putting-oil
		{0,	0,	0,	0,	0,	0,	1,	0,	0},//18 baking
		{0,	0,	0,	0,	0,	1,	0,	0,	0},	//19  boiling
		{0,	0,	0,	0,	0,	0,	0,	0,	0}//20 none
};

	int array[20];

	for(int i=0;i<20;i++)
	{
		array[i] = 0;

		for(int j=0;j<9;j++)
		{
			if(matrix[i][j] == *(arr+j))	array[i]++ ;
		}
	}

	int max=array[0],c = 0;
	for(int i=0;i<20;i++)
	{
		if(array[i] > max)
		{
			max = array[i];
			c = i;
		}
	}

	int x = 25,y=100;
	int rs = 0;

	switch(c)
	{
		case 0: cvPutText(img,"Seasoning", cvPoint(x,y), &font, cvScalar(0,255,0)); rs =0; break;
		case 1: cvPutText(img,"Seasoning", cvPoint(x,y), &font, cvScalar(0,255,0)); rs =0; break;
		case 2: cvPutText(img,"Seasoning", cvPoint(x,y), &font, cvScalar(0,255,0)); rs =0; break;
		case 3: cvPutText(img,"Cutting", cvPoint(x,y), &font, cvScalar(0,255,0)); rs = 3; break;
		case 4: cvPutText(img,"Mixing", cvPoint(x,y), &font, cvScalar(0,255,0));  rs = 6; break;
		case 5: cvPutText(img,"Mixing", cvPoint(x,y), &font, cvScalar(0,255,0));  rs = 6;break;
		case 6: cvPutText(img,"Mixing", cvPoint(x,y), &font, cvScalar(0,255,0));  rs = 6; break;
		case 7: cvPutText(img,"Mixing", cvPoint(x,y), &font, cvScalar(0,255,0));  rs = 6; break;
		case 8: cvPutText(img,"Grating", cvPoint(x,y), &font, cvScalar(0,255,0)); rs = 2; break;
		case 9: cvPutText(img,"Grating", cvPoint(x,y), &font, cvScalar(0,255,0)); rs = 2; break;
		case 10: cvPutText(img,"Grating", cvPoint(x,y), &font, cvScalar(0,255,0)); rs = 2; break;
		case 11: cvPutText(img,"Grating", cvPoint(x,y), &font, cvScalar(0,255,0)); rs = 2; break;
		case 12: cvPutText(img,"Peeling", cvPoint(x,y), &font, cvScalar(0,255,0)); rs = 4; break;
		case 13: cvPutText(img,"Peeling", cvPoint(x,y), &font, cvScalar(0,255,0)); rs = 4; break;
		case 14: cvPutText(img,"Peeling", cvPoint(x,y), &font, cvScalar(0,255,0)); rs = 4; break;
		case 15: cvPutText(img,"Putting-oil", cvPoint(x,y), &font, cvScalar(0,255,0)); rs = 1; break;
		case 16: cvPutText(img,"Putting-oil", cvPoint(x,y), &font, cvScalar(0,255,0)); rs = 1; break;
		case 17: cvPutText(img,"Baking", cvPoint(x,y), &font, cvScalar(0,255,0)); rs = 7; break;
		case 18: cvPutText(img,"Boiling", cvPoint(x,y), &font, cvScalar(0,255,0)); rs = 5; break;
		case 19: cvPutText(img,"None", cvPoint(x,y), &font, cvScalar(0,255,0)); rs = 8; break;
	}

	return rs;
}


//------------------------------------------------------------------------------------------------------

void labelStatus(IplImage *img,int *arr,int size)
{
	for(int i=0;i<img->imageSize;i++)
		img->imageData[i] = 255;

	CvFont font;
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, .5, .5, 0, 1, CV_AA);
	cvPutText(img, "OBJECT USE STATUS", cvPoint(40,20), &font, cvScalar(0,0,0));

	char *str[9] = {"bowl","saltbox","chopstick","knife","grater","Pan","Frying-pan","Bottle","Peeler"}; 
    
	int x = 25;
	int y = 70;

	for(int i=0;i<size;i++,y+=50)
	{
		cvPutText(img, str[i], cvPoint(x,y), &font, cvScalar(255,0,0));
		
		if (*(arr+i) == 1)
			cvPutText(img,"in use", cvPoint(x+110,y), &font, cvScalar(0,255,0));	
		else 
			cvPutText(img,"not in use", cvPoint(x+110,y), &font, cvScalar(0,0,255));
		
	}
	
}
