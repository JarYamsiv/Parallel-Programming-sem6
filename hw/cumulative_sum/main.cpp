#include <iostream>
#include <stdio.h>
#include <thread>
#include <chrono>



#define SUM_LIMIT 1000000000
#define MAX_NUM_THREADS 1000

void partial_sum(int my_id,int my_start,int my_end);
long double partial_sum_holder[MAX_NUM_THREADS];

int main(int argc,char* argv[])
{
    std::thread t_partial_sums[MAX_NUM_THREADS];

    printf("Commencing program\n");

    auto start = std::chrono::steady_clock::now();

    for(int thread_id=0; thread_id<MAX_NUM_THREADS; thread_id++)
    {
        int thread_start=(SUM_LIMIT/MAX_NUM_THREADS)*thread_id;
        int thread_end=(SUM_LIMIT/MAX_NUM_THREADS)*(thread_id+1);
        t_partial_sums[thread_id] = std::thread(partial_sum,thread_id,thread_start,thread_end);
    }

    long double final_sum = 0.0;
    for(int thread_id=0; thread_id < MAX_NUM_THREADS; thread_id++)
    {
        t_partial_sums[thread_id].join();
        final_sum += partial_sum_holder[thread_id];
    }

    auto end = std::chrono::steady_clock::now();

    printf("Sum is : %Lf \n",4.0*final_sum);
    std::cout<<"time elapsed:"<<std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()<<"ns"<<std::endl;
    
}

void partial_sum(int my_id,int my_start,int my_end)
{
    long double sum = 0.0;
    double sign;
    for(int my_i = my_start; my_i<my_end; my_i++)
    {
        sign = (my_i%2==0)?1.0:-1.0;
        sum += sign*1.0/(2*(double)my_i+1);
    }
    partial_sum_holder[my_id] = sum;
}