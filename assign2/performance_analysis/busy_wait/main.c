#include<stdio.h>
#include<stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <time.h>

typedef struct m_thread_arg_t{
    int id;
    long long int count_limit;
    int num_t;
}m_thread_arg_t;

static void* m_thread_fn(void* args);


//global varaibles
volatile long long int counter;
volatile long long int count_lim;
volatile int flag;
pthread_mutex_t counter_mut;

//global vairables end


int main(int argc,char *argv[]){
    counter = 0;
    flag = 0;
    //the program takes arguments through command line	
    if(argc<3){
        printf("usage <pgm> count_lim num_thread\n");
        return 0;
    }
    count_lim = atoi(argv[1]);
    int num_thread = atoi(argv[2]);

    if(count_lim<=0 || num_thread <=0 ||count_lim<num_thread){
        printf("invalid arguments\n");
        return 0;
    }
    else{
        //printf("count_limit: %lld ; threads used:%d \n",count_lim,num_thread);
    }




	int result_code;
    int t_id;
	pthread_t *mthread = malloc(sizeof(pthread_t)*num_thread);



	//assigning arguments of each thread
	m_thread_arg_t *arg_m_thread = malloc(sizeof(m_thread_arg_t)*num_thread);
    for (t_id = 0; t_id < num_thread; ++t_id){
        arg_m_thread[t_id].id = t_id;
        arg_m_thread[t_id].count_limit = count_lim;
        arg_m_thread[t_id].num_t = num_thread;
    }

    long double cpu_time_used;
    struct timespec start,end;

    clock_gettime(CLOCK_REALTIME, &start);

    //launchinh thread
    for (t_id = 0; t_id < num_thread; ++t_id){
        result_code = pthread_create(&mthread[t_id],NULL,m_thread_fn,(void*)&arg_m_thread[t_id]);
        assert(!result_code);
    }
	
	
    //joining all threads
    for (int t_id = 0; t_id < num_thread; ++t_id){
        result_code = pthread_join(mthread[t_id],NULL);
        assert(!result_code);
    }

    clock_gettime(CLOCK_REALTIME, &end);

    cpu_time_used = (end.tv_sec - start.tv_sec) + ((end.tv_nsec - start.tv_nsec) / 1000000000.0);

    //printf("seconds : %ld\n",(end.tv_sec - start.tv_sec) );
    //printf("nanoseconds : %ld\n",(end.tv_nsec - start.tv_nsec) );
    //printf("counter is %lld time elapsed_seconds is %Lf\n",counter,cpu_time_used );
    printf("%Lf\n",cpu_time_used );

    //freeing allocalted heaps
    free(arg_m_thread);
    free(mthread);
	return 0;
}

static void* m_thread_fn(void* args){
	m_thread_arg_t* arguments = (m_thread_arg_t*)args;

	


    long long int i;
    long long int my_count_lim = (count_lim) / (arguments->num_t);


    for(i = 0 ; i < my_count_lim; i++){
        if(counter< count_lim){
            while(flag!=arguments->id){}
            counter++;
            flag = (flag+1)%(arguments->num_t);
        }
        else{
            break;
        }
        
    }

    //printf("id is %d and my_count_limit is %lld \n", arguments->id,i);
}

