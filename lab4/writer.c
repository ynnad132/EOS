#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
int main(int argc,char* argv[]){
	
	int fd;
	fd=open("/dev/lab4_led",O_RDWR);
	write(fd,argv[1],5);
	close(fd);
	return 0;
	
}
