#include "Font.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <memory>

using json = nlohmann::json;

Font::Font(const uint8_t ID, sf::Font *font) : m_ID(ID), m_font(font)
{
}

uint8_t Font::GetID() const
{
    return m_ID;
}

sf::Font *Font::GetFont() const
{
    return m_font;
}
