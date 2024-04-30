#pragma once
#include "Texture.h"
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <string>

using namespace std;

class ItemCfg : public Texture
{
public:
    ItemCfg(sf::Texture *texture,
            const sf::IntRect textureData,
            const uint8_t ID,
            const string_view name,
            const uint8_t maxDrop,
            const uint8_t fontID);
    ~ItemCfg() = default;

    sf::IntRect GetTextureData() const;

    uint8_t GetID() const;
    uint8_t GetMaxDrop() const;
    uint8_t GetFontID() const;

private:
    sf::IntRect m_textureData;
    uint8_t m_ID;
    string m_name;
    uint8_t m_maxDrop;
    uint8_t m_fontID;
};
