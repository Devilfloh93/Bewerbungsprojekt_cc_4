#include "Item.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

AllItems::AllItems(const sf::Texture *texture,
                   const sf::IntRect &textureCoords,
                   const std::uint16_t ID,
                   const std::string name)
    : m_texture(texture), m_textureCoords(textureCoords), m_ID(ID), m_name(name)
{
}

std::uint16_t AllItems::GetID() const
{
    return this->m_ID;
}

std::string AllItems::GetName() const
{
    return this->m_name;
}

const sf::Texture *AllItems::GetTexture() const
{
    return this->m_texture;
}

sf::IntRect AllItems::GetTextureCoords() const
{
    return this->m_textureCoords;
}

Item::Item(const sf::Sprite &sprite, const std::uint16_t ID, const std::string name)
    : m_sprite(sprite), m_ID(ID), m_name(name)
{
}

std::uint16_t Item::GetID() const
{
    return this->m_ID;
}

std::string Item::GetName() const
{
    return this->m_name;
}

sf::Sprite Item::GetSprite() const
{
    return this->m_sprite;
}

void InitItems(std::vector<std::unique_ptr<AllItems>> &items)
{
    std::ifstream file("./data/items.json");

    std::vector<nlohmann::json_abi_v3_11_2::ordered_json> jsonItems;

    if (file.is_open())
    {
        auto jsonData = nlohmann::ordered_json::parse(file);

        for (const auto &data : jsonData)
        {
            auto texturePath = data["texture"];
            auto textureCoords = sf::IntRect(data["textureCoords"][0],
                                             data["textureCoords"][1],
                                             data["textureCoords"][2],
                                             data["textureCoords"][3]);

            auto texture = new sf::Texture();

            texture->loadFromFile(texturePath);

            items.push_back(std::make_unique<AllItems>(texture, textureCoords, data["id"], data["name"]));
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
