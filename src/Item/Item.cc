#include "Item.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

ItemCfg::ItemCfg(const sf::Texture *texture,
                 const sf::IntRect &textureCoords,
                 const std::uint16_t ID,
                 const std::string name)
    : m_texture(texture), m_textureCoords(textureCoords), m_ID(ID), m_name(name)
{
}

std::uint16_t ItemCfg::GetID() const
{
    return this->m_ID;
}

std::string ItemCfg::GetName() const
{
    return this->m_name;
}

const sf::Texture *ItemCfg::GetTexture() const
{
    return this->m_texture;
}

sf::IntRect ItemCfg::GetTextureCoords() const
{
    return this->m_textureCoords;
}

Item::Item(const sf::Sprite &sprite, const std::uint16_t ID, const std::uint16_t count)
    : m_sprite(sprite), m_ID(ID), m_count(count)
{
}

std::uint16_t Item::GetCount() const
{
    return this->m_count;
}


std::uint16_t Item::GetID() const
{
    return this->m_ID;
}

sf::Sprite Item::GetSprite() const
{
    return this->m_sprite;
}

void InitItemCfg(std::vector<std::unique_ptr<ItemCfg>> &items)
{
    std::ifstream file("./data/itemCfg.json");

    std::vector<nlohmann::json_abi_v3_11_2::ordered_json> jsonItems;

    if (file.is_open())
    {
        auto jsonData = nlohmann::ordered_json::parse(file);

        for (const auto &data : jsonData)
        {
            auto ID = data["id"];
            auto name = data["name"];
            auto texturePath = data["texture"];
            auto textureCoords = sf::IntRect(data["textureCoords"][0],
                                             data["textureCoords"][1],
                                             data["textureCoords"][2],
                                             data["textureCoords"][3]);

            auto texture = new sf::Texture();

            texture->loadFromFile(texturePath);

            items.push_back(std::make_unique<ItemCfg>(texture, textureCoords, ID, name));
        }
    }
}

void DrawItems(sf::RenderWindow &window, const std::vector<std::unique_ptr<Item>> &items)
{
    for (auto &data : items)
    {
        auto tileSprite = data->GetSprite();

        window.draw(tileSprite);
    }
}
