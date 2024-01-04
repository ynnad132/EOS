#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h> // Network related functions, e.g. gethostbyname()
#include <netinet/in.h> // struct sockaddr_in
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> // system socket define
#include <sys/types.h> // system types
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <pthread.h>
#include <signal.h> 
#define errexit(format,arg...) exit(printf(format,##arg))
#define BUFSIZE 1024
typedef struct num_data{
	int guess;
	char result[8];
}Data;

int ans,shmid;
Data *shm;

void singalHandler(int );
void handler(int ,siginfo_t* , void* );


int main(int argc,char *argv[]){
	
	printf("[game] Game PID: %d\n",getpid());
	key_t shm_key = atoi(argv[1]);
	ans = atoi(argv[2]);
	signal(SIGINT, singalHandler);
	struct sigaction guess_action;
	memset(&guess_action,0, sizeof(struct sigaction));
	guess_action.sa_flags = SA_SIGINFO;
	guess_action.sa_sigaction = handler;
	sigaction(SIGUSR1, &guess_action,NULL);
	shmid = (shmget(shm_key,sizeof(Data*),IPC_CREAT|0666));
	shm = shmat(shmid,NULL,0);
	while(1){}
	return 0;
}


void singalHandler(int signum){
	shmdt(shm);
	shmctl(shmid,IPC_RMID,NULL);
	exit(0);
}

void handler(int signo,siginfo_t *info, void* context){
	if(shm->guess ==ans){
		strcpy(shm->result, "bingo");
	}
	else if(shm->guess >ans){
		strcpy(shm->result, "smaller");
	}
	else{
		strcpy(shm->result, "bigger");
	}
	printf("[game] Guess %d, %s\n",shm->guess,shm->result);
	kill(info->si_pid,SIGUSR2);
}

