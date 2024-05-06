#include "ItemCfg.h"

ItemCfg::ItemCfg(const uint8_t textureID,
                 const sf::IntRect textureData,
                 const uint8_t ID,
                 const string_view name,
                 const uint8_t maxDrop,
                 const uint8_t fontID)
    : m_textureID(textureID), m_textureData(textureData), m_ID(ID), m_name(name), m_maxDrop(maxDrop), m_fontID(fontID)
{
}

uint8_t ItemCfg::GetID() const
{
    return m_ID;
}

uint8_t ItemCfg::GetTextureID() const
{
    return m_textureID;
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
