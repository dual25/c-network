#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>


void* PrintHello(void *threadid)
{
   pthread_detach(pthread_self());
   std::cout << *(int*)(threadid) <<"wdqwd" << std::endl;
//   sleep(1000);

   pthread_exit(NULL);
}

// pthread_detach() 

int main(int arge, char *argv[])
{
    pthread_t id;
    int nParam = 5;
    int rc = pthread_create(&id, NULL, PrintHello, (void*)&nParam);
 
    void* status;
    pthread_join(id, &status);
    //pthread_detach(id);

    std::cout << "main line" << std::endl;

    return 0;
}

/*
int main(int argc, char *argv[])
{
    std::cout << "begin" << std::endl;

    pthread_t t;
    int param = 5;
    int rc = pthread_create(&t, NULL, PrintHello, (void*)&param);
    std::cout << rc << std::endl;

    void * status;
    pthread_join(t, &status); 
    return 0;
}

*/


