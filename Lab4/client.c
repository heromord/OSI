#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/un.h>
#include <errno.h>

int main()
{
	errno =0;
	FILE* pipe;
	char buf[1024], comand[2048];
       	memset(&buf, 0, sizeof(buf));
	memset(&comand, 0, sizeof(comand));
	struct sockaddr_un serveraddr, clientaddr;
	memset(&clientaddr, 0, sizeof(clientaddr));
	memset(&serveraddr, 0, sizeof(serveraddr));
	socklen_t serverleng = sizeof(serveraddr);
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

	if (bind(sock_fd, (const struct sockaddr *)&clientaddr, sizeof(clientaddr)) == -1)
	{
		printf("error - name soket\n");
		exit(1);
	}



	if (recvfrom(sock_fd, buf, 1024, 0, (struct sockaddr *) &serveraddr, &serverleng)==-1)
	{
		printf("error - recv\n");
		exit(1);
	}
	
	int i = 0;
	while(buf[i] !='\0')
	{
		if(buf[i] == '\n')
		{
			buf[i] = '\t';
		}
		i++;

	}

		sprintf(comand, "ps -f %s | grep 'user' | awk '{print $2}' ", buf);
		pipe = popen(comand,"r");
		if (pipe == NULL)
		{
			printf("error - pipe\n");
			exit(1);
		}

		memset(&buf, 0, 1024);
	
		fread(buf, 4, 256, pipe);

	
	

	if (sendto(sock_fd, buf, 1024, 0, (struct sockaddr *) &serveraddr, serverleng) == - 1)
	{
		printf(strerror(errno));
		printf("error - send socket\n");
		exit(1);
	}

	pclose(pipe);

	close(sock_fd);
	return(0);
}
