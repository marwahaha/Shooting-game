#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
	int ticks = 4;
	double uticks = 1/(double)ticks*1000;
	printf("%d %.5f\n",(int)uticks,uticks);
}

int loop(int argc, char *argv[]) {
	struct sockaddr_in sime,sicl;
	int s = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP),slen = sizeof(sicl);
	if (s<0) {
		printf("ERROR: No socket! %d\n",s);
		return -1;
	}
	sime.sin_family = AF_INET;
	sime.sin_addr.s_addr = INADDR_ANY;
	sime.sin_port = htons(1234);
	if (bind(s,(struct sockaddr *)&sime,sizeof(sime))!=0){
		printf("Error: Could not bind port\n");
		return -2;
	}
	if (fcntl(s,F_SETFL,O_NONBLOCK)!=0) printf("ERROR NON BLOCKING\n");
 	fd_set set;
	while (1) {
		FD_ZERO(&set);
		FD_SET(s,&set);
		struct timeval tv;
		tv.tv_sec=1;
		tv.tv_usec = 0;
		int se = select(s+1,&set,NULL,NULL,&tv);
		if (se<0) {
			printf("Select error\n");
		} else if (se==0) {
			printf("Timeout\n");
		} else {
			printf("Received shit\n");
			//Activity on seocket
			char *buf = malloc(100);
			int r = recvfrom(s,buf,100,0,(struct sockaddr *)&sicl,&slen);
			printf("Recvfrom: (%d)%s\n",r,buf);
			free(buf);
		}
	}
	return 0;
}
