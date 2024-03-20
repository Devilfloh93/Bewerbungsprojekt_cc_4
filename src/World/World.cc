#include "World.h"
#include "nlohmann/json.hpp"
#include <fstream>

using json = nlohmann::json;

World::World(const sf::Sprite sprite,
             const Collision collision,
             const std::uint8_t itemOutputID,
             const sf::IntRect textureProg)
    : m_sprite(sprite), m_collision(collision), m_itemOutputID(itemOutputID), m_textureProg(textureProg)
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

void World::UpdateTextureRect()
{
    if (m_textureProg.getSize().x > 0 && m_textureProg.getSize().y > 0)
        this->m_sprite.setTextureRect(m_textureProg);
}

void InitWorld(std::vector<std::unique_ptr<World>> &world, const std::vector<std::unique_ptr<Texture>> &textures)
{
    sf::Sprite tileSprite;

    std::ifstream file("./data/worldCfg.json");

    if (file.is_open())
    {
        auto jsonData = json::parse(file);

        sf::Texture *texture;
        for (const auto &data : jsonData)
        {
            auto collision = Collision{.x = data["collision"][0], .y = data["collision"][1]};
            std::uint8_t itemOutputID = data["itemOutputID"];
            auto textureCoords = sf::IntRect(data["textureCoords"][0],
                                             data["textureCoords"][1],
                                             data["textureCoords"][2],
                                             data["textureCoords"][3]);
            auto textureProg = sf::IntRect{data["textureProgressCoords"][0],
                                           data["textureProgressCoords"][1],
                                           data["textureProgressCoords"][2],
                                           data["textureProgressCoords"][3]};
            std::uint8_t textureID = data["textureID"];

            for (const auto &data2 : textures)
            {
                auto texID = data2->GetID();
                if (texID == textureID)
                {
                    texture = data2->GetTexture();
                }
            }

            tileSprite.setTexture(*texture);
            tileSprite.setTextureRect(textureCoords);

            for (const auto &data1 : data["pos"])
            {
                tileSprite.setPosition(data1[0], data1[1]);
                world.push_back(std::make_unique<World>(tileSprite, collision, itemOutputID, textureProg));
            }
        }
        file.close();
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
