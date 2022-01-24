#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<unistd.h>

int main(){

	//Fork return process id
	//in parent process
	pid_t child_pid = fork();

	// Parent process
	if (child_pid >0)
	{	wait(NULL);
		sleep(20);}
	// Child process
	else 
		exit(0);

	return 0;

}
