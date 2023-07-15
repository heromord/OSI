#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/un.h>
#include <errno.h>
int main()
{
	FILE* pipe;
	errno = 0;
	char buf[1024];
       	memset(&buf, 0, sizeof(buf));
	struct sockaddr_un clientaddr, serveraddr;
	memset(&clientaddr, 0, sizeof(clientaddr));
	memset(&serveraddr, 0, sizeof(serveraddr));
	socklen_t clientleng = sizeof(clientaddr);
	int sock_fd;
	serveraddr.sun_family = AF_UNIX;
	clientaddr.sun_family = AF_UNIX;
	strncpy(serveraddr.sun_path,"/home/user/Lab5/labadab.socket",sizeof(serveraddr.sun_path));
	strncpy(clientaddr.sun_path,"/home/user/Lab5/dab.socket",sizeof(clientaddr.sun_path));

	sock_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (sock_fd == -1)
	{
		printf("error - get soket\n");
		exit(1);
	}

	if (bind(sock_fd, (const struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
	{
		printf("error - name soket\n");
		exit(1);
	}

	pipe = popen("ps -A | grep 'tty' | awk '{print $1}'","r");
	if (pipe == NULL)
	{
		printf("error - pipe\n");
		exit(1);
	}

	fread(buf, 4, 256, pipe);
	pclose(pipe);
	if (sendto(sock_fd, buf, 1024, 0, (struct sockaddr *) &clientaddr, clientleng) == -1)
	{
		printf(strerror(errno));
		printf("error - send socket\n");
		exit(1);
	}

	memset(&buf, 0, sizeof(buf));

	if (recvfrom(sock_fd, buf, 1024, 0, (struct sockaddr *) &clientaddr, &clientleng) == - 1)
	{
		printf("error - recv\n");
		exit(1);
	}

	printf("%s\n", buf);
	close(sock_fd);
	
	return(0);
}
