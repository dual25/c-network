#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <errno.h>
#include <fcntl.h>


void setnonblocking(int sock)
{
    int opts;
    opts = fcntl(sock, F_GETFL);
    if(opts < 0)
    {
        return;
    }

    opts = opts| O_NONBLOCK;
    if(fcntl(sock,F_SETFL,opts) < 0)
    {
        return;
    }
}

int main(int argc, char* agrv[])
{

    int nsockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(10000);
    addr.sin_addr.s_addr= inet_addr("127.0.0.1");
    bind(nsockfd, (sockaddr*)&addr, sizeof(addr));
    listen(nsockfd, 255);

    int nfd = epoll_create(10000);
    struct epoll_event ev, events[20];
    ev.data.fd = nsockfd;
    ev.events = EPOLLIN;
    epoll_ctl(nfd, EPOLL_CTL_ADD, nsockfd, &ev);

    for(; ;)
    {
        std::cout << "in" << std::endl;
        int nfds = epoll_wait(nfd, events, 20, 1000);
        for(int i = 0; i < nfds; ++i)
        {
            if(events[i].data.fd == nsockfd)
            {
                std::cout << "accept" << std::endl;
                int connfd = accept(nsockfd, (struct sockaddr*)NULL, NULL);
                setnonblocking(connfd);

                ev.data.fd = connfd;
                ev.events= EPOLLIN|EPOLLET;
                epoll_ctl(nfd, EPOLL_CTL_ADD, connfd, &ev);
            }
            else if(events[i].events & EPOLLIN)
            {
                char line[1024];
                char* head = line;

                int nRecvNum = 0;
                int count = 0;
                bool bReadOk = false;
                while(1)
                {
                    nRecvNum = recv(events[i].data.fd, head+count, 1024,0);
                    if(nRecvNum < 0)
                    {
                        if(errno == EAGAIN)
                        {
                            bReadOk = true;
                            break;
                        }         
                        else if(errno == ECONNRESET)
                        {
                            break;
                        }
                        else if (errno == EINTR)
                        {
                            continue;
                        }
                        else
                        {
                            break;
                        }
                    }
                    else if(nRecvNum == 0)
                    {
                        bReadOk = true;
                        break;
                    }
                    
                    count += nRecvNum;
                    if(nRecvNum == 1024)
                    {
                        continue;
                    }
                    else
                    {
                        bReadOk = true;
                        break;
                    }

                }

                if(bReadOk)
                {
                    line[count] = '\0';
                    std::cout << line << std::endl;
                    ev.data.fd = events[i].data.fd;
                    ev.events = EPOLLOUT|EPOLLET;
                    epoll_ctl(nfd, EPOLL_CTL_MOD, ev.data.fd, &ev);
                }
            }
            else if(events[i].events & EPOLLOUT)
            {
                std::cout << "out" << std::endl;
                char line[1024];
                bool bWrite = false;
                int writelen = 0;
                int count = 0;
               char* head = line;
              while(1)
              {
                    char str[] = "we3wadwdawdw\n"; 
                    memcpy(line, str, sizeof(str));
                    writelen = send(events[i].data.fd, head+count, 1024, 0);
                    if(writelen == -1)
                    {
                        if(errno == EAGAIN)
                        {
                            bWrite = true;
                            break;
                        }
                        else if(errno == ECONNRESET)
                        {
                            break;
                        }
                        else if(errno == EINTR)
                        {
                        }
                    }

                    if(writelen == 0)
                    {
                        break;
                    }

                    count += writelen;
                    if(writelen == 1024)
                    {
                        continue;
                    }
                    else
                    {
                        bWrite = true;
                        break;
                    }
              }

            }
        }
    }

}
