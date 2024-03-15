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
    World(const sf::Sprite sprite, const Collision collision, const std::uint8_t itemOutputID);
    ~World() = default;

    sf::Sprite GetSprite() const;
    Collision GetCollision() const;
    std::uint8_t GetItemOutputID() const;

private:
    sf::Sprite m_sprite;
    Collision m_collision;
    std::uint8_t m_itemOutputID;
};

void InitWorld(std::vector<std::unique_ptr<World>> &world);

void DrawWorld(sf::RenderWindow &window, const std::vector<std::unique_ptr<World>> &world);
