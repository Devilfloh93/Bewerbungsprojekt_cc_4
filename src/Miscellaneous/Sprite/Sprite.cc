#include "Sprite.h"

Sprite::Sprite(unique_ptr<sf::Sprite> sprite) : m_sprite(move(sprite))
{
}

sf::Sprite *Sprite::GetSprite() const
{
    return m_sprite.get();
}

SelectableSprite::SelectableSprite(unique_ptr<sf::Sprite> sprite,
                                   const uint16_t selectedTextID,
                                   const SelectedTextCategorie selectedCategorie)
    : Sprite(move(sprite)), m_selectedTextID(selectedTextID), m_selectedCategorie(selectedCategorie)
{
}

uint16_t SelectableSprite::GetSelectedTextID() const
{
    return m_selectedTextID;
}

SelectedTextCategorie SelectableSprite::GetSelectedCategorie() const
{
    return m_selectedCategorie;
}
