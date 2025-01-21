#pragma once

#include "timer.h"
#include "vector2.h"

#include <SDL.h>

class Camera {
public:
	Camera(SDL_Renderer* renderer) {
		this->renderer = renderer;

		timer_shake.set_one_shot(true);
		timer_shake.set_on_timeout([&]() {
            is_shaking = false;
			reset();
        });
	}

	~Camera() = default;

	const Vector2& get_position() const {
		return position;
	}

	void shake(float strength, float duration) {
		is_shaking = true;
		shaking_strength = strength;

        timer_shake.set_wait_time(duration);
		timer_shake.restart();
	}

	void reset() {
		position.x = position.y = 0;
	}

	void on_update(float delta) {
		timer_shake.on_update(delta);
		if (is_shaking) {
			position.x = (-50 + rand() % 100) / 50.0f * shaking_strength;
            position.y = (-50 + rand() % 100) / 50.0f * shaking_strength;
		}
	}
	// rotate_center is in texture space
	void render_texture(SDL_Texture* texture, const SDL_Rect* rect_src, const SDL_FRect* rect_dst, double angle, const SDL_FPoint* rotate_center)const {
		SDL_FRect rect_dst_final = *rect_dst;
		rect_dst_final.x -= position.x;
        rect_dst_final.y -= position.y;
        SDL_RenderCopyExF(renderer, texture, rect_src, &rect_dst_final, angle, rotate_center, SDL_FLIP_NONE);
	}

private:
	Vector2 position;
	Timer timer_shake;
	bool is_shaking = false;
	float shaking_strength = 0;
	SDL_Renderer* renderer = nullptr;
};