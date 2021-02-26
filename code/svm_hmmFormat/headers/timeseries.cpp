#include <stdio.h>
#include<stdlib.h>
#include<math.h>
#include "timeseries.h"

void avgCluster(float dat[][42],int size,float *dst)
{
	int i,j;
	
	for(i=0;i<42;i++)
		*(dst+i) = 0;						//zero-initialisation	

	for(i=0;i<size;i++)
	{
		for(j=0;j<42;j++)
		{
			*(dst+j) += dat[i][j];
		}
	}	

	for(i=0;i<42;i++)
		*(dst+i) = (float)*(dst+i)/(float)size ;							
	
}

//---------------------------------------------------------------------------------------------------------------------------------------------


void numZeroSlope(float dat[][42],int size,float *dst)
{
	int i,j,count1,count2;
	
	
	for(i=0;i<42;i++)
		*(dst+i) = 0;						//zero-initialisation	

	for(i=0;i<size-2;i++)
	{
		for(j=0;j<42;j++)
		{
			if((dat[i+1][j] - dat[i][j]) > 0 && (dat[i+2][j] - dat[i+1][j]) < 0)
				*(dst+j) += 1;
			else if((dat[i+1][j] - dat[i][j]) < 0 && (dat[i+2][j] - dat[i+1][j]) > 0)
				*(dst+j) += 1;
		}
	}	

	for(i=0;i<42;i++)						//normalization
		*(dst+i) = (float)*(dst+i)/(float)size ;

}

//---------------------------------------------------------------------------------------------------------------------------------------------

void varCluster(float dat[][42],int size,float avg[],float *dst)
{
	int i,j;
	
	for(i=0;i<42;i++)
		*(dst+i) = 0;						//zero-initialisation	

	for(i=0;i<size;i++)
	{
		for(j=0;j<42;j++)
		{
			*(dst+j) += (dat[i][j]-avg[j]) * (dat[i][j]-avg[j]);
		}
	}	

	for(i=0;i<42;i++)
		*(dst+i) = (float)*(dst+i)/(float)size ;							
	
}

//-------------------------------------------------------------------------------------------------------------------------------

/* qsort int comparison function */ 
int int_cmp(const void *a, const void *b) 
{ 
    const int *ia = (const int *)a; 
    const int *ib = (const int *)b;
    return *ia  - *ib; 
    
	
} 

void medianCluster(float dat[][42],int size,float *dst)
{
	int i,j;
	int arr[size];	

	for(i=0;i<42;i++)
		*(dst+i) = 0;						//zero-initialisation	

	
	for(j=0;j<42;j++)
	{
		for(i=0;i<size;i++)
		{
			arr[i] = dat[i][j];	
			qsort(arr, size, sizeof(int), int_cmp);
			
			if(size % 2 == 0)
				*(dst+j) = (arr[size/2] + arr[size/2 + 1]) / 2; 
			else
				*(dst+j) = arr[(size+1)/2]; 
			
		}	
	}
		

}

//--------------------------------------------------------------------------------------------------------------------------------

void peakCluster(float dat[][42],int size,float *dst)
{
	int i,j;
	float max;

	for(i=0;i<42;i++)
		*(dst+i) = 0;						//zero-initialisation	

	
	for(j=0;j<42;j++)
	{
		max = dat[0][j];
		for(i=0;i<size;i++)
		{
			if(dat[i][j] > max)   	max = dat[i][j]; 	
		}
		*(dst+j) = max;	
	}
		

}


//------------------------------------------------------------------------------------------------------------------------------------------------------

void velCluster(float dat[][42],int size,float *dst)
{
	int i,j;
	
	for(i=0;i<42;i++)
		*(dst+i) = 0;						//zero-initialisation	

	
	for(j=0;j<42;j++)
	{
		for(i=0;i<size-1;i++)
		{
			*(dst+j) += (dat[i+1][j] - dat[i][j]) ;	
		}
	
	}
		
	for(i=0;i<42;i++)						//avg velocity
		*(dst+i) = fabs((float)*(dst+i)/(float)size) ;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------

void entropy(float dat[][42],int size,float *dst)
{
	int i,j,r,k,flag;
	float dist[size];
	int freq[size];

	for(i=0;i<42;i++)
		*(dst+i) = 0;						//zero-initialisation	

	
	for(j=0;j<42;j++)
	{
		r=0;		
		for(i=0;i<size-1;i++)
		{
			flag = 0;			
			for(k=0;k<r;k++)
			{
				if(dat[i][j] == dist[k])
				{
					freq[k]++;
					flag = 1;
					break;
				}
			}
			if(flag == 0)
			{
				dist[r] = dat[i][j];
				freq[r] = 1;
				r++;
			}	
		}
		for(k=0;k<r;k++)
			*(dst+j) += ((float)freq[k]/size)*log((float)freq[k]/size)*-1;	
	
	}
		
}

//----------------------------------------------------------------------------------------------------------------------------------------


void normalisation(char *file1,char *file2,int size)
{
	char str[25],str2[20];
	float y[size],upper,lower;
	int j,l,ll,lock,label;
	FILE *fp = fopen(file1,"r");
	FILE *op = fopen(file2,"w");
	
	while(!feof(fp))
	{
		fscanf(fp,"%s",str);
		label = atoi(str);   

		//if(label == 55) printf("gadbad\n");
		fprintf(op,"%d  ",label);	
		if(feof(fp)) break;	

		for(j=0;j<size;j++)
		{
			fscanf(fp,"%s",str);
			l=0;
			ll=0;
			lock = 0;
			while(str[l] != '\0')
			{
				if(lock == 1) 	
				{
					str2[ll] = str[l];
					ll++;	
				}				
				else if(str[l] == ':') lock = 1; 
				l++;
			}
			str2[ll] = '\0';
			y[j] = atof(str2);
		}
		
			//--------------------------------normalization of feature vector------------------------------ 
				
		upper = y[0];
		lower = y[0];
		for(j=0;j<size;j++)
		{
			if(y[j] > upper)  upper = y[j];	//u
			if(y[j] < lower)  lower = y[j]; //l
		}		
		for(j=0;j<size;j++)
		{
			y[j] = (y[j] - lower) / (upper-lower);
		}


			//--------------------------------------writing to file-----------------------------------------------
		//fprintf(op,"%d  ",label);
		for(j=0;j<size;j++)
			fprintf(op,"%d:%f ",j+1,y[j]);
		fprintf(op,"\n");

	}		

	fclose(fp);
	fclose(op);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------


void smoothingData(int arr[][9],int size,int win_size,char *dst)
{
	int i,j,sum,k;
	int arr2[size][9];
	FILE *fp = fopen(dst,"w");
	int l = (int)(win_size/2);		

	for(i=0;i<l;i++)
	{
		for(j=0;j<9;j++)
			arr2[i][j] = arr[i][j];
	}	 

	for(i=0;i<9;i++)
	{
		for(j=l;j<size-l;j++)    
		{
			sum = 0;
			for(k=j-l;k <= j+l; k++)
				sum += arr[k][i];
										
			arr2[j][i] = (int)(sum / win_size) ;				
		}
	}

	for(i=size-l;i<=size-1;i++)
	{
		for(j=0;j<9;j++)
			arr2[i][j] = arr[i][j];
	}		

	for(i=0;i<size;i++)
	{
		for(j=0;j<9;j++)
			fprintf(fp,"%d ",arr2[i][j]);
		fprintf(fp,"\n");
	}		

	fclose(fp);
}

