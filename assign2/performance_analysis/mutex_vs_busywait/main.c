#include<stdio.h>
#include<stdlib.h>

typedef struct m_thread_arg_t{
	int id;
	int x;
}m_thread_arg_t;

typedef struct b_thread_arg_t{
	int id;
	int x;
}b_thread_arg_t;

void m_thread(void* args);
void busy_thread(void* args);

int main()
{	
	printf("hellow world\n");

	m_thread_arg_t arg_m_thread;
	arg_m_thread.id = 0;
	arg_m_thread.x = 1;

	m_thread((void*)(&arg_m_thread));
	return 0;
}

void m_thread(void* args)
{
	m_thread_arg_t* arguments = (m_thread_arg_t*)args;
	printf("id is %d and x is %d \n", arguments->id,arguments->x );
}

void b_thread(void* args)
{
	b_thread_arg_t* arguments = (b_thread_arg_t*)args;
}

/*

#include <stdio.h>
#include <time.h>
#include <thread>
#include <mutex>
#include <unistd.h>


std::mutex counter_m;

long long int counter;
int flag = 0;

void m_thread(int,int,int,int);
void busy_thread(int,int,int,int);


int main(int argc,char* argv[])
{
    int seed = -1;
    if(argc<3)
    {
        printf("usage <pgm> count_lim num_thread\n");
        return 0;
    }

    if(argc==4)
    {
        srand(atoi(argv[3]));
        seed = atoi(argv[3]);
    }

    int count_lim = atoi(argv[1]);
    int num_thread = atoi(argv[2]);

    if(count_lim<=0 || num_thread <=0 ||count_lim<num_thread)
    {
        printf("invalid arguments\n");
        return 0;
    }
    else
    {
        printf("count_limit: %d ; threads used:%d \n",count_lim,num_thread);
    }
    

    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> elapsed_seconds;


    printf("commncing  mutex program; hardware_concurrancy = %d\n",std::thread::hardware_concurrency());
    std::thread m_t[num_thread];

    start = std::chrono::system_clock::now();
    for(int t_id=0; t_id<num_thread; t_id++)
    {
        m_t[t_id] = std::thread(m_thread,t_id,count_lim,num_thread,seed);
    }
    for(int t_id =0 ; t_id < num_thread; t_id++)
    {
        m_t[t_id].join();
    }
    end = std::chrono::system_clock::now();
    elapsed_seconds = end - start; 
    printf("time used for mutex = %f s\n",elapsed_seconds.count());

    printf("commncing  busy program; hardware_concurrancy = %d\n",std::thread::hardware_concurrency());
    std::thread b_t[num_thread];

    start = std::chrono::system_clock::now();
    for(int t_id=0; t_id<num_thread; t_id++)
    {
        b_t[t_id] = std::thread(busy_thread,t_id,count_lim,num_thread,seed);
    }
    for(int t_id =0 ; t_id < num_thread; t_id++)
    {
        b_t[t_id].join();
    }
    end = std::chrono::system_clock::now();
    elapsed_seconds = end - start; 
    printf("time used for busy wait = %f s\n",elapsed_seconds.count());


    return 0;
}

void m_thread(int my_id,int count_lim,int num_t,int seed)
{
    int count_lim_thread = count_lim/num_t;
    for(int i=0; i<count_lim_thread; i++)
    {
        if(seed!=-1){usleep(rand()%1000);}
        counter_m.lock();
        if(counter<count_lim)
        {
            counter++;
            counter_m.unlock();
        }
        else
        {
            counter_m.unlock();
            return;
        }
        
    }
}

void busy_thread(int my_id,int count_lim,int num_t,int seed)
{
    int count_lim_thread = count_lim/num_t;
    for(int i=0; i<count_lim_thread; i++)
    {
        if(seed!=-1){usleep(rand()%1000);}
        while(flag!=my_id);
        counter++;
        flag = (flag+1)%num_t;
        
    }
}

*/
		
