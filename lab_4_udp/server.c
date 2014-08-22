#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h>

int main()
{
	int sock;
	char buf[2048];
	int bytes_read;
	struct sockaddr_in addr;
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0) {
		perror("sock\n");
		return 0;
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(7777);
	addr.sin_addr.s_addr = htons(INADDR_ANY);

	if(bind(sock, (struct sockaddr *)&addr,
				sizeof(struct sockaddr_in)) < 0){
		perror("bind");
		return 0;
	}
	
	bytes_read = recvfrom(sock, buf, 2048, 0, NULL, NULL);
	printf("%s\n", &buf);
	return 0;
}

