
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    
	int nsockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(10000);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bind(nsockfd, (sockaddr*)&addr, sizeof(addr));

    listen(nsockfd, 1024);
    int nconnectfd = accept(nsockfd, (struct sockaddr*)NULL, NULL);
    std::cout << nconnectfd << std::endl;



    for(; ;)
    {
        std::cout << "in" << std::endl;

        char buff[1024];

        struct timeval timeout = {30, 0};
        setsockopt(nconnectfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(struct timeval));

        int n = recv(nconnectfd, buff,1024, 0);
        if(n <= 0)
        {
            std::cout << "close socket" << std::endl;
            close(nconnectfd);
        }
        buff[n] = '\0';
        std::cout << "1111" << buff << std::endl;
        shutdown(nconnectfd, 2);
        close(nconnectfd);
        sleep(100);

    }    
}

