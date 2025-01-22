#include "chicken_manager.h"
#include "chicken_fast.h"
#include "chicken_medium.h"
#include "chicken_slow.h"
#include "iostream"

extern int score;
extern int hp;

ChickenManager* ChickenManager::instance = nullptr;

ChickenManager* ChickenManager::get_instance() {
	if (!instance) {
		instance = new ChickenManager();
	}
	return instance;
}

void ChickenManager::init() {
	timer_generate.set_one_shot(false);
	timer_generate.set_wait_time(1.5f);
	timer_generate.set_on_timeout([&]() {
		for (int i = 0; i < num_per_gen; i++) {
			int val = rand() % 100;
			Chicken* chicken = nullptr;
			if (val < 50) {
				chicken = new ChickenSlow();
			}
			else if (val < 80) {
				chicken = new ChickenMediun();
			}
			else {
				chicken = new ChickenFast();
			}
			chicken_list.push_back(chicken);
		}
		});

	timer_increase_num_per_gen.set_one_shot(false);
	timer_increase_num_per_gen.set_wait_time(8.0f);
	timer_increase_num_per_gen.set_on_timeout([&]() {
		num_per_gen++;
		});
}

void ChickenManager::on_update(float delta) {
	timer_generate.on_update(delta);
	timer_increase_num_per_gen.on_update(delta);

	for (Chicken* chicken : chicken_list) {
		chicken->on_update(delta);
		std::vector<Bullet*> bullet_list = BulletManager::get_instance()->get_bullet_list();
		for (Bullet* bullet : bullet_list) {
			if (!chicken->check_alive() || bullet->can_remove()) {
				continue;
			}
			const Vector2& pos_bullet = bullet->get_position();
			const Vector2& pos_chicken = chicken->get_position();
			static const Vector2 size_chicken = { 30,40 };

			if (pos_bullet.x >= pos_chicken.x - size_chicken.x / 2 &&
				pos_bullet.x <= pos_chicken.x + size_chicken.x / 2 &&
				pos_bullet.y >= pos_chicken.y - size_chicken.y / 2 &&
				pos_bullet.y <= pos_chicken.y + size_chicken.y / 2) {
				chicken->on_hurt();
				bullet->on_hit();
				score++;
			}
		}
		if (!chicken->check_alive()) {
			continue;
		}

		if (chicken->get_position().y >= 720) {
			chicken->make_invalid();
			Mix_PlayChannel(-1, rs::sound_hurt, 0);
			std::cout << hp << std::endl;
			hp--;
		}
	}

	chicken_list.erase(std::remove_if(chicken_list.begin(), chicken_list.end(), [](Chicken* chicken) {
		bool can_remove = chicken->can_remove();
		if (can_remove) delete chicken;
		return can_remove;
		}), chicken_list.end());

	std::sort(chicken_list.begin(), chicken_list.end(), [](Chicken* a, Chicken* b) {
		return a->get_position().y < b->get_position().y;
		});
}

void ChickenManager::on_render() {
	for (Chicken* chicken : chicken_list) {
		chicken->on_render(camera);
	}
}
