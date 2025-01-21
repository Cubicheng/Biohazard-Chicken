#pragma once

#include "timer.h"
#include "atlas.h"
#include "camera.h"
#include "vector2.h"

#include <SDL.h>

#include <vector>
#include <functional>

class Animation {
public:
	Animation() {
		timer.set_one_shot(false);
		timer.set_on_timeout([&]() {
			idx_frame++;
			if (idx_frame >= frames.size()) {
				idx_frame = is_loop?0:frames.size()-1;
				if (!is_loop && on_timeout) {
                    on_timeout();
				}
			}
		});
	}

	~Animation() = default;

	void reset() {
        timer.restart();
		idx_frame = 0;
	}

	void set_position(const Vector2& position) {
		this->position = position;
	}

	void set_rotate(double angle) {
        this->angle = angle;
	}

	void set_loop(bool flag) {
		is_loop = flag;
	}

	void set_center(const SDL_FPoint& center) {
        this->center = center;
	}

	void set_interval(float interval) {
        timer.set_wait_time(interval);
	}

	void set_on_timeout(std::function<void()> on_timeout) {
        this->on_timeout = on_timeout;
    }

	void add_frame(Atlas* atlas) {
		for (int i = 0; i < atlas->get_size(); i++) {
			SDL_Texture* texture = atlas->get_texture(i);

			int width, height;
            SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

			SDL_Rect rect_dst = { 0, 0, width, height };

            frames.emplace_back(texture, rect_dst);
		}
	}

	void on_render(const Camera& camera)const {
		const Frame& frame = frames[idx_frame];

		SDL_FRect rect_dst = {
			position.x - frame.rect_src.w/2,
			position.y - frame.rect_src.h/2,
			static_cast<float>(frame.rect_src.w),
			static_cast<float>(frame.rect_src.h)
		};

		camera.render_texture(frame.texture, &frame.rect_src, &rect_dst, angle, &center);
	}

private:
	struct Frame {
		SDL_Texture* texture = nullptr;
		SDL_Rect rect_src;

		Frame() = default;
		Frame(SDL_Texture* texture, const SDL_Rect& rect_src)
			:texture(texture), rect_src(rect_src) {}

		~Frame() = default;
	};

	Vector2 position;
	double angle = 0;
	SDL_FPoint center = { 0 };

	Timer timer;
	bool is_loop = true;
	size_t idx_frame = 0;
	std::vector<Frame> frames;
	std::function<void()> on_timeout;
};