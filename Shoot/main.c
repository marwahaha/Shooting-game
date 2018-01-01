#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <pthread.h>

#include "main.h"
#include "network.h"

void initMap(GAME_CORE *core) {
	for (int y=0;y<MAP_MAXHEIGHT;y++) {
		for (int x=0;x<MAP_MAXWIDTH;x++) {
			core->map[y][x]=0x00;
		}
	}
	for (int x=0;x<MAP_MAXWIDTH;x++) {
		core->map[1][x] = 0x01;
	}
}

void handleEvent(GAME_CORE *core) {
	SDL_Event event;
	const Uint8 *state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_SPACE]) {
		// Jump
		core->player.y--;
		if (core->player.can_jump==1) {
			core->player.vel_y=-core->gravity*core->player.JUMP_POWER;
			core->player.can_jump=0;
		}
        }/* if (state[SDL_SCANCODE_W]) {
		core->player.y--;
	} if (state[SDL_SCANCODE_S]) {
                core->player.y++;
        } */if (state[SDL_SCANCODE_A]) { // Left
                if (core->player.vel_x > -1*(MAX_VEL_X_SPEED)) core->player.vel_x-=core->player.speed;
        } if (state[SDL_SCANCODE_D]) { // Right
                if (core->player.vel_x < MAX_VEL_X_SPEED) core->player.vel_x+=core->player.speed;
        }
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				core->running=0;
			case SDL_WINDOWEVENT_CLOSE:
				if (core->window) {
					SDL_DestroyWindow(core->window);
					core->window=NULL;
				}
				core->running = 0;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
						core->running = 0;
						break;
				/*	case SDLK_w: // UP
						core->player.y-=(int)core->gravity;
						break;
					case SDLK_s: // Down
						core->player.y+=(int)core->gravity;
						break;
					case SDLK_a: // Left
						core->player.x-=(int)core->player.vel;
						break;
					case SDLK_d: // Right
						core->player.x+=(int)core->player.vel;
						break;
				*/	case SDLK_LEFT: // Shoot left
					case SDLK_RIGHT: // Shoot right
					case SDLK_UP: // Shoot up
					case SDLK_DOWN: // Shoot down
						break;
				}
		}
	}
/*	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_W]) {
		core->player.y-=(int)core->gravity;
	} else if (state[SDL_SCANCODE_S]) {
                core->player.y+=(int)core->gravity;
	} else if (state[SDL_SCANCODE_A]) {
		core->player.x-=(int)core->player.vel;
        } else if (state[SDL_SCANCODE_D]) {
		core->player.x+=(int)core->player.vel;
        }*/
}

void dispText(GAME_CORE *core, char *text, int x, int y) {
	SDL_Surface *surface;
	SDL_Texture *texture;
	SDL_Rect pos;
	SDL_Color color = {255,255,255}; //White
	surface = TTF_RenderText_Solid(core->font,text,color);
	texture = SDL_CreateTextureFromSurface(core->renderer,surface);
	pos.w = surface->w;
	pos.h = surface->h;
	pos.x = x;
	pos.y = y;
	SDL_FreeSurface(surface);
	SDL_RenderCopy(core->renderer,texture,NULL,&pos);
}

void doUpdate(GAME_CORE *core) {
	// physics // CollisionCheck
	if (core->player.x >= core->player.width/2 && core->player.x <= WIN_WIDTH-(core->player.width/2)) {
		core->player.x+=core->player.vel_x;
	} else {
		// Outside area
		if (core->player.x < core->player.width/2) {
			core->player.x=(core->player.width/2);
			core->player.vel_x = 0;
		}
		if (core->player.x > WIN_WIDTH-(core->player.width/2)) {
			core->player.x=WIN_WIDTH-(core->player.width/2);
			core->player.vel_x = 0;
		}
	}
	if (core->player.vel_x<0) core->player.vel_x++;
	if (core->player.vel_x>0) core->player.vel_x--;

	// if jumping. Slow Y velocity down by gravity
	if (core->player.can_jump==0) core->player.vel_y+=core->gravity;

	// Hit the ground
	if (core->player.y >= WIN_HEIGHT-core->player.width) {
		// Collision
		core->player.can_jump = 1;
		core->player.vel_y = 0;
		core->player.y = WIN_HEIGHT-core->player.width;
	} else {
		// no collision
/*		if (core->player.vel_y>0) {
			//Falling down
			core->player.can_jump=1;
			core->player.vel_y = 0;
		}
		if (core->player.vel_y<0) {
			//Going up
			core->player.vel_y = 0;
		}
*/	core->player.y+=core->player.vel_y;
	}
}

void doRender(GAME_CORE *core) {
	SDL_SetRenderDrawColor(core->renderer,0,0,0,0);
	SDL_RenderClear(core->renderer);
//	SDL_SetRenderDrawColor(core->renderer,255,0,0,255);
	// Render Text UI
	char tmp[64]; sprintf(tmp,"PING: %d, X: %d, Y: %d, Vel_X: %d, Vel_Y: %d, Jumping: %d",core->ping,core->player.x,core->player.y,core->player.vel_x, core->player.vel_y,core->player.can_jump);
	dispText(core,tmp,1,4);

	// Render map
	SDL_Rect src;
	src.w = MAP_TILESIZE;
	src.h = MAP_TILESIZE;
	src.x = 0;
	src.y = 0;
	SDL_Rect rect;
	rect.w = MAP_TILESIZE;
	rect.h = MAP_TILESIZE;
	for (int y=0;y<MAP_MAXHEIGHT;y++) {
		for (int x=0;x<MAP_MAXWIDTH;x++) {
			rect.x = x*MAP_TILESIZE;
			rect.y = y*MAP_TILESIZE;
			switch (core->map[y][x]) {
				case (MAP_ID_GRASS):
					SDL_RenderCopy(core->renderer,core->Texture_Terrain,&src,&rect);
					break;
			}
		}
	}

	// Render other players
	for (int i=0;i<MAXPLAYERS;i++) {
		
	}

	// Render local player
	SDL_SetRenderDrawColor(core->renderer,100,10,150,200);
	for (int i=(core->player.y)-(core->player.width/2);i < core->player.y+(core->player.width/2);i++) {
		for (int y=(core->player.x)-(core->player.width/2);y<core->player.x+(core->player.width/2);y++) {
			SDL_RenderDrawPoint(core->renderer,y,i);
		}
	}

	SDL_SetRenderDrawColor(core->renderer,0,255,0,255);
	SDL_RenderDrawPoint(core->renderer,core->player.x,core->player.y);
	SDL_RenderPresent(core->renderer);
}

int main(int argc, char *argv[]) {
	if (SDL_Init(SDL_INIT_EVERYTHING) !=0) {
		printf("Error: Could not initialize SDL2!\n");
		return -1;
	}
	if (TTF_Init() !=0) {
		printf("Error: Could not inialize TTF!\n");
		return -2;
	}
	GAME_CORE *core = malloc(sizeof(GAME_CORE));
	if (!(core->font = TTF_OpenFont("m5x7.ttf",24))) {
		printf("Error: Could not open font!\n");
		return -3;
	}
	core->window = SDL_CreateWindow("SDL_Game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIN_WIDTH,WIN_HEIGHT,0);
	core->renderer = SDL_CreateRenderer(core->window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE);
	core->running = 1;
	core->gravity = 2; core->player.width = 20; core->player.x=50;core->player.y=50;core->player.vel_x=0;core->player.vel_y=0;
	core->player.speed = 2; core->player.can_jump =0;core->player.JUMP_POWER = 20;
	// Surface
	core->Surface_Terrain = IMG_Load("Surface_Terrain.jpg");
	if (core->Surface_Terrain==NULL) { printf("Error: Could not load image Surface_Terrain.png\n"); goto cleanup; }
	// Texture
	core->Texture_Terrain = SDL_CreateTextureFromSurface(core->renderer,core->Surface_Terrain);
	initMap(core);

	// Multiplayer
	pthread_t tid_internal_server;
	core->portl = 60000;
	core->ports = 1234;
	strcpy(core->ipv4,"127.0.0.1");
/*	if (pthread_create(&tid_internal_server,NULL,internal_server,(void *)core)!=0) {
		printf("Error: Could not Create Thread!\n");
		goto cleanup;
	}
*/


	int ticks_a=0,ticks_b=0,delta;
	double fps, set_fps=120, tps, set_tps=30;
	char title[32];
	while (core->running) {
		delta=(ticks_b-ticks_a);
		if (delta>0) {
			fps = (1/(float)delta)*1000;
			sprintf(title,"SDL_Game [FPS] %.2f [DELTA] %d\n",fps,delta);
			SDL_SetWindowTitle(core->window,title);
			core->delta = delta;
			core->tps = tps;
			core->fps = fps;
		}

		ticks_a = SDL_GetTicks();
		SDL_Delay((int)1/set_fps*1000);
		doRender(core);
		doUpdate(core);
		handleEvent(core);
		ticks_b = SDL_GetTicks();
	}
//	pthread_cancel(tid_internal_server);
cleanup:
	SDL_DestroyRenderer(core->renderer);
	SDL_DestroyWindow(core->window);
	free(core);
	return 0;
}
