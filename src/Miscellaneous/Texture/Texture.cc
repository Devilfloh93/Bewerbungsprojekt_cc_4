#include "Texture.h"


Texture::Texture(sf::Texture *texture) : m_texture(texture)
{
}

sf::Texture *Texture::GetTexture() const
{
    return m_texture;
}
