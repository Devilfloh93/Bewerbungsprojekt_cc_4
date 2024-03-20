#include "World.h"
#include "nlohmann/json.hpp"
#include <fstream>

using json = nlohmann::json;

World::World(const sf::Sprite &sprite,
             const Collision collision,
             const std::uint8_t itemOutputID,
             const TextureProgData textureProgData)
    : m_sprite(sprite), m_collision(collision), m_itemOutputID(itemOutputID), m_textureProgData(textureProgData)
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
    if (m_textureProgData.rect.getSize().x > 0 && m_textureProgData.rect.getSize().y > 0)
        this->m_sprite.setTextureRect(m_textureProgData.rect);
}

void World::UpdatePosition()
{
    auto pos = this->m_sprite.getPosition();
    auto size = this->m_sprite.getLocalBounds().getSize();
    auto textureSize = this->m_textureProgData.rect.getSize();

    this->m_sprite.setPosition(pos.x + ((size.x / 2) - (textureSize.x / 2)), pos.y + (size.y - textureSize.y));
    this->m_collision.x = this->m_textureProgData.collision.x;
    this->m_collision.y = this->m_textureProgData.collision.y;
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
            auto collision = Collision{.x = data["textureData"][4], .y = data["textureData"][5]};
            std::uint8_t itemOutputID = data["itemOutputID"];
            auto textureCoords = sf::IntRect(data["textureData"][0],
                                             data["textureData"][1],
                                             data["textureData"][2],
                                             data["textureData"][3]);
            auto textureProgData = TextureProgData{
                .rect = sf::IntRect{data["textureProgData"][0],
                                    data["textureProgData"][1],
                                    data["textureProgData"][2],
                                    data["textureProgData"][3]},
                .collision = Collision{.x = data["textureProgData"][4], .y = data["textureProgData"][5]}};

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
                world.push_back(std::make_unique<World>(tileSprite, collision, itemOutputID, textureProgData));
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
