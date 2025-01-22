#pragma once

#include "chicken.h"
#include "resources.h"

class ChickenFast : public Chicken {
public:
    ChickenFast() {
        animation_run.add_frames(&rs::atlas_chicken_fast);
        speed = 80.0f;
    }
    ~ChickenFast() = default;
};