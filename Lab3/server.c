#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 4096
#define MAT_ERROR ((void *) -1)

int main()
{
	FILE* pipe;
	int shm_fd, sem_fd;
	void * shm_mat;
	char buf[2048];
	const key_t key = ftok("/bin/bash",'M');
	if (key<0)
	{
		printf("errr - create key");
		exit(1);
	}
	pipe = popen("ls -A", "r");
	fread(buf, 100, 64, pipe);
	pclose(pipe);
	
	shm_fd = shmget(key, MAX_SIZE, IPC_CREAT|IPC_EXCL|0664);
	if (shm_fd<0)
	{
		printf("error- create shared memory");
		return 1;
	}

	shm_mat = shmat(shm_fd, NULL, 0);
	if (shm_mat == MAT_ERROR)
	{
		printf("error - attach shared memory");
		shmctl(shm_fd, IPC_RMID, NULL);
		return 2;
	}

	sem_fd = semget(key, 1, IPC_CREAT|IPC_EXCL|0664);
	if (sem_fd<0)
	{
		printf("error - create sem");
		shmctl(shm_fd, IPC_RMID, NULL);
		semctl(sem_fd,0 ,IPC_RMID);
		return 3;
	}
	
	strcpy(shm_mat, buf);
	
	struct sembuf server_lock = {0, -1, 0};
	struct sembuf server_lock2 = {0, -5, 0};
	struct sembuf client2_unlock = {0, 2, 0};
	semop(sem_fd, &server_lock, 1);
	semop(sem_fd, &client2_unlock, 1);
	semop(sem_fd, &server_lock2, 1);
	shmctl(shm_fd, IPC_RMID, NULL);
	semctl(sem_fd, 0, IPC_RMID);
	return 0;
}
