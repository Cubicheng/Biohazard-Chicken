#define SDL_MAIN_HANDLED

#include "atlas.h"
#include "camera.h"
#include "bullet.h"
#include "chicken.h"
#include "chicken_fast.h"
#include "chicken_medium.h"
#include "chicken_slow.h"
#include "bullet_manager.h"
#include "chicken_manager.h"
#include "barrel.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <chrono>
#include <thread>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

Camera* camera = nullptr;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

Barrel* barrel;

bool is_running = true;

int hp = 10;
int score = 0;

const int fps = 60;

void init() {
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	Mix_Init(MIX_INIT_MP3);
	TTF_Init();
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_AllocateChannels(32);

	window = SDL_CreateWindow(u8"Biohazard Chicken", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	rs::load_resources();

	SDL_ShowCursor(SDL_DISABLE);

	camera = new Camera(renderer);
	barrel = new Barrel();

	ChickenManager::get_instance()->init();

	Mix_PlayMusic(rs::music_bgm, -1);
	Mix_VolumeMusic(64);
}

void deinit() {
	
	delete barrel;
	delete camera;

	rs::unload_resources();
	
	SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
    SDL_Quit();
}

void on_update(float delta) {
	barrel->on_update(delta);
	BulletManager::get_instance()->on_update(delta);
	ChickenManager::get_instance()->on_update(delta);
	camera->on_update(delta);

	if (hp <= 0) {
		is_running = false;
		Mix_HaltMusic();
		Mix_PlayMusic(rs::music_loss, 0);
		std::string msg = u8"游戏最终得分" + std::to_string(score);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, u8"游戏结束", msg.c_str(), window);
	}
}

void on_render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	{
		int width_bg, height_bg;
        SDL_QueryTexture(rs::tex_background, nullptr, nullptr, &width_bg, &height_bg);
		const SDL_FRect rect_dst = { 
			(1280 - width_bg) / 2.0f, (720 - height_bg) / 2.0f, 
			(float)width_bg, (float)height_bg 
		};
		camera->render_texture(rs::tex_background, nullptr, &rect_dst, 0, nullptr);
	}
	
	BulletManager::get_instance()->on_render();
	ChickenManager::get_instance()->on_render();
	barrel->on_render();

	//std::cout << hp << std::endl;

	{
		int width_heart, height_heart;
        SDL_QueryTexture(rs::tex_heart, nullptr, nullptr, &width_heart, &height_heart);
		for (int i = 0; i < hp; i++) {
			const SDL_Rect rect_dst = {
				(float)(i * (width_heart + 2) + 12), 12,
				(float)width_heart, (float)height_heart
			};
			SDL_RenderCopy(renderer, rs::tex_heart, nullptr, &rect_dst);
		}
	}

	{
		std::string str_score = "SCORE: " + std::to_string(score);
		SDL_Surface* suf_score_bg = TTF_RenderUTF8_Blended(rs::font, str_score.c_str(), { 55, 55, 55, 255 });
		SDL_Surface* suf_score_fg = TTF_RenderUTF8_Blended(rs::font, str_score.c_str(), { 255, 255, 255, 255 });
		SDL_Texture* tex_score_bg = SDL_CreateTextureFromSurface(renderer, suf_score_bg);
		SDL_Texture* tex_score_fg = SDL_CreateTextureFromSurface(renderer, suf_score_fg);
		SDL_Rect rect_dst = { 1280 - suf_score_bg->w - 15, 15, suf_score_bg->w, suf_score_bg->h };
		SDL_RenderCopy(renderer, tex_score_bg, nullptr, &rect_dst);
		rect_dst.x -= 2, rect_dst.y -= 2;
        SDL_RenderCopy(renderer, tex_score_fg, nullptr, &rect_dst);
		SDL_DestroyTexture(tex_score_bg);
		SDL_DestroyTexture(tex_score_bg);
		SDL_FreeSurface(suf_score_bg);
        SDL_FreeSurface(suf_score_fg);
	}

	{
		int width_crosshair, height_crosshair;
		SDL_QueryTexture(rs::tex_crosshair, nullptr, nullptr, &width_crosshair, &height_crosshair);
		Vector2 pos_crosshair = barrel->get_pos_crosshair();
		const SDL_FRect rect_dst = {
			pos_crosshair.x - width_crosshair / 2.0f, pos_crosshair.y - height_crosshair / 2.0f,
			(float)width_crosshair, (float)height_crosshair
		};
		camera->render_texture(rs::tex_crosshair, nullptr, &rect_dst, 0, nullptr);
	}

	SDL_RenderPresent(renderer);
}

void mainloop() {
	SDL_Event event;

	Uint64 last_counter = SDL_GetPerformanceCounter();
	Uint64 counter_freq = SDL_GetPerformanceFrequency();

	while (is_running) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT: {
				is_running = false;
				break;
			}
			case SDL_MOUSEMOTION: {
				barrel->set_pos_crosshair(Vector2(event.motion.x, event.motion.y));
				break;
			}
			case SDL_MOUSEBUTTONDOWN: {
				barrel->set_is_fire_key_down(true);
				break;
			}
			case SDL_MOUSEBUTTONUP: {
				barrel->set_is_fire_key_down(false);
				break;
			}
			}
		}

		Uint64 current_counter = SDL_GetPerformanceCounter();
		double delta = (double)(current_counter - last_counter) / counter_freq;
		last_counter = current_counter;

		on_update(delta);
		
		on_render();

		if (delta * 1000 < 1000.0 / fps) {
			SDL_Delay((1000.0 / fps) - (delta * 1000));
		}
	}
}

int main() {
	init();
	mainloop();
	deinit();
	return 0;
}