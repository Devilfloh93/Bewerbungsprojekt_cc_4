#include "Texture.h"

AllTextures::AllTextures(sf::Texture *texture, const uint8_t ID) : Texture(texture), m_ID(ID)
{
}

Texture::Texture(sf::Texture *texture) : m_texture(texture)
{
}

uint8_t AllTextures::GetID() const
{
    return m_ID;
}

sf::Texture *Texture::GetTexture() const
{
    return m_texture;
}
