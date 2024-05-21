#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <string>

using namespace std;

class ItemCfg
{
public:
    ItemCfg(const uint8_t textureID,
            const sf::IntRect textureData,
            const uint8_t ID,
            const string_view name,
            const uint8_t maxDrop,
            const uint8_t fontID);
    ~ItemCfg() = default;

    sf::IntRect GetTextureData() const;

    uint8_t GetID() const;
    uint8_t GetTextureID() const;
    uint8_t GetMaxDrop() const;
    uint8_t GetFontID() const;
    string_view GetName() const;

private:
    sf::IntRect m_textureData;
    uint8_t m_ID;
    uint8_t m_textureID;
    string m_name;
    uint8_t m_maxDrop;
    uint8_t m_fontID;
};
