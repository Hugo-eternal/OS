#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdio.h>

int value = 5;

int main(){

	pid_t pid;
	pid = fork();

	if(pid == 0)
		value += 15;
	else{
		wait(NULL);
		printf("Parent:value:  %d\n",value);
	}
	return 0;

}










