#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <vector>
#include <string>

class Atlas {
public:
	Atlas() = default;
	~Atlas() {
        for (auto& tex : tex_list) {
            SDL_DestroyTexture(tex);
        }
	}

	void load(SDL_Renderer* renderer, const char* path_template, int num) {
		static char path_file[256];
		for (int i = 0; i < num; i++) {
			sprintf_s(path_file, path_template, i + 1);
			SDL_Texture* texture = IMG_LoadTexture(renderer, path_file);
			tex_list.push_back(texture);
		}
	}

	void clear() {
        tex_list.clear();
	}

	int get_size() const {
		return tex_list.size();
	}

	SDL_Texture* get_texture(int index) const {
		if (index < 0 || index >= tex_list.size()) {
            return nullptr;
		}
		return tex_list[index];
	}

	void add_texture(SDL_Texture* texture) {
		tex_list.push_back(texture);
	}

private:
	std::vector<SDL_Texture*> tex_list;
};