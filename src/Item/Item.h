#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

class ItemCfg
{

public:
    ItemCfg(const sf::Texture *texture,
            const sf::IntRect &textureCoords,
            const std::uint16_t ID,
            const std::string name);
    ~ItemCfg() = default;

    const sf::Texture *GetTexture() const;
    sf::IntRect GetTextureCoords() const;

    std::uint16_t GetID() const;
    std::string GetName() const;

private:
    const sf::Texture *m_texture;
    sf::IntRect m_textureCoords;
    std::uint16_t m_ID;
    std::string m_name;
};

class Item
{

public:
    Item(const sf::Sprite &sprite, const std::uint16_t ID, const std::string name);
    ~Item() = default;

    sf::Sprite GetSprite() const;

    std::uint16_t GetID() const;
    std::string GetName() const;

private:
    sf::Sprite m_sprite;
    std::uint16_t m_ID;
    std::string m_name;
};

void InitItemCfg(std::vector<std::unique_ptr<ItemCfg>> &items);

void DrawItems(sf::RenderWindow &window, const std::vector<std::unique_ptr<Item>> &items);
