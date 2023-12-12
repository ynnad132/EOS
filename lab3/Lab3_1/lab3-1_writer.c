#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
int main(int argc,char* argv[]){
	
	int fd;
	fd=open("/dev/etx_device",O_RDWR);
	write(fd,argv[1],10);
	close(fd);
	return 0;
	
}
