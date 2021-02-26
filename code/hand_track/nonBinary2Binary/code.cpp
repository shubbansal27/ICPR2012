#include<stdio.h>
#include<stdlib.h>

int main(int argc,char *argv[])
{
	FILE *fp1,*op;
	int i,k,m;
	float dat[16];
	
	fp1 = fopen(argv[1],"r"); //ftrs 16
	op = fopen(argv[2],"w"); //output
	m = atoi(argv[3]);	

	i = 1;	
	int l,lx,ly,hx,hy;
	while(fscanf(fp1,"%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",&dat[0],&dat[1],&dat[2],&dat[3],&dat[4],&dat[5],&dat[6],&dat[7],&dat[8],&dat[9],&dat[10],&dat[11],&dat[12],&dat[13],&dat[14],&dat[15]) == 16)
	{
		
		for(l=0;l<2;l++)
		{		
			lx = l*(640/m);
			hx = lx + (640/m);

				
				ly = 0;
				hy = 240;			
				if(dat[6] > lx && dat[6] < hx && dat[7] > ly && dat[7] < hy)
					fprintf(op,"%d ",1);		
				else
					fprintf(op,"%d ",0);

				if(dat[8] > lx && dat[8] < hx && dat[9] > ly && dat[9] < hy)
					fprintf(op,"%d ",1);		
				else
					fprintf(op,"%d ",0);
			
		}


		for(l=0;l<m;l++)
		{		
			lx = l*(640/m);
			hx = lx + (640/m);

			for(k=0;k<m;k++)
			{	
				ly = 240+k*(240/m);
				hy = ly + (240/m);			
				if(dat[6] > lx && dat[6] < hx && dat[7] > ly && dat[7] < hy)
					fprintf(op,"%d ",1);		
				else
					fprintf(op,"%d ",0);

				if(dat[8] > lx && dat[8] < hx && dat[9] > ly && dat[9] < hy)
					fprintf(op,"%d ",1);		
				else
					fprintf(op,"%d ",0);
			}
		}

		for(l=0;l<4;l++)
		{
			lx = l*90;
			hx = lx + 90;

			if(dat[0] > lx && dat[0] < hx)
				fprintf(op,"%d ",1);		
			else
				fprintf(op,"%d ",0);

			if(dat[3] > lx && dat[3] < hx)
				fprintf(op,"%d ",1);		
			else
				fprintf(op,"%d ",0);

		}

		fprintf(op,"\n");
		i++;
	}

	printf("\nlines = %d\n",i-1);
	fclose(fp1);
	fclose(op);
	return 0;
}
