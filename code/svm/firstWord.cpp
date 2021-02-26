#include <stdio.h>

int main(int argc,char *argv[])
{
	if(argc != 3) printf("Invalid Parameters !!\n\n");
	
	FILE *fp,*op;
    	char word[30];
    	fp = fopen(argv[1], "r");
	op = fopen(argv[2], "w");

    	if (fp == NULL)
        	printf("Error: input file not found\n");
    	else 
	{
        	while (!feof(fp)) 
		{
			fscanf(fp,"%s%*[^\n]",word);
			if(feof(fp)) break;            		
			fprintf(op,"%s\n", word);
			
            	}
    	}
    	fclose(fp);
	fclose(op);

	return 0;
}
