#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include "skindetection.h"

int skin_det(IplImage *src, IplImage *dst, double hue_lo, double hue_hi, double sat_lo, double sat_hi) {

  long i, j, Value, R, G, B, tmp;

  double Saturation, Hue, Cr, Cg, Cb; 

  uchar *ps = (uchar*)src->imageData; // use pointers to access frame data

  uchar *pd = (uchar*)dst->imageData;

  for ( i = 0 ; i < src->height ; i++ ) {

  for ( j = 0 ; j < src->width ; j++ ) {

      R = ps[i*src->widthStep + j*src->nChannels + 2]; // R

      G = ps[i*src->widthStep + j*src->nChannels + 1]; // G

      B = ps[i*src->widthStep + j*src->nChannels    ]; // B

      // find maximum VALUE

           if ( (R >= G) && (R >= B)) Value = R;

      else if ( (G >= R) && (G >= B)) Value = G;

      else if ( (B >= G) && (B >= R)) Value = B;

           if ( (R <= G) && (R <= B)) tmp = R;

      else if ( (G <= R) && (G <= B)) tmp = G;

      else if ( (B <= G) && (B <= R)) tmp = B;

 // find minimum SATURATION 

      if (Value == 0) Saturation = 0;

      else            Saturation = ((double)Value - (double)tmp)/(double)Value;

      // SATURATION

      if (Saturation == 0) {

Hue = -1;

 }

      else {

        Cr = ((double)Value-(double)R)/((double)Value-(double)tmp);

        Cg = ((double)Value-(double)G)/((double)Value-(double)tmp);

        Cb = ((double)Value-(double)B)/((double)Value-(double)tmp);

        if (R == Value) Hue = Cb - Cg;

        if (G == Value) Hue = 2 + Cr - Cb;

        if (B == Value) Hue = 4 + Cg - Cr;

        Hue *= 60;

        if (Hue < 0) Hue +=360;

      }

      // DETECT SKIN COLOR 

      if ( (Hue >= hue_lo) && (Hue <= hue_hi) && (Saturation >= sat_lo) && (Saturation <= sat_hi) ) {

        pd[i*src->widthStep + j*src->nChannels + 2] = (uchar)R; // R

        pd[i*src->widthStep + j*src->nChannels + 1] = (uchar)G; // G

        pd[i*src->widthStep + j*src->nChannels    ] = (uchar)B; // B

      }

      else { // force pixel to white if non-skin tone

        pd[i*src->widthStep + j*src->nChannels + 2] = 0xFF; // R

        pd[i*src->widthStep + j*src->nChannels + 1] = 0xFF; // G

        pd[i*src->widthStep + j*src->nChannels    ] = 0xFF; // B

      }

    }

  }

  return 0;

} 


void superFastBlur(unsigned char *pix, int w, int h, int radius) {

   if (radius<1) return;

   int wm=w-1;

   int hm=h-1;

   int wh=w*h;

   int div=radius+radius+1;

   unsigned char *r=new unsigned char[wh];

   unsigned char *g=new unsigned char[wh];

   unsigned char *b=new unsigned char[wh];

   int rsum,gsum,bsum,x,y,i,p,p1,p2,yp,yi,yw;

   int *vMIN = new int[MAX(w,h)];

   int *vMAX = new int[MAX(w,h)];

   unsigned char *dv=new unsigned char[256*div];

   for (i=0;i<256*div;i++) dv[i]=(i/div);

   yw=yi=0;

   for (y=0;y<h;y++){

      rsum=gsum=bsum=0;

      for(i=-radius;i<=radius;i++){

         p = (yi + MIN(wm, MAX(i,0))) * 3;

         rsum += pix[p];

         gsum += pix[p+1];

         bsum += pix[p+2];

      }

      for (x=0;x<w;x++){

         r[yi]=dv[rsum];

         g[yi]=dv[gsum];

         b[yi]=dv[bsum];

         if(y==0){

            vMIN[x]=MIN(x+radius+1,wm);

            vMAX[x]=MAX(x-radius,0);

         } 

         p1 = (yw+vMIN[x])*3;

         p2 = (yw+vMAX[x])*3;

         rsum += pix[p1]      - pix[p2];

         gsum += pix[p1+1]   - pix[p2+1];

         bsum += pix[p1+2]   - pix[p2+2];

         yi++;

      }

      yw+=w;

   }

   for (x=0;x<w;x++){

      rsum=gsum=bsum=0;

      yp=-radius*w;

      for(i=-radius;i<=radius;i++){

         yi=MAX(0,yp)+x;

         rsum+=r[yi];

         gsum+=g[yi];

         bsum+=b[yi];

         yp+=w;

      }

      yi=x;

      for (y=0;y<h;y++){

         pix[yi*3]      = dv[rsum];

         pix[yi*3 + 1]   = dv[gsum];

         pix[yi*3 + 2]   = dv[bsum];

         if(x==0){

            vMIN[y]=MIN(y+radius+1,hm)*w;

            vMAX[y]=MAX(y-radius,0)*w;

         } 

         p1=x+vMIN[y];

         p2=x+vMAX[y];

         rsum+=r[p1]-r[p2];

         gsum+=g[p1]-g[p2];

         bsum+=b[p1]-b[p2];

         yi+=w;

      }

   }

   delete r;

   delete g;

   delete b;

   delete vMIN;

   delete vMAX;

   delete dv;

} 


void OpenCVsuperFastBlur(IplImage* src, IplImage* dst, int radius) {

   int i;

   unsigned char *pix;  

   uchar *ps = (uchar*)src->imageData; // use pointers to access source data

   uchar *pd = (uchar*)dst->imageData; // use pointers to access destination data

   pix = new unsigned char[src->width*src->height*3]; // allocate memory for pixel array … 3 chars for each pixel!

   for (i=0; i<(src->height*src->width*3); i++) pix[i] = (unsigned char) ps[i];

   superFastBlur(pix, src->width, src->height, radius);

   for (i=0; i<(src->height*src->width*3); i++) pd[i] = (char) pix[i];

   delete pix;

} 
