#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>


void* PrintHello(void *threadid)
{
   std::cout << *(int*)(threadid) <<"wdqwd" << std::endl;
   pthread_exit(NULL);
}


int main(int argc, char *argv[])
{
    std::cout << "begin" << std::endl;

    pthread_t t;
    int param = 5;
    int rc = pthread_create(&t, NULL, PrintHello, (void*)&param);
    std::cout << rc << std::endl;

    sleep(1000);    
    return 0;
}


