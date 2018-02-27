#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>

int sockmgr[2] = {0}; 

void InintialzeGameNet();

int main(int argc, char* argv[])
{

    InintialzeGameNet();

    std::cout << "send begin" << std::endl;
    char* sendline = {"gate1"};
    send(sockmgr[0], sendline, strlen(sendline), 0);

    // 接收 client 链接
    return 0;
}

void InintialzeGameNet()
{
    // 主动去链接gameserver
    int nfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(10000);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    std::cout << "connect 1" << std::endl;

    int nerr = connect(nfd, (struct sockaddr*)&addr, sizeof(addr));
    std::cout << "connect" << std::endl;
    if (nerr < 0) return;
    sockmgr[0] = nfd;

    int nfd1 = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr1;
    memset(&addr1, 0, sizeof(addr1));
    addr1.sin_family = AF_INET;
    addr1.sin_port = htons(10001);
    addr1.sin_addr.s_addr = inet_addr("127.0.0.1");
    int nerr1 = connect(nfd1, (struct sockaddr*)&addr1, sizeof(addr1));
    if (nerr1 < 0) return;
    sockmgr[1] = nfd;
}
