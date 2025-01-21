#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL2_gfxprimitives.h>

int main() {

	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	Mix_Init(MIX_INIT_MP3);
	TTF_Init();

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	SDL_Window* window = SDL_CreateWindow(u8"Biohazard Chicken", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	
	SDL_Surface* suf_img = IMG_Load("resources/background.png");
	SDL_Texture* tex_img = SDL_CreateTextureFromSurface(renderer, suf_img);

	TTF_Font* font = TTF_OpenFont("resources/IPix.ttf", 32);
	SDL_Color font_color = { 255, 255, 255, 255 };
	SDL_Surface* suf_text = TTF_RenderUTF8_Blended(font, u8"Biohazard Chicken", font_color);
	SDL_Texture* tex_text = SDL_CreateTextureFromSurface(renderer, suf_text);

	SDL_Rect rect_text = { 0, 0, suf_text->w, suf_text->h };

	bool is_running = true;

	SDL_Event event;

	Mix_Music* music = Mix_LoadMUS("resources/bgm.mp3");
	Mix_FadeInMusic(music, -1, 1500);

	int fps = 60;
	Uint64 last_counter = SDL_GetPerformanceCounter();
	Uint64 counter_freq = SDL_GetPerformanceFrequency();

	while (is_running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				is_running = false;
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_RenderCopy(renderer, tex_img, NULL, NULL);
        SDL_RenderCopy(renderer, tex_text, NULL, &rect_text);
		filledCircleRGBA(renderer, 640, 360, 100, 255, 255, 255, 255);
		
		SDL_RenderPresent(renderer);

		Uint64 current_counter = SDL_GetPerformanceCounter();
		double delta = (double)(current_counter - last_counter) / counter_freq;
		last_counter = current_counter;

		if (delta * 1000 < 1000.0 / fps) {
            SDL_Delay((1000.0 / fps) - (delta * 1000));
		}
	}

	return 0;
}