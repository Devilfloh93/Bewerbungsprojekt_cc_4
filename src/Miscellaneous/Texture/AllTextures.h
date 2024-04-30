#pragma once
#include "Texture.h"
#include <SFML/Graphics.hpp>

struct CollisionData
{
    uint8_t x;
    uint8_t y;
};

struct TextureProgData
{
    sf::IntRect rect;
    CollisionData collision;
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
