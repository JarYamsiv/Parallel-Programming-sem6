#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unistd.h>
#include <ctime>
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
=======================================================

CREATED BY : Vismay Raj T
ROLL NO.   : 111601030

->  both hare and turtle will update thier position after sleeping for a
    certain time called the "step_time" this is to prevent the value being
    incremented too fast


->  both hare and turtle and god will access the global variables
    which describes the current position of both hare and turtle
    and the access is made exclusive by keeping certain mutexes

->  the first one to get 500 pos wins

->  the hare will sleep for a random amount of time if it is 25 units
    ahead of turtle . and this sleep cannot be interupted even if god
    repositions them

->  even if the race is over without god's permission the process cannot end

->  once the race is finished it will not start again even if god repositions
    the animals




=======================================================
*/


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
    finished_m.unlock();

    if(finished)break;

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
}

void turtle()
{


    while(1)
    {

    finished_m.lock();
    if(turtle_pos>500){printf("turtle won at:%d\n press q to exit\n",turtle_pos);finished=1;}
    finished_m.unlock();

    if(finished)break;

    usleep(step_time);



    turtle_pos_m.lock();
    turtle_pos++;
    turtle_pos_m.unlock();


    }

}

void god()
{
    char option;
    int h_pos,t_pos;
    
 while(1)
    {
    finished_m.lock();
    if(finished)break;
    finished_m.unlock();

    printf(">>>");
    std::cin>>option;
        if(option=='q')
        {
            finished_m.lock();
            finished = 1;
            finished_m.unlock();
        }
        else{
            std::cin>>h_pos;
            std::cin>>t_pos;

            hare_pos_m.lock();
            turtle_pos_m.lock();

            hare_pos = h_pos;
            turtle_pos = t_pos;

            hare_pos_m.unlock();
            turtle_pos_m.unlock();
        }



    }


    
}

Display *dis;
Window win;
int x11_fd;
fd_set in_fds;

struct timeval tv;
XEvent ev;

int main(void)
{
    dis = XOpenDisplay(NULL);
    int s = DefaultScreen(dis);
    win = XCreateSimpleWindow(dis, RootWindow(dis, s), 10, 10, 600, 200, 1, BlackPixel (dis, s), WhitePixel(dis, s));

    // You don't need all of these. Make the mask as you normally would.
    XSelectInput(dis, win, 
        ExposureMask | KeyPressMask 
        );

    XMapWindow(dis, win);
    //XFlush(dis);

    // This returns the FD of the X11 display (or something like that)
    x11_fd = ConnectionNumber(dis);

    

    srand(time(NULL));
    std::thread hare_t(hare);
    std::thread turtle_t(turtle);
    std::thread god_t(god);

    char hare_title[] = "hare:";
    char turtle_title[] = "turtle:";

    printf("top one is turtle and bottom one is hare\n");
    printf("type p harepos turtlepos to reposition\n");
    printf("press them x icon to close\n");

    // Main loop
    while(1) {
        finished_m.lock();
        if(finished)break;
        finished_m.unlock();
        // Create a File Description Set containing x11_fd
        FD_ZERO(&in_fds);
        FD_SET(x11_fd, &in_fds);

        // Set our timer.  One second sounds good.
        tv.tv_usec = 75000;
        tv.tv_sec = 0;

        // Wait for X Event or a Timer
        int num_ready_fds = select(x11_fd + 1, &in_fds, NULL, NULL, &tv);
        if (num_ready_fds == 0)
            {
                        XClearWindow(dis,win);
                        XFlush(dis);
                        
                        XFillRectangle(dis, win, DefaultGC(dis, s), turtle_pos, 20, 20, 20);
                        XFillRectangle(dis, win, DefaultGC(dis, s), hare_pos, 60, 20, 20);
            }
        else
            {//printf("An error occured!\n");
            }

        // Handle XEvents and flush the input 
        while(XPending(dis))
            {
                XFlush(dis);
                XNextEvent(dis, &ev);
            }
    }
    XCloseDisplay(dis);
    finished_m.lock();
    finished = 1;
    finished_m.unlock();

    hare_t.join();
    turtle_t.join();
    god_t.join();
    return(0);
 }