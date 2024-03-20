#pragma once
#include "Texture.h"
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

class ItemCfg
{

public:
    ItemCfg(sf::Texture *texture,
            const sf::IntRect textureCoords,
            const std::uint8_t ID,
            const std::string_view name,
            const std::uint8_t maxDrop);
    ~ItemCfg() = default;

    sf::Texture *GetTexture() const;
    sf::IntRect GetTextureCoords() const;

    std::uint8_t GetID() const;
    std::string GetName() const;
    std::uint8_t GetMaxDrop() const;

private:
    sf::Texture *m_texture;
    sf::IntRect m_textureCoords;
    std::uint16_t m_ID;
    std::string m_name;
    std::uint8_t m_maxDrop;
};

class Item
{

public:
    Item(const sf::Sprite &sprite, const std::uint16_t ID, const std::uint16_t count);
    ~Item() = default;

    sf::Sprite GetSprite() const;

    std::uint16_t GetID() const;
    std::uint16_t GetCount() const;

private:
    sf::Sprite m_sprite;
    std::uint16_t m_ID;
    std::uint16_t m_count;
};

void InitItemCfg(std::vector<std::unique_ptr<ItemCfg>> &items, const std::vector<std::unique_ptr<Texture>> &textures);

void DrawItems(sf::RenderWindow &window, const std::vector<std::unique_ptr<Item>> &items);
