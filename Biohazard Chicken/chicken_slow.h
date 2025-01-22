#pragma once

#include "chicken.h"
#include "resources.h"

class ChickenSlow : public Chicken {
public:
    ChickenSlow() {
        animation_run.add_frames(&rs::atlas_chicken_slow);
        speed = 30.0f;
    }
    ~ChickenSlow() = default;
};