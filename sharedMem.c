#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <stdarg.h>

static char *generateData(int kbNum)
{
	int bytes = 1024 * kbNum;
	char *data = malloc(bytes);

	if (data == NULL)
		printf("failed to allocate %d bytes of memory\n", bytes);

	memset(data, '*', bytes);

return data;
}


int main(){

pid_t pid;
key_t key = 1235;
int shmId;
int status, cpid, r;
char *data;

	
int a = 0;

clock_t start_t, end_t;
double total_t;

start_t = clock();

		shmId = shmget(key, 1024 , 0666|IPC_CREAT);	
	
		if(shmId < 0){
			perror("Error en shmget");
		exit(-1);
		}
	
		data = (char *)shmat(shmId,0,0);

		if ((data < 0)) {
			perror("Error en el shmat");
		exit(-1);
		}
	
		data = generateData(102400);

    	pid = fork();
	
		if(pid < 0){
			perror("Error fork:");
		exit(-1);
		}

		if(pid == 0){
			printf("%ld \n", start_t);
		}else{
			if(wait(&status) == pid){
			}
		}

    	r = shmdt(data);

		end_t = clock();
		total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC ;
		//printf("%ld \n ", end_t);
		printf("Tiempo total: %f \n", total_t);
	
		if(r < 0){
		}
	
	//printf("%d %ld \n",sizes[a], times[a]);

	//times[a] = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;	
		
exit(1);
}
