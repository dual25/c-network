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
/*
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
*/

// mutex


pthread_mutex_t mutexnum;

void* CalcNum(void* param)
{
   std::cout << "in" << std::endl;
   pthread_t t = pthread_self();
   pthread_mutex_lock(&mutexnum);
   (*(int*)param) += 5;
   std::cout << (int)t << ":" << *(int*)param << std::endl;
   pthread_mutex_unlock(&mutexnum);
}

int main(int argc, char *argv[])
{
    int nNumber = 0;
    pthread_t t[2];

    
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    for(int i = 0; i < 2; ++i)
    {
        pthread_create(&t[i], &attr, CalcNum, &nNumber);
    }

    for(int i = 0; i < 2; ++i)
    {
        void* status;
        pthread_join(t[i], &status);
    }

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


