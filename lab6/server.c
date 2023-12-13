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
#define errexit(format,arg...) exit(printf(format,##arg))
#define BUFSIZE 1024
#define SEM_MODE 0666
#define SEM_KEY 1122334455
typedef struct num_data{
	int num_d;
	int time_d;
}Data;

typedef struct{
	pthread_t thread_id;
	int num;
	int time;
}thread_info_t;	


int sockfd, connfd;
int *shm, s, num;
int shmid, sem;
//char buffer[BUFSIZE];
pid_t childpid;
//key_t shm_key = 1234;
//Data buffer;
int total_money=0;
int num_threads = 5;
/*creat server*/
int passivesock(const char *, const char * ,int );
void singalHandler(int );
void Increment(int , int );
sem_t semp;

void *thread_func(void *arg){
	int time,num;
	thread_info_t *data;
	data = (thread_info_t *)arg;
	num = data->num;
	time = data->time;
	Increment(num,time);
	return NULL;
}




void Increment(int num , int time){
	while(time){
		sem_wait(&semp);
		if(num>0){
			
			total_money += num;
			printf("After deposit: %d\n",total_money);
		}
		else{
			if((total_money + num) < 0){
				sem_post(&semp);
				//usleep(10);
				continue;	
			}
			total_money += num;
			printf("After withdraw: %d\n",total_money);	
		}
		sem_post(&semp);
		usleep(1000);
		time--;
	}
}



int main(int argc,char *argv[]){
	int num,time;
	int i=0;
	void *status;
	struct sockaddr_in addr_cln;
	socklen_t sLen = sizeof(addr_cln);
	sem_init(&semp,0,1);
	
	signal(SIGINT,singalHandler);
	
	// wait for connection
	sockfd = passivesock(argv[1],"tcp",5);
	thread_info_t *thread_info = (thread_info_t *)malloc(num_threads*sizeof(thread_info_t));
	
	
	while(1){
		connfd = accept(sockfd, (struct sockaddr *)&addr_cln, &sLen);
		Data *buffer = (Data *)malloc(sizeof(Data));
		if(recv(connfd,buffer,sizeof(buffer),0)>0){
			thread_info[i].num = buffer->num_d;
			thread_info[i].time = buffer->time_d;
			pthread_create(&thread_info[i].thread_id,NULL,thread_func,&thread_info[i]);
			pthread_detach(thread_info[i].thread_id);
			i++;
		}
		
				
	}
	close(sockfd);	
	return 0;
}


void singalHandler(int signum){
	sem_destroy(&semp);
	close(sockfd);
	exit(1);
}

int passivesock(const char *service, const char *transport ,int qlen){
	struct servent *pse;
	struct sockaddr_in sin;
	int s ,type;
	
	memset(&sin,0,sizeof(sin));
	sin.sin_family = AF_INET;     //AF_INET = IPv4
	sin.sin_addr.s_addr = INADDR_ANY;
	
	/*MAP SERVICE NAME TO PORT NUMBER*/
	if((pse = getservbyname(service, transport)))
		sin.sin_port=htons(ntohs((unsigned short)pse->s_port));
	else if((sin.sin_port=htons((unsigned short)atoi(service)))==0)
		errexit("can't find \"%s\" srvice entry\n",service);
	
	
	if(strcmp(transport,"udp")==0)
		type = SOCK_DGRAM;
	else
		type = SOCK_STREAM;
	
	/*allocate a socket*/
	s = socket(PF_INET,type,0);
	
	int y= 1;
	setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&y,sizeof(y));
	
	if(s<0)
		errexit("can't create socket: %s\n",strerror(errno));
	
	/*bind a socket*/
	if(bind(s, (struct sockaddr *)&sin, sizeof(sin))<0)
		errexit("can't bind to port%s:%s\n",service, strerror(errno));
	
	/*set the maximum number of waiting connection*/
	if(type == SOCK_STREAM && listen(s,qlen)<0)
		errexit("can't listen on port%s:%s\n",service, strerror(errno));	
	
	return s;
}
