#pragma once
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
    Surface(const sf::Sprite &sprite, const float speed);
    ~Surface() = default;

    sf::Sprite GetSprite() const;
    float GetSpeed() const;

private:
    sf::Sprite m_sprite;
    float m_speed;
};
