#pragma once
#include "Entity.h"
#include "Tongue.h"
class Player :
    public Entity
{
public:
    Tongue *tongue;

    Player(std::string spritePath, float zoom = 1.0f, int mode = 0x01);
    Player(std::string spritePath, Vec2 &pos, float zoom = 1.0f, int mode = 0x01);
    Player(std::string spritePath, float x, float y, float zoom = 1.0f, int mode = 0x01);
    ~Player();

    void doAction(SDL_Event &event);
    void update();

    void render(SDL_Renderer *renderer, Vec2 const &orig = Vec2(0.0f, 0.0f), Vec2 const &offset = Vec2(0.0f, 0.0f), float phi = 0.0f, float zx = 1.0f, float zy = 1.0f);
};

