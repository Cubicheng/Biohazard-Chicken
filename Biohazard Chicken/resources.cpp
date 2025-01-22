#include "resources.h"

namespace rs {
	SDL_Texture* tex_heart = nullptr;
	SDL_Texture* tex_bullet = nullptr;
	SDL_Texture* tex_battery = nullptr;
	SDL_Texture* tex_crosshair = nullptr;
	SDL_Texture* tex_background = nullptr;
	SDL_Texture* tex_barrel_idle = nullptr;

	Atlas atlas_barrel_fire;
	Atlas atlas_chicken_fast;
	Atlas atlas_chicken_medium;
	Atlas atlas_chicken_slow;
	Atlas atlas_explosion;

	Mix_Music* music_bgm = nullptr;
	Mix_Music* music_loss = nullptr;

	Mix_Chunk* sound_hurt = nullptr;
	Mix_Chunk* sound_fire_1 = nullptr;
	Mix_Chunk* sound_fire_2 = nullptr;
	Mix_Chunk* sound_fire_3 = nullptr;
	Mix_Chunk* sound_explosion = nullptr;

	TTF_Font* font = nullptr;

	void load_resources() {
		tex_heart = IMG_LoadTexture(renderer, "resources/heart.png");
		tex_bullet = IMG_LoadTexture(renderer, "resources/bullet.png");
		tex_battery = IMG_LoadTexture(renderer, "resources/battery.png");
		tex_crosshair = IMG_LoadTexture(renderer, "resources/crosshair.png");
		tex_background = IMG_LoadTexture(renderer, "resources/background.png");
		tex_barrel_idle = IMG_LoadTexture(renderer, "resources/barrel_idle.png");

		atlas_barrel_fire.load(renderer, "resources/barrel_fire_%d.png", 3);
		atlas_chicken_fast.load(renderer, "resources/chicken_fast_%d.png", 4);
		atlas_chicken_medium.load(renderer, "resources/chicken_medium_%d.png", 6);
		atlas_chicken_slow.load(renderer, "resources/chicken_slow_%d.png", 8);
		atlas_explosion.load(renderer, "resources/explosion_%d.png", 5);

		music_bgm = Mix_LoadMUS("resources/bgm.mp3");
		music_loss = Mix_LoadMUS("resources/loss.mp3");

		sound_hurt = Mix_LoadWAV("resources/hurt.mp3");
		sound_fire_1 = Mix_LoadWAV("resources/fire_1.wav");
		sound_fire_2 = Mix_LoadWAV("resources/fire_2.wav");
		sound_fire_3 = Mix_LoadWAV("resources/fire_3.wav");
		sound_explosion = Mix_LoadWAV("resources/explosion.wav");

		font = TTF_OpenFont("resources/IPix.ttf", 28);
	}

	void unload_resources() {
		SDL_DestroyTexture(tex_heart);
		SDL_DestroyTexture(tex_bullet);
		SDL_DestroyTexture(tex_battery);
		SDL_DestroyTexture(tex_crosshair);
		SDL_DestroyTexture(tex_background);
		SDL_DestroyTexture(tex_barrel_idle);

		Mix_FreeMusic(music_bgm);
		Mix_FreeMusic(music_loss);

		Mix_FreeChunk(sound_hurt);
		Mix_FreeChunk(sound_fire_1);
		Mix_FreeChunk(sound_fire_2);
		Mix_FreeChunk(sound_fire_3);
		Mix_FreeChunk(sound_explosion);
	}
}