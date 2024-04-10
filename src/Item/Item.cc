#include "Item.h"

ItemCfg::ItemCfg(sf::Texture *texture,
                 const sf::IntRect textureData,
                 const uint8_t ID,
                 const string_view name,
                 const uint8_t maxDrop)
    : Texture(texture), m_textureData(textureData), m_ID(ID), m_name(name), m_maxDrop(maxDrop)
{
}

uint8_t ItemCfg::GetID() const
{
    return m_ID;
}

sf::IntRect ItemCfg::GetTextureData() const
{
    return m_textureData;
}

uint8_t ItemCfg::GetMaxDrop() const
{
    return m_maxDrop;
}

Item::Item(sf::Sprite *sprite, const uint16_t ID, const uint16_t count) : Sprite(sprite), m_ID(ID), m_count(count)
{
}

uint16_t Item::GetCount() const
{
    return m_count;
}


uint16_t Item::GetID() const
{
    return m_ID;
}
