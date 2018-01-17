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
/*
pthread_mutex_t mutexnum;
void* CalcNum(void* param)
{
   std::cout << "in" << std::endl;
   pthread_t t = pthread_self();
   for(int i = 0; i < 10; ++i)
   {
   pthread_mutex_lock(&mutexnum);
   (*(int*)param) += 5;
   std::cout << (int)t << ":" << *(int*)param << std::endl;
   pthread_mutex_unlock(&mutexnum);
   sleep(1);
   }
}

int main(int argc, char *argv[])
{
    int nNumber = 0;
    pthread_t t[3];
 
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    for(int i = 0; i < 3; ++i)
    {
        pthread_create(&t[i], &attr, CalcNum, &nNumber);
    }

    for(int i = 0; i < 3; ++i)
    {
        void* status;
        pthread_join(t[i], &status);
    }

    return 0;
}
*/

// cond
    pthread_cond_t cont;
    pthread_mutex_t mutex;

void* waitcond(void* param)
{
    for(int i = 0; i < 10; ++i)
    {
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cont, &mutex);
    (*(int*)param) += 5;
    std::cout << "wait" << ":" << *(int*)param << std::endl;
    pthread_mutex_unlock(&mutex);
    }

}

void* sendcond(void* param)
{
        for(int i = 0; i < 10; ++i)
    {
    pthread_mutex_lock(&mutex);
    (*(int*)param) += 5;
    std::cout << "send" << ":" << *(int*)param << std::endl;
    pthread_mutex_unlock(&mutex);
    int n = pthread_cond_signal(&cont);
    std::cout << n << std::endl;

    sleep(1);
    }
}

int main(int argc, char* argv[])
{

    pthread_t id[2];

    pthread_attr_t attr;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cont, NULL);

    pthread_attr_init(&attr);
    int nParam = 0;
    pthread_create(&id[0], &attr, waitcond, &nParam);
    pthread_create(&id[1], &attr, sendcond, &nParam);

    void* status;
    pthread_join(id[0], &status);
    pthread_join(id[1], &status);

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


