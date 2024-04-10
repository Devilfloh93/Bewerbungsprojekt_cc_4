#pragma once
#include "Sprite.h"
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
            const uint8_t maxDrop);
    ~ItemCfg() = default;

    sf::IntRect GetTextureData() const;

    uint8_t GetID() const;
    uint8_t GetMaxDrop() const;

private:
    sf::IntRect m_textureData;
    uint16_t m_ID;
    string m_name;
    uint8_t m_maxDrop;
};

class Item : public Sprite
{

public:
    Item(sf::Sprite *sprite, const uint16_t ID, const uint16_t count);
    ~Item() = default;

    uint16_t GetID() const;
    uint16_t GetCount() const;

private:
    uint16_t m_ID;
    uint16_t m_count;
};
