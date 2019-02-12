#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unistd.h>
#include <ctime>

//step time in micro seconds
#define step_time 50000

//report delay in micro secons
#define report_delay 1000000

int hare_pos=0;
int turtle_pos=0;

//this is to prevent to threads accessing the position variable at the same time
std::mutex hare_pos_m;
std::mutex turtle_pos_m;

//used by all threads to conclude whether the process has finished
std::mutex finished_m;
int finished;



void hare()
{

    int step_size;
    while(1)
    {
    finished_m.lock();
    if(hare_pos>500){printf("hare won at:%d\n press q to exit\n",hare_pos);finished=1;}
    if(finished)break;
    finished_m.unlock();

    usleep(step_time);

    step_size = 1+rand()%3;
    hare_pos_m.lock();
    hare_pos+=step_size;
    hare_pos_m.unlock();

    hare_pos_m.lock();
    turtle_pos_m.lock();
    if(hare_pos-turtle_pos>25)
    {
        hare_pos_m.unlock();
        turtle_pos_m.unlock();
        int sleep_time = 25+rand()%100;
        usleep(50000*sleep_time);
    }
    else
    {
        hare_pos_m.unlock();
        turtle_pos_m.unlock();
    }
    

    }
    finished_m.unlock();
}

void turtle()
{


    while(1)
    {
    finished_m.lock();
    if(turtle_pos>500){printf("turtle won at:%d\n press q to exit\n",turtle_pos);finished=1;}
    if(finished)break;
    finished_m.unlock();

    usleep(step_time);



    turtle_pos_m.lock();
    turtle_pos++;
    turtle_pos_m.unlock();


    }
    finished_m.unlock();
}

void reporter()
{
    while(1)
    {
    finished_m.lock();
    if(finished)break;
    finished_m.unlock();

    hare_pos_m.lock();
    turtle_pos_m.lock();
    printf("hare at %d, turtle at %d\n",hare_pos,turtle_pos);
    hare_pos_m.unlock();
    turtle_pos_m.unlock();
    usleep(report_delay);


    }
    
    finished_m.unlock();

    
}

int main()
{
    srand(time(NULL));
    std::thread hare_t(hare);
    std::thread turtle_t(turtle);
    std::thread reporter_t(reporter);

    char option;
    int h_pos,t_pos;

    printf(" starting..\n type q to exit\n type p harepos,turtlepos to reposition\n");

    while(1)
    {
        scanf("%c",&option);
        if(option=='q')
        {
            break;
        }
        else{
            scanf("%d %d",&h_pos,&t_pos);
            hare_pos_m.lock();
            turtle_pos_m.lock();
            hare_pos = h_pos;
            turtle_pos = t_pos;
            hare_pos_m.unlock();
            turtle_pos_m.unlock();
        }

        
    }
    
    //sleep(5);
    finished_m.lock();
    finished = 1;
    finished_m.unlock();

    hare_t.join();
    turtle_t.join();
    reporter_t.join();
    return 0;
}