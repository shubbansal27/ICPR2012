#include<highgui.h>
#include<cv.h>
#include "plot.h"

const CvScalar BLACK = CV_RGB(0,0,0);
const CvScalar WHITE = CV_RGB(255,255,255);
const CvScalar GREY = CV_RGB(150,150,150);
int countGraph = 0;	
CvScalar customGraphColor;
int usingCustomGraphColor = 0;

CvScalar getGraphColor(void)
{
	if (usingCustomGraphColor) {
		usingCustomGraphColor = 0;
		return customGraphColor;
	}

	countGraph++;
	switch (countGraph) {
	case 1:	return CV_RGB(60,60,255);	// light-blue
	case 2:	return CV_RGB(60,255,60);	// light-green
	case 3:	return CV_RGB(255,60,40);	// light-red
	case 4:	return CV_RGB(0,210,210);	// blue-green
	case 5:	return CV_RGB(180,210,0);	// red-green
	case 6:	return CV_RGB(210,0,180);	// red-blue
	case 7:	return CV_RGB(0,0,185);		// dark-blue
	case 8:	return CV_RGB(0,185,0);		// dark-green
	case 9:	return CV_RGB(185,0,0);		// dark-red
	default:
		countGraph = 0;	// start rotating through colors again.
		return CV_RGB(200,200,200);	// grey
	}
}

IplImage* drawIntGraph(const int *arraySrc, int nArrayLength, int width, int height,IplImage *imageDst)
{
	int minV=0,maxV=0 ;
	int w = width;
	int h = height;
	int b = 10;		// border around graph within the image
	if (w <= 20)
		w = nArrayLength + b*2;	// width of the image
	if (h <= 20)
		h = 220;

	int s = h - b*2;// size of graph height
	float xscale = 1.0;
	if (nArrayLength > 1)
		xscale = (w - b*2) / (float)(nArrayLength-1);	// horizontal scale
	
	IplImage *imageGraph;

	if(imageDst == NULL)
	{
		imageGraph = cvCreateImage(cvSize(w,h), 8, 3);
		cvSet(imageGraph, WHITE);
 	}
	else
		imageGraph =  imageDst;

	CvScalar colorGraph = getGraphColor();	// use a different color each time.

	if (minV == 0 && maxV == 0) {
		for (int i=0; i<nArrayLength; i++) {
			int v = arraySrc[i];
			if (v < minV)
				minV = v;
			if (v > maxV)
				maxV = v;
		}
	}
	int diffV = maxV - minV;
	if (diffV == 0)
		diffV = 1;	
	float fscale = (float)s / (float)diffV;

	// Draw the horizontal & vertical axis
	int y0 = cvRound(minV*fscale);
	cvLine(imageGraph, cvPoint(b,h-(b-y0)), cvPoint(w-b, h-(b-y0)), BLACK);
	cvLine(imageGraph, cvPoint(b,h-(b)), cvPoint(b, h-(b+s)), BLACK);

	// Draw the values
	CvPoint ptPrev = cvPoint(b,h-(b-y0));	// Start the lines at the 1st point.
	for (int i=0; i<nArrayLength; i++) {
		int y = cvRound((arraySrc[i] - minV) * fscale);	// Get the values at a bigger scale
		int x = cvRound(i * xscale);
		CvPoint ptNew = cvPoint(b+x, h-(b+y));
		cvLine(imageGraph, ptPrev, ptNew, colorGraph, 1, CV_AA);	// Draw a line from the previous point to the new point
		ptPrev = ptNew;
	}

	return imageGraph;
}

