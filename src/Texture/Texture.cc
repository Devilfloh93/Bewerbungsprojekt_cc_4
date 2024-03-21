#include "Texture.h"

Texture::Texture(const std::uint8_t ID, sf::Texture *texture) : m_ID(ID), m_texture(texture)
{
}

std::uint8_t Texture::GetID() const
{
    return m_ID;
}

sf::Texture *Texture::GetTexture() const
{
    return m_texture;
}
