#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <string.h>


#define MAX_SIZE 4096
#define MAT_ERROR ((void *) -1)

int main()
{
	int shm_fd, sem_fd = -1;
	void * shm_mat;
	struct sembuf server_unlock = {0,1,0};
	const key_t key = ftok("/bin/bash",'M');
	if (key<0)
	{
		printf("error - create key");
		exit(3);
	}

	while(sem_fd < 0)
	{
		sem_fd = semget(key, 1, 0664);
		write(1, "wait server\n", 13);
		sleep(1);
	}

	shm_fd = shmget(key, MAX_SIZE, 0664);
	if(shm_fd<0)
	{
		printf("error - get shm");
		exit(2);
	}
	shm_mat = shmat(shm_fd, NULL, 0);
	if(shm_mat == MAT_ERROR)
	{
		printf("error - attach shm");
		exit(3);
	}

	char buf[2048];
	strcpy(buf,shm_mat);

	char file_name[64];
	char request[100];
	char binari[2048];
	int a =0;
	int i,j = 0;
	

	while(buf[i] != '\0')
	{
		while(buf[i] != '\n')
		{
			file_name[j] = buf[i];
			i++;
			j++;
		}
		j=0;
		
		sprintf(request,"file -i %s | grep 'binary' > /dev/null", file_name);
		a = system(request);
	
		if (a==0)
		{
			strcat(binari, file_name);
			strcat(binari, "\n");
		}
		i++;
		memset(file_name, 0, 64);
		
		
	}
	memset(shm_mat, 0, 4096);
	strcpy(shm_mat, binari);
	struct shmid_ds buf1;
	shmctl(shm_fd, IPC_STAT, &buf1);
	printf("%d",buf1.shm_lpid);
	printf("\n");
	semop(sem_fd, &server_unlock, 1);
	
	return 0;
}
