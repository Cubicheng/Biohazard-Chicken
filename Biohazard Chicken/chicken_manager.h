#pragma once

#include "chicken.h"
#include "bullet_manager.h"
#include <algorithm>

#include <vector>

extern int score;
extern int hp;

class ChickenManager {
public:
	static ChickenManager* get_instance();

	void init();

	void on_update(float delta);

	void on_render();

private:
	int num_per_gen = 2;
	Timer timer_generate;
	Timer timer_increase_num_per_gen;

	ChickenManager() = default;
    ~ChickenManager() = default;
	static ChickenManager* instance;
	std::vector<Chicken*> chicken_list;
};