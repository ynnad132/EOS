#include <errno.h> /* Errors */
#include <stdio.h> /* Input/Output */
#include <stdlib.h> /* General Utilities */
#include <sys/types.h> /* Primitive System Data Types */
#include <sys/wait.h> /* Wait for Process Termination */
#include <unistd.h> /* Symbolic Constants */
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <signal.h>
#define errexit(format,arg...) exit(printf(format,##arg))

#define BUFSIZE 1024
pid_t childpid;


/*creat server*/
int passivesock(const char *, const char * ,int );
/*connect to server*/
int connectsock(const char *, const char * ,int );
void handler(int);


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



void handler(int signum){
	while(waitpid(-1,NULL,WNOHANG)>0);
}




int main(int argc,char *argv[]){

	int sockfd,connfd;
	struct sockaddr_in addr_cln;
	socklen_t sLen = sizeof(addr_cln);
	int n;
	char snd[BUFSIZE],rev[BUFSIZE];
	
	if(argc!=2)
		errexit("Usage: %s port \n",argv[0]);
	
	/*create socket and bind socket to port*/
	sockfd=passivesock(argv[1],"tcp",10);
	
	while(1){
	
		/*delete zombie processes*/
		signal(SIGCHLD,handler);
	
		/*waiting for connection*/
		connfd = accept(sockfd,(struct sockaddr *) &addr_cln,&sLen);
		if(connfd== -1){
			errexit("error: accept()\n");
		}
		childpid = fork();
							
		if(childpid == -1){
			errexit("error: fork()\n");
		}
		else if(childpid == 0){
			printf("Train ID: %d\n",getpid());
			dup2(connfd,STDOUT_FILENO);
			execlp("sl","sl","-l",NULL);
			close(connfd);
			exit(0);
		}
		
		
		
	}
	
	close(sockfd);
	return 0;
}































