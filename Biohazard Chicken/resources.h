#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "atlas.h"

extern SDL_Renderer* renderer;

namespace rs{
	extern SDL_Texture* tex_heart;
	extern SDL_Texture* tex_bullet;
	extern SDL_Texture* tex_battery;
	extern SDL_Texture* tex_crosshair;
	extern SDL_Texture* tex_background;
	extern SDL_Texture* tex_barrel_idle;

	extern Atlas atlas_barrel_fire;
	extern Atlas atlas_chicken_fast;
	extern Atlas atlas_chicken_medium;
	extern Atlas atlas_chicken_slow;
	extern Atlas atlas_explosion;

	extern Mix_Music* music_bgm;
	extern Mix_Music* music_loss;

	extern Mix_Chunk* sound_hurt;
	extern Mix_Chunk* sound_fire_1;
	extern Mix_Chunk* sound_fire_2;
	extern Mix_Chunk* sound_fire_3;
	extern Mix_Chunk* sound_explosion;

	extern TTF_Font* font;

	void load_resources();

	void unload_resources();
};