#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<omp.h>

#define N 100
#define NUM_THREADS 4

int A[N][N];
int B[N][N];
int C[N][N];
int B_transpose[N][N];
int goldenC[N][N];
int transC[N][N];
int openMPC[N][N];


void *runner(void *param);
void *runnerForBT(void *param);


int main(){
	int i,j,k;
	pthread_t tid[NUM_THREADS];
	pthread_attr_t attr[NUM_THREADS];
	struct timespec t_start, t_end;
	double elapsedTime;
	printf("\nThis is %d x %d matrix multiplication by using %d Threads.\n\n",N,N,NUM_THREADS);
//initialized
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			A[i][j] = rand()%100;//include in stdlib.h
			B[i][j] = rand()%100;
		}
	}

//transpose
	for(i=0;i<N;i++){
                for(j=0;j<N;j++){
                        B_transpose[i][j] = B[j][i];
                }
        }

//Parallel without transpose
	clock_gettime(CLOCK_REALTIME,&t_start); //CLOCK_REALTIME (system time)

	for(i=0;i<NUM_THREADS;i++){
		int *data;
		data = (int *)malloc(sizeof(int));
		*data = i;
		pthread_attr_init(&attr[i]);
		pthread_create(&tid[i],&attr[i],runner,(void *)data);         
	}
	
	for(i=0;i<NUM_THREADS;i++){
		pthread_join(tid[i],NULL);	
	}
	
	clock_gettime(CLOCK_REALTIME,&t_end); //CLOCK_REALTIME (system time)
	
	elapsedTime = (t_end.tv_sec - t_start.tv_sec) *1000;
	elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
	printf("Parallel elapsedTime : %lf ms\n",elapsedTime);

//parallel with B_matrix_transpose

	clock_gettime(CLOCK_REALTIME,&t_start); //CLOCK_REALTIME (system time)
	for(i=0;i<NUM_THREADS;i++){
		int *data;
		data = (int *)malloc(sizeof(int));
		*data = i;
		pthread_attr_init(&attr[i]);
		pthread_create(&tid[i],&attr[i],runnerForBT,(void *)data);         
	}
	for(i=0;i<NUM_THREADS;i++){
		pthread_join(tid[i],NULL);	
	}
	
	clock_gettime(CLOCK_REALTIME,&t_end); //CLOCK_REALTIME (system time)
	
	elapsedTime = (t_end.tv_sec - t_start.tv_sec) *1000;
	elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
	printf("Parallel with transpose elapsedTime : %lf ms\n",elapsedTime);

//sequential

	clock_gettime( CLOCK_REALTIME, &t_start);  
	
	for(i = 0; i < N; i++) {
		for(j = 0; j < N; j++) {
			for(k=0; k<N; k++){
				goldenC[i][j]+=A[i][k] * B[k][j];
			}
		}
	}

	clock_gettime( CLOCK_REALTIME, &t_end);

	elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
	elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
	printf("Sequential elapsedTime: %lf ms\n", elapsedTime);	

//openmp

	clock_gettime( CLOCK_REALTIME, &t_start);  
	
	#pragma omp parallel for private(j,k)
	for(i = 0; i < N; i++) {
		for(j = 0; j < N; j++) {
			for(k=0; k<N; k++){
				openMPC[i][j]+=A[i][k] * B[k][j];
			}
		}
	}

	clock_gettime( CLOCK_REALTIME, &t_end);

	elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
	elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;

	printf("OpenMP elapsedTime: %lf ms\n", elapsedTime);	

//test
	int pass = 1,trans_pass=1,openmp_pass=1;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			if(goldenC[i][j]!=C[i][j])
				pass = 0;
		}
	}
	if(pass==1)	
		printf("Origin Test Pass~ \n");

//trans_test
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			if(goldenC[i][j]!=transC[i][j])
				trans_pass = 0;
		}
	}
	if(trans_pass==1)	
		printf("Transpose Test Pass~ \n");
//openMP_test	
	
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			if(goldenC[i][j]!=openMPC[i][j])
				openmp_pass = 0;
		}
	}
	if(pass==1)	
		printf("OpenMP Test Pass~ \n");


	return 0;
}

void *runner(void *param){
	
	int i,j,k;
	int tid,portion_size,row_start,row_end;
	double sum;
	
	tid = *(int *)(param);
	portion_size = (N+NUM_THREADS-1)/NUM_THREADS;
	row_start = tid * portion_size;
	row_end = (tid+1) * portion_size;
	if(row_end > N){
		row_end = N;
	}
	for(i = row_start;i<row_end;++i){
		for(j = 0; j<N;++j){
			sum = 0;
			for(k=0;k<N;++k)
				sum += A[i][k] * B[k][j];
			C[i][j] = sum;
		}
	}
	pthread_exit(0);
}
void *runnerForBT(void *param){
	int i,j,k;
	int tid,portion_size,row_start,row_end;
	double sum;
	
	tid = *(int *)(param);
	portion_size = (N+NUM_THREADS-1)/NUM_THREADS;
	row_start = tid * portion_size;
	row_end = (tid+1) * portion_size;
	if(row_end > N){
		row_end = N;
	}
	for(i = row_start;i<row_end;++i){
		for(j = 0; j<N;++j){
			sum = 0;
			for(k=0;k<N;++k)
				sum += A[i][k] * B_transpose[j][k];
			transC[i][j] = sum;
		}
	}
	pthread_exit(0);
}





