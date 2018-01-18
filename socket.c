
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string.h>

int main(int argc, char* argv[]){
    
	int nsockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(10000);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bind(nsockfd, (sockaddr*)&addr, sizeof(addr));

    listen(nsockfd, 1024);

    for(; ;)
    {
        std::cout << "in" << std::endl;
        int nconnectfd = accept(nsockfd, (struct sockaddr*)NULL, NULL);
        std::cout << nconnectfd << std::endl;

        char buff[1024];
        int n = recv(nconnectfd, buff,1024, 0);
        buff[n] = '\0';
        std::cout << buff << std::endl;

    }    
}

