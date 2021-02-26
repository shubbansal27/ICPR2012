#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main(int argc,char *argv[])
{
	int total;
        total = atoi(argv[4]);
	int i,j,k;
	//int labelMat[1000000];
	//float mat[1000000][total];
	float upper,lower;
	int * labelMat = (int *)malloc(sizeof(int)*100000000);
	int * qidMat = (int *)malloc(sizeof(int)*100000000);
	float *mat = (float *)malloc(sizeof(float)*100000000);

	FILE *fp,*op_svm,*op_hmm;
	printf("feature vector size: %d\n",total);

	fp = fopen(argv[1],"r");
	op_hmm = fopen(argv[2],"w");
	op_svm = fopen(argv[3],"w");

	i=0;
	while(!feof(fp))
	{
		fscanf(fp,"%d",(labelMat+i));
		fscanf(fp,"%d",(qidMat+i));
		for(j=0;j<total;j++)
			fscanf(fp,"%f",(mat+i*total+j));

		if(feof(fp)) break;
		i++;
	}

	printf("\nTotal lines read = %d\n",i);

	for(j=0;j<total;j++)
	{
		upper = *(mat+j);								//finding upper lower
		lower = *(mat+j);
		
		for(k=0;k<i;k++)
		{
			if(*(mat+k*total+j) > upper) upper = *(mat+k*total+j);
			if(*(mat+k*total+j) < lower) lower = *(mat+k*total+j);
		}

		for(k=0;k<i;k++)
		{
			if((upper - lower) != 0)			
				*(mat+k*total+j) = fabs((float)(*(mat+k*total+j) - lower)/(upper-lower));
			else
				*(mat+k*total+j) = 0;
		}	
	}

		
		for(k=0;k<i;k++)
		{
			fprintf(op_hmm,"%d ",*(labelMat+k));
			fprintf(op_svm,"%d qid:%d ",*(labelMat+k),*(qidMat+k));		
			for(j=0;j<total;j++)
			{
				fprintf(op_hmm,"%f ",*(mat+k*total+j));
				fprintf(op_svm,"%d:%f ",j+1,*(mat+k*total+j));	
			}
			fprintf(op_hmm,"\n");
			fprintf(op_svm,"\n");
		}
	
	
	fclose(fp);
	fclose(op_hmm);
	fclose(op_svm);	
	return 0 ;
}
