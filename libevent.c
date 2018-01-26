
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <event.h>
#include <iostream>

struct event_base* base;
void onaccept(int sock, short event, void* arg);
void onread(int sock, short event, void* arg);
void onwrite(int sock, short event, void* arg);

int main(int argc, char *argv[])
{

    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_port = htons(10000);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bind(sfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    listen(sfd, 5);

    struct event listen_ev;
    base = event_base_new();

    event_set(&listen_ev, sfd, EV_READ|EV_PERSIST, onaccept, NULL);
    event_base_set(base, &listen_ev);
    event_add(&listen_ev, NULL);
    event_base_dispatch(base);

    return 0;
}

void onaccept(int sock, short event, void* arg)
{
    struct sockaddr_in clientaddr;
    memset(&clientaddr, 0, sizeof(clientaddr));

    int addrsize = sizeof(struct sockaddr_in);
    int nfd = accept(sock, (struct sockaddr*)&clientaddr, (socklen_t*)&addrsize);
    std::cout << "accept" << std::endl;
    
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




