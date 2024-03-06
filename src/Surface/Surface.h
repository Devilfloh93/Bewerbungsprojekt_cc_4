#pragma once
#include "Player.h"
#include <SFML/Graphics.hpp>

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

void InitSurface(std::vector<std::unique_ptr<Surface>> &surfaces,
                 const std::uint32_t tileSize,
                 const std::uint32_t maxTiles,
                 const std::uint32_t width,
                 const std::uint32_t height,
                 const sf::Texture &texture);

void DrawSurface(sf::RenderWindow &window,
                 const std::vector<std::unique_ptr<Surface>> &surfaces,
                 Player &player,
                 const sf::Sprite &playerSprite,
                 const uint32_t surfaceTileSize);
