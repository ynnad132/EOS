#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#define errexit(format,arg...) exit(printf(format,##arg))
#define BUFSIZE 1024


typedef struct num_data{
	int num;
	int time;
}Data;



/*connect to server*/
int connectsock(const char *, const char * ,const char * );

int main(int argc,char *argv[]){
	
	int connfd;
	int shmid,i;
	
	Data buf;
	
	
	connfd = connectsock(argv[1], argv[2],"tcp");
	
	
	fflush(stdout);
	
	if((strcmp(argv[3],"deposit")==0) && (strcmp(argv[4],"1")==0)){
		buf.num = 1;
		buf.time = atoi(argv[5]);
		send(connfd,&buf,sizeof(buf),0);
	
	}
	else if((strcmp(argv[3],"deposit")==0) && (strcmp(argv[4],"2")==0)){
		buf.num = 2;
		buf.time = atoi(argv[5]);
		send(connfd,&buf,sizeof(buf),0);
		
	}
	else if((strcmp(argv[3],"withdraw")==0) && (strcmp(argv[4],"1")==0)){
		buf.num = -1;
		buf.time = atoi(argv[5]);
		send(connfd,&buf,sizeof(buf),0);
	
	}
	else if((strcmp(argv[3],"withdraw")==0) && (strcmp(argv[4],"2")==0)){
		buf.num = -2;
		buf.time = atoi(argv[5]);
		send(connfd,&buf,sizeof(buf),0);
	
	}
	
	close(connfd);
	return 0;
}






int connectsock(const char *host, const char *service, const char *transport)
{
    struct hostent *phe;    
    struct servent *pse;    
    struct sockaddr_in sin; 
    int s, type;            
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;

    // 
    if ((pse = getservbyname(service, transport)))
        sin.sin_port = pse->s_port;
    else if ((sin.sin_port = htons((unsigned short)atoi(service))) == 0)
        return -1;

    // 
    if (strcmp(transport, "udp") == 0)
        type = SOCK_DGRAM;
    else
        type = SOCK_STREAM;

    if ((s = socket(PF_INET, type, 0)) < 0)
        return -1;

    
    if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
        return -1;

    return s;
}
