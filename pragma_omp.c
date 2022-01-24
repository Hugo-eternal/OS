#include<stdio.h>
#include<omp.h>

int main(){
	int i;
	omp_set_num_threads(16);
	#pragma omp parallel for
	for(i=0;i<16;i++){
		printf("%d ",i);
	}
	printf("\n");

	return 0;

}
