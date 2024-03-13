#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <memory>

struct Collision
{
    std::uint8_t x;
    std::uint8_t y;
};

class World
{

public:
    World(const sf::Sprite sprite, const Collision collision);
    ~World() = default;

    sf::Sprite GetSprite() const;
    Collision GetCollision() const;

private:
    sf::Sprite m_sprite;
    Collision m_collision;
};

void InitWorld(std::vector<std::unique_ptr<World>> &world);

void DrawWorld(sf::RenderWindow &window, const std::vector<std::unique_ptr<World>> &world);
