/*! Simple example with read/write locks */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

/*--- macro that simplifies handling errors with function calls ------------- */
#define ACT_WARN	0
#define ACT_STOP	1
#define CALL(ACT,FUNC,...)                                \
do {                                                      \
    if ( FUNC ( __VA_ARGS__ ) ) {                         \
        perror ( #FUNC );                                 \
        if ( ACT == ACT_STOP )                            \
            exit (1);                                     \
    }                                                     \
} while(0)

volatile long long int counter;
volatile long long int count_lim;
int thread_num;

static pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

static void *worker ( void *p )
{
	long id = (long) p;


	int i;
	int my_limit = (count_lim/thread_num);
	for(i = 0; i<my_limit; i++)
	{
		CALL ( ACT_STOP, pthread_rwlock_wrlock, &rwlock );
		if(counter < count_lim){
			counter++;
			CALL ( ACT_STOP, pthread_rwlock_unlock, &rwlock );
		}
		else{
			CALL ( ACT_STOP, pthread_rwlock_unlock, &rwlock );
			break;
		}
		
	}

	return NULL;
}

int main (int argc,char *argv[])
{
	counter = 0;
    //the program takes arguments through command line	
    if(argc<3){
        // printf("usage <pgm> count_lim num_thread\n");
        return -1;
    }
    count_lim = atoi(argv[1]);
    thread_num = atoi(argv[2]);

    if(count_lim<=0 || thread_num <=0 ||count_lim<thread_num){
        // printf("invalid arguments\n");
        return -1;
    }
    else{
        //printf("count_limit: %lld ; threads used:%d \n",count_lim,num_thread);
    }

	long i;
	pthread_t *thr = (pthread_t*)malloc(sizeof(pthread_t)*thread_num);


	long double cpu_time_used;
    struct timespec start,end;

    clock_gettime(CLOCK_REALTIME, &start);

	for ( i = 0; i < thread_num; i++ ) {
		CALL ( ACT_STOP, pthread_create, &thr[i], NULL, worker, (void *) i+1 );
	}

	for ( i = 0; i < thread_num; i++ ){
		CALL ( ACT_WARN, pthread_join, thr[i], NULL );
	}

	clock_gettime(CLOCK_REALTIME, &end);

    cpu_time_used = (end.tv_sec - start.tv_sec) + ((end.tv_nsec - start.tv_nsec) / 1000000000.0);

    printf("%Lf\n",cpu_time_used );

    free(thr);

	return 0;
}