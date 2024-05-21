#include "ItemCfg.h"

/**
 * @brief Construct a new Item Cfg:: Item Cfg object
 *
 * @param textureID
 * @param textureData pixel coords of the texture [x , y, sizeX, sizeY]
 * @param ID unique ID of the item loaded from json file - data/entities/item/item.json
 * @param name name of the item loaded from json file - gets created by LanguageID found in data/entities/item/item.json and data/language/item.json
 * @param maxDrop max possible item drop 1 - maxDrop loaded from json file - data/entities/item/item.json
 * @param fontID
 */

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

string_view ItemCfg::GetName() const
{
    return m_name;
}
