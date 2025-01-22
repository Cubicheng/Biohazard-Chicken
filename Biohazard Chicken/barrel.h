#pragma once

#include "vector2.h"
#include "animation.h"
#include "resources.h"
#include "bullet_manager.h"
#include <SDL.h>

extern Camera* camera;

class Barrel {
public:

	Barrel() {
		animation_barrel_fire.set_loop(false);
		animation_barrel_fire.set_interval(0.04f);
		animation_barrel_fire.add_frames(&rs::atlas_barrel_fire);
		animation_barrel_fire.set_position({ 718,610 });
		animation_barrel_fire.set_center(center_barrel);
        animation_barrel_fire.set_on_timeout([&]() {
			is_cool_down = true;
		});
	}

	void set_pos_crosshair(const Vector2 pos_crosshair) {
		this->pos_crosshair = pos_crosshair;
		update_angle_barrel();
	}

	void update_angle_barrel() {
		Vector2 dir = pos_crosshair - pos_barrel;
		angle_barrel = std::atan2(dir.y, dir.x) * 180 / M_PI;
	}

	void set_is_fire_key_down(bool flag) {
		is_fire_key_down = flag;
	}

	void on_update(float delta) {
		if (!is_cool_down) {
			camera->shake(3.0f, 0.3f);
			animation_barrel_fire.on_update(delta);
		}

		if (is_cool_down && is_fire_key_down) {
			animation_barrel_fire.reset();
			is_cool_down = false;

            Bullet* bullet = new Bullet(angle_barrel);
			double angle_bullet = angle_barrel + (rand() % 30 - 15);
			double radiant = angle_bullet * M_PI / 180;
			Vector2 dir = { (float)std::cos(radiant), (float)std::sin(radiant) };
			bullet->set_position(pos_barrel_center + dir * length_barrel);

			BulletManager::get_instance()->get_bullet_list().push_back(bullet);

			switch (rand() % 3) {
			case 0: {
				Mix_PlayChannel(-1, rs::sound_fire_1, 0);
				break;
			}
			case 1: {
                Mix_PlayChannel(-1, rs::sound_fire_2, 0);
				break;
			}
			case 2: {
				Mix_PlayChannel(-1, rs::sound_fire_3, 0);
				break;
			}
			}
		}
	}

	void on_render() {
		int width_battery,height_battery;
        SDL_QueryTexture(rs::tex_battery, NULL, NULL, &width_battery, &height_battery);
		SDL_FRect rect_battery = {
			pos_battery.x - width_battery / 2.0f,
			pos_battery.y - height_battery / 2.0f,
			width_battery,height_battery
		};
		camera->render_texture(rs::tex_battery, NULL, &rect_battery, 0, NULL);

		int width_barrel, height_barrel;
		SDL_QueryTexture(rs::tex_barrel_idle, NULL, NULL, &width_barrel, &height_barrel);
		const SDL_FRect rect_barrel = {
			pos_barrel.x,
			pos_barrel.y,
			(float)width_barrel,
			(float)height_barrel
		};
		if (is_cool_down) {
			camera->render_texture(rs::tex_barrel_idle, NULL, &rect_barrel, angle_barrel, &center_barrel);
		}
		else {
			animation_barrel_fire.set_rotate(angle_barrel);
			animation_barrel_fire.on_render(*camera);
		}
	}

	const Vector2& get_pos_crosshair() const {
		return pos_crosshair;
	}

private:
	const Vector2 pos_barrel = { 592,585 };
	const SDL_FPoint center_barrel = { 48,25 };
	const Vector2 pos_battery = { 640,600 };
	const float length_barrel = 105;
	const Vector2 pos_barrel_center = { 640,610 };

	double angle_barrel = 0;
	Animation animation_barrel_fire;
	Vector2 pos_crosshair;
	bool is_cool_down = true;
	bool is_fire_key_down = false;
};