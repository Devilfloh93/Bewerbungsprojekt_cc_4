#pragma once
#include "Sprite.h"
#include <SFML/Graphics.hpp>

enum class StatType
{
    Empty = 0,
    Health,
    Food,
    Water
};

class Stats : public Sprite
{

public:
    Stats(unique_ptr<sf::Sprite> sprite, const sf::Vector2i textureSize, const StatType type);
    ~Stats() = default;

    StatType GetType() const;
    sf::Vector2i GetTextureSize() const;

private:
    StatType m_type;
    sf::Vector2i m_textureSize;
};
