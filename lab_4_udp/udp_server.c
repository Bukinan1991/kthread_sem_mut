#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

int main()
{
    int s;
    struct sockaddr_in addr;
    char buf[1000];
    int rbytes;
    s = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if(s < 0)
    {
	perror("socket");
	return 1;
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(7777);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
	perror("bind");
	return 1;
    }
    while(1)
    {
	rbytes = recvfrom(s, buf, 1000, 0, (struct sockaddr *)&addr,(int *)sizeof(addr));
	printf("%s", buf);
    }
    close(s);
    return 0;
}
