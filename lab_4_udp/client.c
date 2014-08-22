#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h>

#define MSGLEN 11

int main()
{
	int sock;
	char msg[] = "Hello world";
	struct sockaddr_in addr;
	sock = socket(AF_INET, SOCK_DGRAM, 0);

	if(sock < 0) {
		perror("sock\n");
		return 0;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(7778);
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	sendto(sock, msg, sizeof(char) * MSGLEN, 0, 
			(struct sockaddr*)&addr, sizeof(struct sockaddr_in));

	close(sock);
}
