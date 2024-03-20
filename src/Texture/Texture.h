#pragma once
#include <SFML/Graphics.hpp>

class Texture
{

public:
    Texture(const std::uint8_t ID, sf::Texture *texture);
    ~Texture() = default;

    std::uint8_t GetID() const;
    sf::Texture *GetTexture() const;

private:
    std::uint8_t m_ID;
    sf::Texture *m_texture;
};

void InitTexture(std::vector<std::unique_ptr<Texture>> &textures);
