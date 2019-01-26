#include <stdio.h>
#include <vector>

/*

the elments ranges from 1 to 100
and there are 20 buckets each of size 5

*/

#define MAX_ARRAY_SIZE 1000000
#define MAX_NUM_THREADS 1000
#define histogram int[20]

histogram H_array[MAX_NUM_THREADS];
int array[MAX_ARRAY_SIZE];


void individual_histo(int my_id,int my_start,int my_end)
{
    for(int index = my_start; index<my_end; index++)
    {
        int bucket = array[index]/5;
        H_array[my_id][bucket]++;
    }

}

int main(int argc , char* argv [])
{


}