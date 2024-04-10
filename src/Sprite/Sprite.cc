#include "Sprite.h"

Sprite::Sprite(sf::Sprite *sprite) : m_sprite(sprite)
{
}

sf::Sprite *Sprite::GetSprite() const
{
    return m_sprite;
}
