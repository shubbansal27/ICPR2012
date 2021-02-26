#include<stdio.h>
#include<string.h>
#include "./headers/train.h"

															
int main(int argc,char *argv[])
{
  
	train(argv[1],4,80,10,argv[2]);		// [in-file][scale][comp][%-mod]								
	
	return 0;
}//main ends

