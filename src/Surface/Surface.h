#pragma once
#include "Sprite.h"
#include <SFML/Graphics.hpp>

class Surface final : public Sprite
{

public:
    Surface(unique_ptr<sf::Sprite> sprite, const float speed);
    ~Surface() = default;

    float GetSpeed() const;

private:
    float m_speed;
};
