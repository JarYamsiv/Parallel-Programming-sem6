#include <stdio.h> 
#include <string.h> 
#include <thread>
#include <random>


/*

the elments ranges from 1 to 100
and there are 20 buckets each of size 5

*/
#define MAX_NUM_BUCKETS 20
#define MAX_ARRAY_SIZE 100000000
#define MAX_NUM_THREADS 10000


int H_array[MAX_NUM_THREADS][MAX_NUM_BUCKETS];
int array[MAX_ARRAY_SIZE];
int Global_histo[MAX_NUM_BUCKETS];

void individual_histo(int,long int,long int);
void individual_bucket(int);
int Randomise(int);


int main(int argc , char* argv [])
{
    /*
        the following part includes randomisation of the array and setting up of the 
        program
    */
    int seed;
    if(argc<2)
    {
        seed = time(0);
    }
    else
    {
        seed = atoi(argv[1]);
    }
    
    time_t start,end;
    double time_used;
    start = clock();
    Randomise(seed);
    end = clock();
    time_used = (double)(end-start)/CLOCKS_PER_SEC;

    printf("Time elapsed for randomisation :%f\n",time_used);
    /*
        program initialised
        Anad algorithm starts from below
    */

    std::thread individual_histo_t[MAX_NUM_THREADS];
    std::thread individual_bucket_t[MAX_NUM_BUCKETS];
    int t_start,t_end;

    start = clock();
    memset(H_array,0,sizeof(H_array[0][0])*MAX_NUM_BUCKETS*MAX_NUM_THREADS);
    memset(Global_histo,0,sizeof(Global_histo[0])*MAX_NUM_BUCKETS);

    for(int t_id=0; t_id<MAX_NUM_THREADS; t_id++)
    {
        t_start = (MAX_ARRAY_SIZE/MAX_NUM_THREADS)*(t_id);
        t_end   = (MAX_ARRAY_SIZE/MAX_NUM_THREADS)*(t_id+1);
        individual_histo_t[t_id] = std::thread(individual_histo,t_id,t_start,t_end);
    }

    for(int t_id=0; t_id<MAX_NUM_THREADS; t_id++)
    {
        individual_histo_t[t_id].join();
    }

    for(int b_id=0; b_id<MAX_NUM_BUCKETS; b_id++)
    {
        individual_bucket_t[b_id] = std::thread(individual_bucket,b_id);
    }

    for(int b_id=0; b_id<MAX_NUM_BUCKETS; b_id++)
    {
        individual_bucket_t[b_id].join();
    }

    end = clock();
    time_used = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Time elapsed for calculation using threads: %f\n",time_used);

    for(int b_id=0; b_id<MAX_NUM_BUCKETS; b_id++)
    {
        printf("b:%2d has %7d elements\n",b_id,Global_histo[b_id]);
    }

    printf("\n=== Proceding to serial calculation === \n");

    /*
        Calculation using threads done proceding to serial calculation
    */

   start = clock();
   memset(Global_histo,0,sizeof(Global_histo[0])*MAX_NUM_BUCKETS);
   for(int i=0; i<MAX_ARRAY_SIZE; i++)
   {
       int b_id = array[i]/5;
       Global_histo[b_id]++;
   }
   end = clock();
   time_used = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Time elapsed for serial calculation: %f\n",time_used);
   for(int b_id=0; b_id<MAX_NUM_BUCKETS; b_id++)
    {
        printf("b:%2d has %7d elements\n",b_id,Global_histo[b_id]);
    }
    
    


    

}

void individual_histo(int my_id,long int my_start,long int my_end)
{
    for(long int index = my_start; index<my_end; index++)
    {
        int bucket = array[index]/5;
        H_array[my_id][bucket]++;
    }

}

void individual_bucket(int b_id)
{
    for(int h_id=0; h_id<MAX_NUM_THREADS; h_id++)
    {
        Global_histo[b_id] += H_array[h_id][b_id];
    }
}

int Randomise(int seed)
{
    srand(seed);
    for(long int i=0;i<MAX_ARRAY_SIZE;i++)
    {
        array[i] = rand()%100;
    }
}