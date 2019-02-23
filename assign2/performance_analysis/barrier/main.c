/*
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define THREAD_COUNT 4

pthread_barrier_t mybarrier;

void* threadFn(void *id_ptr) {
  int thread_id = *(int*)id_ptr;
  int wait_sec = 1 + rand() % 5;
  printf("thread %d: Wait for %d seconds.\n", thread_id, wait_sec);
  sleep(wait_sec);
  printf("thread %d: I'm ready...\n", thread_id);

  pthread_barrier_wait(&mybarrier);

  printf("thread %d: going!\n", thread_id);
  return NULL;
}


int main() {
  int i;
  pthread_t ids[THREAD_COUNT];
  int short_ids[THREAD_COUNT];

  srand(time(NULL));
  pthread_barrier_init(&mybarrier, NULL, THREAD_COUNT + 1);

  for (i=0; i < THREAD_COUNT; i++) {
    short_ids[i] = i;
    pthread_create(&ids[i], NULL, threadFn, &short_ids[i]);
  }

  printf("main() is ready.\n");

  pthread_barrier_wait(&mybarrier);

  printf("main() is going!\n");

  for (i=0; i < THREAD_COUNT; i++) {
    pthread_join(ids[i], NULL);
  }

  pthread_barrier_destroy(&mybarrier);

  return 0;
}*/

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
pthread_mutex_t counter_mut;

//global vairables end


int main(int argc,char *argv[]){
    counter = 0;
    //the program takes arguments through command line  
    if(argc<3){
        // printf("usage <pgm> count_lim num_thread\n");
        return 0;
    }
    count_lim = atoi(argv[1]);
    int num_thread = atoi(argv[2]);

    if(count_lim<=0 || num_thread <=0 ||count_lim<num_thread){
        // printf("invalid arguments\n");
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

        pthread_mutex_lock(&counter_mut);
        if(counter < count_lim){   
            counter++;
            pthread_mutex_unlock(&counter_mut);
        }
        else{
            //printf("reached count_lim from thread %d\n",arguments->id );
            pthread_mutex_unlock(&counter_mut);
            break;
        }
    }

    //printf("id is %d and my_count_limit is %lld \n", arguments->id,i);
}

