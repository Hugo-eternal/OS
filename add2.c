#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(int argc ,char*argv[])
{	
	int i=0;
	long long int sum =0;
	if(argc<2){
		printf("Please use command %s [Contains at least one number]\n",argv[0]);
		exit(1);
	}
	for(i=1;i<argc;i++){
		sum+=atoll(argv[i]);
	}
	printf("sum of num = %lld\n",sum);
	return 0;
}
