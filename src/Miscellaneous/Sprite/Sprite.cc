#include "Sprite.h"

Sprite::Sprite(unique_ptr<sf::Sprite> sprite) : m_sprite(move(sprite))
{
}

sf::Sprite *Sprite::GetSprite() const
{
    return m_sprite.get();
}
