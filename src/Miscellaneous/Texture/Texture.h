#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

using namespace std;

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

class Texture
{
public:
    Texture(unique_ptr<sf::Texture> texture, const uint8_t ID);
    ~Texture() = default;

    uint8_t GetID() const;
    const sf::Texture *GetTexture() const;

private:
    uint8_t m_ID;
    unique_ptr<sf::Texture> m_texture;
};
