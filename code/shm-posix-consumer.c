#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<sys/mman.h>

int main(){

	const char *name = "OS";
	const int SIZE = 4096;

	int shm_fd;
	void *ptr;
	int i;

	/* open the shared memory segment*/
	shm_fd = shm_open(name, O_RDONLY, 0666);
	if (shm_fd == -1){
		printf("shared memory failed\n");
		exit(-1);
	}
	ptr = mmap(0,SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED){
		printf("Map Failed\n");
		exit(-1);
	}
	printf("%s\n",ptr);

	if (shm_unlink(name) == -1){
		printf("Error removing %s\n",name);
		exit(-1);
	}

	return 0;
}
