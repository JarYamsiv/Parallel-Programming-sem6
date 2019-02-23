#include <stdio.h>
#include <time.h>
#include <thread>
#include <mutex>
#include <unistd.h>



long long int counter;
volatile int flag = 0;

static void m_thread(int,int,int,int);
static void busy_thread(int,int,int,int);


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



    return 0;
}

void m_thread(int my_id,int count_lim,int num_t,int seed)
{
    int count_lim_thread = count_lim/num_t;
    for(int i=0; i<count_lim_thread; i++)
    {
        if(seed!=-1){usleep(rand()%1000);}
        while(flag!=my_id);

        if(counter>=count_lim)
        {
            flag = (flag+1)%num_t;
            break;
        }
        else
        {
            counter++;
            flag = (flag + 1)%num_t;
        }
        
    }
}

