#pragma once
#include "Sprite.h"
#include "Text.h"
#include "Texture.h"
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

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

class ItemGround : public Sprite
{
public:
    ItemGround(sf::Sprite *sprite, const uint8_t ID, const uint16_t count);
    ~ItemGround() = default;

    uint8_t GetID() const;
    uint16_t GetCount() const;

private:
    uint8_t m_ID;
    uint16_t m_count;
};
