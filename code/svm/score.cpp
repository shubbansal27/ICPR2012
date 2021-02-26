#include <stdio.h>
#include <string.h>
#define num_classes 9

struct ConfusionMat
{
	int tp;
	int tn;
	int fp;
	int fn;
};


int main(int argc,char *argv[])			
{
	FILE *fp1,*fp2;
	struct ConfusionMat labels[num_classes];
	char *naming[9] = {"1000","1","2","3","4","5","6","7","8"};
	char word1[30],word2[30],word[30];
	int i,count;

	fp1 = fopen(argv[1], "r");	//original
	fp2 = fopen(argv[2], "r");	//predicted

	for(i=0;i<num_classes;i++)
	{	
		labels[i].tp = 0;
		labels[i].tn = 0;
		labels[i].fp = 0;
		labels[i].fn = 0;	
	}
	count = 0;
        while (!feof(fp1) && !feof(fp2)) 
	{
            fscanf(fp1,"%s",word1);
	    fscanf(fp2,"%s",word2); 
	    if(feof(fp1) || feof(fp2)) break;
	    
	    
	    for(i=0;i<num_classes;i++)
	    {			
		strcpy(word,naming[i]);
		//fp
	    	if((strcmp(word,word2) == 0) && (strcmp(word,word1) != 0))
			labels[i].fp++;
		//fn
	    	if((strcmp(word,word2) != 0) && (strcmp(word,word1) == 0))
			labels[i].fn++;
		//tp
	    	if((strcmp(word,word2) == 0) && (strcmp(word,word1) == 0))
			labels[i].tp++;
		//tn
	    	if((strcmp(word,word2) != 0) && (strcmp(word,word1) != 0))
			labels[i].tn++;

	    }
	    count++;		
       	}
    	
	//Output
	float precision,recall,Fmeasure;
	int TP,TN,FP,FN;

	for(i=0;i<num_classes;i++)
	{
		strcpy(word,naming[i]);
		TP = labels[i].tp ;
		TN = labels[i].tn ; 
		FP = labels[i].fp ;
		FN = labels[i].fn ;
		precision = (float)TP/(TP+FP); 
		recall = (float)TP/(TP+FN) ;
		Fmeasure = 2*(precision*recall)/(precision+recall) ;
		printf("Class %s\n tp = %d    tn = %d    fp = %d    fn = %d\n Precision = %f     Recall = %f     F-measure = %f\n\n",word,TP,TN,FP,FN,precision,recall,Fmeasure);
	}
	return 0;
}
