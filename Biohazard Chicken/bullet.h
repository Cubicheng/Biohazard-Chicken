#pragma once

#include "camera.h"
#include "vector2.h"
#include "resources.h"

#include <SDL.h>

class Bullet {
public:
	Bullet(double angle) {
		this->angle = angle;

		double rad = angle * M_PI / 180.0;
		velocity = Vector2(cos(rad), sin(rad)).normalize() * SPEED;
	}

	~Bullet() = default;

	void set_position(const Vector2& position) {
        this->position = position;
	}

	const Vector2& get_position() {
        return position;
	}

	void on_update(float delta) {
		position += velocity * delta;

		if (position.x <= 0 || position.x >= 1280 || position.y <= 0 || position.y >= 720) {
			is_valid = false;
		}
	}

	void on_render(Camera& camera) {
		const SDL_FRect rect = { position.x - WIDTH / 2, position.y - HEIGHT / 2, WIDTH, HEIGHT };
		camera.render_texture(rs::tex_bullet, nullptr, &rect, angle, nullptr);
	}

	void on_hit() {
		is_valid = false;
	}

	bool can_remove() const {
		return !is_valid;
	}

private:
	const int WIDTH = 8;
	const int HEIGHT = 4;
	double angle = 0;
	Vector2 position;
	Vector2 velocity;
	bool is_valid = true;
	const float SPEED = 800.0f;
};