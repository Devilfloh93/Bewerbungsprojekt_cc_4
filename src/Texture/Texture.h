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
    Texture(sf::Texture *texture);
    ~Texture() = default;

    sf::Texture *GetTexture() const;

protected:
    sf::Texture *m_texture;
};

class AllTextures : public Texture
{
public:
    AllTextures(sf::Texture *texture, const uint8_t ID);
    ~AllTextures() = default;

    uint8_t GetID() const;

private:
    uint8_t m_ID;
};
