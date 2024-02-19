#include "Surface.h"


Surface::Surface(const sf::Sprite sprite, const bool canPass) : m_sprite(sprite), m_canPass(canPass)
{
}

sf::Sprite Surface::GetSprite() const
{
    return this->m_sprite;
}
