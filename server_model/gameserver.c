#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <event.h>
#include <iostream>
#include <stdlib.h>

void InitializeScoket(int nport);
void onaccept(int sock, short event, void* arg);
void onread(int sock, short event, void* arg);
void onwrite(int sock, short event, void* arg);


struct event_base* base;

int main(int argc, char* argv[])
{
    if (argc < 2) return 0;

    int nport = atoi(argv[1]);

    // 提供链接服务
    InitializeScoket(nport);

    // 逻辑线程 消息线程 db线程 日志线程
    return 0;
}

void* LibeventProc(void* param);
void InitializeScoket(int nport)
{
    int nfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(nport);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bind(nfd, (struct sockaddr*)&addr, sizeof(addr));
    listen(nfd, 1024); 
    std::cout << "sock init end" << std::endl;

    pthread_t pth_id;
    pthread_attr_t pth_attr;
    pthread_attr_init(&pth_attr);
    pthread_create(&pth_id, &pth_attr, LibeventProc, &nfd);
    pthread_join(pth_id, NULL);
}

void* LibeventProc(void* param)
{
    std::cout << "libevent" << std::endl;
    int nfd = *(int*)param;
    struct event listen_ev;
    base = event_base_new();

    event_set(&listen_ev, nfd, EV_READ|EV_PERSIST, onaccept, NULL);
    event_base_set(base, &listen_ev);
    event_add(&listen_ev, NULL);
    event_base_dispatch(base);
}

void onaccept(int sock, short event, void* arg)
{
    struct sockaddr_in clientaddr;
    memset(&clientaddr, 0, sizeof(clientaddr));

    int addrsize = sizeof(struct sockaddr_in);
    int nfd = accept(sock, (struct sockaddr*)&clientaddr, (socklen_t*)&addrsize);
  //  std::cout << "accept" << std::endl;
    
    struct event* readev = new struct event();
    event_set(readev, nfd, EV_READ|EV_PERSIST, onread, readev);
    event_base_set(base, readev);
    event_add(readev, NULL);
}

void onread(int sock, short event, void* arg)
{
    struct event* write_ev;
    char* buffer = new char[1024];
    memset(buffer, 0, 1024);
    int size = recv(sock, buffer, 1024, 0);
    std::cout << buffer << std::endl;
    
    //return;    
    write_ev = new struct event();
    event_set(write_ev, sock, EV_WRITE, onwrite, buffer);
    event_base_set(base, write_ev);
    event_add(write_ev, NULL);
}

void onwrite(int sock, short event, void* arg)
{
    char* buffer = (char*)arg;
    send(sock, buffer, strlen(buffer), 0);
}



