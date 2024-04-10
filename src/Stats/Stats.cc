#include "Stats.h"

Stats::Stats(sf::Sprite *sprite, const sf::Vector2i textureSize, const StatType type)
    : Sprite(sprite), m_textureSize(textureSize), m_type(type)
{
}

StatType Stats::GetType() const
{
    return m_type;
}

sf::Vector2i Stats::GetTextureSize() const
{
    return m_textureSize;
}
