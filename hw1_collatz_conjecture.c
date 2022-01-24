#include<sys/wait.h>
#include<stdio.h>
#include<unistd.h>//fork using
#include<sys/types.h>

int CollatzConjecture(int );
int main(){
	int num;
	pid_t pid;
	do{
		printf("Please enter the number greater than 0 :\n");
		scanf("%d",&num);
	}while(num<=0);
	
	pid  = fork();
	if(pid == 0)//child
	{	
		printf("\nIn child Process!\n");
		printf("\n%d ",num);
		CollatzConjecture(num);
		printf("\n");
		printf("\nThe Child Process' End\n");
	}
	else{//parent 
		printf("\nParents Process is waiting on Child Process!\n");
		wait(NULL);
		printf("\nThe Parents Process' End\n");
	}
	return 0;
}
int CollatzConjecture(int n){
	if(n%2 == 0)
 	{
		printf("%d ",n/2);
		n/=2;
	}
	else if(n%2==1)
	{
		printf("%d ",3*n+1);
		n = 3*n +1;
	}
	 if(n>1){
		return CollatzConjecture(n);		
	}
}
