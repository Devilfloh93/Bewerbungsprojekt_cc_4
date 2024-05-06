#include "ItemGround.h"

ItemGround::ItemGround(unique_ptr<sf::Sprite> sprite, const uint8_t ID, const uint16_t count)
    : Sprite(move(sprite)), m_ID(ID), m_count(count)
{
}

uint16_t ItemGround::GetCount() const
{
    return m_count;
}

uint8_t ItemGround::GetID() const
{
    return m_ID;
}
