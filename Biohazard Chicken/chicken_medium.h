#pragma once

#include "chicken.h"
#include "resources.h"

class ChickenMediun : public Chicken {
public:
    ChickenMediun() {
        animation_run.add_frames(&rs::atlas_chicken_medium);
        speed = 50.0f;
    }
    ~ChickenMediun() = default;
};