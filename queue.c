


#include <list>
#include <unistd.h>
#include <pthread.h>
#include <iostream>

template <typename T>
class LockList
{
    private:
        std::list<T> list;

private :
        int mutex;
        int lock;
        int unlock;
public:
    LockList():mutex(0), lock(0), unlock(1){};
    ~LockList(){}

    void Lock()
    {
        while(!__sync_bool_compare_and_swap(&mutex, lock, 1))
        {
            usleep(100);
        }
    }

    void Unlock()
    {   
        __sync_bool_compare_and_swap(&mutex, unlock, 0);
    }

    void Push(T data)
    {
        Lock();
        list.push_back(data);
        Unlock();
    }
    T Front()
    {
        Lock();
        T data = list.front();
        Unlock();
        return data;
    }


};


LockList<int> TT;

void* GetProc(void* param)
{
    while(1)
    {   
        int a = TT.Front();
        std::cout << a << std::endl;
        usleep(10000000);
    }
}

void* PushProc(void* param)
{
    TT.Push(2);
}

int main(int argc,char *argv[])
{
    pthread_t id;
    pthread_create(&id, NULL, GetProc, NULL);

    pthread_t id1;
    pthread_create(&id1, NULL, PushProc, NULL);

    void* status;
    pthread_join(id, &status);
    pthread_join(id1, &status);
    return 0;
}
