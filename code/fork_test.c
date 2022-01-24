#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int x;
int main(){

	pid_t pid;
	pid = vfork(); // fork another process 

	if(pid<0){
		fprintf(stderr,"Fork Failed");
		exit(-1);
	}

	else if(pid==0){ //child process
		x++;
		exit(0);
	}
	else{ //parent process
		wait(NULL);
		printf("%d\n",x);
		exit(0);
	}
 //x=1 child and parent use the same stack
}
