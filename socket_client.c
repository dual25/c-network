
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    int nfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(10000);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    int err = connect(nfd, (struct sockaddr*)&addr, sizeof(addr));
    std::cout << err << std::endl;
    if(err < 0)
    {
        std::cout << "exit" << std::endl;
        return 0;
    }

    std::cout << "in" << std::endl;
    while(1)
    {
    char sendline[1024];
    fgets(sendline, 1024,stdin);
    
    struct timeval timeout={10, 0};
    setsockopt(nfd, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(struct timeval));
    int n = send(nfd, sendline, strlen(sendline), 0);
    if(n <= 0)
    {
        std::cout << "cl" << std::endl;
    }
    }

    char readline[1024];
    while(1)
    {
        int n = read(nfd, readline, 1024);
        break;
    }
    std::cout << readline << std::endl;
}
