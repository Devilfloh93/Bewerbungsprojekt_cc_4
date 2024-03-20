#include "Texture.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <memory>

using json = nlohmann::json;

Texture::Texture(const std::uint8_t ID, sf::Texture *texture) : m_ID(ID), m_texture(texture)
{
}

std::uint8_t Texture::GetID() const
{
    return this->m_ID;
}

sf::Texture *Texture::GetTexture() const
{
    return this->m_texture;
}


void InitTexture(std::vector<std::unique_ptr<Texture>> &textures)
{
    std::ifstream file("./data/textureCfg.json");

    if (file.is_open())
    {
        auto jsonData = json::parse(file);

        for (const auto &data : jsonData)
        {
            auto texture = new sf::Texture();
            std::uint8_t ID = data["id"];
            std::string path = data["path"];

            texture->loadFromFile(path);

            textures.push_back(std::make_unique<Texture>(ID, texture));
        }
        file.close();
    }
}
