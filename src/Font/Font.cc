#include "Font.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <memory>

using json = nlohmann::json;

Font::Font(const std::uint8_t ID, sf::Font *font) : m_ID(ID), m_font(font)
{
}

std::uint8_t Font::GetID() const
{
    return this->m_ID;
}

sf::Font *Font::GetFont() const
{
    return this->m_font;
}

void InitFont(std::vector<std::unique_ptr<Font>> &fonts)
{
    std::ifstream file("./data/fontCfg.json");

    if (file.is_open())
    {
        auto jsonData = json::parse(file);

        for (const auto &data : jsonData)
        {
            auto font = new sf::Font();
            std::uint8_t ID = data["id"];
            std::string path = data["path"];

            font->loadFromFile(path);

            fonts.push_back(std::make_unique<Font>(ID, font));
        }
        file.close();
    }
}
