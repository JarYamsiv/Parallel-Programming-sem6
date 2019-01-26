#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include "com_var.h"



int reporter_function(int* pipe1)
{
    printf("reporter fun\n");
    close(pipe1[1]);
    char msg[32];
    int n;
    while(read(pipe1[0],msg,sizeof(msg))==-1){n=0;}
    close(pipe1[0]);
    printf("eee %d,%s\n",n,msg);
    return 0;
}

int main(int argc,char* argv[])
{
    //common_variable<int> v;
    //v.update(3);

    printf("commencing programme..\n");
    pid_t reporter;
    reporter = fork();
    int pipe1[2];
    if(pipe(pipe1)<0)
    {
        fprintf(stderr,"pipe failed\n");
        exit(0);
    }
    if(reporter<0)
    {
        printf("fork failed...");
        return 1;
    }
    else if(reporter>0)
    {
        //god
        int status;
        char msg[32];
        printf("god..\n");
        close(pipe1[0]);
        scanf("%s",msg);
        write(pipe1[1],msg,sizeof(msg));
        printf("got key..\n");
        close(pipe1[1]);
        //waitpid(reporter,&status,0);
        wait(NULL);

    }
    else{
        //reporter
        printf("reporter fun\n");
    close(pipe1[1]);
    char msg[32];
    int n;
    while(read(pipe1[0],msg,sizeof(msg))==-1){n=0;}
    close(pipe1[0]);
    printf("eee %d,%s\n",n,msg);
    return 0;
    }
}