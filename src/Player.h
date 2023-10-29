#pragma once
#include <memory>
#include <box2d.h>
#include "Entity.h"
#include "Tongue.h"
#include "Vec2.h"

class Player : public Entity
{
private:
    SDL_KeyCode held_key = SDLK_UNKNOWN;
    std::unique_ptr<Tongue> tongue;

protected:
    virtual b2Body *add_to(b2World &world, const Vec2 &pos);

public:

    Player(std::shared_ptr<BaseSprite> sprite, b2World &world, const Vec2 &pos = {});
    virtual ~Player() = default;

    Tongue &get_tongue();

    void do_action(SDL_Event &event);
    void update();

};

