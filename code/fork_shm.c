#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/mman.h>
#include<sys/wait.h>
#define SIZE 5

int main(){
	
	int i;
	pid_t pid;
	int *nums;

	nums = mmap(NULL,SIZE*sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);

	for(i=0;i<SIZE;i++)
		nums[i]=i; //0 1 2 3 4 

	pid=fork();
	if(pid==0){//child
		for(i=0;i<SIZE;i++){
			nums[i] *= -i;
			printf("CHILD %d\n",nums[i]);		
		}
	}
	else if(pid > 0){
		wait(NULL);
		for(i=0;i<SIZE;i++){
                        printf("PARENT %d\n",nums[i]);
                }	
	}

	return 0;
}
