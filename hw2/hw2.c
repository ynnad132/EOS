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
#define BUFSIZE 256

/*creat server*/
int passivesock(const char *, const char * ,int );
/*connect to server*/
int connectsock(const char *, const char * ,int );
void handler(int);
int calcualte_total(int,int,int,int);


char Function[4][15] = {"shop","order","confirm","cancel"};
int dessert_cost[6]={60,80,40,70,120,50};



void handler(int signum){
	while(waitpid(-1,NULL,WNOHANG)>0);
}


int main(int argc,char *argv[]){
	
	char buffer[BUFSIZE];
	struct sockaddr_in addr_cln;
	socklen_t sLen = sizeof(addr_cln);
	int sockfd,connfd;
	
	int func_bit[4]={0};
	char temp_meal[2][15];
	char result_meal[4][15];
	int first_order=1;
	int determine_restaurnt[3]={0};
	
	
	// dessert
	int dessert1_index=0,dessert2_index=0;
	int dessert1_num=0,dessert2_num=0;
	int order_yet=0;
	int total_cost=0;
	char total_str[20]={};
	
	
	char reply_msg[BUFSIZE];
	memset(reply_msg,'\0',BUFSIZE);
	memset(temp_meal,'\0',sizeof(temp_meal));
	memset(result_meal,'\0',sizeof(result_meal));
	if(argc!=2)
		errexit("Usage: %s port \n",argv[0]);
	
	// wait for connection
	sockfd = passivesock(argv[1],"tcp",5);
	
	while(1){
		connfd = accept(sockfd,(struct sockaddr *) &addr_cln,&sLen);
		memset(func_bit,0,sizeof(func_bit));
		order_yet = 0;
		
		while(1){
			read(connfd,buffer,BUFSIZE);
			//printf("%s\n",buffer);
			//split buffer command
			char *substr;
			substr = strtok(buffer," |");
			
			//read which function is be set
			for(int i=0;i<4;i++){
				if(strcmp(substr,Function[i])==0){
					func_bit[i]=1;
				}
			}
			
			//shop list
			if(func_bit[0]==1){                                              
				strcpy(buffer, "Dessert shop:3km\n- cookie:$60|cake:$80\nBeverage shop:5km\n- tea:$40|boba:$70\nDiner:8km\n- fried-rice:$120|Egg-drop-soup:$50\n");
				write(connfd, buffer,strlen(buffer)+1);
				memset(func_bit,0,sizeof(func_bit));
			
			}
			
			//order
			else if(func_bit[1]==1){
				
				if(first_order==1){
					for(int j=0;j<2;j++){
						substr = strtok(NULL," |");
						strncpy(temp_meal[j],substr,15);
						strncpy(result_meal[j],substr,15);
					}
					//write to client
					strcat(reply_msg,temp_meal[0]);
					strcat(reply_msg," ");
					strcat(reply_msg,temp_meal[1]);
					strcat(reply_msg,"\n");
					strncpy(buffer,reply_msg,BUFSIZE);
					memset(reply_msg,'\0',sizeof(reply_msg));
					write(connfd, buffer,strlen(buffer)+1);
					
					//determine which restaurnt
					if(strcmp(temp_meal[0],"cookie")==0){
						determine_restaurnt[0]=1;
						dessert1_index = 0;
						dessert1_num = atoi(temp_meal[1]);
					}
					else if(strcmp(temp_meal[0],"cake")==0){
						determine_restaurnt[0]=1;
						dessert1_index = 1;
						dessert1_num = atoi(temp_meal[1]);
					}
					else if(strcmp(temp_meal[0],"tea")==0){
						determine_restaurnt[1]=1;
						dessert1_index = 2;
						dessert1_num = atoi(temp_meal[1]);
					}
					else if(strcmp(temp_meal[0],"boba")==0){
						determine_restaurnt[1]=1;
						dessert1_index = 3;
						dessert1_num = atoi(temp_meal[1]);
					}
					else if(strcmp(temp_meal[0],"fried-rice")==0){
						determine_restaurnt[2]=1;
						dessert1_index = 4;
						dessert1_num = atoi(temp_meal[1]);
					}
					else if(strcmp(temp_meal[0],"Egg-drop-soup")==0){
						determine_restaurnt[2]=1;
						dessert1_index = 5;
						dessert1_num = atoi(temp_meal[1]);
					}
					order_yet=1;
					
					// initial								
					memset(temp_meal,'\0',sizeof(temp_meal));
					memset(func_bit,0,sizeof(func_bit));
					first_order=0;
				}
				
				else if(first_order!=1){
					for(int j=0;j<2;j++){
						substr = strtok(NULL," |");
						strncpy(temp_meal[j],substr,15);
					}
			
					if((determine_restaurnt[0]==1) && (dessert1_index == 0) && (strcmp(temp_meal[0],"cake")==0)){
						dessert2_index = 1;
						dessert2_num = atoi(temp_meal[1]);
						for(int k=0;k<2;k++){
							strncpy(result_meal[k+2],temp_meal[k],15);
						}
					}
					////////////////// dup cookie
					else if((determine_restaurnt[0]==1) && (dessert1_index == 0) && (strcmp(temp_meal[0],"cookie")==0)){
						dessert1_num += atoi(temp_meal[1]);
						sprintf(total_str,"%d",dessert1_num);
						strncpy(result_meal[1],total_str,15);
						// write reply message
						strcat(reply_msg,result_meal[0]);
						strcat(reply_msg," ");
						strcat(reply_msg,result_meal[1]);
						strcat(reply_msg,"\n");
						strncpy(buffer,reply_msg,BUFSIZE);
						// memset(reply_msg,'\0',sizeof(reply_msg));
						write(connfd, buffer,strlen(buffer)+1);
						memset(func_bit,0,sizeof(func_bit));
						continue;
						
					}
					
					else if((determine_restaurnt[0]==1) && (dessert1_index == 1) && (strcmp(temp_meal[0],"cookie")==0)){
						dessert2_index = 0;
						dessert2_num = atoi(temp_meal[1]);
						for(int k=0;k<2;k++){
							strncpy(result_meal[k+2],temp_meal[k],15);
						}
					}
					else if((determine_restaurnt[1]==1) && (dessert1_index == 2) && (strcmp(temp_meal[0],"boba")==0)){
						dessert2_index = 3;
						dessert2_num = atoi(temp_meal[1]);
						for(int k=0;k<2;k++){
							strncpy(result_meal[k+2],temp_meal[k],15);
						}
					}
					else if((determine_restaurnt[1]==1) && (dessert1_index == 3) && (strcmp(temp_meal[0],"tea")==0)){
						dessert2_index = 2;
						dessert2_num = atoi(temp_meal[1]);
						for(int k=0;k<2;k++){
							strncpy(result_meal[k+2],temp_meal[k],15);
						}
					}
					else if((determine_restaurnt[2]==1) && (dessert1_index == 4) && (strcmp(temp_meal[0],"Egg-drop-soup")==0)){
						dessert2_index = 5;
						dessert2_num = atoi(temp_meal[1]);
						for(int k=0;k<2;k++){
							strncpy(result_meal[k+2],temp_meal[k],15);
						}
					}
					else if((determine_restaurnt[2]==1) && (dessert1_index == 5) && (strcmp(temp_meal[0],"fried-rice")==0)){
						dessert2_index = 4;
						dessert2_num = atoi(temp_meal[1]);
						for(int k=0;k<2;k++){
							strncpy(result_meal[k+2],temp_meal[k],15);
						}
					}
					// not belong this restaurnt  1  
					else if((determine_restaurnt[0]==1) && (strcmp(temp_meal[0],"cake")!=0)&&(strcmp(temp_meal[0],"cookie")!=0)){
						strncpy(buffer,reply_msg,BUFSIZE);
						write(connfd, buffer,strlen(buffer)+1);
						memset(func_bit,0,sizeof(func_bit));
						continue;
					}
					// not belong this restaurnt  2  
					else if((determine_restaurnt[1]==1) && (strcmp(temp_meal[0],"tea")!=0)&&(strcmp(temp_meal[0],"boba")!=0)){
						strncpy(buffer,reply_msg,BUFSIZE);
						write(connfd, buffer,strlen(buffer)+1);
						memset(func_bit,0,sizeof(func_bit));
						continue;
					}
					// not belong this restaurnt  3 
					else if((determine_restaurnt[2]==1) && (strcmp(temp_meal[0],"fried-rice")!=0)&&(strcmp(temp_meal[0],"Egg-drop-soup")!=0)){
						strncpy(buffer,reply_msg,BUFSIZE);
						write(connfd, buffer,strlen(buffer)+1);
						memset(func_bit,0,sizeof(func_bit));
						continue;
					}
						
					// write reply message
					strcat(reply_msg,result_meal[2]);
					strcat(reply_msg," ");
					strcat(reply_msg,result_meal[3]);
					strcat(reply_msg,"|");
					strcat(reply_msg,result_meal[0]);
					strcat(reply_msg," ");
					strcat(reply_msg,result_meal[1]);
					strcat(reply_msg,"\n");
					strncpy(buffer,reply_msg,BUFSIZE);
					
					write(connfd, buffer,strlen(buffer)+1);
					memset(func_bit,0,sizeof(func_bit));
				}
			
			}
			// confirm
			else if(func_bit[2]==1){
				if(order_yet){
					strncpy(buffer,"Please wait a few minutes...\n",BUFSIZE);
					write(connfd, buffer,strlen(buffer)+1);
					
					total_cost = calcualte_total(dessert_cost[dessert1_index],dessert1_num,dessert_cost[dessert2_index],dessert2_num);
					if(determine_restaurnt[0]==1){
						sleep(3);
					}
					else if(determine_restaurnt[1]==1){
						sleep(5);
					}
					else if(determine_restaurnt[2]==1){
						sleep(8);
					}
					
					memset(reply_msg,'\0',sizeof(reply_msg));
					strcat(reply_msg,"Delivery has arrived and you need to pay ");
					sprintf(total_str,"%d",total_cost);
					strcat(reply_msg,total_str);
					strcat(reply_msg,"$");
					strcat(reply_msg,"\n");
					strncpy(buffer,reply_msg,BUFSIZE);
					memset(reply_msg,'\0',sizeof(reply_msg));
					write(connfd, buffer,strlen(buffer)+1);
					
					
					// initial
					memset(temp_meal,'\0',sizeof(temp_meal));
					memset(result_meal,'\0',sizeof(result_meal));
					memset(func_bit,0,sizeof(func_bit));
					memset(determine_restaurnt,0,sizeof(determine_restaurnt));
					first_order=1;
					dessert1_index = 0;
					dessert2_index = 0;
					dessert1_num = 0;
					dessert2_num = 0;
					total_cost = 0;
					break;
				
				}
				
				else{
					strncpy(buffer,"Please order some meals\n",BUFSIZE);
					write(connfd, buffer,strlen(buffer)+1);
					memset(func_bit,0,sizeof(func_bit));
					first_order=1;
				}
				
			
			}
			//cancel 
			else if(func_bit[3]==1){
				// initial
				memset(temp_meal,'\0',sizeof(temp_meal));
				memset(result_meal,'\0',sizeof(result_meal));
				memset(func_bit,0,sizeof(func_bit));
				memset(determine_restaurnt,0,sizeof(determine_restaurnt));
				memset(reply_msg,'\0',sizeof(reply_msg));
				first_order=1;
				dessert1_index = 0;
				dessert2_index = 0;
				dessert1_num = 0;
				dessert2_num = 0;
				total_cost = 0;
				break;
			
			}
		}	
	}
	return 0;
}


int calcualte_total(int price1,int num1,int price2,int num2){
	int ans = 0;
	ans = price1*num1 + price2*num2;	
	return ans;
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

