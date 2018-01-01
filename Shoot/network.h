#ifndef __NETWORK_H_
#define __NETWORK_H_

#include <inttypes.h>
#define MAXBUF 1000
void *internal_server(void *);

#define UI_CONNECT		0x01
#define UI_DISCONNECT		0x02
#define UI_SHOOT		0x03

#define PACKET_ID_CONNECT	0x01	// Send connect with username
#define PACKET_ID_DISCONNECT	0x02	// Send disconnect
#define PACKET_ID_CONFIRMCONNECT 0x03	// Server reply with ID
#define PACKET_ID_PING		0x04

#define PACKET_ID_LOBBY_GETLIST	0x21	// Server will reply with a lobby
#define PACKET_ID_LOBBY_JOIN	0x22	// Join specific lobby
#define PACKET_ID_LOBBY_LEAVE	0x23	// Leave joined lobby
#define PACKET_ID_LOBBY_NEW	0x24	// Create new Lobby
#define PACKET_ID_LOBBY_KICK	0x25	// Kick player

// Client -> Server
#define PACKET_ID_SETPOS	0x10	// Send position to server
#define PACKET_ID_SETBULLET	0x11	// Send Server we shot a bullet
#define PACKET_ID_GETLIST	0x12	// Ask server: "Give me the player list"
// Server -> Client
#define PACKET_ID_PUTPOS	0x13	// Server position fix
#define PACKET_ID_SENDPLAYER	0x14	// Server reply: Player data
#define PACKET_ID_KILL		0x15	// Server broadcast: Who killed who
#define PACKET_ID_REMPLAYER	0x16	// Remove player

typedef struct {
	uint8_t ID;
	uint8_t DATA[MAXBUF-1];
} PACKET;

#endif
