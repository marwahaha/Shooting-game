#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <fcntl.h>

#include "main.h"

void *internal_server(void *args) {
	struct sockaddr_in sime,sicl;
	int s, portl = (int *)args,slen=sizeof(sicl);
	if ((s=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP))!=0) {
		return (void *)-1;
	}
	// Could not set non blocking, We need this if we have packet dropso
	if (fcntl(s,F_SETFL,O_NONBLOCK)!=0) return (void *)-2;
	sime.sin_family = AF_INET;
	sime.sin_addr.s_addr = inet_addr("127.0.0.1");
	sime.sin_port = htons(portl);
	if (bind(s,(struct sockaddr *)&sime,slen)!=0) {
		return (void *)-3;
	}
	char *buf = malloc(MAXBUF);
	fd_set set;
	int ticks = 4; // 4 per sec
	double uticks = 1/(double)ticks*1000;
	while (1) {
		FD_ZERO(&set);
		FD_SET(s,&set);
		// SERVER TICKS
		struct timeval tv;
		tv.tv_sec=1;
		tv.tv_usec=(int)uticks;
		int se = select(s+1,&set,NULL,NULL,&tv);
		if (se<0) {
			// Select error. Let's ignore it
		} else if (se==0) {
			// Tick passed so update!
			
		} else {
			// Data from clients
		}
	}
	free(buf);
}

