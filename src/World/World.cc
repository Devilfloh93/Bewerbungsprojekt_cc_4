#include "World.h"
#include "nlohmann/json.hpp"
#include <fstream>

using json = nlohmann::json;

World::World(const sf::Sprite sprite, const Collision collision) : m_sprite(sprite), m_collision(collision)
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


void InitWorld(std::vector<std::unique_ptr<World>> &world, const sf::Texture &texture)
{

    sf::Sprite tileSprite;

    std::ifstream file("./data/world.json");

    if (file.is_open())
    {
        auto jsonData = json::parse(file);

        for (const auto &data : jsonData)
        {
            auto collision = data["collision"];
            tileSprite.setTexture(texture);
            tileSprite.setTextureRect({data["textureCoords"][0],
                                       data["textureCoords"][1],
                                       data["textureCoords"][2],
                                       data["textureCoords"][3]});

            for (const auto &data1 : data["pos"])
            {
                tileSprite.setPosition(data1[0], data1[1]);
                world.push_back(std::make_unique<World>(tileSprite, Collision{.x = collision[0], .y = collision[1]}));
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
