#ifndef __MAIN_H
#define __MAIN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define WIN_HEIGHT	720
#define WIN_WIDTH	1280

#define MAP_MAXWIDTH	5
#define MAP_MAXHEIGHT	5
#define MAP_TILESIZE	64 	// 64x64

#define MAXPLAYERS 	4
#define MAXBULLETS 	100

#define MAX_VEL_X_SPEED	10
#define MAX_VEL_Y_SPEED 10

#define MAP_ID_GRASS	0x01

typedef struct {
	int x,y;
	char width;
	char can_jump,JUMP_POWER;
	int vel_y,vel_x,speed;
} Local_Player;

typedef struct {
	int x,y,width;
} Player;

typedef struct {
	int x,y;
} Bullet;

typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;
	int running,delta;
	double fps,tps;
	TTF_Font *font;
	SDL_Surface *Surface_Terrain;
	SDL_Texture *Texture_Terrain;
	Local_Player player; double gravity;
	Player players[MAXPLAYERS];
	Bullet bullets[MAXBULLETS];
	int map[MAP_MAXHEIGHT][MAP_MAXWIDTH];
	int portl,ports;
	char ipv4[16];
	int ping;
} GAME_CORE;
#endif
