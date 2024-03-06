#pragma once
#include "Game.h"
#include "Player.h"
#include <SFML/Graphics.hpp>

struct SurfaceSpeed
{
    float grass;
    float water;
};

enum class SurfaceType
{
    Grass = 0,
    Water
};

class Surface
{

public:
    Surface(const sf::Sprite sprite, const float speed);
    ~Surface() = default;

    sf::Sprite GetSprite() const;
    float GetSpeed() const;

private:
    sf::Sprite m_sprite;
    float m_speed;
};

class Water : public Surface
{

public:
    Water(const sf::Sprite sprite, const float speed);
    ~Water() = default;

private:
};

class Grass : public Surface
{

public:
    Grass(const sf::Sprite sprite, const float speed);
    ~Grass() = default;

private:
};

void InitSurface(std::vector<std::unique_ptr<Surface>> &surfaces, const Game &game, const sf::Texture &texture);

void DrawSurface(sf::RenderWindow &window,
                 const std::vector<std::unique_ptr<Surface>> &surfaces,
                 Player &player,
                 const sf::Sprite &playerSprite,
                 const Game &game);
