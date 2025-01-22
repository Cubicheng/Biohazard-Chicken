#include "bullet_manager.h"

extern Camera* camera;

BulletManager* BulletManager::instance = nullptr;

BulletManager* BulletManager::get_instance() {
    if (instance == nullptr) {
        instance = new BulletManager();
    }
    return instance;
}

void BulletManager::on_update(float delta) {
    for (Bullet* bullet : bullet_list) {
        bullet->on_update(delta);
    }

    bullet_list.erase(std::remove_if(bullet_list.begin(), bullet_list.end(), [](Bullet* bullet) {
        bool can_remove = bullet->can_remove();
        if (can_remove) delete bullet;
        return can_remove;
        }), bullet_list.end());
}

void BulletManager::on_render() const {
    for (Bullet* bullet : bullet_list) {
        bullet->on_render(*camera);
    }
}