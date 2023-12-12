#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <string.h>
//LED is connected to this GPIO
//value
#define led0 "/sys/class/gpio/gpio20/value"	//20
#define led1 "/sys/class/gpio/gpio19/value"	//19
#define led2 "/sys/class/gpio/gpio18/value"	//18
#define led3 "/sys/class/gpio/gpio17/value"	//17
#define led4 "/sys/class/gpio/gpio16/value"	//16
#define led5 "/sys/class/gpio/gpio15/value"	//15
#define led6 "/sys/class/gpio/gpio14/value"	//14
#define led7 "/sys/class/gpio/gpio13/value"	//13
//export
#define led0_export "/sys/class/gpio/export"
#define led1_export "/sys/class/gpio/export"
#define led2_export "/sys/class/gpio//export"
#define led3_export "/sys/class/gpio/export"
#define led4_export "/sys/class/gpio/export"
#define led5_export "/sys/class/gpio/export"
#define led6_export "/sys/class/gpio/export"
#define led7_export "/sys/class/gpio/export"
//direction
#define led0_direction "/sys/class/gpio/gpio20/direction"
#define led1_direction "/sys/class/gpio/gpio19/direction"
#define led2_direction "/sys/class/gpio/gpio18/direction"
#define led3_direction "/sys/class/gpio/gpio17/direction"
#define led4_direction "/sys/class/gpio/gpio16/direction"
#define led5_direction "/sys/class/gpio/gpio15/direction"
#define led6_direction "/sys/class/gpio/gpio14/direction"
#define led7_direction "/sys/class/gpio/gpio13/direction"
//  7 seg GPIO
#define seg_a "/sys/class/gpio/gpio21/value"      //21
#define seg_b "/sys/class/gpio/gpio22/value"      //22
#define seg_c "/sys/class/gpio/gpio23/value"	  //23	
#define seg_d "/sys/class/gpio/gpio24/value"	  //24
#define seg_e "/sys/class/gpio/gpio25/value"	  //25	
#define seg_f "/sys/class/gpio/gpio26/value"      //26
#define seg_g "/sys/class/gpio/gpio27/value"	  //27	
//export
#define seg_a_export "/sys/class/gpio/export"
#define seg_b_export "/sys/class/gpio/export"
#define seg_c_export "/sys/class/gpio/export"
#define seg_d_export "/sys/class/gpio/export"
#define seg_e_export "/sys/class/gpio/export"
#define seg_f_export "/sys/class/gpio/export"
#define seg_g_export "/sys/class/gpio/export"
//direction
#define seg_a_direction "/sys/class/gpio/gpio21/direction"
#define seg_b_direction "/sys/class/gpio/gpio22/direction"
#define seg_c_direction "/sys/class/gpio/gpio23/direction"
#define seg_d_direction "/sys/class/gpio/gpio24/direction"
#define seg_e_direction "/sys/class/gpio/gpio25/direction"
#define seg_f_direction "/sys/class/gpio/gpio26/direction"
#define seg_g_direction "/sys/class/gpio/gpio27/direction"
#define _out "out"
/***************************************************/
void led(int in);
void seven_segment_led(char);
int calcualte_total(int,int,int,int);
void setup_gpip();

/******************************************************/
void setup_gpip(){
	int fd;
	fd = open(led0_export,O_WRONLY);
	write(fd,"20",sizeof("20"));
	fd = open(led0_direction,O_WRONLY);
	write(fd,_out,sizeof("out"));
	
	fd = open(led1_export,O_WRONLY);
	write(fd,"19",sizeof("19"));
	fd = open(led1_direction,O_WRONLY);
	write(fd,_out,sizeof("out"));
	
	fd = open(led2_export,O_WRONLY);
	write(fd,"18",sizeof("18"));
	fd = open(led2_direction,O_WRONLY);
	write(fd,_out,sizeof("out"));
	
	fd = open(led3_export,O_WRONLY);
	write(fd,"17",sizeof("17"));
	fd = open(led3_direction,O_WRONLY);
	write(fd,_out,sizeof("out"));
	
	fd = open(led4_export,O_WRONLY);
	write(fd,"16",sizeof("16"));
	fd = open(led4_direction,O_WRONLY);
	write(fd,_out,sizeof("out"));
	
	fd = open(led5_export,O_WRONLY);
	write(fd,"15",sizeof("15"));
	fd = open(led5_direction,O_WRONLY);
	write(fd,_out,sizeof("out"));
	
	fd = open(led6_export,O_WRONLY);
	write(fd,"14",sizeof("14"));
	fd = open(led6_direction,O_WRONLY);
	write(fd,_out,sizeof("out"));
	
	fd = open(led7_export,O_WRONLY);
	write(fd,"13",sizeof("13"));
	fd = open(led7_direction,O_WRONLY);
	write(fd,_out,sizeof("out"));
	
	fd = open(seg_a_export,O_WRONLY);
	write(fd,"21",sizeof("21"));
	fd = open(seg_a_direction,O_WRONLY);
	write(fd,_out,sizeof("out"));
	
	fd = open(seg_b_export,O_WRONLY);
	write(fd,"22",sizeof("22"));
	fd = open(seg_b_direction,O_WRONLY);
	write(fd,_out,sizeof("out"));
	
	fd = open(seg_c_export,O_WRONLY);
	write(fd,"23",sizeof("23"));
	fd = open(seg_c_direction,O_WRONLY);
	write(fd,_out,sizeof("out"));
	
	fd = open(seg_d_export,O_WRONLY);
	write(fd,"24",sizeof("24"));
	fd = open(seg_d_direction,O_WRONLY);
	write(fd,_out,sizeof("out"));
	
	fd = open(seg_e_export,O_WRONLY);
	write(fd,"25",sizeof("25"));
	fd = open(seg_e_direction,O_WRONLY);
	write(fd,_out,sizeof("out"));
	
	fd = open(seg_f_export,O_WRONLY);
	write(fd,"26",sizeof("26"));
	fd = open(seg_f_direction,O_WRONLY);
	write(fd,_out,sizeof("out"));
	
	fd = open(seg_g_export,O_WRONLY);
	write(fd,"27",sizeof("27"));
	fd = open(seg_g_direction,O_WRONLY);
	write(fd,_out,sizeof("out"));
}


/* control LED*/
void led(int in)
{	int fd;
	if(in==0){
		fd = open(led0,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led1,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led2,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led3,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led4,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led5,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led6,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led7,O_RDWR);
		write(fd,"0",sizeof("0"));
	}
	else if(in==1){
		fd = open(led0,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led1,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led2,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led3,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led4,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led5,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led6,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led7,O_RDWR);
		write(fd,"1",sizeof("1"));
	}
	else if(in==2){
		fd = open(led0,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led1,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led2,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led3,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led4,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led5,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led6,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(led7,O_RDWR);
		write(fd,"1",sizeof("1"));
	}
	else if(in==3){
		fd = open(led0,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led1,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led2,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led3,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led4,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led5,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(led6,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(led7,O_RDWR);
		write(fd,"1",sizeof("1"));
	}
	else if(in==4){
		fd = open(led0,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led1,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led2,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led3,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led4,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(led5,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(led6,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(led7,O_RDWR);
		write(fd,"1",sizeof("1"));
	}
	else if(in==5){
		fd = open(led0,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led1,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led2,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led3,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(led4,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(led5,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(led6,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(led7,O_RDWR);
		write(fd,"1",sizeof("1"));
	}
	else if(in==6){
		fd = open(led0,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led1,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led2,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(led3,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(led4,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(led5,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(led6,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(led7,O_RDWR);
		write(fd,"1",sizeof("1"));
	}
	else if(in==7){
		fd = open(led0,O_RDWR);
		write(fd,"0",sizeof("0"));
		fd = open(led1,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(led2,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(led3,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(led4,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(led5,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(led6,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(led7,O_RDWR);
		write(fd,"1",sizeof("1"));
	}
	else if(in==8){
		fd = open(led0,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(led1,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(led2,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(led3,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(led4,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(led5,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(led6,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(led7,O_RDWR);
		write(fd,"1",sizeof("1"));
	}
}



/* control 7_seg*/
void seven_segment_led(char seven_seg_in)
{
	int fd;
	if(seven_seg_in=='0'){
		fd = open(seg_a,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_b,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_c,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_d,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_e,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_f,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_g,O_RDWR);
		write(fd,"0",sizeof("1"));
	}
	else if(seven_seg_in=='1'){
		fd = open(seg_a,O_RDWR);
		write(fd,"0",sizeof("1"));
		fd = open(seg_b,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_c,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_d,O_RDWR);
		write(fd,"0",sizeof("1"));
		fd = open(seg_e,O_RDWR);
		write(fd,"0",sizeof("1"));
		fd = open(seg_f,O_RDWR);
		write(fd,"0",sizeof("1"));
		fd = open(seg_g,O_RDWR);
		write(fd,"0",sizeof("1"));
	}
	else if(seven_seg_in=='2'){
		fd = open(seg_a,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_b,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_c,O_RDWR);
		write(fd,"0",sizeof("1"));
		fd = open(seg_d,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_e,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_f,O_RDWR);
		write(fd,"0",sizeof("1"));
		fd = open(seg_g,O_RDWR);
		write(fd,"1",sizeof("1"));
	}
	else if(seven_seg_in=='3'){
		fd = open(seg_a,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_b,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_c,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_d,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_e,O_RDWR);
		write(fd,"0",sizeof("1"));
		fd = open(seg_f,O_RDWR);
		write(fd,"0",sizeof("1"));
		fd = open(seg_g,O_RDWR);
		write(fd,"1",sizeof("1"));
	}
	else if(seven_seg_in=='4'){
		fd = open(seg_a,O_RDWR);
		write(fd,"0",sizeof("1"));
		fd = open(seg_b,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_c,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_d,O_RDWR);
		write(fd,"0",sizeof("1"));
		fd = open(seg_e,O_RDWR);
		write(fd,"0",sizeof("1"));
		fd = open(seg_f,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_g,O_RDWR);
		write(fd,"1",sizeof("1"));
	}
	else if(seven_seg_in=='5'){
		fd = open(seg_a,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_b,O_RDWR);
		write(fd,"0",sizeof("1"));
		fd = open(seg_c,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_d,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_e,O_RDWR);
		write(fd,"0",sizeof("1"));
		fd = open(seg_f,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_g,O_RDWR);
		write(fd,"1",sizeof("1"));
	}
	else if(seven_seg_in=='6'){
		fd = open(seg_a,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_b,O_RDWR);
		write(fd,"0",sizeof("1"));
		fd = open(seg_c,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_d,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_e,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_f,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_g,O_RDWR);
		write(fd,"1",sizeof("1"));
	}
	else if(seven_seg_in=='7'){
		fd = open(seg_a,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_b,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_c,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_d,O_RDWR);
		write(fd,"0",sizeof("1"));
		fd = open(seg_e,O_RDWR);
		write(fd,"0",sizeof("1"));
		fd = open(seg_f,O_RDWR);
		write(fd,"0",sizeof("1"));
		fd = open(seg_g,O_RDWR);
		write(fd,"0",sizeof("1"));
	}
	else if(seven_seg_in=='8'){
		fd = open(seg_a,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_b,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_c,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_d,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_e,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_f,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_g,O_RDWR);
		write(fd,"1",sizeof("1"));
	}
	else if(seven_seg_in=='9'){
		fd = open(seg_a,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_b,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_c,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_d,O_RDWR);
		write(fd,"0",sizeof("1"));
		fd = open(seg_e,O_RDWR);
		write(fd,"0",sizeof("1"));
		fd = open(seg_f,O_RDWR);
		write(fd,"1",sizeof("1"));
		fd = open(seg_g,O_RDWR);
		write(fd,"1",sizeof("1"));
	}
	else{
		fd = open(seg_a,O_RDWR);
		write(fd,"0",sizeof("1"));
		fd = open(seg_b,O_RDWR);
		write(fd,"0",sizeof("1"));
		fd = open(seg_c,O_RDWR);
		write(fd,"0",sizeof("1"));
		fd = open(seg_d,O_RDWR);
		write(fd,"0",sizeof("1"));
		fd = open(seg_e,O_RDWR);
		write(fd,"0",sizeof("1"));
		fd = open(seg_f,O_RDWR);
		write(fd,"0",sizeof("1"));
		fd = open(seg_g,O_RDWR);
		write(fd,"0",sizeof("1"));

	}
		
}

int calcualte_total(int price1,int num1,int price2,int num2){
	int ans = 0;
	ans = price1*num1 + price2*num2;	
	return ans;
}

int main(){
	
	setup_gpip();
	int i = 0;
	int opt = -1;  
	int shop = -1;  // choose what shop
	int choose_order = -1;  // choose_order
	
	int shop_loop = -1;  
	
	int dessert1_num=0,dessert2_num=0;
	int total_num = 0;
	
	char total_str[20]={};
	int numsize = 0;
	
	while(opt!=0){
	
		printf("------------------------------\n");
		printf("1. shop list\n");
		printf("2. order\n");
		scanf("%d",&opt);
		
		switch(opt){
			
			case 1:
				printf("Dessert shop : 3km\n");
				printf("Beverage shop : 5km\n");
				printf("Dinner: 8km\n");
				break;
			
			case 2:
				printf("please choose from 1~3\n");
				printf("1. Dessert shop\n");
				printf("2. Beverage shop\n");
				printf("3. Dinner\n");
				
				scanf("%d",&shop);
				
				switch(shop){
					
					case 1:
						shop_loop = 1;
						while(shop_loop!=0){
							printf("please choose from 1~4\n");
							printf("1. cookie: $60 \n");
							printf("2. cake: $80 \n");
							printf("3. confirm \n");
							printf("4. cancel \n");
						
							scanf("%d",&choose_order);
							
							switch(choose_order){
								case 1:
									printf("How many?\n");
									scanf("%d",&dessert1_num);
									break;
								case 2:
									printf("How many?\n");
									scanf("%d",&dessert2_num);
									break;
								case 3:
									total_num = calcualte_total(60,dessert1_num,80,dessert2_num);
									
									sprintf(total_str,"%d",total_num);
									numsize = strlen(total_str);
									for(int j=0;j<numsize;j++){
										seven_segment_led(total_str[j]);
										usleep(500000);
									}
									
									printf("Please wait for few miuntes\n");
									
									for(int k=3;k>=0;k--){
										led(k);
										sleep(1);
									}
									
									shop = -1;
									dessert1_num = 0;
									dessert2_num = 0;
									choose_order=-1;
									shop_loop = 0;
									total_num = 0;
									numsize = 0;
									memset(total_str,'0',20);
									seven_segment_led('a');
									break;
								case 4:
									printf("cancel\n");
									dessert1_num = 0;
									dessert2_num = 0;
									choose_order=-1;
									shop_loop = 0;
									total_num = 0;
									numsize = 0;
									memset(total_str,'0',20);
									break;
								default:
									printf("choose again\n");
									break;
								
							}
						}
						break;
					case 2:
						shop_loop = 1;
						while(shop_loop!=0){
							printf("please choose from 1~4\n");
							printf("1. tea: $40 \n");
							printf("2. cake: $70 \n");
							printf("3. confirm \n");
							printf("4. cancel \n");
						
							scanf("%d",&choose_order);
							
							switch(choose_order){
								case 1:
									printf("How many?\n");
									scanf("%d",&dessert1_num);
									break;
								case 2:
									printf("How many?\n");
									scanf("%d",&dessert2_num);
									break;
								case 3:
									total_num = calcualte_total(40,dessert1_num,70,dessert2_num);
									sprintf(total_str,"%d",total_num);
									numsize = strlen(total_str);
									for(int l=0;l<numsize;l++){
										seven_segment_led(total_str[l]);
										usleep(500000);
									}
									
									printf("Please wait for few miuntes\n");
									
									for(int m=5;m>=0;m--){
										led(m);
										sleep(1);
									}
									
									shop = -1;
									dessert1_num = 0;
									dessert2_num = 0;
									choose_order=-1;
									shop_loop = 0;
									total_num = 0;
									numsize = 0;
									memset(total_str,'0',20);
									seven_segment_led('a');
									break;
								case 4:
									printf("cancel");
									dessert1_num = 0;
									dessert2_num = 0;
									choose_order=-1;
									shop_loop = 0;
									total_num = 0;
									numsize = 0;
									memset(total_str,'0',20);											
									break;
								default:
									printf("choose again\n");
									break;
								
							}
						}
						break;
					case 3:
						shop_loop = 1;
						while(shop_loop!=0){
							printf("please choose from 1~4\n");
							printf("1. fried rice: $120 \n");
							printf("2. egg_drop soup: $50 \n");
							printf("3. confirm \n");
							printf("4. cancel \n");
						
							scanf("%d",&choose_order);
							
							switch(choose_order){
								case 1:
									printf("How many?\n");
									scanf("%d",&dessert1_num);
									break;
								case 2:
									printf("How many?\n");
									scanf("%d",&dessert2_num);
									break;
								case 3:
									total_num = calcualte_total(120,dessert1_num,50,dessert2_num);
									sprintf(total_str,"%d",total_num);
									numsize = strlen(total_str);
									for(int n=0;n<numsize;n++){
										seven_segment_led(total_str[n]);
										usleep(500000);
									}
									
									printf("Please wait for few miuntes\n");
									
									for(int o=8;o>=0;o--){
										led(o);
										sleep(1);
									}
									
									shop = -1;
									dessert1_num = 0;
									dessert2_num = 0;
									choose_order=-1;
									shop_loop = 0;
									total_num = 0;
									numsize = 0;
									memset(total_str,'0',20);
									seven_segment_led('a');
									break;
								case 4:
									printf("cancel\n");
									dessert1_num = 0;
									dessert2_num = 0;
									choose_order=-1;
									shop_loop = 0;
									total_num = 0;
									numsize = 0;
									memset(total_str,'0',20);
									break;
								default:
									printf("choose again\n");
									break;
								
							}
						}
						break;
					
					
						
					default:
						printf("choose again\n");
						break;
				}
				break;
				
		
		
			default:
				printf("choose again,please chose 1~2\n");
				break;
		}
	}	
	return 0;
}





