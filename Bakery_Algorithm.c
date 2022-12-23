#include "pthread.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "time.h"
#include "stdlib.h"

#define MEMBAR __sync_synchronize()

#define THREAD_COUNT 100
#define max_bread 100 //設定麵包數量

void lock(int thread);
void unlock(int thread);
void use_resource(int thread);
void *thread_body(void *arg);

volatile int tickets[THREAD_COUNT];
//紀錄每個人拿到的號碼牌數量，越小代表越早拿到

volatile int request[THREAD_COUNT];
//紀錄每個人是否再要求號碼牌，如果有再要求設置1，沒有再要求設置0。

volatile int resource;
volatile int count = 0; //紀錄多少麵包被拿走

int main(int argc, char **argv) {
    memset((void*)tickets, 0, sizeof(tickets));
    memset((void*)request, 0, sizeof(request));
    resource = 0;

    pthread_t threads[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; ++i) {
        pthread_create(&threads[i], NULL, &thread_body, (void*)((long)i));
    }//創建執行緒,並配發同時要求

    for (int i = 0; i < THREAD_COUNT; ++i) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}
void *thread_body(void *arg){
	
	srand(time(NULL));
	int var;
    long thread = (long)arg;
    lock(thread);
    
	if(count < max_bread){ //設置已經拿多少麵包
		use_resource(thread);
		var = count + rand()%3+1;
		if(var<max_bread)
			count = var;//隨機分配售出多少麵包
		else{
			count = max_bread;
			printf("目前剩下 %d 個麵包 ",max_bread-count);
			count = max_bread +1 ;
			printf("本日限定麵包已售完,歡迎下次再度光臨\n");
		}
		if((max_bread-count)>0)
			printf("目前剩下 %d 個麵包 ",max_bread-count);
		
	}
	else if(count == max_bread)
		printf("本日限定麵包已售完,歡迎下次再度光臨\n");

	unlock(thread);
    return NULL;
}

void lock(int thread) {
    request[thread] = 1;
    printf("%d user repuest\n",thread);
    MEMBAR;

    int max_ticket = 0;				//排隊
    for (int i = 0; i < THREAD_COUNT; ++i) {	//檢查現在已排隊人數的號碼牌大小
        int ticket = tickets[i];
        max_ticket = ticket > max_ticket ? ticket : max_ticket;
    }
    tickets[thread] = max_ticket +  1 ;		//設定現在排隊的人的號碼牌

    MEMBAR;
    request[thread] = 0;
    MEMBAR;
   	printf("user %d ticket is %d\n",thread,tickets[thread]);
    for (int other = 0; other < THREAD_COUNT; ++other) {
        while (request[other]) { }		//有人再要求號碼牌，但還沒拿到，需要等他拿到
        MEMBAR;
        while (tickets[other] != 0 &&
               (tickets[other]< tickets[thread] ||
                (tickets[other] == tickets[thread] && other < thread))) { } 
				//其他人拿到號碼牌的數字比現在的人還要小，需要等他完成才能進入
                //或其他人拿到的號碼牌數字跟現在的人一樣，但tread id 比較小，需要等他完成才能進入
									    
    }
}

void unlock(int thread) {
    MEMBAR;
    tickets[thread] = 0;
}

void use_resource(int thread) {
    if (resource != 0) {
        printf("Resource was acquired by %d, but is still in-use by %d!\n",
               thread, resource);
    }
    resource = thread;
    printf("User No.%d (Enter)...\n", thread);
    MEMBAR;
    sleep(1);
    resource = 0;
}
