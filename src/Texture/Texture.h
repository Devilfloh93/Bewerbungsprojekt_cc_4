#pragma once
#include <SFML/Graphics.hpp>

struct Collision
{
    uint8_t x;
    uint8_t y;
};

struct TextureProgData
{
    sf::IntRect rect;
    Collision collision;
};

class Texture
{

public:
    Texture(const uint8_t ID, sf::Texture *texture);
    ~Texture() = default;

    uint8_t GetID() const;
    sf::Texture *GetTexture() const;

private:
    uint8_t m_ID;
    sf::Texture *m_texture;
};
