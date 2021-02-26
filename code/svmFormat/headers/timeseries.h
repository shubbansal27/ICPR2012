#ifndef timeseries_header_included
#define timeseries_header_included

void avgCluster(float [][42],int,float *);

void numZeroSlope(float [][42],int,float *);

void varCluster(float [][42],int ,float[],float *);

void medianCluster(float [][42],int ,float *);

void peakCluster(float [][42],int ,float *);

void velCluster(float [][42],int ,float *);

void entropy(float [][42],int ,float *);

void normalisation(char *,char *,int);

void smoothingData(int [][9],int,int,char *);

#endif
