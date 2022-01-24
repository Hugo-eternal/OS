#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<time.h>
#define N 1000

int main(){
	struct timespec t_start, t_end;
	double elapsedTime;
	int i,j,k;
	int **a = (int**)malloc(N* sizeof(int *));
	int **b = (int**)malloc(N* sizeof(int *));
	int **c = (int**)malloc(N* sizeof(int *));
	int **cc = (int**)malloc(N* sizeof(int *));
	
	for(i=0;i<N;i++){
		a[i] = (int*)malloc(N * sizeof(int));
		b[i] = (int*)malloc(N * sizeof(int));
		c[i] = (int*)malloc(N * sizeof(int));
		cc[i] = (int*)malloc(N * sizeof(int));
	}

	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			a[i][j] = rand()%10;
			b[i][j] = rand()%10;
		}
	}
	
	/*             sequential            */

	// start time 
	clock_gettime(CLOCK_REALTIME, &t_start);

	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			c[i][j] = 0;
			for(k=0;k<N;k++){
				c[i][j] += a[i][k]*b[k][j];
			}
		}
	}
	//stop time
	clock_gettime(CLOCK_REALTIME, &t_end);
	
	// compute and print the elapsed time in millisec
	elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000;
	elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0 ;
	printf("Sequential elapsedTime : %lf ms\n",elapsedTime);

	/*        parallel  - pragma omp 		*/

	// start time 
	clock_gettime(CLOCK_REALTIME, &t_start);
	
	#pragma omp parallel for private(j, k)
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			cc[i][j] = 0;
			for(k=0;k<N;k++){
				cc[i][j] += a[i][k]*b[k][j];
			}
		}
	}

	//stop time
	clock_gettime(CLOCK_REALTIME,&t_end);
	
	// compute and print the elapsed time in millisec
	elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000;
	elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0 ;
	printf("Parallel  elapsedTime : %lf ms\n",elapsedTime);

	int pass = 1;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			if(c[i][j] != cc[i][j])
				pass = 0;
		}
	}
	if(pass==1)
		printf("Test pass !\n");
	
	return 0;
}






