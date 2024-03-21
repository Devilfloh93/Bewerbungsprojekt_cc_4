#include "Surface.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <random>

using json = nlohmann::json;

Surface::Surface(const sf::Sprite &sprite, const float speed) : m_sprite(sprite), m_speed(speed)
{
}

sf::Sprite Surface::GetSprite() const
{
    return this->m_sprite;
}

float Surface::GetSpeed() const
{
    return this->m_speed;
}

void InitSurface(std::vector<std::unique_ptr<Surface>> &surfaces,
                 const Game &game,
                 const std::vector<std::unique_ptr<Texture>> &textures)
{
    auto maxTiles = game.GetMaxTiles();
    auto tileSize = game.GetTileSize();
    auto width = game.GetGameWidth();
    auto height = game.GetGameHeight();

    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()

    // Random Grass Texture Selector
    std::uniform_int_distribution<> dist(0U, 3U);

    size_t j = 0;
    size_t k = 0;

    std::ifstream file("./data/surfaceCfg.json");

    if (file.is_open())
    {
        auto jsonData = json::parse(file);

        for (size_t i = 0; i < maxTiles; ++i)
        {
            sf::Sprite tileSprite;
            sf::Texture *texture;
            std::uint8_t id;
            std::uint8_t textureID;
            sf::IntRect textureData;
            float speed;
            auto rnd = dist(gen);
            bool canCreate = false;

            if (j * tileSize >= width)
            {
                j = 0;
                ++k;
            }

            for (const auto &data : jsonData)
            {
                id = data["id"];
                textureID = data["textureID"];
                textureData = sf::IntRect(data["textureData"][0], data["textureData"][1], tileSize, tileSize);
                speed = data["speed"];

                for (const auto &data1 : textures)
                {
                    auto texID = data1->GetID();
                    if (texID == textureID)
                    {
                        texture = data1->GetTexture();
                    }
                }

                if (id == 0 && j == 0 && k == 0 || (j * tileSize == (width - tileSize) && k == 0) ||
                    (k * tileSize == (height - tileSize) && j == 0) ||
                    (j * tileSize == (width - tileSize) && k * tileSize == (height - tileSize)))
                {
                    // FULL WATER
                    canCreate = true;
                }
                else if (id == 1 && j == 0)
                {
                    // LEFT WATER
                    canCreate = true;
                }
                else if (id == 2 && j * tileSize == (width - tileSize))
                {
                    // RIGHT WATER
                    canCreate = true;
                }
                else if (id == 3 && k == 0)
                {
                    // TOP WATER
                    canCreate = true;
                }
                else if (id == 4 && k * tileSize == (height - tileSize))
                {
                    // BOT WATER
                    canCreate = true;
                }
                else if (id == 5 && rnd == 0)
                {
                    // RND GRASS
                    canCreate = true;
                }
                else if (id == 6 && rnd == 1)
                {
                    // RND GRASS
                    canCreate = true;
                }
                else if (id == 7 && rnd == 2)
                {
                    // RND GRASS
                    canCreate = true;
                }
                else if (id == 8 && rnd == 3)
                {
                    // RND GRASS
                    canCreate = true;
                }

                if (canCreate)
                {
                    tileSprite.setTextureRect(textureData);
                    tileSprite.setTexture(*texture);
                    tileSprite.setPosition(j * tileSize, k * tileSize);
                    surfaces.push_back(std::make_unique<Surface>(tileSprite, speed));
                    ++j;
                    break;
                }
            }
        }
        file.close();
    }
}

void DrawSurface(sf::RenderWindow &window,
                 const std::vector<std::unique_ptr<Surface>> &surfaces,
                 Player &player,
                 const Game &game)
{
    auto playerSprite = player.GetSprite();
    auto playerPos = playerSprite->getPosition();
    auto surfaceTileSize = game.GetTileSize();

    for (auto &data : surfaces)
    {
        auto sprite = data->GetSprite();
        auto spritePos = sprite.getPosition();
        auto tileSize = surfaceTileSize / 2;

        if (playerPos.x >= spritePos.x - tileSize && playerPos.x <= spritePos.x + tileSize &&
            playerPos.y >= spritePos.y - tileSize && playerPos.y <= spritePos.y + tileSize)
        {
            auto speed = data->GetSpeed();
            player.SetSpeed(speed);
        }

        window.draw(sprite);
    }
}
