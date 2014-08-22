#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>

struct Tudp
{
    u_short port;
    u_short len;
    u_short csum;
};

int main()
{
    char m[1000];
    char buf[sizeof(m)];
    int s;
    struct sockaddr_in addr;
    struct Tudp udp;
    printf("Please write message\n");
    fgets(m, 1000, stdin);
    s = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if(s < 0)
    {
	perror("socket");
	return 1;
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    udp.port = htons(7777);
    udp.len = htons(sizeof(udp) + sizeof(m));
    udp.csum = 0;
    memcpy((void *)buf, (void *)&udp, sizeof(udp));
    memcpy((void *)(buf + sizeof(udp)), (void *)m, sizeof(m));
    sendto(s, m, sizeof(m),  0, (struct sockaddr *)&addr, sizeof(addr));
    close(s);
    return 0;
}
