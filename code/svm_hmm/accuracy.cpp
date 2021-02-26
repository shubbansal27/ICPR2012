#include <stdio.h>
#include <string.h>

int minimumExpected(char *path)
{
	FILE *fp;
	int word,i,max;
	int result[9] = {0,0,0,0,0,0,0,0,0};
    	fp = fopen(path, "r");
	
	if (fp == NULL)
        	printf("Error: reading file\n");
    	else 
	{
        	while (!feof(fp)) 
		{
            		fscanf(fp,"%d",&word);
			if(feof(fp)) break;
			switch(word)
			{
				case 1: result[0]++; break;
				case 2: result[1]++; break;
				case 3: result[2]++; break;
				case 4: result[3]++; break;
				case 5: result[4]++; break;
				case 6: result[5]++; break;
				case 7: result[6]++; break;
				case 8: result[7]++; break;								
				case 1000: result[8]++; break;
			}
		}
    	}
	max = result[0];
	for(i=0;i<9;i++)
	{
		if(result[i] > max)   max = result[i];
	}	
	fclose(fp);

	return max;
}



int main(int argc,char *argv[])			//first:Original     second:output
{
	//if(argc != 4) printf("Invalid Parameters !!\n\n");
	
	FILE *fp1,*fp2;
	int correct=0,i=0,exp;
    	char word1[30],word2[30];
    	fp1 = fopen(argv[1], "r");
	fp2 = fopen(argv[2], "r");

    	if (fp1 == NULL || fp2 == NULL)
        	printf("Error: reading file\n");
    	else 
	{
        	while (!feof(fp1) && !feof(fp2)) 
		{
            		fscanf(fp1,"%s",word1);
			fscanf(fp2,"%s",word2);
			if(strcmp(word1,"1000") == 0)
				strcpy(word1,"9");	 
			if(feof(fp1) || feof(fp2)) break;
			i++;
			if(strcmp(word1,word2) == 0) correct++;
            	}
    	}
	printf("%s\n--------\n",argv[3]);
	printf("Total = %d\n",i);
	printf("Correct = %d\n",correct);
	printf("Incorrect = %d\n",i-correct);
	printf("Accuracy = %f\n",((float)correct/(float)i)*100);
	fclose(fp1);
	fclose(fp2);

	exp = minimumExpected(argv[1]);
	
	printf("minimum Expected = %f\n\n",((float)exp/(float)i)*100);
	return 0;
}
