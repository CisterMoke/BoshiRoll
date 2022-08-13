#pragma once
#include "Entity.h"
#include "Tongue.h"
class Player :
    public Entity
{
public:
    Tongue *tongue;

    Player(std::string sprite_path, float zoom = 1.0f, int mode = 0x01);
    Player(std::string sprite_path, Vec2 &pos, float zoom = 1.0f, int mode = 0x01);
    Player(std::string sprite_path, float x, float y, float zoom = 1.0f, int mode = 0x01);
    ~Player();

    void do_action(SDL_Event &event);
    void update();

private:
    SDL_KeyCode held_key = SDLK_UNKNOWN;
};

