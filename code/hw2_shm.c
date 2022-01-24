#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
//complier need to use gcc shm-posix-producer.c -o shm-posix-producer -lrt

int main(int argc ,char **argv){
	const int SIZE = 4096;
	const char *name = "Shm";
	const int NUM_SIZE = 2048; 
	int shm_fd;
	void *ptr;
	//below build the shared memory object name and set the authorities -rw-rw-rw
	shm_fd = shm_open(name,O_CREAT|O_RDWR,0666);
	//below set the shared memory size
	ftruncate(shm_fd,SIZE);
	//below set the memory-mapped file include the shared memory object
	ptr = mmap(0,SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,shm_fd,0);
		
	pid_t pid;
	pid = fork();//child

	if (pid == 0){
		char num[NUM_SIZE];
		memset(num,0,sizeof(char)*NUM_SIZE);
		char *num_p = &num[0];
		if(argc == 1 || argc>2){
			fprintf(stderr,"pass invaild args!\n");
			shm_unlink(name);
			return 1;
		}
		printf("In Child Process\n");
		int n = atoi(argv[1]);
		num_p += sprintf(num_p,"%d ",n);
		while(n!=1){
			if(n%2==0){
				n /= 2;
				num_p += sprintf(num_p,"%d ",n);}
			else{
				n = 3 * n + 1;
				num_p += sprintf(num_p,"%d ",n);
			}
		}
		num_p += sprintf(num_p,"\n");
		sprintf(ptr,"%s",num);
		printf("Written!\n");
		printf("Child's End\n");
	}
	else {
		wait(NULL);
		printf("\nIn Parents\n");
		printf("Reading!\n\n");
		
		shm_fd = shm_open(name,O_RDONLY,0666);
		ptr = mmap(0,SIZE,PROT_READ,MAP_SHARED,shm_fd,0);
		printf("%s",(char *)ptr);
		shm_unlink(name);
		printf("\nParents' End");
	}
	return 0;
}


