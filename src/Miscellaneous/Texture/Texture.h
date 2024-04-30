#pragma once
#include <SFML/Graphics.hpp>

class Texture
{
public:
    Texture(sf::Texture *texture);
    ~Texture() = default;

    sf::Texture *GetTexture() const;

protected:
    sf::Texture *m_texture;
};
