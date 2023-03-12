#pragma once
#include <memory>
#include "Entity.h"
#include "Tongue.h"
#include "Vec2.h"

class Player : public Entity
{
public:
    std::unique_ptr<CircleCollider> collider;

    Player(std::shared_ptr<BaseSprite> sprite, const Vec2 &pos = Vec2(0.0f, 0.0f));
    Player(std::shared_ptr<BaseSprite> sprite, float x, float y);
    Player(const Player &other);

    Tongue &get_tongue();

    void do_action(SDL_Event &event);
    void update();

private:
    SDL_KeyCode held_key = SDLK_UNKNOWN;
    std::unique_ptr<Tongue> tongue;
};

