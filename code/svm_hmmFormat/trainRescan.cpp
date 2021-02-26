#include<stdio.h>
#include<stdlib.h>
#include "headers/timeseries.h"

int main(int argc,char *argv[])
{
	FILE *fp1,*fp2,*fp3,*fp4,*fp5,*fp6,*fp7,*fp8,*fp9,*fp10,*fp11,*fp12,*fp13,*fp14,*lb,*op;
	int cluster,i,rnd,k;
	char arr[6];
	float dat[88];
	fp1 = fopen(argv[1],"r"); //left hand 9
	fp2 = fopen(argv[2],"r"); //right hand 9
	fp3 = fopen(argv[3],"r"); //left angle 11
	fp4 = fopen(argv[4],"r"); //right angle 11
    	fp5 = fopen(argv[5],"r"); //left-right hand angle 2
	fp6 = fopen(argv[6],"r"); //out 4
	fp7 = fopen(argv[7],"r"); //stick 1
	fp8 = fopen(argv[8],"r"); //katori 1
	fp9 = fopen(argv[9],"r"); //knife 1
	fp10 = fopen(argv[10],"r"); //turner 1
	fp11 = fopen(argv[11],"r"); //namak 1
	fp12 = fopen(argv[12],"r"); //ftrs 16
	fp13 = fopen(argv[15],"r"); //breaking 2 
	lb = fopen(argv[13],"r"); //label	
	op = fopen(argv[14],"a"); //output
	int qid = atoi(argv[16]);
	fp14 = fopen(argv[17],"r"); //non binary to binary

	int lines=0;
	while(fscanf(fp9,"%f",&dat[48]) == 1)
	{
		lines++;
	}
	fclose(fp9);
	fp9 = fopen(argv[9],"r"); //knife 1	


	i = 0;	
	int count = 0;

	while(fscanf(fp7,"%f",&dat[46]) == 1)
	{
		fscanf(lb,"%s",arr);
		fscanf(lb,"%s",arr);
		cluster = atoi(arr);
		
		
		fscanf(fp1,"%f %f %f %f %f %f %f %f %f",&dat[0],&dat[1],&dat[2],&dat[3],&dat[4],&dat[5],&dat[6],&dat[7],&dat[8]);

		fscanf(fp2,"%f %f %f %f %f %f %f %f %f",&dat[9],&dat[10],&dat[11],&dat[12],&dat[13],&dat[14],&dat[15],&dat[16],&dat[17]);
	
		fscanf(fp3,"%f %f %f %f %f %f %f %f %f %f %f",&dat[18],&dat[19],&dat[20],&dat[21],&dat[22],&dat[23],&dat[24],&dat[25],&dat[26],&dat[27],&dat[28]);

		fscanf(fp4,"%f %f %f %f %f %f %f %f %f %f %f",&dat[29],&dat[30],&dat[31],&dat[32],&dat[33],&dat[34],&dat[35],&dat[36],&dat[37],&dat[38],&dat[39]);

		fscanf(fp5,"%f %f",&dat[40],&dat[41]);

		fscanf(fp6,"%f %f %f %f",&dat[42],&dat[43],&dat[44],&dat[45]);

		//fscanf(fp7,"%f",&dat[46]); //above using

		fscanf(fp8,"%f",&dat[47]); //katori updated

		fscanf(fp13,"%f",&dat[67]); //breaking file's extra unwanted ftr

		fscanf(fp9,"%f",&dat[48]);

		fscanf(fp10,"%f",&dat[49]);

		fscanf(fp11,"%f",&dat[50]);

		fscanf(fp12,"%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",&dat[51],&dat[52],&dat[53],&dat[54],&dat[55],&dat[56],&dat[57],&dat[58],&dat[59],&dat[60],&dat[61],&dat[62],&dat[63],&dat[64],&dat[65],&dat[66]);

		fscanf(fp13,"%f",&dat[67]);

		fscanf(fp14,"%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",&dat[68],&dat[69],&dat[70],&dat[71],&dat[72],&dat[73],&dat[74],&dat[75],&dat[76],&dat[77],&dat[78],&dat[79],&dat[80],&dat[81],&dat[82],&dat[83],&dat[84],&dat[85],&dat[86],&dat[87]);
/*------------------------------------------------------------------------------------------------------------------------------------------------------*/
		//Recalculate data from dat[42] to dat[50] : 9

		/*if(dat[42]) //bhaguna 
		{
			//dat[42] = 0;  //bhaguna
			dat[43] = 0;	//bottle
			dat[44] = 0;	//kadai
			dat[45] = 0;	//none
			//dat[46] = 0;	//stick
			dat[47] = 0;	//katori
			dat[48] = 0;	//knife
			dat[49] = 0;	//turner
			dat[50] = 0;	//namak
		}
		if(dat[43]) //bottle 
		{
			dat[42] = 0;  //bhaguna
			dat[43] = 0;	//bottle
			dat[44] = 0;	//kadai
			//dat[45] = 0;	//none
			dat[46] = 0;	//stick
			dat[47] = 0;	//katori
			dat[48] = 0;	//knife
			dat[49] = 0;	//turner
			dat[50] = 0;	//namak
		}
		if(dat[44]) //kadai 
		{
			dat[42] = 0;  //bhaguna
			dat[43] = 0;	//bottle
			//dat[44] = 0;	//kadai
			dat[45] = 0;	//none
			//dat[46] = 0;	//stick
			dat[47] = 0;	//katori
			dat[48] = 0;	//knife
			//dat[49] = 0;	//turner
			//dat[50] = 0;	//namak
		}		
		if(dat[45]) //none
		{
			dat[42] = 0;  //bhaguna
			dat[43] = 0;	//bottle
			dat[44] = 0;	//kadai
			//dat[45] = 0;	//none
			dat[46] = 0;	//stick
			dat[47] = 0;	//katori
			dat[48] = 0;	//knife
			dat[49] = 0;	//turner
			dat[50] = 0;	//namak
		}
		if(dat[46]) //stick
		{
			dat[42] = 0;    //bhaguna
			dat[43] = 0;	//bottle
			dat[44] = 0;	//kadai
			dat[45] = 0;	//none
			//dat[46] = 0;	//stick
			dat[47] = 0;	//katori
			dat[48] = 0;	//knife
			dat[49] = 0;	//turner
			dat[50] = 0;	//namak
		}
		if(dat[47]) //katori 
		{
			dat[42] = 0;  //bhaguna
			dat[43] = 0;	//bottle
			dat[44] = 0;	//kadai
			dat[45] = 0;	//none
			//dat[46] = 0;	//stick
			//dat[47] = 0;	//katori
			dat[48] = 0;	//knife
			dat[49] = 0;	//turner
			dat[50] = 0;	//namak
		}
		if(dat[48]) //knife 
		{
			dat[42] = 0;  //bhaguna
			dat[43] = 0;	//bottle
			dat[44] = 0;	//kadai
			dat[45] = 0;	//none
			dat[46] = 0;	//stick
			dat[47] = 0;	//katori
			//dat[48] = 0;	//knife
			dat[49] = 0;	//turner
			dat[50] = 0;	//namak
		}
		if(dat[49]) //turner 
		{
			dat[42] = 0;  //bhaguna
			dat[43] = 0;	//bottle
			dat[44] = 0;	//kadai
			dat[45] = 0;	//none
			dat[46] = 0;	//stick
			dat[47] = 0;	//katori
			dat[48] = 0;	//knife
			//dat[49] = 0;	//turner
			dat[50] = 0;	//namak
		}
		if(dat[50]) //namak 
		{
			dat[42] = 0;  //bhaguna
			dat[43] = 0;	//bottle
			dat[44] = 0;	//kadai
			dat[45] = 0;	//none
			dat[46] = 0;	//stick
			dat[47] = 0;	//katori
			dat[48] = 0;	//knife
			dat[49] = 0;	//turner
			//dat[50] = 0;	//namak
		}*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------*/

		if(cluster == -1000) cluster = 9;	
		
				
			fprintf(op,"%d ",cluster);
			fprintf(op,"%d ",qid);

			for(k=42;k<=50;k++)
				fprintf(op,"%f ",dat[k]);
			fprintf(op,"%f ",dat[67]);

			for(k=68;k<=79;k++)
				fprintf(op,"%f ",dat[k]);


			int ll,hh,m=4;
			for(int l=0;l<m;l++)
			{
				ll= l*(lines/m);
				hh = ll+(lines/m);

				if(i >= ll && i < hh) fprintf(op,"1 ");
				else fprintf(op,"0 "); 
			}

			m=18;
			dat[64] = (float)((int)(dat[64]+180) % 360);
			for(int l=0;l<m;l++)
			{
				ll= l*(360/m);
				hh = ll+(360/m);

				if(dat[64] >= ll && dat[64] < hh) 
				{
					fprintf(op,"%d ",l);
					//printf("i=%d %d\n",i,l);					
					break;
				}
			}

			m=18;
			dat[66] = (float)((int)(dat[66]+180) % 360);
			for(int l=0;l<m;l++)
			{
				ll= l*(360/m);
				hh = ll+(360/m);

				if(dat[66] >= ll && dat[66] < hh) 
				{
					fprintf(op,"%d ",l);
					//printf("i=%d %d\n",i,l);					
					break;
				}
			}

			
			fprintf(op,"\n");
		

		i++;
		
	}

	printf("\nlines = %d\n",i);
	fclose(fp1);
	fclose(fp2);
	fclose(fp3); 
	fclose(fp4);
	fclose(fp5);
	fclose(fp6);
	fclose(fp7);
	fclose(fp8);
	fclose(fp9);
	fclose(fp10);
	fclose(fp11);
	fclose(fp12); 
	fclose(fp13); 
	fclose(fp14); 
 	fclose(lb);
	fclose(op);
	return 0;
}