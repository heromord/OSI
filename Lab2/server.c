
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#ifndef MSGMAX
#define MSGMAX 2048
#endif

struct msgbuf
{
	long mtype;
	char text[MSGMAX];

}buf1,buf2,buf3;
void main()
{
int type;
printf("what type \n");
scanf("%d",&type);

FILE* pipe;
int fd;
key_t key = ftok("/bin/bash",'6');
if (key<0)
{
	printf("Faild to create key");
	exit(1);
}
fd = msgget(key, 0);
if (fd==-1)
{
	printf("Faild to get massage queue");
	exit(1);
}

if (type == 1)
{
	msgrcv(fd,&buf1,MSGMAX,1,0);
	printf("%s\n",buf1.text); 
}
else if(type==2)
{
	msgrcv(fd,&buf2,MSGMAX,2,0);
	printf("%s\n",buf2.text);
}
else if(type==3)
{
	msgrcv(fd,&buf3,MSGMAX,3,0);
	printf("%s\n",buf3.text);
}
else
{
	printf("there is no such type\n");
	exit(2);
}
struct msqid_ds buf;
msgctl(fd,IPC_STAT,&buf);
printf("%d",buf.msg_qbytes);
msgctl(fd,IPC_RMID,0);
}
