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
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#define errexit(format,arg...) exit(printf(format,##arg))
#define BUFSIZE 256

typedef struct {
	int customer_num;
	int income;
}DeliveryIncomeandnum;	

typedef struct {
	int time;
	pthread_mutex_t person_lock;
}Deliveryperson;



/*creat server*/
int passivesock(const char *, const char * ,int );

// handler 
void handler_sigint(int );
int calcualte_total(int,int,int,int);

//semaphore
sem_t sem_time;
sem_t sem_income;


//delivery
//int delivery_time[2]={0};
DeliveryIncomeandnum deliveryincomenum;
Deliveryperson delivery_person[2];
int find_available_delivery();


//mutex
pthread_mutex_t income_lock;
pthread_mutex_t determin_min_lock;
pthread_mutex_t sub_time_lock;
int delivery_index=-1;



//thread func
void *thread_func(void *arg){
	
	
	//meal information
	char Function[4][15] = {"shop","order","confirm","cancel"};
	int dessert_cost[6]={60,80,40,70,120,50};
	char buffer[BUFSIZE];
	
	int connfd = *(int*)arg;
	free(arg);
	int func_bit[4]={0};
	char temp_meal[2][15];
	char result_meal[4][15];
	int first_order=1;
	int determine_restaurnt[3]={0};
	int distance_restaurnt[3] = {3,5,8};
	
	// dessert
	int dessert1_index=0,dessert2_index=0;
	int dessert1_num=0,dessert2_num=0;
	int order_yet=0;
	int total_cost=0;
	char total_str[20]={};
	
	//delivery
	int index_available=-1;
	int wait_time=0;
	
	char reply_msg[BUFSIZE];
	memset(reply_msg,'\0',BUFSIZE);
	memset(temp_meal,'\0',sizeof(temp_meal));
	memset(result_meal,'\0',sizeof(result_meal));
	

	//start
	while(1){
		memset(buffer,'\0',BUFSIZE);
		if(read(connfd,buffer,BUFSIZE) <0){
			printf("read error\n");
		};
		//buffer[strlen(buffer)-1] = '\0';
		//usleep(1000);
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
					substr = strtok(NULL," ");
					strncpy(temp_meal[j],substr,15);
					strncpy(result_meal[j],substr,15);
				}
				//write to client
				strcat(reply_msg,temp_meal[0]);
				strcat(reply_msg," ");
				strcat(reply_msg,temp_meal[1]);
				//strcat(reply_msg,"\n");                                                     //////////////*********here***********/////////////////////
				memset(buffer,'\0',BUFSIZE);
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
						substr = strtok(NULL," ");
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
						memset(reply_msg,'\0',sizeof(reply_msg));
						write(connfd, buffer,strlen(buffer)+1);
						memset(func_bit,0,sizeof(func_bit));
						continue;
						
					}
					////////////////// dup EGG
					else if((determine_restaurnt[2]==1) && (dessert1_index == 5) && (strcmp(temp_meal[0],"Egg-drop-soup")==0)){
						dessert1_num += atoi(temp_meal[1]);
						sprintf(total_str,"%d",dessert1_num);
						strncpy(result_meal[1],total_str,15);
						// write reply message
						strcat(reply_msg,result_meal[0]);
						strcat(reply_msg," ");
						strcat(reply_msg,result_meal[1]);
						//strcat(reply_msg,"\n");                               //////////////*********here***********/////////////////////
						strncpy(buffer,reply_msg,BUFSIZE);
						memset(reply_msg,'\0',sizeof(reply_msg));
						write(connfd, buffer,strlen(buffer)+1);
						memset(func_bit,0,sizeof(func_bit));
						continue;
						
					}
					////////////////// dup boba
					else if((determine_restaurnt[1]==1) && (dessert1_index == 3) && (strcmp(temp_meal[0],"boba")==0)){
						dessert1_num += atoi(temp_meal[1]);
						sprintf(total_str,"%d",dessert1_num);
						strncpy(result_meal[1],total_str,15);
						// write reply message
						strcat(reply_msg,result_meal[0]);
						strcat(reply_msg," ");
						strcat(reply_msg,result_meal[1]);
						//strcat(reply_msg,"\n");                               //////////////*********here***********/////////////////////
						strncpy(buffer,reply_msg,BUFSIZE);
						//memset(reply_msg,'\0',sizeof(reply_msg));
						write(connfd, buffer,BUFSIZE);
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
					strcat(reply_msg,result_meal[0]);
					strcat(reply_msg," ");
					strcat(reply_msg,result_meal[1]);
					strcat(reply_msg,"|");
					strcat(reply_msg,result_meal[2]);
					strcat(reply_msg," ");
					strcat(reply_msg,result_meal[3]);
					memset(buffer,'\0',BUFSIZE);
					strncpy(buffer,reply_msg,BUFSIZE);
					write(connfd, buffer,BUFSIZE);
					memset(func_bit,0,sizeof(func_bit));
				}
			
			}
		//confirm    lock
		else if(func_bit[2]==1){
			if(order_yet){
				// maybe need mutex
				//////////////////////////////////////////////////////////mutex///////////////////////////////////////////////////////////////////////////
				pthread_mutex_lock(&determin_min_lock);
				//choose delivery
				index_available = find_available_delivery();
				pthread_mutex_unlock(&determin_min_lock);
				
				pthread_mutex_lock(&delivery_person[index_available].person_lock);
				
				wait_time = delivery_person[index_available].time;
				
				///determine which restaurent
				for(int a = 0;a<3;a++){
					if(determine_restaurnt[a]==1){
						wait_time += distance_restaurnt[a];
					}
				}
				
				
				
				
				// if exceed 30s
				if(wait_time>30){
					strncpy(buffer, "Your delivery will take a long time, do you want to wait?",BUFSIZE);
					write(connfd, buffer,strlen(buffer)+1);
					read(connfd,buffer,BUFSIZE);
					// if Yes
					if(strcmp(buffer,"Yes")==0){
						//maybe need a lock
						//strncpy(buffer,"Please wait a few minutes...",BUFSIZE);
						//write(connfd, buffer,strlen(buffer)+1);
					}
					//if answer no
					else{
						//////////////mutex///////////////////////////////////////////////////////////////////////////
						pthread_mutex_unlock(&delivery_person[index_available].person_lock);
						break;
					}
					//update
					delivery_person[index_available].time = wait_time;
					//pthread_mutex_unlock(&delivery_person[index_available].person_lock);
					
					strncpy(buffer,"Please wait a few minutes...",BUFSIZE);
					write(connfd, buffer,strlen(buffer)+1);
					pthread_mutex_unlock(&delivery_person[index_available].person_lock);
					sleep(wait_time);
					
					//sub time
					pthread_mutex_lock(&delivery_person[index_available].person_lock);
					for(int a = 0;a<3;a++){
						if(determine_restaurnt[a]==1){
							delivery_person[index_available].time -= distance_restaurnt[a];
						}
					}
					pthread_mutex_unlock(&delivery_person[index_available].person_lock);
					
					// calcualte_total
					total_cost = calcualte_total(dessert_cost[dessert1_index],dessert1_num,dessert_cost[dessert2_index],dessert2_num);
					memset(reply_msg,'\0',sizeof(reply_msg));
					strcat(reply_msg,"Delivery has arrived and you need to pay ");
					sprintf(total_str,"%d",total_cost);
					strcat(reply_msg,total_str);
					strcat(reply_msg,"$");
					strncpy(buffer,reply_msg,BUFSIZE);
					memset(reply_msg,'\0',sizeof(reply_msg));
					write(connfd, buffer,BUFSIZE);
				
					//delivery income    maybe need a lock
					//mutex///////////////////////////////////////////////////////////////////////////
					pthread_mutex_lock(&income_lock);
					deliveryincomenum.customer_num++;
					deliveryincomenum.income += total_cost;
					pthread_mutex_unlock(&income_lock);
					break;
				}
				
				
				
				//////mutex unlock////
				//update time
				delivery_person[index_available].time = wait_time;
				//pthread_mutex_unlock(&delivery_person[index_available].person_lock);
				strncpy(buffer,"Please wait a few minutes...",BUFSIZE);
				write(connfd, buffer,strlen(buffer)+1);
				
				
				pthread_mutex_unlock(&delivery_person[index_available].person_lock);
				sleep(wait_time);
				
				//sub time
				pthread_mutex_lock(&delivery_person[index_available].person_lock);
				for(int a = 0;a<3;a++){
						if(determine_restaurnt[a]==1){
							delivery_person[index_available].time -= distance_restaurnt[a];
					}
				}
				pthread_mutex_unlock(&delivery_person[index_available].person_lock);
				
				// calcualte_total
				total_cost = calcualte_total(dessert_cost[dessert1_index],dessert1_num,dessert_cost[dessert2_index],dessert2_num);
				memset(reply_msg,'\0',sizeof(reply_msg));
				strcat(reply_msg,"Delivery has arrived and you need to pay ");
				sprintf(total_str,"%d",total_cost);
				strcat(reply_msg,total_str);
				strcat(reply_msg,"$");
				strncpy(buffer,reply_msg,BUFSIZE);
				memset(reply_msg,'\0',sizeof(reply_msg));
				write(connfd, buffer,BUFSIZE);
				
				//delivery income    maybe need a lock
				//mutex///////////////////////////////////////////////////////////////////////////
				pthread_mutex_lock(&income_lock);
				deliveryincomenum.customer_num++;
				deliveryincomenum.income += total_cost;
				pthread_mutex_unlock(&income_lock);
				
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
			//not order
			else{
				//strncpy(buffer,"Please order some meals\n",BUFSIZE);                        //////////////*********here***********/////////////////////
				strncpy(buffer,"Please order some meals",BUFSIZE);
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
			order_yet = 0;
			dessert1_index = 0;
			dessert2_index = 0;
			dessert1_num = 0;
			dessert2_num = 0;
			total_cost = 0;
			break;
		}	
	}
	close(connfd);
}



int main(int argc,char *argv[]){
	
	
	struct sockaddr_in addr_cln;
	socklen_t sLen = sizeof(addr_cln);
	int sockfd,connfd;
	
	sem_init(&sem_time,0,1);
	sem_init(&sem_income,0,1);
	//initial
	pthread_mutex_init(&delivery_person[0].person_lock,NULL);
	pthread_mutex_init(&delivery_person[1].person_lock,NULL);
	pthread_mutex_init(&income_lock,NULL);
	pthread_mutex_init(&determin_min_lock,NULL);
	pthread_mutex_init(&sub_time_lock,NULL);
	signal(SIGINT,handler_sigint);
	
	deliveryincomenum.customer_num = 0;
	deliveryincomenum.income = 0;
	
	if(argc!=2)
		errexit("Usage: %s port \n",argv[0]);
	
	// wait for connection
	sockfd = passivesock(argv[1],"tcp",5);
	
	while(1){
		if((connfd = accept(sockfd,(struct sockaddr *) &addr_cln,&sLen))>0){
			pthread_t thread_id;
			int* client = (int*)malloc(sizeof(int));
			*client = connfd;
			pthread_create(&thread_id,NULL,thread_func,client);
			//pthread_detach(thread_id);
		};
	}
	close(sockfd);
	return 0;
}

//signal handler for SIGINT
void handler_sigint(int sig){
	FILE *file = fopen("result.txt", "w");
	if(file != NULL){
		fprintf(file,"customer: %d\nincome: %d$\n",deliveryincomenum.customer_num,deliveryincomenum.income);
	}
	exit(0);
}

// available delivery
int find_available_delivery(){
	int index = -1;
	if(delivery_person[1].time >= delivery_person[0].time){
		index = 0;	
	}
	else{
		index = 1;
	}
	return index;
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

