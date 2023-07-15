#include <unistd.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#ifndef MSGMAX
#define MSGMAX 2048
#endif

struct msgbuf
{
	long mtype;
	char text[MSGMAX];

} buf1,buf2,buf3;

void main()
{

FILE* pipe;
int fd;
key_t key = ftok("/bin/bash" ,'6');
if (key<0)
{
	printf("Faild to creat key");
	exit(1);
}
fd = msgget(key,IPC_CREAT|0640);
if (fd==-1)
{
	printf("mistake in creation massage queue\n");
	exit(1);
}


pipe = popen("cat /home/user/mail/names","r");
fread(buf1.text,1,MSGMAX,pipe);
pclose(pipe);
buf1.mtype = 1;

pipe = popen("cat /home/user/mail/terminals","r");
fread(buf2.text,1,MSGMAX,pipe);
pclose(pipe);
buf2.mtype = 2;

pipe = popen("cat /home/user/mail/quantity","r");
fread(buf3.text,1,MSGMAX,pipe);
pclose(pipe);
buf3.mtype = 3;

if(msgsnd(fd,&buf1,strlen(buf1.text)+1,IPC_NOWAIT)==-1)
{
	printf("mistake in sending massage\n");
	exit(1);
}
if(msgsnd(fd,&buf2,strlen(buf2.text)+1,IPC_NOWAIT)==-1)
{
	printf("mistake in sending massage\n");
	exit(1);
}
if(msgsnd(fd,&buf3,strlen(buf3.text)+1,IPC_NOWAIT)==-1)
{
	printf("mistake in sending massage\n");
	exit(1);
}

sleep(20);

}
