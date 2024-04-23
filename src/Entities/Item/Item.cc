#include "Item.h"

ItemCfg::ItemCfg(sf::Texture *texture,
                 const sf::IntRect textureData,
                 const uint8_t ID,
                 const string_view name,
                 const uint8_t maxDrop,
                 const uint8_t fontID)
    : Texture(texture), m_textureData(textureData), m_ID(ID), m_name(name), m_maxDrop(maxDrop), m_fontID(fontID)
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

uint8_t ItemCfg::GetFontID() const
{
    return m_fontID;
}

ItemGround::ItemGround(sf::Sprite *sprite, const uint8_t ID, const uint16_t count)
    : Sprite(sprite), m_ID(ID), m_count(count)
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
