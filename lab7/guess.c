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
#include <semaphore.h>
#include <pthread.h>
#include <signal.h> 
#define errexit(format,arg...) exit(printf(format,##arg))
typedef struct num_data{
	int guess;
	char result[8];
}Data;

int gamepid, upper,pre_upper,shmid;
Data * data;
void handler(int );
void timer_handler(int );

int main(int argc,char *argv[]){
	key_t shm_key = atoi(argv[1]);
	upper = atoi(argv[2]);
	gamepid = atoi(argv[3]);
	shmid = shmget(shm_key,sizeof(Data*),0666);
	data = shmat(shmid,NULL,0);
	data->guess = upper/2;
	//signal
	struct sigaction sa1;
	memset(&sa1,0, sizeof(struct sigaction));
	sa1.sa_handler = handler;
	sigaction(SIGUSR2, &sa1,NULL);
	//timer
	struct sigaction sa_time;
	struct itimerval timer;
	memset(&sa_time,0, sizeof(struct sigaction));
	sa_time.sa_handler = &timer_handler;
	sigaction(SIGVTALRM, &sa_time,NULL);
	timer.it_value.tv_sec = 1;
	timer.it_value.tv_usec = 0;
	timer.it_interval.tv_sec = 1;
	timer.it_interval.tv_usec = 0;
	//send SIGVTALRM signal
	setitimer(ITIMER_VIRTUAL, &timer,NULL);
	
	
	fflush(stdout);
	printf("[game] Guess %d\n",data->guess);
	kill(gamepid,SIGUSR1);
	while(1){}
	return 0;
}


void handler(int signum){
	if(signum == SIGUSR2){
		if(strcmp(data->result , "smaller")==0){
			pre_upper = data->guess;
			data->guess /=2;
		}		
		else if (strcmp(data->result , "bigger")==0){
			data->guess = (pre_upper + data->guess)/2;
		}
		else{
			shmdt(data);
			exit(0);
		}
	}
}
void timer_handler(int signum){
	fflush(stdout);
	printf("[game] Guess %d\n",data->guess);
	kill(gamepid,SIGUSR1);
}

