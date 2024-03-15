#include "World.h"
#include "nlohmann/json.hpp"
#include <fstream>

using json = nlohmann::json;

World::World(const sf::Sprite sprite, const Collision collision, const std::uint8_t itemOutputID)
    : m_sprite(sprite), m_collision(collision), m_itemOutputID(itemOutputID)
{
}

sf::Sprite World::GetSprite() const
{
    return this->m_sprite;
}

Collision World::GetCollision() const
{
    return this->m_collision;
}
std::uint8_t World::GetItemOutputID() const
{
    return this->m_itemOutputID;
}

void InitWorld(std::vector<std::unique_ptr<World>> &world)
{
    sf::Sprite tileSprite;

    std::ifstream file("./data/worldCfg.json");

    if (file.is_open())
    {
        auto jsonData = json::parse(file);

        for (const auto &data : jsonData)
        {
            auto texturePath = data["texture"];

            auto texture = new sf::Texture();

            texture->loadFromFile(texturePath);

            auto collision = data["collision"];
            auto itemOutputID = data["itemOutputID"];
            tileSprite.setTexture(*texture);
            tileSprite.setTextureRect({data["textureCoords"][0],
                                       data["textureCoords"][1],
                                       data["textureCoords"][2],
                                       data["textureCoords"][3]});

            for (const auto &data1 : data["pos"])
            {
                tileSprite.setPosition(data1[0], data1[1]);
                world.push_back(
                    std::make_unique<World>(tileSprite, Collision{.x = collision[0], .y = collision[1]}, itemOutputID));
            }
        }
    }
}

void DrawWorld(sf::RenderWindow &window, const std::vector<std::unique_ptr<World>> &world)
{
    for (auto &data : world)
    {
        auto sprite = data->GetSprite();

        window.draw(sprite);
    }
}
