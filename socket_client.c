
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>

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
    char sendline[1024];
    fgets(sendline, 1024,stdin);
    send(nfd, sendline, strlen(sendline), 0);
}
