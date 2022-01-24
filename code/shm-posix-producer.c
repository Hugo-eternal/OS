#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<unistd.h>
#include<string.h>
//complier need to use gcc shm-posix-producer.c -o shm-posix-producer -lrt
int main(){
	
	const int SIZE = 4096;
	const char *name = "OS";
	const char *message0 = "Operating System ";
	const char *message1 = "Is Fun";
	
	int shm_fd;
	void *ptr;

	shm_fd = shm_open(name,O_CREAT|O_RDWR,0666);

	ftruncate(shm_fd,SIZE);

	ptr = mmap(0,SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,shm_fd,0);
	if(ptr == MAP_FAILED){
		printf("Map Failed\n");
		exit(-1);
	}

	sprintf(ptr,"%s",message0);
	ptr += strlen(message0);
	sprintf(ptr,"%s",message1);
	ptr += strlen(message1);

	return 0;


}
