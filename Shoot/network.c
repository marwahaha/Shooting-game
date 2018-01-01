#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <fcntl.h>

#include "main.h"
#include "network.h"

void handleServer(char *buf, size_t r) {

}

void handleUI(char *buf,size_t r) {
	PACKET *p = malloc(sizeof(PACKET));
	memcpy(p,buf,r);
	switch(p->ID) {
		case (UI_CONNECT): // UI entered IP and wants to connect
			
			break;
	}
	free(p);
}

void *internal_server(void *args) {
	GAME_CORE *core = (void *)args;
	struct sockaddr_in sime,sicl,sise;
	int s,slen=sizeof(sicl),ss;
	if ((s=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP))!=0) {
		return (void *)-1;
	}
	if ((ss=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP))!=0) {
		return (void *)-1;
	}
	// Could not set non blocking, We need this if we have packet dropso
	if (fcntl(s,F_SETFL,O_NONBLOCK)!=0) return (void *)-2;
	if (fcntl(ss,F_SETFL,O_NONBLOCK)!=0) return (void *)-2;
	sime.sin_family = AF_INET;
	sime.sin_addr.s_addr = inet_addr("127.0.0.1");
	sime.sin_port = htons(core->portl);
	sise.sin_family = AF_INET;
        sise.sin_addr.s_addr = inet_addr(core->ipv4);
        sise.sin_port = htons(core->ports);
	if (bind(s,(struct sockaddr *)&sime,slen)!=0) {
		return (void *)-3;
	}
	char *buf = malloc(MAXBUF);
	fd_set set;
	PACKET *p =malloc(sizeof(PACKET));
	Player *pl = malloc(sizeof(Player));
	while (1) {
		FD_ZERO(&set);
		FD_SET(s,&set);
		// SERVER TICKS
		struct timeval tv;
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		int se = select(s+1,&set,NULL,NULL,&tv);
		if (se<0) {
			// Select error. Let's ignore it
		} else if (se==0) {
			// Send Pos to server
			p->ID=PACKET_ID_SETPOS;
			pl->x = core->player.x;
			pl->y = core->player.y;
			memcpy(p->DATA,pl,sizeof(Player));
			sendto(s,buf,MAXBUF,0,(struct sockaddr *)&sise,slen);
		} else {
			// Got a message from UI
			int r = recvfrom(s,buf,MAXBUF,0,(struct sockaddr *)&sicl,&slen);
			if (r>0) {
				handleUI(buf,r);
			}
		}
	}
	free(pl);
	free(p);
	free(buf);
}

