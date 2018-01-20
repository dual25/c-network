#include <pthread.h>
//#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <fcntl.h>

int clientfd = 0;
int nfd = 0;

class slot
{
public:
    int fd;
    char pBuff[1024];

    slot(int nfd)
    {
        fd = nfd;
        memset(&pBuff, 0, 1024);
    }
};

std::vector<int> vecfd;
pthread_mutex_t mutx;


void* sendproc(void* param)
{
    std::vector<slot*>& vecslot = *(std::vector<slot*>*)param;
    while(1)
    {
        pthread_mutex_lock(&mutx);
        for(std::vector<slot*>::iterator it = vecslot.begin(); it != vecslot.end(); ++it)
        {
            if((*it)->fd > 0 && (*it)->pBuff[0] != '\0')
            {
                std::cout << "send:" << (*it)->pBuff << std::endl;
                send((*it)->fd, (*it)->pBuff, 1024, MSG_DONTWAIT);
                memset((*it)->pBuff, 0, 1024);
            }
            
        }
        pthread_mutex_unlock(&mutx);
    }    
}

void * acceptproc(void* param)
{
    std::vector<slot*>& vecslot = *(std::vector<slot*>*)param;

    while(1)
    {
        //std::cout << "accept" << std::endl;
        clientfd = accept(nfd, NULL, NULL);
        if(clientfd > 0)
        {
            std::cout << "accept:" << clientfd << std::endl;
            slot* s = new slot(clientfd);
            std::cout << s->fd << s->pBuff << std::endl;

            pthread_mutex_lock(&mutx);
            vecslot.push_back(s);
            pthread_mutex_unlock(&mutx);
        }
    }
}

void* recvproc(void* param)
{
    std::vector<slot*>& vecslot = *(std::vector<slot*>*)param;

    for(;;)
    {
        pthread_mutex_lock(&mutx);
        //std::cout << "recv proc" << std::endl;
        for(std::vector<slot*>::iterator it = vecslot.begin(); it != vecslot.end(); ++it)
        {
            if((*it)->fd > 0)
            {
                char line[1024];
                int n = recv((*it)->fd, line, 1024, MSG_DONTWAIT);
                if(n > 0)
                {
                    std::cout << (*it)->fd << ":" << line << std::endl;
                    memcpy((*it)->pBuff, "12255565", 1024);
                    //it->pBuff = "12adwdwaw2";                
                }
            }
        }
        pthread_mutex_unlock(&mutx);
    }
}

int main(int arc, char *argv[])
{
    std::vector<slot*> vecslot;

    pthread_mutex_init(&mutx, NULL);


    pthread_attr_t acceptattr;
    pthread_attr_init(&acceptattr);
    pthread_t acceptid;
    pthread_create(&acceptid, &acceptattr, acceptproc, &vecslot);

    pthread_attr_t sendattr;
    pthread_attr_init(&sendattr);
    pthread_t sendthreadid;
    pthread_create(&sendthreadid, &sendattr, sendproc, &vecslot);
    

    pthread_attr_t recvattr;
    pthread_attr_init(&recvattr);
    pthread_t recvthreadid;
    pthread_create(&recvthreadid, &recvattr, recvproc, &vecslot);



    nfd = socket(AF_INET, SOCK_STREAM, 0);
    //fcntl(nfd, F_SETFL, O_NONBLOCK);
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(10000);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bind(nfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    listen(nfd, 1024);
    
    void* status;
    pthread_join(sendthreadid, &status);
    pthread_join(recvthreadid, &status);
    pthread_join(acceptid, &status);
   

    return 0;
}
