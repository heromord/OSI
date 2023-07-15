#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
int counter=0;
char source_[83];
int all=0;

void signal_hand()
{
	int  stat,stat2;
	char names_d[80];
	char names[80];
	char empty[] = "";

	int blocks_;

	memset(names_d,80,0);
	memset(names,80,0);
	counter ++;
	if(counter==2)
	{
		counter=0;
		if(strcmp(source_,empty))	
		{
			printf("all_blocks = %d\t",all);
			printf("\n");
			printf("Vvedite ima kataloga iz dostupnix\n");
			system("ls ..");
		}
	} 
}

int main()
{
int fd[2],stat,stat2;
int fd_2[2];
char buf[1024],name[80];
char buf_2[1024];

unsigned blocks;
char names[80]; 
char names_d[80];
void signal_hand();
signal(SIGINT,signal_hand);

while(1)
{

pipe(fd);
memset(names_d,80,0);
memset(buf_2,1024,0);
memset(names,80,0);
memset(buf,1024,0);
memset(name,80,0);

printf("Vvedite ima kataloga iz dostupnix\n");
system("ls ..");
scanf("%s",name);
char  source[83] = "../";

strcat(source,name) ;
printf("\n");
if(access(source,F_OK)){printf("TAKOGO KATALOGA NET\n");break;}
if(fork()==0)
{
	close(fd[0]);
	close(1);
	dup(fd[1]);
	close(fd[1]);
	execl("/bin/du","du","-a",source,NULL);
} 
else
{
	strncpy(source_,source,83);
	all=0;
	wait(0);
	close(fd[1]);
	FILE *blocks_names = fopen("names_blocks.txt","w+");
	read(fd[0],buf,2048);
	close(fd[0]);

	fprintf(blocks_names,"%s",buf);

	fclose(blocks_names); 
	FILE *blocks_names2 = fopen("names_blocks.txt","r");
	
	pipe(fd_2);
	if(fork()==0){
		close(fd[0]);
		close(fd_2[0]);
		close(1);
		dup(fd_2[1]);
		close(fd_2[1]);
		execl("/usr/bin/find","find",source,"-atime","-5",NULL);
			}
	else
	{
		wait(0);
		close(fd_2[1]);
		read(fd_2[0],buf_2,2048);
		FILE *names_date = fopen("names_date.txt","w+");

		fprintf(names_date,"%s",buf_2);
	
		fclose(names_date); 
		FILE *names_date2 = fopen("names_date.txt","r");
		while((stat2=fscanf(names_date2,"%s",names_d))>0)
		{
		while((stat=fscanf(blocks_names2,"%u%s",&blocks,names))>0)
		{
			if (blocks<=3 || !strcmp(names,source) || strcmp(names,names_d))
			{
				continue;
			}
			else
			{	
				
				all += (int) blocks;
				printf("blocks = %d\t",blocks);
				printf("Name = %s\n",names);
				break;
			}
		}
		fseek(blocks_names2,0,0);
		}
		close(fd_2[0]);
		fclose(names_date2);
		fclose(blocks_names2);
}
}
}
}


