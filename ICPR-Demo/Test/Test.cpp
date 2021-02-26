#include "stdafx.h"
#include <Windows.h>
#include <NuiApi.h>
#include<opencv/cv.h>
#include<opencv/highgui.h>
#include<stdio.h>
#include "hand.h"
#include "algos.h"
#include "disp.h"

int main( )
{
	/*------------------------------------------ NUI - variables ----------------------------------------------------------*/
	
	HRESULT hr;
	const NUI_IMAGE_FRAME* imageFrame,* imageFrame2;
	INuiFrameTexture * pTexture,* pTexture2;
	NUI_LOCKED_RECT LockedRect,LockedRect2;
	BYTE * pBuffer; 
	USHORT * pBuff;
	int CHANNEL = 3;
	BYTE buf[640*480*3];
	
	//NUI initialize
	DWORD nuiFlags = NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_DEPTH;
	
	hr = NuiInitialize(nuiFlags);
	if (FAILED(hr))
	{
		printf("nui_initialize failed \n");
		return hr;
	}
	else printf("nui initialized ! \n");

	//create image stream
	HANDLE pColorStreamHandle = INVALID_HANDLE_VALUE;
	HANDLE pDepthStreamHandle = INVALID_HANDLE_VALUE;

	HANDLE hNextColorFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	HANDLE hNextDepthFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	hr = NuiImageStreamOpen( 
		NUI_IMAGE_TYPE_COLOR, 
		NUI_IMAGE_RESOLUTION_640x480,
		0,
		2,
		hNextColorFrameEvent,
		&pColorStreamHandle );
	if( FAILED(hr) )
	{
		printf("color video stream failed to open ! \n");
	}
	else printf("color video stream opened ! \n");

	hr = NuiImageStreamOpen( 
		NUI_IMAGE_TYPE_DEPTH, 
		NUI_IMAGE_RESOLUTION_640x480, 
		0,
		2,
		hNextDepthFrameEvent,
		&pDepthStreamHandle );

	if( FAILED(hr) )
	{
		printf("Depth video stream failed to open ! \n");
	}
	else printf("Depth video stream opened ! \n"); 


	/*-------------------------------------------   Code - variables ---------------------------------------------------------*/
	
	int lx=0,hx=100,lz=0,hz=100,ly=0,hy = 100;
	//lz = 15;hz = 81;ly = 123;hy = 200;lx = 77;hx = 242;

	IplImage* color = cvCreateImageHeader(cvSize(640,480),IPL_DEPTH_8U,4);
	IplImage* depth = cvCreateImageHeader(cvSize(640,480),IPL_DEPTH_8U,3);
	IplImage* depthS = cvCreateImage(cvSize(200,200),IPL_DEPTH_8U,3);

	IplImage* R = cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);
	IplImage* G = cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);
	IplImage* B = cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);
	IplImage* D = cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);
	IplImage* RGB = cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);
	IplImage *RGB_clone = cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);

	IplImage *status_win = cvCreateImage(cvSize(240,490),IPL_DEPTH_8U,3);
	//IplImage *recipe_win = cvCreateImage(cvSize(200,600),IPL_DEPTH_8U,3);
	IplImage *action_win = cvCreateImage(cvSize(200,200),IPL_DEPTH_8U,3);
	IplImage *graph = cvCreateImage(cvSize(1100,60),IPL_DEPTH_8U,3);
	cvZero(graph);
	cvAddS(graph,cvScalar(255,255,255),graph,0);

	cvNamedWindow("RGB", CV_WINDOW_AUTOSIZE); 
	cvMoveWindow("RGB", 0, 5);

	cvNamedWindow("status_win", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("status_win", 880, 0);

	cvNamedWindow("recipe_win", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("recipe_win", 1140, 0);

	cvNamedWindow("action_win", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("action_win", 660, 0);

	cvNamedWindow("depth_win", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("depth_win", 660, 290);

	cvNamedWindow("graph_win", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("graph_win", 5, 530);

	char buffer[50],key = '\0';
	int use_stat[10];

	int bool_i = 0;
	int fc=0,ffc=0;
	CvPoint pan,fp,bowl,chopstick,bottle,knife,turner,saltbox,potato,peeler;
	IplImage *Pan = cvLoadImage("templates/Pan.jpg",0);
	IplImage *Fp = cvLoadImage("templates/FP.jpg",0);
	IplImage *Bottle = cvLoadImage("templates/Bottle.jpg",0);
	//IplImage *Bowl = cvLoadImage("templates/Bowl.jpg",0);
	//IplImage *Chopstick = cvLoadImage("templates/Chopstick.jpg",0);
	//IplImage *Knife = cvLoadImage("templates/Knife.jpg",0);
	//IplImage *Potato = cvLoadImage("templates/Potato.jpg",0);
	//IplImage *Saltbox = cvLoadImage("templates/Saltbox.jpg",0);
	//IplImage *Turner = cvLoadImage("templates/Turner.jpg",0);
	//IplImage *Peeler = cvLoadImage("templates/Peeler.jpg",0);
	IplImage *btn = 0; 
	IplImage *img;
	IplImage *final_result;

	if (!Pan || !Fp || !Bottle)
	{
		printf("template loading error - 101 !\n");
		exit(0);
	}

	CvRect rPan,rFp,rBowl,rChopstick,rBottle,rKnife,rTurner,rSaltbox,rPotato,rPeeler,orig_bowl,orig_peeler,orig_saltbox;
	CvScalar tmp;
	int sumPan = 0,sumFp = 0,sumBowl = 0,sumChopstick = 0,sumBottle = 0,sumKnife = 0,sumTurner = 0,sumSaltbox = 0,sumPotato = 0;
	int anmFlag=0,in_region=0,nc = 0;
	char anm[60];
	CvFont font;
	cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX,1,1,0,1);
	int debug = 1;

	///////////////////////// recipe-check list variable/////////////////////////////////

	IplImage *checkbox_template  = cvLoadImage("Recipe/recipe_template.jpg");
	IplImage *checkbox_ok  = cvLoadImage("Recipe/checkbox_checked.jpg");
	IplImage *checkbox  = cvLoadImage("Recipe/checkbox_unchkd.jpg");
	int count_actions[10] = {0};
	int checklist[20] = {0};
	int num_steps_recipe=0;

	///////////////////////////done////////////////////////////////////


	///////////////////////  Reading Ycbcr from file///////////////////////////
	//Bowl
	int Blx,Bhx,Blz,Bhz,Bly,Bhy;
	int object = 0,xy = -1;
	FILE *p = fopen("threshold/Bowl.txt","r");
	fscanf(p,"%d %d %d %d %d %d",&Blx,&Bhx,&Blz,&Bhz,&Bly,&Bhy);
	fclose(p);

	//Pan
	int PTh,tth;
	p = fopen("threshold/Pan.txt","r");
	fscanf(p,"%d",&PTh);
	fclose(p);

	//Frying-Pan
	int FTh;
	p = fopen("threshold/FP.txt","r");
	fscanf(p,"%d",&FTh);
	fclose(p);


	//Saltbox
	int Slx,Shx,Slz,Shz,Sly,Shy;
	p = fopen("threshold/Saltbox.txt","r");
	fscanf(p,"%d %d %d %d %d %d",&Slx,&Shx,&Slz,&Shz,&Sly,&Shy);
	fclose(p);

	//chopstick
	int Clx,Chx,Clz,Chz,Cly,Chy;
	p = fopen("threshold/Chopstick.txt","r");
	fscanf(p,"%d %d %d %d %d %d",&Clx,&Chx,&Clz,&Chz,&Cly,&Chy);
	fclose(p);

	//Knife
	int Klx,Khx,Klz,Khz,Kly,Khy;
	p = fopen("threshold/Knife.txt","r");
	fscanf(p,"%d %d %d %d %d %d",&Klx,&Khx,&Klz,&Khz,&Kly,&Khy);
	fclose(p);

	//grater
	int Glx,Ghx,Glz,Ghz,Gly,Ghy;
	p = fopen("threshold/Grater.txt","r");
	fscanf(p,"%d %d %d %d %d %d",&Glx,&Ghx,&Glz,&Ghz,&Gly,&Ghy);
	fclose(p);

	//pan
	//frying-pan
	//bottle

	//peeler
	int Plx,Phx,Plz,Phz,Ply,Phy;
	p = fopen("threshold/Peeler.txt","r");
	fscanf(p,"%d %d %d %d %d %d",&Plx,&Phx,&Plz,&Phz,&Ply,&Phy);
	fclose(p);


	///////////////////////////////done///////////////////////////////////


	while(1)
	{
		// wait for event to be triggered by Kinect
		WaitForSingleObject( hNextColorFrameEvent, INFINITE );
		WaitForSingleObject( hNextDepthFrameEvent, INFINITE );

		// process the image
		
		hr = NuiImageStreamGetNextFrame( pColorStreamHandle, 0, &imageFrame );
		/*if (FAILED(hr))
		{
			printf("get color frame failed \n");
		}
		else printf("get color frame \n");*/

		hr = NuiImageStreamGetNextFrame( pDepthStreamHandle, 0, &imageFrame2 );
		/*if (FAILED(hr))
		{
			printf("get depth frame failed \n");
		}
		else printf("get depth frame \n");*/

		pTexture = imageFrame->pFrameTexture;
		pTexture2 = imageFrame2->pFrameTexture;
	
		// Lock the frame data so the Kinect knows not to modify it while we're reading it
		pTexture->LockRect(0, &LockedRect, NULL, 0);
		pTexture2->LockRect(0, &LockedRect2, NULL, 0);

		if( LockedRect.Pitch != 0 )
		{
			pBuffer = (BYTE*) LockedRect.pBits;
			cvSetData(color,pBuffer,LockedRect.Pitch);
		}
		
		if( LockedRect2.Pitch != 0 )
		{
			pBuff = (USHORT*) LockedRect2.pBits;
			for(int i=0; i<(640*480); i++)
			{
				BYTE index = pBuff[i]&0x07;
				USHORT realDepth = (pBuff[i]&0xFFF8)>>3;
				BYTE scale = 255 - (BYTE)(256*realDepth/0x0fff);
				buf[CHANNEL*i] = buf[CHANNEL*i+1] =buf[CHANNEL *i+2] =0;

				switch(index)
				{
					case 0:	buf[CHANNEL*i] = scale/2;
							buf[CHANNEL*i+1] = scale/2;
							buf[CHANNEL*i+2]=scale/2;
							break;

					case 1:	buf[CHANNEL*i]=scale;
							break;

					case 2:	buf[CHANNEL*i+1]=scale;
							break;

					case 3:	buf[CHANNEL*i+2]=scale;
							break;

					case 4:	buf[CHANNEL*i]=scale;
							buf[CHANNEL*i+1]=scale;
							break;

					case 5:	buf[CHANNEL*i]=scale;
							buf[CHANNEL*i+2]=scale;
							break;

					case 6:	buf[CHANNEL*i+1]=scale;
							buf[CHANNEL*i+2]=scale;
							break;

					case 7:	buf[CHANNEL*i]=255-scale/2;
							buf[CHANNEL*i+1]=255-scale/2;
							buf[CHANNEL*i+2]=255-scale/2;
							break;
				}
			}
				cvSetData(depth, buf, 640*CHANNEL);
		}

		// cleanup
		// Release the frame
		NuiImageStreamReleaseFrame( pColorStreamHandle, imageFrame );
		NuiImageStreamReleaseFrame( pDepthStreamHandle, imageFrame2 );


		//--------------------------------------------- Start from here ---------------------------------------------------------

		cvSplit(color,R,G,B,D);
		cvMerge(R,G,B,NULL,RGB);
		
		if(ffc == 0)																				//Initialisation Block
		{
			printf("Initialising System...\n\n");

			cvCopy(RGB,RGB_clone);
			template_matchingAll(RGB_clone,&pan,&fp,&bowl,&chopstick,&bottle,&knife,&turner,&saltbox,&potato,&peeler);

			rPan = cvRect(pan.x-(Pan->width/2),pan.y-(Pan->height/2),Pan->width,Pan->height);
			rFp = cvRect(fp.x-(Fp->width/2),fp.y-(Fp->height/2),Fp->width,Fp->height);
			//rBowl = cvRect(bowl.x-(Bowl->width/2),bowl.y-(Bowl->height/2),Bowl->width,Bowl->height);
			//orig_bowl = rBowl; 
			//rPeeler = cvRect(peeler.x-(Peeler->width/2),peeler.y-(Peeler->height/2),Peeler->width,Peeler->height);
			//orig_peeler = rPeeler; 
			//rChopstick = cvRect(chopstick.x-(Chopstick->width/2),chopstick.y-(Chopstick->height/2),Chopstick->width,Chopstick->height);
			rBottle = cvRect(bottle.x-(Bottle->width/2),bottle.y-(Bottle->height/2),Bottle->width,Bottle->height);
			//rKnife = cvRect(knife.x-(Knife->width/2),knife.y-(Knife->height/2),Knife->width,Knife->height);
			//rTurner = cvRect(turner.x-(Turner->width/2),turner.y-(Turner->height/2),Turner->width,Turner->height);
			//rSaltbox = cvRect(saltbox.x-(Saltbox->width/2),saltbox.y-(Saltbox->height/2),Saltbox->width,Saltbox->height);
			//orig_saltbox = rSaltbox;
			//rPotato = cvRect(potato.x-(Potato->width/2),potato.y-(Potato->height/2),Potato->width,Potato->height);


			//num_steps_recipe = recipe_chklst_init(checkbox_template, checkbox, "Recipe/boiled_potato.txt", checklist);
			//num_steps_recipe = recipe_chklst_init(checkbox_template, checkbox, "Recipe/french_fries.txt", checklist);
			num_steps_recipe = recipe_chklst_init(checkbox_template, checkbox, "Recipe/rostedpo.txt", checklist);
			//num_steps_recipe = recipe_chklst_init(checkbox_template, checkbox, "Recipe/salad.txt", checklist);

		}

		if(debug == 1){
		
		//Bowl
		//lx=0;hx=105;lz=89;hz=113;ly=176;hy=242; 
		//use_stat[0] = Bowl_ycbcr(RGB,10,lx,hx,lz,hz,ly,hy,&rBowl,orig_bowl,1500);
		use_stat[0] = Bowl_ycbcr2(RGB,Blx,Bhx,Blz,Bhz,Bly,Bhy,1500);
		//if(use_stat[0]) printf("in use...\n");
		//else printf("not in use...\n");
		 


		 //saltbox
		//lx=61;hx=163;lz=150;hz=203;ly=138;hy=183; 
		//use_stat[1] = saltbox_ycbcr(RGB,lx,hx,lz,hz,ly,hy,85000);
		use_stat[1] = saltbox_ycbcr(RGB,Slx,Shx,Slz,Shz,Sly,Shy,85000);
		//if(use_stat[1]) printf("in use...\n");
		//else printf("not in use...\n");
		

		//chopstick-yellow
		//lz = 57;hz = 101;ly = 117;hy = 233;lx = 133;hx = 210;
		//use_stat[2] = chopstickYellow_ycbcr(RGB,lx,hx,lz,hz,ly,hy,90000);
		use_stat[2] = chopstickYellow_ycbcr(RGB,Clx,Chx,Clz,Chz,Cly,Chy,90000);
		//if(use_stat[2]) printf("in use...\n");
		//else printf("not in use...\n");
		
		
		//knife
		//lz = 80;hz = 216;ly = 184;hy = 226;lx = 106;hx = 136;
		//use_stat[3] = knife_ycbcr(RGB,lx,hx,lz,hz,ly,hy,30000);
		use_stat[3] = knife_ycbcr(RGB,Klx,Khx,Klz,Khz,Kly,Khy,30000);
		//if(use_stat[3]) printf("in use...\n");
		//else printf("not in use...\n");
		
		
		//grater
		//lz = 154;hz = 255;ly = 99;hy = 126;lx = 0;hx = 214;
		//use_stat[4] = grater_ycbcr(RGB,lx,hx,lz,hz,ly,hy,10000);
		use_stat[4] = grater_ycbcr(RGB,Glx,Ghx,Glz,Ghz,Gly,Ghy,10000);
		//if(use_stat[4]) printf("in use...\n");
		//else printf("not in use...\n");
		

		//Pan
		//lz = 109;hz = 144;ly = 122;hy = 149;lx = 114;hx = 179;
		use_stat[5] = Pan_template2(RGB,Pan,rPan,114,179,109,144,122,149,PTh);
		//if(use_stat[5]) printf("in use...\n");
		//else printf("not in use...\n");
		

		//Frying-Pan
		//lz = 102;hz = 171;ly = 95;hy = 139;lx = 14;hx = 58;
		use_stat[6] = FP_template2(RGB,Fp,rFp,14,58,102,171,95,139,FTh);
		//if(use_stat[6]) printf("in use...\n");
		//else printf("not in use...\n");
		

		//Bottle
		use_stat[7] = bottle_depth(RGB,depth,rBottle,1000);
		//if(use_stat[7]) printf("in use...\n");
		//else printf("not in use...\n");
		
		
		//Peeler
		//lz = 58;hz = 115;ly = 70;hy = 131;lx = 72;hx = 222;
		//use_stat[8] = Peeler_ycbcr(RGB,lx,hx,lz,hz,ly,hy,95000);
		use_stat[8] = Peeler_ycbcr(RGB,Plx,Phx,Plz,Phz,Ply,Phy,95000);
		//if(use_stat[8]) printf("in use...\n");
		//else printf("not in use...\n");
		

		labelStatus(status_win,&use_stat[0],9);
		int action = classify_action(action_win,&use_stat[0]);
		count_actions[action]++;
		recipe_checklist(checkbox_template, checkbox_ok, checkbox, count_actions, checklist, num_steps_recipe);
		colorgraph(graph,action,fc+1);


		
//-------------------------------------------------------------------------------------------------------------------------------
		cvDestroyWindow("debug");
		cvDestroyWindow("threshimage");

		cvResize(depth,depthS,1);
		//display image 
		cvShowImage("RGB",RGB);
		cvShowImage("depth_win",depthS);
		cvShowImage("status_win",status_win);
		cvShowImage("recipe_win",checkbox_template);
		cvShowImage("action_win",action_win);
		cvShowImage("graph_win",graph);
		fc++;
		
		//printf("if \n");
		
	}//if-ends
	else if(xy == 1)
	{
		if(object == 6)
		{
			cvShowImage("debug",RGB);
			IplImage *threshimage = cvCreateImage(cvGetSize(RGB),8,1);
			if(Pan_template2(RGB,Pan,rPan,114,179,109,144,122,149,PTh)) cvPutText(threshimage,"Not in use",cvPoint(100,100),&font,cvScalar(255));
			else cvPutText(threshimage,"in use",cvPoint(100,100),&font,cvScalar(255));
			cvShowImage("threshimage",threshimage);
			cvReleaseImage(&threshimage);
		}
		else if(object == 7)
		{
			cvShowImage("debug",RGB);
			IplImage *threshimage = cvCreateImage(cvGetSize(RGB),8,1);
			if(FP_template2(RGB,Fp,rFp,14,58,102,171,95,139,FTh)) cvPutText(threshimage,"Not in use",cvPoint(100,100),&font,cvScalar(255));
			else cvPutText(threshimage,"in use",cvPoint(100,100),&font,cvScalar(255));
			cvShowImage("threshimage",threshimage);
			cvReleaseImage(&threshimage);
		}
	}
	else 
	{
		//Ycbr range deciding
		IplImage *threshimage = cvCreateImage(cvGetSize(RGB),8,1);
		ycbcr_segment2(RGB,threshimage,lx,hx,lz,hz,ly,hy);
		cvShowImage("debug",RGB);
		cvShowImage("threshimage",threshimage);
		cvReleaseImage(&threshimage);
		
		//printf("else \n");

	}
		ffc++;
		key = cvWaitKey(10);
//----------------------------------------------------------------------------------------------------------------------------
		//correction in thresholds

		switch(key)
		{

			case 'b' :	debug *= -1;
						//lx=0;hx=105;lz=89;hz=113;ly=176;hy=242; 
						lx=Blx;hx=Bhx;lz=Blz;hz=Bhz;ly=Bly;hy=Bhy; 
						cvNamedWindow("debug", CV_WINDOW_AUTOSIZE);
						cvCreateTrackbar( "lz: ", "debug", &lz, 255, NULL);
						cvCreateTrackbar( "hz: ", "debug", &hz, 255, NULL);
						cvCreateTrackbar( "ly: ", "debug", &ly, 255, NULL);
						cvCreateTrackbar( "hy: ", "debug", &hy, 255, NULL);
						cvCreateTrackbar( "lx: ", "debug", &lx, 255, NULL);
						cvCreateTrackbar( "hx: ", "debug", &hx, 255, NULL);
						object = 1;
						key = '\0';
						break;

			case 's' :	debug *= -1;
						//lx=61;hx=163;lz=150;hz=203;ly=138;hy=183; 
						lx=Slx;hx=Shx;lz=Slz;hz=Shz;ly=Sly;hy=Shy; 

						cvNamedWindow("debug", CV_WINDOW_AUTOSIZE);
						cvCreateTrackbar( "lz: ", "debug", &lz, 255, NULL);
						cvCreateTrackbar( "hz: ", "debug", &hz, 255, NULL);
						cvCreateTrackbar( "ly: ", "debug", &ly, 255, NULL);
						cvCreateTrackbar( "hy: ", "debug", &hy, 255, NULL);
						cvCreateTrackbar( "lx: ", "debug", &lx, 255, NULL);
						cvCreateTrackbar( "hx: ", "debug", &hx, 255, NULL);
						object = 2;
						key = '\0';
						break;

			case 'c' :	debug *= -1;
						//lz = 57;hz = 101;ly = 117;hy = 233;lx = 133;hx = 210;
						lx=Clx;hx=Chx;lz=Clz;hz=Chz;ly=Cly;hy=Chy; 
						cvNamedWindow("debug", CV_WINDOW_AUTOSIZE);
						cvCreateTrackbar( "lz: ", "debug", &lz, 255, NULL);
						cvCreateTrackbar( "hz: ", "debug", &hz, 255, NULL);
						cvCreateTrackbar( "ly: ", "debug", &ly, 255, NULL);
						cvCreateTrackbar( "hy: ", "debug", &hy, 255, NULL);
						cvCreateTrackbar( "lx: ", "debug", &lx, 255, NULL);
						cvCreateTrackbar( "hx: ", "debug", &hx, 255, NULL);
						object = 3;
						key = '\0';
						break;

			case 'k' :	debug *= -1;
						//lz = 80;hz = 216;ly = 184;hy = 226;lx = 106;hx = 136;
						lx=Klx;hx=Khx;lz=Klz;hz=Khz;ly=Kly;hy=Khy; 
						cvNamedWindow("debug", CV_WINDOW_AUTOSIZE);
						cvCreateTrackbar( "lz: ", "debug", &lz, 255, NULL);
						cvCreateTrackbar( "hz: ", "debug", &hz, 255, NULL);
						cvCreateTrackbar( "ly: ", "debug", &ly, 255, NULL);
						cvCreateTrackbar( "hy: ", "debug", &hy, 255, NULL);
						cvCreateTrackbar( "lx: ", "debug", &lx, 255, NULL);
						cvCreateTrackbar( "hx: ", "debug", &hx, 255, NULL);
						object = 4;
						key = '\0';
						break;

			case 'g' :	debug *= -1;
						//lz = 154;hz = 255;ly = 99;hy = 126;lx = 0;hx = 214;
						lx=Glx;hx=Ghx;lz=Glz;hz=Ghz;ly=Gly;hy=Ghy; 
						cvNamedWindow("debug", CV_WINDOW_AUTOSIZE);
						cvCreateTrackbar( "lz: ", "debug", &lz, 255, NULL);
						cvCreateTrackbar( "hz: ", "debug", &hz, 255, NULL);
						cvCreateTrackbar( "ly: ", "debug", &ly, 255, NULL);
						cvCreateTrackbar( "hy: ", "debug", &hy, 255, NULL);
						cvCreateTrackbar( "lx: ", "debug", &lx, 255, NULL);
						cvCreateTrackbar( "hx: ", "debug", &hx, 255, NULL);
						object = 5;
						key = '\0';
						break;

			case 'p' :	debug *= -1;
						//lz = 58;hz = 115;ly = 70;hy = 131;lx = 72;hx = 222;
						lx=Plx;hx=Phx;lz=Plz;hz=Phz;ly=Ply;hy=Phy; 
						cvNamedWindow("debug", CV_WINDOW_AUTOSIZE);
						cvCreateTrackbar( "lz: ", "debug", &lz, 255, NULL);
						cvCreateTrackbar( "hz: ", "debug", &hz, 255, NULL);
						cvCreateTrackbar( "ly: ", "debug", &ly, 255, NULL);
						cvCreateTrackbar( "hy: ", "debug", &hy, 255, NULL);
						cvCreateTrackbar( "lx: ", "debug", &lx, 255, NULL);
						cvCreateTrackbar( "hx: ", "debug", &hx, 255, NULL);
						object = 9;
						key = '\0';
						break;

			case 'q' :  if(object == 1)//bowl
						{
							FILE *p = fopen("threshold/Bowl.txt","w");
							fprintf(p,"%d %d %d %d %d %d",lx,hx,lz,hz,ly,hy);
							fclose(p);
							Blx=lx;Bhx=hx;Blz=lz;Bhz=hz;Bly=ly;Bhy=hy; 
						}
						else if(object == 2)//saltbox
						{
							FILE *p = fopen("threshold/Saltbox.txt","w");
							fprintf(p,"%d %d %d %d %d %d",lx,hx,lz,hz,ly,hy);
							fclose(p);
							Slx=lx;Shx=hx;Slz=lz;Shz=hz;Sly=ly;Shy=hy;
						}
						else if(object == 3)//chopstick
						{
							FILE *p = fopen("threshold/Chopstick.txt","w");
							fprintf(p,"%d %d %d %d %d %d",lx,hx,lz,hz,ly,hy);
							fclose(p);
							Clx=lx;Chx=hx;Clz=lz;Chz=hz;Cly=ly;Chy=hy;
						}
						else if(object == 4)//knife
						{
							FILE *p = fopen("threshold/Knife.txt","w");
							fprintf(p,"%d %d %d %d %d %d",lx,hx,lz,hz,ly,hy);
							fclose(p);
							Klx=lx;Khx=hx;Klz=lz;Khz=hz;Kly=ly;Khy=hy;
						}
						else if(object == 5)//grater
						{
							FILE *p = fopen("threshold/Grater.txt","w");
							fprintf(p,"%d %d %d %d %d %d",lx,hx,lz,hz,ly,hy);
							fclose(p);
							Glx=lx;Ghx=hx;Glz=lz;Ghz=hz;Gly=ly;Ghy=hy;
						}
						else if(object == 9)//peeler
						{
							FILE *p = fopen("threshold/Peeler.txt","w");
							fprintf(p,"%d %d %d %d %d %d",lx,hx,lz,hz,ly,hy);
							fclose(p);
							Plx=lx;Phx=hx;Plz=lz;Phz=hz;Ply=ly;Phy=hy;
						}
						else if(object == 6)//pan
						{
							FILE *p = fopen("threshold/Pan.txt","w");
							fprintf(p,"%d",tth);
							fclose(p);
							PTh = tth;
						}
						else if(object == 7)//fp
						{
							FILE *p = fopen("threshold/FP.txt","w");
							fprintf(p,"%d",tth);
							fclose(p);
							FTh = tth;
						}
						key = '\0';
						object = 0;
						xy = -1;
						break;
				
			case 't' :cvSaveImage("templates/Bottle.jpg",RGB);
					  cvSaveImage("templates/Pan.jpg",RGB);
					  cvSaveImage("templates/FP.jpg",RGB);
					  ffc = 0;
					  cvWaitKey(0);
					  key = '\0';
					  break;

			case 'x' :	debug *= -1;			//pan
						tth = PTh;
						cvNamedWindow("debug", CV_WINDOW_AUTOSIZE);
						cvCreateTrackbar( "threshold: ", "debug", &tth,20000, NULL);
						xy *= -1;
						
						object = 6;
						key = '\0';
						break;
			
			case 'y' :	debug *= -1;			//frying-pan
						tth = FTh;
						cvNamedWindow("debug", CV_WINDOW_AUTOSIZE);
						cvCreateTrackbar( "threshold: ", "debug", &tth,10000, NULL);
						xy *= -1;
						object = 7;
						key = '\0';
						break;	

		}
	
	}
	
	cvReleaseImage(&color);
	cvReleaseImage(&R);
	cvReleaseImage(&G);
	cvReleaseImage(&B);
	cvReleaseImage(&D);
	cvReleaseImage(&RGB);
	cvReleaseImage(&depth);
	cvReleaseImage(&depthS);
	cvReleaseImage(&RGB_clone);
	cvReleaseImage(&status_win);
	cvReleaseImage(&action_win);
	cvReleaseImage(&graph);

	cvReleaseImage(&Pan);
	cvReleaseImage(&Fp);
	cvReleaseImage(&Bottle);
	//cvReleaseImage(&Bowl);
	//cvReleaseImage(&Chopstick);
	//cvReleaseImage(&Knife);
	//cvReleaseImage(&Potato);
	//cvReleaseImage(&Saltbox);
	//cvReleaseImage(&Turner);
	//cvReleaseImage(&Peeler);
	cvReleaseImage(&checkbox_template);
	cvReleaseImage(&checkbox_ok);
	cvReleaseImage(&checkbox);
	
/*-----------------------------------------------------code ends here -------------------------------------------------------*/

	// release handles we created
	if( hNextColorFrameEvent != INVALID_HANDLE_VALUE )
	{
		CloseHandle( hNextColorFrameEvent );
	}

	if(hNextDepthFrameEvent != INVALID_HANDLE_VALUE )
	{
		CloseHandle( hNextDepthFrameEvent );
	}

	//shutdown kinect
	NuiShutdown();

	getchar();
	return 0;
}