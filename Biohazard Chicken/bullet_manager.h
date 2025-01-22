#pragma once
#include <vector>
#include "bullet.h"
#include "camera.h"

extern Camera* camera;

class BulletManager {
public:
    static BulletManager* get_instance();

    void on_update(float delta);

    std::vector<Bullet*>& get_bullet_list(){
        return bullet_list;
    }

    void on_render() const;

private:
    BulletManager() = default;
    ~BulletManager() = default;
    static BulletManager* instance;
    std::vector<Bullet*> bullet_list;
};